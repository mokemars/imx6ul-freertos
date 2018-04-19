/*******************************************************************************
 MRF24WG Data Tx/Rx 

  Summary: Contains functions pertaining to WiFi data Tx/Rx

  Description: Contains functions that allocate and deallocate Tx/Rx data messages
               as well as functions that perform Tx/Rx processing.
*******************************************************************************/

/* MRF24WG0M Universal Driver
*
* Copyright (c) 2012-2014, Microchip Technology, Inc. <www.microchip.com>
* Contact Microchip for the latest version.
*
* This program is free software; distributed under the terms of BSD
* license:
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1.    Redistributions of source code must retain the above copyright notice, this
*        list of conditions and the following disclaimer.
* 2.    Redistributions in binary form must reproduce the above copyright notice,
*        this list of conditions and the following disclaimer in the documentation
*        and/or other materials provided with the distribution.
* 3.    Neither the name(s) of the above-listed copyright holder(s) nor the names
*        of its contributors may be used to endorse or promote products derived
*        from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//==============================================================================
//                                  INCLUDES
//==============================================================================
#include "wf_universal_driver.h"
#include "wf_global_includes.h"

#if defined(WF_USE_CONSOLE)
#include "wf_console_cmds.h"
#endif

//==============================================================================
//                                  CONSTANTS
//==============================================================================

//---------------
// Rx Data states
//---------------
typedef enum
{
    DATA_RX_IDLE                     = 0,
    WAIT_FOR_TX_IDLE                 = 1,
    WAIT_FOR_DATA_RX_MOUNT           = 2,
    WAIT_FOR_STACK_TO_COPY_RX_PACKET = 3,
    WAIT_FOR_DATA_RX_UNMOUNT         = 4
} t_RxStates;

//---------------
// Tx data states
//---------------
typedef enum
{
    DATA_TX_IDLE                  = 0,
    WAIT_FOR_TX_MEMORY            = 1,
    WAIT_FOR_DATA_TX_MOUNT        = 2,
    WAIT_FOR_DATA_TX_SIGNAL       = 3,
    WAIT_FOR_RX_IDLE              = 4
} t_TxStates;

// SNAP header constants
#define SNAP_HDR_LENGTH             (6)
#define SNAP_VAL                    (0xaa)
#define SNAP_CTRL_VAL               (0x03)
#define SNAP_TYPE_VAL               (0x00)

#define ETH_HEADER_START_OFFSET     (16)                      // offset in Rx packet where Ethernet header starts
#define WF_TX_PREAMBLE_SIZE         sizeof(g_txDataPreamble)  // internal preamble size in Tx packets


//==============================================================================
//                                  MACROS
//==============================================================================
//#define DBG_TX_PRINT(s) printf(s)
#define DBG_TX_PRINT(s)

//#define DBG_RX_PRINT(s) printf(s)
#define DBG_RX_PRINT(s)

#define MAX_RX_ETHERNET_SIZE        1518
#define MAX_RX_PACKET_PLUS_HEADER   (MAX_RX_ETHERNET_SIZE + 16)  /* 16 bytes of Rx header before start of ethernet packet */

//==============================================================================
//                                  LOCAL DATA TYPES
//==============================================================================

// this structure is present in all Rx packets received from the MRF24WG, at the
// start of the Rx RAW buffer, starting at index 0.  Payload data immediately follows
// this structure
typedef struct wfRxHeaderStruct
{
    uint8_t     type;               // always WF_DATA_RX_INDICATE_TYPE (3)
    uint8_t     subtype;
    uint16_t    rssi;               // not used
    uint32_t    arrivalTime;        // not used
    uint16_t    dataLength;         // number of bytes of payload which immediately follow this structure
    uint8_t     snapHeader[6];      // SNAP header
    uint8_t     destAddress[6];     // destination MAC address  (START OF ETHERNET HEADER)
    uint8_t     srcMacAddress[6];   // source MAC address
    uint16_t    ethType;            // Ethernet type code
} t_RxHeader;

typedef struct
{
    uint8_t type;
    uint8_t subType;
} t_rxPreamble;

