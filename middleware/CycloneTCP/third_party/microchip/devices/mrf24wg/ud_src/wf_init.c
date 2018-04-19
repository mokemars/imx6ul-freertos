/*******************************************************************************
 MRF24WG Initialization

  Summary: Contains functions pertaining Universal Driver and MRF24WG
           initialization.

  Description: Same as summary.
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
//                                  CONSTANTS
//==============================================================================
// MRF24WG Init state machine states
enum
{
    INIT_START_STATE                   = 0,     // reset state for UD state machine
    MCU_HARDWARE_INIT                  = 1,
    TOGGLE_INTO_HIBERNATE              = 2,
    TOGGLE_OUT_OF_HIBERNATE            = 3,
    TOGGLE_INTO_RESET                  = 4,
    TOGGLE_OUT_OF_RESET                = 5,
    RESET_PLL                          = 6,
    MRF24WG_RESET                      = 7,
    MRF24WG_WAIT_FOR_RAW_INIT          = 10,
    ENABLE_MRF_MODE                    = 11,
    GET_DEVICE_INFO                    = 12,
    DISABLE_TX_DATA_CONFIRM            = 13,
    CREATE_CP                          = 14,
    MAX_FACTORY_POWER                  = 15,
    GET_MAC_ADDRESS                    = 16,
    GET_DOMAIN                         = 17,
    ENABLE_MULTICAST                   = 18,
    MRF24WG_INIT_SUCCESSFUL            = 19,
    MRF24WG_INIT_FAILED                = 20,

    INIT_SM_INACTIVE                   = 255    // inactive, state machine not running
};

// MRF24WG chip reset state machine states
enum
{
    CHIP_RESET_START                = 1,
    CHIP_RESET_WAIT_FOR_HW_RESET    = 2,
    CHIP_RESET_WAIT_FOR_MRF24W_INIT = 3,
    CHIP_RESET_FAILED               = 4,
    CHIP_RESET_SUCCESSFUL           = 5
};

enum
{
    WF_DISABLED = 0,
    WF_ENABLED  = 1
};


enum
{
    INT_DISABLE = 0,
    INT_ENABLE  = 1
};

#define REQUIRED_ROM_VERSION    0x31
#define MIN_PATCH_VERSION       0x08

//==============================================================================
//                                  LOCAL MACROS
//==============================================================================
#define SetInitState(state)   g_initState = state
#define GetInitState()        g_initState

#define SetChipResetState(state)    g_chipResetState = state
#define GetChipResetState()         g_chipResetState

//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
static uint8_t g_initState = INIT_SM_INACTIVE;  // WiFi init state machine not running
static uint8_t g_chipResetState;

//==============================================================================
//                                  LOCAL FUNCTION PROTOTYPES
//==============================================================================
static void     Init_Interrupts(void);
static void     HostInterrupt2RegInit(uint16_t hostIntMaskRegMask, uint8_t  state);
static void     HostInterruptRegInit(uint8_t hostIntrMaskRegMask, uint8_t state);
static uint8_t  ChipResetSm(void);

// called by WF_Task to reset the init state machine
void ResetInitSm(void)
{
    SetInitState(INIT_START_STATE);
}

bool isInitComplete(void)
{
    return (GetInitState() == MRF24WG_INIT_SUCCESSFUL);
}

/*****************************************************************************
  Function:
    uint8_t InitSm(void)

  Summary:
    State machine to complete the MRF24WG initialization sequence.

  Description:
    Called from WF_Task().  Cycles thru the states needed to perform the MRF24WG
    module reset and initialization.  There are two sub-state machines called by
    by this state machine -- the chip reset and raw init state machines.

 Parameters:
    None

  Returns:
    SM_IN_PROGRESS, SM_SUCCESSFUL, or SM_FAIL

  Remarks:
    None
*****************************************************************************/
uint8_t InitSm(void)
{
   uint8_t       res = SM_IN_PROGRESS;
   uint8_t       chipResetRes;  // return status of chip reset state machine
   uint8_t       rawSmStatus;   // return status of raw init state machine
   t_deviceInfo  deviceInfo;
   uint8_t       macAddress[WF_MAC_ADDRESS_LENGTH];
   
    switch (g_initState)
    {
        //--------------------
        case INIT_START_STATE:
        //--------------------
            UdStateInit();              // initialize internal state machine
            EventQInit();               // initialize WiFi event queue
            InitDataTxStateMachine();   // initialize data tx state machine
            InitDataRxStateMachine();   // initialize data rx state machine
            SetInitState(MCU_HARDWARE_INIT);
            break;

        //--------------------
        case MCU_HARDWARE_INIT:
        //--------------------
            WF_TimerInit();     // initialize and start the 1ms timer
            WF_SpiInit();       // initialize the SPI interface
            WF_GpioInit();      // initialize HIBERNATE and RESET I/O lines
            SetInitState(TOGGLE_INTO_HIBERNATE);
            break;

        //--------------------
        case TOGGLE_INTO_HIBERNATE:
        //--------------------
            WF_GpioSetHibernate(WF_HIGH);  // Put module into hibernate mode
            SetInitState(TOGGLE_OUT_OF_HIBERNATE);
            break;

        //--------------------
        case TOGGLE_OUT_OF_HIBERNATE:
        //--------------------
            WF_GpioSetHibernate(WF_LOW);  // Take module out of hibernate mode
            SetInitState(TOGGLE_INTO_RESET);
            break;

        //--------------------
        case TOGGLE_INTO_RESET:
        //--------------------
            WF_GpioSetReset(WF_LOW);       // Put module into reset state
            SetInitState(TOGGLE_OUT_OF_RESET);
            break;

        //--------------------
        case TOGGLE_OUT_OF_RESET:
        //--------------------
            WF_GpioSetReset(WF_HIGH);      // Take module out of reset
            SetInitState(RESET_PLL);
            break;

        //--------------------
        case RESET_PLL:
        //--------------------
            ResetPll();
            SetChipResetState(CHIP_RESET_START); // reset chip reset state machine which is about to run
            SetInitState(MRF24WG_RESET);
            break;

        //--------------------
        case MRF24WG_RESET:
        //--------------------
            // cycle thru reset state machine until done
            chipResetRes = ChipResetSm();
            if (chipResetRes == SM_SUCCESSFUL)
            {
                // reset the RAW Init state machine,which is about to run
                ResetRawInitSm();
                SetInitState(MRF24WG_WAIT_FOR_RAW_INIT);
            }
            else if (chipResetRes == SM_FAIL)
            {
                SetInitState(MRF24WG_INIT_FAILED);
            }
            else
            {
                // chip reset state machine still in progress
            }
            break;

        //------------------------
        case MRF24WG_WAIT_FOR_RAW_INIT:
        //------------------------
            // cycle thru Raw Init state machine until done
            rawSmStatus = RawInitSm();

            // if Raw Init state machine completed successfully
            if (rawSmStatus == SM_SUCCESSFUL)
            {
                // get start index in Scratch where mgmt message block is located
                InitMgmtBase();   

                // Initialize the MCU External Interrupt allowing the MRF24W to interrupt
                // the Host from this point forward.  The MRF interrupt must not be
                // initialized or enabled until after Raw Init state machine completed.
                WF_EintInit();
                WF_EintEnable();

                SetInitState(ENABLE_MRF_MODE);
            }
            // else if Raw Init state machine failed
            else if (rawSmStatus == SM_FAIL)
            {
                EventEnqueue(WF_EVENT_INITIALIZATION, WF_INIT_RAW_INIT_FAIL);
                SetInitState(MRF24WG_INIT_FAILED);
            }
            else
            {
                // state machine still in process (rawSmStatus == SM_IN_PROGRESS)
            }
            break;

        //------------------------
        case ENABLE_MRF_MODE:
        //------------------------
            EnableMRF24WB0MMode(); // legacy, but still needed
            SetInitState(GET_DEVICE_INFO);
            break;

        //------------------------
        case GET_DEVICE_INFO:
        //------------------------
            DeviceInfoGet(&deviceInfo);   // get MRF24WG module version numbers
            if (deviceInfo.deviceType == WF_UNKNOWN_DEVICE)
            {
                EventEnqueue(WF_EVENT_ERROR, UD_ERROR_UNKNOWN_DEVICE);
                SetInitState(MRF24WG_INIT_FAILED);
            }
            else if (deviceInfo.deviceType == WF_MRF24WB_DEVICE)
            {
                EventEnqueue(WF_EVENT_ERROR, UD_ERROR_MRF24WB_NOT_SUPPORTED);
                SetInitState(MRF24WG_INIT_FAILED);
            }
            else if (deviceInfo.romVersion != REQUIRED_ROM_VERSION)
            {
                EventEnqueue(WF_EVENT_ERROR, UD_ERROR_MISMATCH_ROM_VERSION);
                SetInitState(MRF24WG_INIT_FAILED);
            }
            else if (deviceInfo.patchVersion < MIN_PATCH_VERSION)
            {
                EventEnqueue(WF_EVENT_ERROR, UD_ERROR_MISMATCH_PATCH_VERSION);
                SetInitState(MRF24WG_INIT_FAILED);
            }
            else
            {
                SetInitState(DISABLE_TX_DATA_CONFIRM);
            }
            break;

        //------------------------
        case DISABLE_TX_DATA_CONFIRM:
        //------------------------
            SetTxDataConfirm(WF_DISABLED);     // Disable Tx Data confirms (from the MRF24W)
            SetInitState(CREATE_CP);
            break;

        //------------------------
        case CREATE_CP:
        //------------------------
            CreateCP();                        // create a connection profile, get its ID and store it
            SetInitState(MAX_FACTORY_POWER);
            break;

        //------------------------
        case MAX_FACTORY_POWER:
        //------------------------
            GetFactoryMaxTxPower();
            SetInitState(GET_MAC_ADDRESS);
            break;

        //------------------------
        case GET_MAC_ADDRESS:
        //------------------------
            GetMacAddress(macAddress);
            SetInitState(GET_DOMAIN);
            break;

        //------------------------
        case GET_DOMAIN:
        //------------------------
            GetDomain(); // read domain into global
            SetInitState(ENABLE_MULTICAST);
            break;

        //------------------------
        case ENABLE_MULTICAST:
        //------------------------
            EnableSWMulticastFilter();

            // initialization is complete
            EventEnqueue(WF_EVENT_INITIALIZATION, WF_INIT_SUCCESSFUL); // signal init successful event
            SetInitState(MRF24WG_INIT_SUCCESSFUL);
            break;

        //------------------------
        case MRF24WG_INIT_FAILED:
        //------------------------
            res = SM_FAIL;
            break;  // stay in this state until the next reset

        //----------------------------
        case MRF24WG_INIT_SUCCESSFUL:
        //----------------------------
            res = SM_SUCCESSFUL;
            break;  // stay in this state until the next reset

    } // end switch

    return res;
}

           

