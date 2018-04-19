/*******************************************************************************
 MRF24WG RAW (Random Access Window)

  Summary: Functions to control RAW windows

  Description: Same as summary
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

//==============================================================================
//                                  DEFINES
//==============================================================================
#define WF_RAW_STATUS_REG_BUSY_MASK     ((uint16_t)(0x0001))

typedef enum
{
    RM_INIT_START                    = 0,
    RM_INIT_WAIT_FOR_SCRATCH_UNMOUNT = 1,
    RM_INIT_WAIT_FOR_SCRATCH_MOUNT   = 2,
    RM_INIT_SUCCESSFUL               = 3,
    RM_INIT_FAILED                   = 4
} t_rmInitStates;


//==============================================================================
//                                  MACROS
//==============================================================================

#define SetRawMoveInitState(state)  g_rawMoveInitState = state
#define GetRawMoveInitState()       g_rawMoveInitState

#if 0
/* these macros set a flag bit if the raw index is set past the end of the raw window, or clear the */
/* flag bit if the raw index is set within the raw window.                                              */
#define SetIndexOutOfBoundsFlag(rawId)      g_RawIndexPastEnd |= g_RawAccessOutOfBoundsMask[rawId]
#define ClearIndexOutOfBoundsFlag(rawId)    g_RawIndexPastEnd &= ~g_RawAccessOutOfBoundsMask[rawId]
#define isIndexOutOfBounds(rawId)           ((g_RawIndexPastEnd & g_RawAccessOutOfBoundsMask[rawId]) > 0)
#endif

//==============================================================================
//                                  LOCAL DATA TYPES
//==============================================================================
typedef struct rawMoveWaitState
{
    uint16_t rawId;             // RAW ID of engine we are waiting on
    uint8_t  rawIntMask;        // mask to see if correct raw ID move completed
    bool     intDisabled;       // saved state of eint prior to Raw move
    uint32_t startTickCount;    // start time of raw move
    uint32_t maxAllowedTicks;   // max allowed wait time for a raw move
} t_rawMoveWaitState;

typedef struct
{
    uint32_t startTime;     // time that timer was started at
    uint32_t duration;      // number of milliseconds before timeout declared
} t_rawMoveTimer;



//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
static uint8_t g_rawMoveInitState;

/* raw registers for each raw window being used */
static const uint8_t  g_RawIndexReg[NUM_RAW_WINDOWS]  = {RAW_0_INDEX_REG,  RAW_1_INDEX_REG,  RAW_2_INDEX_REG,  RAW_3_INDEX_REG,  RAW_4_INDEX_REG, RAW_5_INDEX_REG};
static const uint8_t  g_RawStatusReg[NUM_RAW_WINDOWS] = {RAW_0_STATUS_REG, RAW_1_STATUS_REG, RAW_2_STATUS_REG, RAW_3_STATUS_REG, RAW_4_STATUS_REG, RAW_5_STATUS_REG};
static const uint16_t g_RawCtrl0Reg[NUM_RAW_WINDOWS]  = {RAW_0_CTRL_0_REG, RAW_1_CTRL_0_REG, RAW_2_CTRL_0_REG, RAW_3_CTRL_0_REG, RAW_4_CTRL_0_REG, RAW_5_CTRL_0_REG};
static const uint16_t g_RawCtrl1Reg[NUM_RAW_WINDOWS]  = {RAW_0_CTRL_1_REG, RAW_1_CTRL_1_REG, RAW_2_CTRL_1_REG, RAW_3_CTRL_1_REG, RAW_4_CTRL_1_REG, RAW_5_CTRL_1_REG};
static const uint16_t g_RawDataReg[NUM_RAW_WINDOWS]   = {RAW_0_DATA_REG,   RAW_1_DATA_REG,   RAW_2_DATA_REG,   RAW_3_DATA_REG,   RAW_4_DATA_REG, RAW_5_DATA_REG};