typedef struct
{
    uint8_t    snap[SNAP_HDR_LENGTH];
    uint8_t    DestMACAddr[WF_MAC_ADDRESS_LENGTH];    // ethernet header presented to stack starts here
    uint8_t    SourceMACAddr[WF_MAC_ADDRESS_LENGTH];
    uint16_t   Type;
} t_wfRxPreamble;


typedef struct
{
    uint8_t *p_txPacket;
    uint16_t packetLength;
} t_txQueueElement;

typedef struct fifo_struct
{
    int head;
    int tail;
    int count;
    t_txQueueElement element[WF_TX_QUEUE_SIZE + 1];
} t_txQueue;

// Tx Queue Functions
static void TxQueueInit(void);
static bool isTxQueueEmpty(void);
static bool isTxQueueFull(void);
static t_txQueueElement * TxQueueRemove(void);
static t_txQueueElement * TxQueuePeekHead(void);

static bool isTxStateMachineIdle(void);
INLINE static bool isRxPacketAvailable(void);
INLINE static void ClearRxPacketAvailable(void);
static bool isRxPacketHeaderValid(t_RxHeader *p_header);
static INLINE void SignalPacketRx(uint16_t packetLength);
static bool isRxStateMachineIdle(void);

//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
// SNAP header
static const uint8_t  snapHdr[6] = {SNAP_VAL, SNAP_VAL, SNAP_CTRL_VAL, SNAP_TYPE_VAL, SNAP_TYPE_VAL, SNAP_TYPE_VAL};

// Tx state machine globals
static uint8_t        g_txDataState;
static uint8_t        g_txDataPreamble[4] = {WF_DATA_REQUEST_TYPE, WF_STD_DATA_MSG_SUBTYPE, 1, 0};
static t_txQueue      g_txQueue;

// Rx state machine globals
static uint8_t        g_rxDataState;
static volatile bool  g_rxPacketAvailable;      // set by interrupt to notify Rx state machine
static bool           g_rxPacketReadyForStack;  // set by Rx state machine to notify stack
static uint16_t       g_rxPacketLength;
static bool           g_stackDoneWithRxPacket;

static uint8_t        g_mac[WF_MAC_ADDRESS_LENGTH];

// called by Init state machine
void InitDataTxStateMachine(void)
{
    g_txDataState = DATA_TX_IDLE;    // Tx state machine is in the idle state
    TxQueueInit();                   // Init Tx data packet queue
}

// called by Init state machine
void InitDataRxStateMachine(void)
{
    g_rxPacketAvailable     = false;           // no rx data packet available for Rx state machine
    g_rxPacketReadyForStack = false;           // no rx data packet available for TCP/IP stack
    g_rxDataState           = DATA_RX_IDLE;    // Rx state machine is in the idle state
    g_stackDoneWithRxPacket = false;           
}