/*****************************************************************************
  Function:
    uint8_t ChipResetSm(void)

  Summary:
    State machine to complete the MRF24WG chip reset sequence

  Description:
    Called from InitSm().  Cycles thru the states needed to perform the MRF24WG
    module reset.

 Parameters:
    None

  Returns:
    SM_IN_PROGRESS, SM_SUCCESSFUL, or SM_FAIL

  Remarks:
    None
*****************************************************************************/
static uint8_t ChipResetSm(void)
{
    uint8_t res = SM_IN_PROGRESS;
    uint16_t         value;
    static uint32_t  startTime;
    uint32_t         elapsedTime;

    switch (GetChipResetState())
    {
        //--------------------
        case CHIP_RESET_START:
        //--------------------
            // clear the power bit to disable low power mode on the MRF24W
            Write16BitWFRegister(WF_PSPOLL_H_REG, 0x0000);

            // Set HOST_RESET bit in register to put device in reset
            Write16BitWFRegister(WF_HOST_RESET_REG, Read16BitWFRegister(WF_HOST_RESET_REG) | WF_HOST_RESET_MASK);

            // Clear HOST_RESET bit in register to take device out of reset
            Write16BitWFRegister(WF_HOST_RESET_REG, Read16BitWFRegister(WF_HOST_RESET_REG) & ~WF_HOST_RESET_MASK);

            startTime = WF_TimerRead();
            SetChipResetState(CHIP_RESET_WAIT_FOR_HW_RESET);
            break;

        //--------------------------------
        case CHIP_RESET_WAIT_FOR_HW_RESET:
        //--------------------------------
            Write16BitWFRegister(WF_INDEX_ADDR_REG, WF_HW_STATUS_REG);
            value = Read16BitWFRegister(WF_INDEX_DATA_REG);
            if ((value & WF_HW_STATUS_NOT_IN_RESET_MASK) == WF_HW_STATUS_NOT_IN_RESET_MASK)
            {
                if (value == 0xffff)
                {
                    // typically read all 1's when SPI not connected
                    EventEnqueue(WF_EVENT_INITIALIZATION, WF_INIT_ERROR_SPI_NOT_CONNECTED);
                    SetChipResetState(CHIP_RESET_FAILED);
                }
                else
                {
                    startTime = WF_TimerRead();
                    SetChipResetState(CHIP_RESET_WAIT_FOR_MRF24W_INIT);
                }
            }
            // else still waiting
            else
            {
                // if timed out waiting for MRF24WG to come out of reset
                elapsedTime = GetElapsedTime(startTime, WF_TimerRead());
                if (elapsedTime > 1000) // 1000
                {
                    EventEnqueue(WF_EVENT_INITIALIZATION, WF_INIT_ERROR_RESET_TIMEOUT);
                    SetChipResetState(CHIP_RESET_FAILED);
                }
            }
            break;

        //-----------------------------------
        case CHIP_RESET_WAIT_FOR_MRF24W_INIT:
        //-----------------------------------
            // read FIFO byte count; if > 0 than MRF24WG reset complete
            value = Read16BitWFRegister(WF_HOST_WFIFO_BCNT0_REG) & 0x0fff;;  // read FIFO byte count
            if (value > 0)
            {
                // Init MRF interrupts (but not MCU interrupt). During the Raw Init we will
                // be polling the raw move complete interrupt status, not generating the actual interrupt.
                Init_Interrupts();

                SetChipResetState(CHIP_RESET_SUCCESSFUL);
            }
            // else if timed out waiting for chip reset to complete
            else
            {
                // if timed out waiting for MRF24WG init
                elapsedTime = GetElapsedTime(startTime, WF_TimerRead());
                if (elapsedTime > 2000) // 2000ms
                {
                    // generate event
                    EventEnqueue(WF_EVENT_INITIALIZATION, WF_INIT_ERROR_INIT_TIMEOUT);
                    SetChipResetState(CHIP_RESET_FAILED);
                }
            }
            break;

        //-----------------------------------
        case CHIP_RESET_FAILED:
        //-----------------------------------
            res = SM_FAIL;

        //-----------------------------------
        case CHIP_RESET_SUCCESSFUL:
        //-----------------------------------
            res = SM_SUCCESSFUL;
    }

    return res;
}