/* interrupt mask for each raw window; note that raw0 and raw1 are really 8 bit values and will be cast when used  */
static const uint16_t g_RawIntMask[NUM_RAW_WINDOWS]   =
    {WF_HOST_INT_MASK_RAW_0_INT_0,   /* used in HOST_INTR reg (8-bit register)   */
     WF_HOST_INT_MASK_RAW_1_INT_0,   /* used in HOST_INTR reg (8-bit register)   */
     WF_HOST_INT_MASK_RAW_2_INT_0,   /* used in HOST_INTR2 reg (16-bit register) */
     WF_HOST_INT_MASK_RAW_3_INT_0,   /* used in HOST_INTR2 reg (16-bit register) */
     WF_HOST_INT_MASK_RAW_4_INT_0,   /* used in HOST_INTR2 reg (16-bit register) */
     WF_HOST_INT_MASK_RAW_5_INT_0};  /* used in HOST_INTR2 reg (16-bit register) */

static volatile t_rawMoveWaitState g_rawMoveWaitState;
volatile t_RawMoveState    RawMoveState[5];

#if 0
/* bit mask where each bit corresponds to a raw window id.  If set, the raw index has been set past */
/* the end of the raw window.                                                                       */
const uint8_t g_RawAccessOutOfBoundsMask[NUM_RAW_WINDOWS] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20};
uint8_t g_RawIndexPastEnd = 0x00;  // no indexes are past end of window 
#endif

// index 0 corresponds to RAW0, index 1 corresponds to RAW1, and index 2 corresponds
// to RAW4 (Scratch)
static t_rawMoveTimer g_rawMoveTimer[3];
static uint16_t       g_scratchByteCount;

//==============================================================================
//                                  LOCAL FUNCTIONS
//==============================================================================
#if 0
static void RawMoveWithPolling(uint16_t rawId,
                               uint16_t srcDest,
                               bool     rawIsDestination,
                               uint16_t size);
#endif

static void ScratchUnmount(void);
static void ScratchMount(void);
static bool isScratchUnmountComplete(void);
static bool isScratchMountComplete(uint16_t *p_byteCount);

static void RawMove(uint16_t   rawId,
                    uint16_t   srcDest,
                    bool       rawIsDestination,
                    uint16_t   size);

static bool isRawMoveTimeout(uint16_t rawId);


/*****************************************************************************
  Function:
    uint8_t RawInitSm(void);

  Summary:
    Initializes RAW (Random Access Window) engines on MRF

  Description:
    Unmounts Scratch from RAW1 and then mounts scratch to RAW4

  Precondition:
    None

  Parameters:
    None

  Returns:
    SM_IN_PROGRESS, SM_SUCCESSFUL, or SM_FAILED

  Remarks:
    None
*****************************************************************************/
uint8_t RawInitSm(void)
{
    uint8_t res = SM_IN_PROGRESS;

    switch ( GetRawMoveInitState() )
    {
        //------------------------
        case RM_INIT_START:
        //------------------------
            // start the scratch unmount from RAW 1
            ScratchUnmount();
            SetRawMoveInitState(RM_INIT_WAIT_FOR_SCRATCH_UNMOUNT);
            break;

        //------------------------
        case RM_INIT_WAIT_FOR_SCRATCH_UNMOUNT:
        //------------------------
        if (isScratchUnmountComplete())
        {
            // start mount of scratch memory to RAW 4
            ScratchMount();
            SetRawMoveInitState(RM_INIT_WAIT_FOR_SCRATCH_MOUNT);
        }
        else if (isRawMoveTimeout(RAW_ID_1))
        {
            EventEnqueue(WF_EVENT_ERROR, UD_ERROR_SCRATCH_UNMOUNT_TIMEOUT);
            SetRawMoveInitState(RM_INIT_FAILED);
        }
        break;
        
        //------------------------
        case RM_INIT_WAIT_FOR_SCRATCH_MOUNT:
        //------------------------
            if (isScratchMountComplete(&g_scratchByteCount))
            {
                // No Tx data writes or reads have occurred past the end of the window
#if 0
                g_RawIndexPastEnd = 0;
#endif
                SetRawMoveInitState(RM_INIT_SUCCESSFUL);
            }
            else if (isRawMoveTimeout(RAW_SCRATCH_ID))
            {
                EventEnqueue(WF_EVENT_ERROR, UD_ERROR_SCRATCH_MOUNT_TIMEOUT);
                SetRawMoveInitState(RM_INIT_FAILED);
            }
            break;

        //------------------------
        case RM_INIT_SUCCESSFUL:
        //------------------------
            res = SM_SUCCESSFUL;
            break;

        //------------------------
        case RM_INIT_FAILED:
        //------------------------
            res = SM_FAIL;
            break;
    }

    return res;
}