// Data Tx State Machine -- called from WF_Task()
void WiFi_DataTxTask(void)
{
    uint8_t status;
    uint16_t byteCount = 0;
    static uint8_t *p_packet = NULL;    // packet currently being sent
    static uint16_t packetLength = 0;   // length of packet as created by the TCP/IP stack
    static uint16_t totalPacketLength;  // length of packet + 4 bytes for WiFi preamble
    bool txStateMachineActive = true;
    t_txQueueElement *p_head;
    bool intDisabled;

    intDisabled = WF_isEintDisabled();
    WF_EintDisable();

    DBG_TX_PRINT("TX: ");

    while (txStateMachineActive)
    {
        txStateMachineActive = false;

        switch (g_txDataState)
        {
            //======================================================================
            case DATA_TX_IDLE:
                DBG_TX_PRINT("1 ");
                // if any tx packets in queue
                if ( !isTxQueueEmpty() )
                {
                    DBG_TX_PRINT("2 ");
                    // point to Tx packet, but do not yet remove it from the queue
                    p_head = TxQueuePeekHead();
                    p_packet = p_head->p_txPacket;
                    if (p_packet == NULL)
                    {
                        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_TX_INTERNAL_1);
                    }

                    packetLength = p_head->packetLength;
                    totalPacketLength = packetLength + WF_TX_PREAMBLE_SIZE;

                    if (isRxStateMachineIdle())
                    {
                        DBG_TX_PRINT("3 ");
                        // start raw mount if bytes available.  Total packet length
                        // will be the Ethernet packet plus WiFi preamble (filled in later)
                        if (AllocateDataTxBuffer(totalPacketLength))
                        {
                            DBG_TX_PRINT("4 ");
                            g_txDataState = WAIT_FOR_DATA_TX_MOUNT;
                        }
                        // else not enough bytes available on MRF right now
                        else
                        {
                            DBG_TX_PRINT("5 ");
                            g_txDataState = WAIT_FOR_TX_MEMORY;
                        }
                    }
                    // else need to wait for Rx state machine to finish
                    else
                    {
                        DBG_TX_PRINT("6 ");
                        g_txDataState = WAIT_FOR_RX_IDLE;
                    }
                }
                break;

            //======================================================================
            case WAIT_FOR_RX_IDLE:
                DBG_TX_PRINT("7 ");
                if (isRxStateMachineIdle())
                {
                    // start raw mount if bytes available
                    if (AllocateDataTxBuffer(totalPacketLength))
                    {
                        DBG_TX_PRINT("8 ");
                        g_txDataState = WAIT_FOR_DATA_TX_MOUNT;
                    }
                    // else not enough bytes available on MRF right now
                    else
                    {
                        DBG_TX_PRINT("9 ");
                        g_txDataState = WAIT_FOR_TX_MEMORY;
                    }
                }
                break;

            //======================================================================
            case WAIT_FOR_TX_MEMORY:
                DBG_TX_PRINT("10 ");
                if (isRxStateMachineIdle())
                {
                    DBG_TX_PRINT("11 ");
                    // attempt to allocate data buffer on MRF (if bytes available then Raw Move has been initiated)
                    if (AllocateDataTxBuffer(totalPacketLength))
                    {
                        DBG_TX_PRINT("12 ");
                        g_txDataState = WAIT_FOR_DATA_TX_MOUNT;
                    }
                    // else not enough bytes available on MRF right now
                    else
                    {
                        DBG_TX_PRINT("13 ");
                    }
                }
                else
                {
                    DBG_TX_PRINT("14 ");
                    g_txDataState = WAIT_FOR_RX_IDLE;
                }
                break;

            //======================================================================
            case WAIT_FOR_DATA_TX_MOUNT:
                DBG_TX_PRINT("15 ");

                // if tx data buffer successfully mounted (or timed out trying)
                if (isRawMoveComplete(RAW_DATA_TX_ID, &status, &byteCount))
                {
                    DBG_TX_PRINT("16 ");
                    // if tx mount succeeded
                    if ((status == RM_COMPLETE) && (byteCount != 0))
                    {
                        DBG_TX_PRINT("17 ");

                        // if, after the raw mount, we didn't get all the bytes we needed
                        if (byteCount < totalPacketLength)
                        {
                            EventEnqueue(WF_EVENT_ERROR, UD_ERROR_TX_INTERNAL_2);
                        }

                        EnsureWFisAwake();

                        // write out internal preamble, starting at index 0 in the raw window
                        RawWrite(RAW_DATA_TX_ID, 0, WF_TX_PREAMBLE_SIZE, (uint8_t *)&g_txDataPreamble);

                        // overwrite the Ethernet source address in Tx packet with SNAP header
                        // (starts at index 10 within packet, after 4 bytes of preamble and 6 bytes of dest address)
                        memcpy(&p_packet[WF_MAC_ADDRESS_LENGTH], snapHdr, sizeof(snapHdr));

                        // write out tx ethernet packet as originally created by TCP/IP stack
                        RawSetByte(RAW_DATA_TX_ID, p_packet, packetLength);

                        // Now that full packet copied to MRF24WG, signal it that packet ready to transmit (start raw move)
                        SendRAWDataFrame(totalPacketLength);

                        g_txDataState = WAIT_FOR_DATA_TX_SIGNAL;

                    }
                    // else if raw move timeout
                    else if (status == RM_TIMEOUT)
                    {
                        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_TX_INTERNAL_3);
                    }
                    // else allocation had 0 bytes
                    else if (byteCount == 0)
                    {
                        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_TX_INTERNAL_4);
                    }
                }
                break;

            //======================================================================
            case WAIT_FOR_DATA_TX_SIGNAL:
                DBG_TX_PRINT("19 ");
                // if raw move complete that signals MRF24WG that there is a tx data
                // packet to transmit
                if (isRawMoveComplete(RAW_DATA_TX_ID, &status, &byteCount))
                {
                    DBG_TX_PRINT("20 ");
                    if (status == RM_COMPLETE)
                    {
                        DBG_TX_PRINT("21 ");
                        // now that head packet copied to MRF and MRF has begun transmission, remove
                        // head packet from the queue
                        p_head = TxQueueRemove();
                        
                        // notify stack that WiFi driver is done with this packet.
                        WF_TxComplete(p_head->p_txPacket);
                        
                        // if any pending tx packets
                        if ( !isTxQueueEmpty() )
                        {
                            DBG_TX_PRINT("22 ");
                            txStateMachineActive = true; //run state machine again to start Raw Mount
                        }
#if defined(WF_USE_CONSOLE)
                        IncrementTxPackets();
#endif
                        g_txDataState = DATA_TX_IDLE; // ready for next tx data
                    }
                    else if (status == RM_TIMEOUT)
                    {
                        // raw move timeout
                        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_TX_INTERNAL_5);
                    }
                    else
                    {
                        // unknown status
                        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_TX_INTERNAL_6);
                    }
                }
                break;
        } // end switch

    } // end while

    if (!intDisabled)
    {
        WF_EintEnable();
    }

    DBG_TX_PRINT("\r\n");
}