/*****************************************************************************
  Function:
    void Init_Interrupts(void)

  Summary:
    Initializes interrupts generated by MRF24WG module

  Description:
    None

 Parameters:
    None

  Returns:
    None

  Remarks:
    The MCU interrupt is not yet enabled
*****************************************************************************/
static void Init_Interrupts(void)
{
    uint8_t  mask8;
    uint16_t mask16;

    // disable the interrupts gated by the 16-bit host int register
    HostInterrupt2RegInit(WF_HOST_2_INT_MASK_ALL_INT, (uint16_t)INT_DISABLE);

    // disable the interrupts gated the by main 8-bit host int register
    HostInterruptRegInit(WF_HOST_INT_MASK_ALL_INT, INT_DISABLE);

    // enable the following MRF24W interrupts in the INT1 8-bit register
    mask8 = (WF_HOST_INT_MASK_FIFO_0_THRESHOLD |     /* Data Rx Msg interrupt                  */
             WF_HOST_INT_MASK_RAW_0_INT_0      |     /* RAW0 Move Complete (Data Rx) interrupt */
             WF_HOST_INT_MASK_RAW_1_INT_0      |     /* RAW1 Move Complete (Data Tx) interrupt */
             WF_HOST_INT_MASK_INT2);                 /* Interrupt 2 interrupt                  */
    HostInterruptRegInit(mask8, INT_ENABLE);

    // enable the following MRF24W interrupts in the INT2 16-bit register
    mask16 = (WF_HOST_INT_MASK_RAW_4_INT_0     |    /* RAW4 Move Complete (Scratch) interrupt */
              WF_HOST_INT_MASK_MAIL_BOX_1_WRT);     /* used for mgmt msg signalling           */
    HostInterrupt2RegInit(mask16, INT_ENABLE);

}