// called by InitSm() to reset Raw Init state machine
void ResetRawInitSm(void)
{
    SetRawMoveInitState(RM_INIT_START);
}

// called by RawMove().  At init, the scratch ID will be passed in and ignored, as
// we are polling the scratch move.  This timer has a resolution of +-1ms.
void StartRawMoveTimer(uint16_t rawId)
{
    // if Rx or Tx raw id
    if (rawId <= RAW_DATA_TX_ID)
    {
        g_rawMoveTimer[rawId].duration = 3000;              // three seconds
        g_rawMoveTimer[rawId].startTime = WF_TimerRead();
    }
    // must be scratch raw ID (only used at init)
    else
    {
        g_rawMoveTimer[2].duration = 3000;
        g_rawMoveTimer[2].startTime = WF_TimerRead();
    }
}

static bool isRawMoveTimeout(uint16_t rawId)
{
    uint32_t        startTime;
    uint32_t        elapsedTime;
    uint32_t        duration;

    // if raw ID is 0 or 1 (data rx or tx)
    if (rawId <= RAW_DATA_TX_ID)
    {
        startTime = g_rawMoveTimer[rawId].startTime;
        duration = g_rawMoveTimer[rawId].duration;
    }
    // must be scratch raw ID (only used at init)
    else
    {
        startTime = g_rawMoveTimer[2].startTime;
        duration = g_rawMoveTimer[2].duration;
    }

    elapsedTime = GetElapsedTime(startTime, WF_TimerRead());

    return elapsedTime > duration;
}

/*****************************************************************************
  Function:
    bool AllocateDataTxBuffer(uint16_t bytesNeeded)

  Summary:
    Allocates a Data Tx buffer for use by the TCP/IP stack.

  Description:
    Determines if WiFi chip has enough memory to allocate a tx data buffer, and,
    if so, allocates it.

  Precondition:
    None

  Parameters:
    bytesNeeded -- number of bytes needed for the data tx message

  Returns:
    True if data tx buffer successfully allocated, else False

  Remarks:
    None
*****************************************************************************/
bool AllocateDataTxBuffer(uint16_t bytesNeeded)
{
    uint16_t bufAvail;

    /* Ensure the MRF24W is awake (only applies if PS-Poll was enabled) */
    EnsureWFisAwake();

    /* get total bytes available for DATA tx memory pool */
    bufAvail = Read16BitWFRegister(WF_HOST_WFIFO_BCNT0_REG) & 0x0fff; /* LS 12 bits contain length */

    /* if enough bytes available to allocate */
    if ( bufAvail >= bytesNeeded )
    {
        // start the raw move that performs the allocation
        RawMove(RAW_DATA_TX_ID, RAW_DATA_POOL, true, bytesNeeded);
        return true;
    }
    /* else not enough bytes available at this time to satisfy request */
    else
    {
        return false;
    }
}