static void TxQueueInit(void)
{
    memset(&g_txQueue, 0x00, sizeof(t_txQueue));
    g_txQueue.head = g_txQueue.tail = WF_TX_QUEUE_SIZE;
}

static bool isTxQueueEmpty(void)
{
    return g_txQueue.count == 0;
}

static bool isTxQueueFull(void)
{
    return g_txQueue.count == WF_TX_QUEUE_SIZE;
}

// called by TCP/IP stack to add new element to Tx Queue 
bool WF_QueueTxPacket(uint8_t *p_txPacket, uint16_t packetLength)
{
    // if Tx Queue is full, let the caller know
    if (isTxQueueFull())
    {
#if defined(WF_USE_DEBUG_OUTPUT)
    OutputTxQueueFailMsg();
#endif
        return false;
    }

    if (g_txQueue.tail == WF_TX_QUEUE_SIZE)
    {
        g_txQueue.tail = 0;
    }
    else
    {
        ++g_txQueue.tail;
    }

    g_txQueue.element[g_txQueue.tail].p_txPacket   = p_txPacket;
    g_txQueue.element[g_txQueue.tail].packetLength = packetLength;
    ++g_txQueue.count;
    return true;
}

static t_txQueueElement * TxQueueRemove(void)
{
 if (g_txQueue.head == WF_TX_QUEUE_SIZE)
    {
        g_txQueue.head = 0;
    }
    else
    {
        ++g_txQueue.head;
    }

    --g_txQueue.count;
    return &g_txQueue.element[g_txQueue.head];
}

// gets a pointer to head element, but leaves it in queue
static t_txQueueElement * TxQueuePeekHead(void)
{
    uint8_t head = g_txQueue.head;

    if (head < WF_TX_QUEUE_SIZE)
    {
        return &g_txQueue.element[head + 1];
    }
    else
    {
        return &g_txQueue.element[0];
    }
}

// called by interrupt to notify Rx state machine of incoming packet
INLINE void SignalRxPacketAvailable(void)
{
    g_rxPacketAvailable = true;
}

// called by Rx state machine to check if a new Rx packet is ready to process
INLINE static bool isRxPacketAvailable(void)
{
    return g_rxPacketAvailable;
}