/*****************************************************************************
 * FUNCTION: HostInterrupt2RegInit
 *
 * RETURNS: N/A
 *
 * PARAMS:
 *      hostIntrMaskRegMask - The bit mask to be modified.
 *      state               - One of INT_DISABLE, INT_ENABLE where
 *                             Disable implies clearing the bits and enable sets
 *                             the bits.
 *
 *
 *  NOTES: Initializes the 16-bit Host Interrupt register on the WiFi device with the
 *          specified mask value either setting or clearing the mask register as
 *          determined by the input parameter state.
 *****************************************************************************/
static void HostInterrupt2RegInit(uint16_t hostIntMaskRegMask, uint8_t  state)
{
    uint16_t int2MaskValue;

    // Host Int2 Register is a status register where each bit indicates a specific event
    // has occurred. In addition, writing a 1 to a bit location in this register clears
    // the event.

    // Host Int2 Mask Register is used to enable those events activated in Host Int Register
    // to cause an interrupt to the host

    // read current state of int2 mask reg
    int2MaskValue = Read16BitWFRegister(WF_HOST_INTR2_MASK_REG);

    // if caller is disabling a set of interrupts
    if (state == INT_DISABLE)
    {
        // zero out that set of interrupts in the interrupt mask copy
        int2MaskValue &= ~hostIntMaskRegMask;
    }
    // else caller is enabling a set of interrupts
    else
    {
        // set to 1 that set of interrupts in the interrupt mask copy
        int2MaskValue |= hostIntMaskRegMask;
    }

    // write out new interrupt mask value
    Write16BitWFRegister(WF_HOST_INTR2_MASK_REG, int2MaskValue);

    // ensure that pending interrupts from those updated interrupts are cleared
    Write16BitWFRegister(WF_HOST_INTR2_REG, hostIntMaskRegMask);

}