void SendRAWDataFrame(uint16_t bufLen)
{
    /* Notify WiFi device that there is a transmit frame to send .  The frame will */
    /* be automatically deallocated after RF transmission is completed.            */
    RawMove(RAW_DATA_TX_ID, RAW_MAC, false, bufLen);
}

/*****************************************************************************
  Function:
    void DeallocateDataRxBuffer(void)

  Summary:
    Deallocates a Data Rx buffer

  Description:
    Directs the MRF to deallocate the Rx data frame after the stack is done with it.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
*****************************************************************************/
void DeallocateDataRxBuffer(void)
{
    EnsureWFisAwake();

    /* perform deallocation of raw rx buffer */
    RawMove(RAW_DATA_RX_ID, RAW_DATA_POOL, false, 0);
}

void RawMountRxDataBuffer(void)
{
    RawMove(RAW_DATA_RX_ID, RAW_MAC, true, 0);  // start mount process on MRF,
}

/*********************************************************************************************************
*                                   RawRead()
*
* Description : Reads the specified number of bytes from a mounted RAW window from the specified starting
*               index;
*
* Argument(s) : rawId      -- RAW window ID being read from
*               startIndex -- start index within RAW window to read from
*               length     -- number of bytes to read from the RAW window
*               p_dest     -- pointer to Host buffer where read data is copied
*
* Return(s)   : error code
*
* Caller(s)   : WF Driver
*
* Notes:      : None
*
*********************************************************************************************************/
void RawRead(uint8_t rawId, uint16_t startIndex, uint16_t length, uint8_t *p_dest)
{
    RawSetIndex(rawId, startIndex);
    RawGetByte(rawId, p_dest, length);
}

// similar to RawRead, but takes advantage of the auto-increment feature and does
// not set the index before reading
void RawReadRelative(uint8_t rawId, uint16_t length, uint8_t *p_dest)
{
    RawGetByte(rawId, p_dest, length);
}

/*
*********************************************************************************************************
*                                   RawWrite()
*
* Description : Writes the specified number of bytes to a mounted RAW window at the specified starting
*               index
*
* Argument(s) : rawId      -- RAW window ID being written to
*               startIndex -- start index within RAW window to write to
*               length     -- number of bytes to write to RAW window
*               p_src      -- pointer to Host buffer write data
*
* Return(s)   : None
*
* Caller(s)   : WF Driver
*
* Notes:      : None
*
*********************************************************************************************************
*/
void RawWrite(uint8_t rawId, uint16_t startIndex, uint16_t length, uint8_t *p_src)
{
    /*set raw index in dest memory */
    RawSetIndex(rawId, startIndex);

    /* write data to RAW window */
    RawSetByte(rawId, p_src, length);
}