// called by Rx state machine to clear the flag after it was notified of new Rx packet
INLINE static void ClearRxPacketAvailable(void)
{
    g_rxPacketAvailable = false;
}

// called by Rx state machine to check if Tx state machine is inactive
static bool isTxStateMachineIdle(void)
{
    // tx state machine is logically idle if its state is idle or it is waiting for the rx state machine to go idle
    return (g_txDataState == DATA_TX_IDLE) || (g_txDataState == WAIT_FOR_RX_IDLE);
}

void WiFi_DataRxTask(void)
{
    uint8_t     status;
    static uint16_t    byteCount;
    t_RxHeader  rxHeader;
    bool        intDisabled;
    
    intDisabled = WF_isEintDisabled();
    WF_EintDisable();


    DBG_RX_PRINT("RX: ");
    switch (g_rxDataState)
    {
        //======================================================================
        case DATA_RX_IDLE:
            DBG_RX_PRINT("0 ");

            // if a Rx packet has been received by the MRF
            if (isRxPacketAvailable())
            {
                DBG_RX_PRINT("1 ");
                ClearRxPacketAvailable();
                
                if (isTxStateMachineIdle())
                {
                    DBG_RX_PRINT("2 ");
                    RawMountRxDataBuffer();   // start the raw Rx data mount
                    g_rxDataState = WAIT_FOR_DATA_RX_MOUNT;
                }
                else
                {
                    DBG_RX_PRINT("3 ");
                    g_rxDataState = WAIT_FOR_TX_IDLE;
                }
            }
            break;

        //======================================================================
        case WAIT_FOR_TX_IDLE:
            DBG_RX_PRINT("4 ");
            // if tx state machine is idle, or tx state machine is waiting for Rx state machine
            if (isTxStateMachineIdle())
            {
                DBG_RX_PRINT("5 ");
                // then run this (rx) state machine and let tx state machine wait
                RawMountRxDataBuffer();   // start the raw Rx data mount
                g_rxDataState = WAIT_FOR_DATA_RX_MOUNT;
            }
            break;

        //======================================================================
        case WAIT_FOR_DATA_RX_MOUNT:
            DBG_RX_PRINT("5 ");
            // this should always be true as the event is only triggered, and this
            // function should only be called, after the Raw Move has completed.
            if (isRawMoveComplete(RAW_DATA_RX_ID, &status, &byteCount))
            {
                // this will be the next state, unless we received a valid Rx packet
                g_rxDataState = WAIT_FOR_DATA_RX_UNMOUNT;

                DBG_RX_PRINT("6 ");

                // if raw move successfully completed and the Rx packet header is valid
                if (status == RM_COMPLETE)
                {
                    if( !(byteCount > 0) && (byteCount <= MAX_RX_PACKET_PLUS_HEADER))
                    {
                        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_RX_INTERNAL_1);
                    }

                    DBG_RX_PRINT("7 ");
                    // read the Rx header in the packet -- this includes the wifi header and the
                    // the ethernet header
                    RawRead(RAW_DATA_RX_ID, 0, sizeof(t_RxHeader), (uint8_t*)&rxHeader);

                    if (isRxPacketHeaderValid(&rxHeader))
                    {
                        DBG_RX_PRINT("8 ");

                        // if we did not receive our own broadcast
                        if (memcmp(rxHeader.srcMacAddress, g_mac, WF_MAC_ADDRESS_LENGTH) != 0)
                        {
                            // set the raw index back to the start of the ethernet packet (first byte of
                            // destination address)
                            RawSetIndex(RAW_DATA_RX_ID, ETH_HEADER_START_OFFSET);

                            // notify WF_Task that a Rx packet is ready for the stack to read.  The length
                            // of the ethernet packet is the total length less the 16 bytes of WiFi header at the
                            // start of the packet.
                            SignalPacketRx(byteCount - ETH_HEADER_START_OFFSET );

                            g_stackDoneWithRxPacket = false;
                            
                            g_rxDataState = WAIT_FOR_STACK_TO_COPY_RX_PACKET;
                        }
                        // else received our own broadcast (source mac is the WiFi mac)
                        else
                        {
                            // nothing to do, let packet be unmounted and throw it away
                            DBG_RX_PRINT("9 ");
                        }
#if defined(WF_USE_CONSOLE)
                        IncrementRxPackets();
#endif
                    }
                    // else invalid header or proprietary packet; throw it away
                    else
                    {
                        // nothing to do here, let state machine continue so Rx buffer unmounted
                        DBG_RX_PRINT("10 ");
                    }
                }
                // else if raw move timed out 
                else if (status == RM_TIMEOUT)
                {
                    EventEnqueue(WF_EVENT_ERROR, UD_ERROR_RX_INTERNAL_3);
                }
                // else unknown status
                else
                {
                    EventEnqueue(WF_EVENT_ERROR, UD_ERROR_RX_INTERNAL_4);
                }

                // if, after above actions, we are going to unmount the packet and throw it away
                if (g_rxDataState == WAIT_FOR_DATA_RX_UNMOUNT)
                {
                    DBG_RX_PRINT("11 ");
                    // begin raw unmount of data rx packet (it's either been buffered for stack or being thrown away)
                    DeallocateDataRxBuffer();
                }
            }
            break;

        //======================================================================
        case WAIT_FOR_STACK_TO_COPY_RX_PACKET:
            // if stack has completed copying this packet into its buffer
            if (g_stackDoneWithRxPacket)
            {
                // start the packet deallocation on the MRF (raw move)
                g_stackDoneWithRxPacket = false;
                DeallocateDataRxBuffer();
                g_rxDataState = WAIT_FOR_DATA_RX_UNMOUNT;
            }
            break;

        //======================================================================
        case WAIT_FOR_DATA_RX_UNMOUNT:
            if (isRawMoveComplete(RAW_DATA_RX_ID, &status, &byteCount))
            {
                if (status == RM_COMPLETE)
                {
                    // expected behavior; nothing to do here
                }
                else if (status == RM_TIMEOUT)
                {
                    EventEnqueue(WF_EVENT_ERROR, UD_ERROR_RX_INTERNAL_5);
                }
                else
                {
                    EventEnqueue(WF_EVENT_ERROR, UD_ERROR_RX_INTERNAL_6);
                }
                DBG_RX_PRINT("10 ");

                g_rxDataState = DATA_RX_IDLE;
            }
            break;
    } // end switch

    DBG_RX_PRINT("\r\n");

    if (!intDisabled)
    {
        WF_EintEnable();
    }
}