/*****************************************************************************
 * FUNCTION: HostInterruptRegInit
 *
 * RETURNS: N/A
 *
 * PARAMS:
 *      hostIntrMaskRegMask - The bit mask to be modified.
 *      state -  one of INT_DISABLE, INT_ENABLE where
 *                Disable implies clearing the bits and enable sets the bits.
 *
 *
 *  NOTES: Initializes the 8-bit Host Interrupt register on the MRF24W with the
 *      specified mask value either setting or clearing the mask register
 *      as determined by the input parameter state.  
 *****************************************************************************/
static void HostInterruptRegInit(uint8_t hostIntrMaskRegMask, uint8_t state)
{
    uint8_t hostIntMaskValue;

    // Host Int Register is a status register where each bit indicates a specific event
    // has occurred. In addition, writing a 1 to a bit location in this register clears
    // the event.

    // Host Int Mask Register is used to enable those events activated in Host Int Register
    // allowing interrupts to the host

    // read current state of Host Interrupt Mask register
    hostIntMaskValue = Read8BitWFRegister(WF_HOST_MASK_REG);

    // if caller is disabling a set of interrupts
    if (state == INT_DISABLE)
    {
        // zero out that set of interrupts in the interrupt mask copy
        hostIntMaskValue = (hostIntMaskValue & ~hostIntrMaskRegMask);
    }
    // else caller is enabling a set of interrupts
    else
    {
        // set to 1 that set of interrupts in the interrupt mask copy
        hostIntMaskValue = (hostIntMaskValue & ~hostIntrMaskRegMask) | hostIntrMaskRegMask;
    }

    // write out new interrupt mask value
    Write8BitWFRegister(WF_HOST_MASK_REG, hostIntMaskValue);

    // ensure that pending interrupts from those updated interrupts are cleared 
    Write8BitWFRegister(WF_HOST_INTR_REG, hostIntrMaskRegMask);
}