/*****************************************************************************
  Function: void RawMove(uint16_t rawId,
                         uint16_t srcDest,
                         bool     rawIsDestination,
                         uint16_t size)

  Summary:
    Performs RAW Move operation

  Description:
    Raw Moves perform a variety of operations (e.g. allocating tx buffers,
    mounting rx buffers, copying from one raw window to another, etc.)

  Precondition:
    None

  Parameters:
    rawId -- Raw ID 0 thru 5, except is srcDest is RAW_COPY, in which case rawId
             contains the source address in the upper 4 bits and destination
             address in lower 4 bits.

    srcDest -- object that will either be the source or destination of the move:
                  RAW_MAC
                  RAW_MGMT_POOL
                  RAW_DATA_POOL
                  RAW_SCRATCH_POOL
                  RAW_STACK_MEM
                  RAW_COPY (this object not allowed, handled in RawToRawCopy() )

    rawIsDestination -- true if srcDest is the destination, false if srcDest is
                        the source of the move

    size -- number of bytes to overlay (not always applicable)

  Returns:
    None

  Remarks:
    None
*****************************************************************************/
static void RawMove(uint16_t   rawId,
                    uint16_t   srcDest,
                    bool       rawIsDestination,
                    uint16_t   size)
{
    uint8_t  regId;
    uint16_t ctrlVal = 0;

    g_rawMoveWaitState.rawId = rawId;

    /* create control value that will be written to raw control register, which initiates the raw move */
    if (rawIsDestination)
    {
        ctrlVal |= 0x8000;
    }

    ctrlVal |= (srcDest << 8);              /* defines are already shifted by 4 bits */
    ctrlVal |= ((size >> 8) & 0x0f) << 8;   /* MS 4 bits of size (bits 11:8)         */
    ctrlVal |= (size & 0x00ff);             /* LS 8 bits of size (bits 7:0)          */

    // needs to be set before doing raw move
    RawMoveState[rawId].rawInterrupt  = 0;
    RawMoveState[rawId].waitingForRawMoveCompleteInterrupt = true;

    /* create mask to check against for Raw Move complete interrupt for either RAW0 or RAW1 */
    if (rawId <= RAW_ID_1)
    {
        /* will be either raw 0 or raw 1 */
        g_rawMoveWaitState.rawIntMask = (rawId == RAW_ID_0)?WF_HOST_INT_MASK_RAW_0_INT_0:WF_HOST_INT_MASK_RAW_1_INT_0;
    }
    else
    {
        /* will be INTR2 bit in host register, signifying RAW2, RAW3, or RAW4 */
        g_rawMoveWaitState.rawIntMask = WF_HOST_INT_MASK_INT2;
    }

    /*------------------------------------------------------------------------------------------------*/
    /* now that the expected raw move complete interrupt has been cleared and we are ready to receive */
    /* it, initiate the raw move operation by writing to the appropriate RawCtrl0.                    */
    /*------------------------------------------------------------------------------------------------*/
    regId = g_RawCtrl0Reg[rawId];                   /* get RawCtrl0 register address for desired raw ID */
    Write16BitWFRegister(regId, ctrlVal);           /* write ctrl value to register                     */

    StartRawMoveTimer(rawId);
}

/*****************************************************************************
 * FUNCTION: isRawMoveComplete
 *
 * RETURNS: true if raw move complete, else false
 *
 * PARAMS:
 *      rawId    -- RAW ID being checked
 *      p_status -- pointer to current state of Raw move
 *                   RM_COMPLETE -- Raw move completed successfully, function will return true in conjunction with this state
 *                   RM_WAITING  -- Still waiting for Raw move to complete, function will return true in conjunction with this state
 *                   RM_TIMEOUT  -- Raw move failed due to timeout, function will return true in conjunction with this state
 *      p_byteCount -- pointer to where byte count is written upon raw move completion; only
 *                     valid when function returns true (not always used)
 *
 *  NOTES: Checks if a RAW move to complete.
 *****************************************************************************/
bool isRawMoveComplete(uint8_t rawId, uint8_t *p_status, uint16_t *p_byteCount)
{
    uint8_t  regId;

    bool retCode = false;
    *p_byteCount = 0;
    *p_status    = RM_WAITING;

    // if received an external interrupt that signalled the RAW Move completed
    if(RawMoveState[rawId].rawInterrupt & g_rawMoveWaitState.rawIntMask)
    {
        // set back to false
        RawMoveState[rawId].waitingForRawMoveCompleteInterrupt = false;

        // read the byte count and save it (not all raw moves have relevant byte counts)
        regId = g_RawCtrl1Reg[g_rawMoveWaitState.rawId];
        *p_byteCount = Read16BitWFRegister(regId) & 0x0fff; // LS 12 bits contain byte count
        *p_status = RM_COMPLETE;

        retCode = true;
    }
    else if (isRawMoveTimeout(rawId))
    {
        *p_status = RM_TIMEOUT;
        retCode = true;
    }

    return retCode;
}

