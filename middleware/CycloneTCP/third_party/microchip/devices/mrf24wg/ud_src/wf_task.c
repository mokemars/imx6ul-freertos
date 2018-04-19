/*******************************************************************************
 MRF24WG run-time task

  Summary: Called from round-robin to check for WiFi events

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

#if defined(WF_USE_CONSOLE)
#include "wf_console.h"
#endif


//==============================================================================
//                                  MACROS
//==============================================================================
#define SetWfState(state)  g_wfState = state
#define GetWfState()       g_wfState

//==============================================================================
//                                  LOCAL FUNCTION PROTOTYPES
//==============================================================================

static void EventCheck(void);
#if defined (WF_USE_PSPOLL)
static void PsPollCheck(void);
#endif // WF_USE_PSPOLL

enum
{
    WF_STARTUP_STATE            = 0,
    WF_WAIT_FOR_INIT_COMPLETE   = 1,
    WF_INIT_COMPLETE            = 2,
    WF_WAIT_FOR_CONNECTION      = 3,
    WF_OPERATIONAL              = 4,
    
    WF_INACTIVE_STATE           = 255

} t_wfStates;

//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
static uint8_t g_wfState = WF_INACTIVE_STATE;
static bool    g_startConnect = false;

//==============================================================================
//                                  GLOBAL FUNCTION PROTOTYPES
//==============================================================================
static void  RxPacketCheck(void);


/*****************************************************************************
  Function:
    void WF_Init(void)

  Summary:
    Signals the Univesal Driver to initialize itself for operations.

  Description:
    This function does not perform the actual initialization -- it simply sets a
    flag so WF_Task(), when called in the main loop, will begin cycling through
    the necessary state machines to perform the initialization.
 
  Parameters:
    None

  Returns:
    None

  Remarks:
    None
*****************************************************************************/
void WF_Init(void)
{
#if defined(WF_USE_DEBUG_OUTPUT)
    WF_PRINTF("\r\nWF_Init() called\r\n");
#endif

#if defined(WF_USE_CONSOLE)
    ConsoleQInit();
#endif

    SetWfState(WF_STARTUP_STATE);
    g_startConnect = false;
}

/*****************************************************************************
  Function:
    void WF_Task(void)

  Summary:
    Primary Wi-Fi task

  Description:
    Called from the superloop to perform all Wi-Fi operations.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
*****************************************************************************/
void WF_Task(void)
{
    uint16_t res;

    // check if mgmt event message has occurred
    MgmtRxTask();

    // check if any events have occurred
    EventCheck();

#if defined(WF_USE_CONSOLE)
    ConsoleStateMachine();
#endif

    switch ( GetWfState() )
    {
        //-----------------------
        case WF_STARTUP_STATE:
        //-----------------------
            ResetInitSm();
            SetWfState(WF_WAIT_FOR_INIT_COMPLETE);
            break;

        //-----------------------
        case WF_WAIT_FOR_INIT_COMPLETE:
        //-----------------------
            res = InitSm();
            if (res == SM_SUCCESSFUL)
            {
                SetWfState(WF_INIT_COMPLETE);
            }
            else if (res == SM_FAIL)
            {
                SetWfState(WF_INACTIVE_STATE);
            }
            else
            {
                // still in progress
            }
            break;

        //-----------------------
        case WF_INIT_COMPLETE:
        //-----------------------
            // if WF_Connect called
            if (g_startConnect)
            {
                g_startConnect = false;
                ResetConnectionSm();
                SetWfState(WF_WAIT_FOR_CONNECTION);
            }
            break;

        //-----------------------
        case WF_WAIT_FOR_CONNECTION:
        //-----------------------
            res = ConnectionSm();
            if (res == SM_SUCCESSFUL)
            {
                SetWfState(WF_OPERATIONAL);
            }
            else if (res == SM_FAIL)
            {
                SetWfState(WF_INACTIVE_STATE);
            }
            else
            {
                // still in progress
            }
            break;

        //-----------------------
        case WF_OPERATIONAL:
        //-----------------------
            WiFi_DataTxTask();      // run data rx task
            WiFi_DataRxTask();      // run data tx task
            RxPacketCheck();        // check if Rx packet needs to be given to stack
#if defined(WF_USE_PSPOLL)
            PsPollCheck();
#endif // WF_USE_PSPOLL

            break;

        //-----------------------
        case WF_INACTIVE_STATE:
        //-----------------------

            break;
    }


}

/*****************************************************************************
  Function:
    void StartConnectStateMachine(void)

  Summary:
    Called by WF_Connect to start Wi-Fi connection state machine.

  Description:
    Simply sets a flag which will start the Wi-Fi connection machine running.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
*****************************************************************************/
void StartConnectStateMachine(void)
{
    g_startConnect = true;
}


/*****************************************************************************
  Function:
    bool isConnectionOperational(void)

  Summary:
    Determines if connection is operational (even if trying to regain connection)

  Description:
    Primary use is prevent a scan after a SoftAP connection, which is not allowed.

  Parameters:
    None

  Returns:
    True if connection operational, else False

  Remarks:
    None
*****************************************************************************/
bool isConnectionOperational(void)
{
    return (GetWfState() == WF_OPERATIONAL);
}

/*****************************************************************************
  Function:
    void NotifyConnectionLost(void)

  Summary:
    Called when connection lost and Wi-Fi state machine must run again.

  Description:
    See Summary

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
*****************************************************************************/
void NotifyConnectionLost(void)
{
    // go back to init complete state and wait for WF_Connect() to be called again
    SetWfState(WF_INIT_COMPLETE);
}


/*****************************************************************************
  Function:
    static void  RxPacketCheck(void)

  Summary:
    Called from WF_Task to check if an Rx data packet is ready for the TCP/IP stack.

  Description:
    See Summary

  Parameters:
    None

  Returns:
    True if Rx data packet ready for stack, else False

  Remarks:
    None
*****************************************************************************/
static void  RxPacketCheck(void)
{
    // if rx packet received
    if (isPacketRx())
    {
        // clear the flag
        ClearPacketRx();
        // notify the stack via callback
        WF_RxPacketReady();
    }
}

/*****************************************************************************
  Function:
    static void EventCheck(void)

  Summary:
    Called from WF_Task to check if an event (connection, disconnection, scan results, etc.)
    has occurred.

  Description:
    If a Wi-Fi event has occurred than it is processed in this function.

  Parameters:
    None

  Returns:
    True if Rx data packet ready for stack, else False

  Remarks:
    None
*****************************************************************************/
static void EventCheck(void)
{
    t_event event;

    // if event queue empty then nothing to do
    if ( isEventQEmpty() )
    {
        return;
    }

    // get next event from queue
    EventDequeue(&event);

    // notify application of event
    WF_ProcessEvent(event.eventType, event.eventData);
}

#if defined (WF_USE_PSPOLL)
/*****************************************************************************
  Function:
    static void PsPollCheck(void)

  Summary:
    Called from WF_Task to check if PS-Poll actions are needed.

  Description:
    See Summary.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None
*****************************************************************************/
static void PsPollCheck(void)
{
    // if PS-Poll was disabled temporarily and needs to be reenabled, and, we are in
    // a connected state
    if ((isPsPollNeedReactivate()) && (UdGetConnectionState() == CS_CONNECTED))
    {
        ClearPsPollReactivate();
        ConfigureLowPowerMode(WF_LOW_POWER_MODE_ON);
    }
}
#endif // WF_USE_PSPOLL