static bool isRxStateMachineIdle(void)
{
    return (g_rxDataState == DATA_RX_IDLE);
}


// called by Rx state machine to determine if packet has valid header
static bool isRxPacketHeaderValid(t_RxHeader *p_header)
{
    // verify that this packet is a Rx data packet (should always be true!)
    if (p_header->type != WF_DATA_RX_INDICATE_TYPE)
    {
        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_RX_INTERNAL_2);
        return false;
    }

    // verify that the SNAP header is correct; if not, this is a
    // proprietary packet that will be thrown away by Rx state machine.
    if (memcmp(p_header->snapHeader, snapHdr, SNAP_HDR_LENGTH) != 0)
    {
        return false;
    }

    return true;
}

INLINE bool isPacketRx(void)
{
    return g_rxPacketReadyForStack;
}

INLINE void ClearPacketRx(void)
{
    g_rxPacketReadyForStack = false;
}

// sets a flag that notifies WF_Task of Rx packet ready
static INLINE void SignalPacketRx(uint16_t packetLength)
{
    g_rxPacketLength = packetLength;
    g_rxPacketReadyForStack = true;
}

// called by stack to get the Rx packet length
uint16_t WF_RxPacketLengthGet(void)
{
    return g_rxPacketLength;
}

// called by stack to read Rx packet into its buffer(s)
void WF_RxPacketCopy(uint8_t *p_buf, uint16_t length)
{
    RawGetByte(RAW_DATA_RX_ID, p_buf, length);
}

// called by stack to signal it has completed copying the packet
void WF_RxPacketDeallocate(void)
{
    g_stackDoneWithRxPacket = true;
}