/*****************************************************************************
  Function:
    void RawSetIndex(uint16_t rawId, uint16_t index)

  Summary:
    Sets the index within the specified RAW window.

  Description:
    Sets the index within the specified RAW window. If attempt to set RAW index
    outside boundaries of RAW window (past the end) this function will time out.
    It's legal to set the index past the end of the raw window so long as there
    is no attempt to read or write at that index.  For now, flag an event.

  Precondition:
    None

  Parameters:
    rawId -- RAW window ID
    index -- desired index within RAW window

  Returns:
     None

  Remarks:
    None
*****************************************************************************/
void RawSetIndex(uint16_t rawId, uint16_t index)
{
    uint8_t  regId;
    uint16_t regValue;
    uint32_t elapsedTime;
    uint32_t startTime;

    /* get the index register associated with the raw ID and write to it */
    regId = g_RawIndexReg[rawId];
    Write16BitWFRegister(regId, index);

    /* Get the raw status register address associated with the raw ID.  This will be polled to         */
    /* determine that:                                                                                 */
    /*  1) raw set index completed successfully  OR                                                    */
    /*  2) raw set index failed, implying that the raw index was set past the end of the raw window    */
    regId = g_RawStatusReg[rawId];

    /* read the status register until set index operation completes or times out */
    startTime = WF_TimerRead();
    while (1)
    {
        regValue = Read16BitWFRegister(regId);

        if ((regValue & WF_RAW_STATUS_REG_BUSY_MASK) == 0)
        {
#if 0
            ClearIndexOutOfBoundsFlag(rawId);
#endif
            break;
        }
        elapsedTime = GetElapsedTime(startTime, WF_TimerRead());
        if (elapsedTime > 5)
        {
            // if we timed out that means that the caller is trying to set the index
            // past the end of the raw window.  Not illegal in of itself so long
            // as there is no attempt to read or write at this location.
#if 0
            SetIndexOutOfBoundsFlag(rawId);
#endif
            EventEnqueue(WF_EVENT_ERROR, UD_ERROR_RAW_SET_INDEX_OUT_OF_BOUNDS);
            break;
        }
    }
}

/*****************************************************************************
 * FUNCTION: RawSetByte
 *
 * RETURNS: None
 *
 * PARAMS:
 *      rawId   - RAW ID
 *      pBuffer - Buffer containing bytes to write
 *      length  - number of bytes to read
 *
 *  NOTES: Writes bytes to RAW window
 *****************************************************************************/
void RawSetByte(uint16_t rawId, uint8_t *pBuffer, uint16_t length)
{
    uint8_t regId;
#if 0
    if (isIndexOutOfBounds(rawId))
    {
        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_RAW_SET_BYTE_OUT_OF_BOUNDS);
    }
#endif
    /* write data to raw window */
    regId = g_RawDataReg[rawId];
    WriteWFArray(regId, pBuffer, length);
}

/*****************************************************************************
 * FUNCTION: RawGetByte
 *
 * RETURNS: error code
 *
 * PARAMS:
 *      rawId   - RAW ID
 *      pBuffer - Buffer to read bytes into
 *      length  - number of bytes to read
 *
 *  NOTES: Reads bytes from the RAW engine
 *****************************************************************************/
void RawGetByte(uint16_t rawId, uint8_t *pBuffer, uint16_t length)
{

    uint8_t regId;
#if 0
    if (isIndexOutOfBounds(rawId))
    {
        EventEnqueue(WF_EVENT_ERROR, UD_ERROR_RAW_GET_BYTE_OUT_OF_BOUNDS);
    }
#endif
    regId = g_RawDataReg[rawId];
    ReadWFArray(regId, pBuffer, length);
}


/*****************************************************************************
  Function:
    void ScratchUnmount(void)

  Summary:
    Unmounts RAW scratch window.

  Description:
    This is called during the reset process to unmount Scratch from RAW 1, where
    the MRF mounts it after a reset.  We are using RAW1 for data Tx, so we need
    to unmount the scratch from RAW1 so it can be used for data Tx.

  Precondition:
    WiFi interrupts are not yet initialized.

  Parameters:
    None

  Returns:
    None

  Remarks:
    Not using the raw move complete interrupt to determine when the raw move has
    completed.  Instead we are polling the raw status register.  The scratch
    unmount is only done at init, and it simplifies the interrupt routine not
    to mix interrupts occurring at init vs interrupts occurring during normal operations.
*****************************************************************************/
void ScratchUnmount(void)
{
    RawMove(RAW_ID_1, RAW_SCRATCH_POOL, false, 0);
}

static bool isScratchUnmountComplete(void)
{
    uint8_t hostIntReg;
    uint8_t res = false;

    // read the interrupt status register to determine if the raw move is complete
    hostIntReg = Read8BitWFRegister(WF_HOST_INTR_REG);

    // if raw move complete
    if ((hostIntReg & WF_HOST_INT_MASK_RAW_1_INT_0) == WF_HOST_INT_MASK_RAW_1_INT_0)
    {
        // clear RAW Move complete interrupt bit
        Write8BitWFRegister(WF_HOST_INTR_REG, WF_HOST_INT_MASK_RAW_1_INT_0);
        res = true;
    }

    return res;
}


/*****************************************************************************
  Function:
    uint16_t ScratchMount(void)

  Summary:
    Mounts RAW scratch window

  Description:
    This is called during the reset process to mount Scratch to RAW 4.  The Scratch
    memory is being used for mgmt tx and rx messages.  Once mounted it is never
    unmounted.

  Precondition:
    WiFi interrupts are not yet initialized

  Parameters:
    None

  Returns:
    Size, in bytes, of Scratch buffer

  Remarks:
    Not using the raw move complete interrupt to determine when the raw move has
    completed.  Instead we are polling the raw status register.  The scratch
    unmount is only done at init, and it simplifies the interrupt routine not
    to mix interrupts occurring at init vs interrupts occurring during normal operations.
*****************************************************************************/
static void ScratchMount(void)
{
    // command MRF to mount Scratch memory to RAW_4
    RawMove(RAW_SCRATCH_ID, RAW_SCRATCH_POOL, true, 0);
}

static bool isScratchMountComplete(uint16_t *p_byteCount)
{
    uint8_t  hostIntReg;
    uint16_t hostInt2 = 0;  // avoid warning
    uint8_t  regId;
    bool     res = false;
    
    // read hostint reg and check if level 2 (RAW4 complete is in level 2)
    hostIntReg = Read8BitWFRegister(WF_HOST_INTR_REG);

    // if level 2 event has occured
    if (hostIntReg & WF_HOST_INT_MASK_INT2)
    {
        // read hostint2
        hostInt2 = Read16BitWFRegister(WF_HOST_INTR2_REG);

        // if scratch mount to RAW 4 complete
        if ((hostInt2 & WF_HOST_INT_MASK_RAW_4_INT_0) == WF_HOST_INT_MASK_RAW_4_INT_0)
        {
            // read the byte count (or how many bytes are in Scratch memory)
            regId = g_RawCtrl1Reg[RAW_SCRATCH_ID];
            *p_byteCount = Read16BitWFRegister(regId);

            // clear host2 int bit
            Write16BitWFRegister(WF_HOST_INTR2_REG, hostInt2);
            res = true;
        }
    }

    return res;
}



#if 0

/*****************************************************************************
 * FUNCTION: RawGetIndex
 *
 * RETURNS: Returns the current RAW index for the specified RAW engine.
 *
 * PARAMS:
 *      rawId - RAW ID
 *
 *  NOTES: None
 *****************************************************************************/
uint16_t RawGetIndex(uint16_t rawId)
{
    uint8_t  regId;
    uint16_t index;

    regId = g_RawIndexReg[rawId];
    index = Read16BitWFRegister(regId);
    return index;
}

#endif

