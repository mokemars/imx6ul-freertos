//###########################################################################
// FILE:   ipc_lite.c
// TITLE:  M3 Driver for the IPC module (Lite version).
//         The IPC Lite functions only allow for basic inter-processor
//         communication functions such as data writes, reads, bit setting,
//         and bit setting.  The Lite functions do not require the usage of
//         the MSG RAM's or shared memories and can only be used with a
//         single IPC interrupt channel. Commands can only be processed
//         one at a time without queuing.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright:
// Copyright (C) 2011-2015 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

//*****************************************************************************
//! \addtogroup ipc_lite_api_m3 M3 IPC-Lite API Drivers
//! @{
//*****************************************************************************

#include "inc/hw_ipc.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/ipc.h"
#include "driverlib/debug.h"

//*****************************************************************************
//! Reads single word data result of MtoC IPC command
//!
//! \param pvData is a pointer to the 16/32-bit variable where the result data
//! will be stored.
//! \param usLength designates 16- or 32-bit read.
//! \param ulStatusFlag indicates the M to C Flag number mask used to report
//! the status of the command sent back from the control  system. If a
//! status flag was not used with the command call, set this parameter to 0.
//!
//! Allows the caller to read the 16/32-bit data result of non-blocking IPC
//! functions from the MTOCIPCDATAR register if the status flag is cleared
//! indicating the IPC command was successfully interpreted. If the status
//! flag is not cleared, the command was not recognized, and the function
//! will return STATUS_FAIL. To determine what data is read from a call to
//! this function, see the descriptions of the non-blocking IPC functions.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The  \e ulStatusFlag
//! parameter accepts any of the flag values \b IPC_FLAG1 - \b IPC_FLAG32
//! and \b NO_FLAG. The function returns \b STATUS_PASS or \b STATUS_FAIL.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCLiteMtoCGetResult (void *pvData, unsigned short usLength,
                      unsigned long ulStatusFlag)
{
    unsigned short returnStatus;
    
    // If control system never acknowledged Status Task, indicates command
    // failure.
    if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & ulStatusFlag)
    {
        returnStatus = STATUS_FAIL;
    }
    else
    {
        // Read data.
        if (usLength == IPC_LENGTH_16_BITS)
        {
            *(unsigned short *)pvData = HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAR);
        }
        else if (usLength == IPC_LENGTH_32_BITS)
        {
            *(unsigned long *)pvData =  HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAR);
        }
        
        returnStatus = STATUS_PASS;
    }
    
    return returnStatus;
}

//*****************************************************************************
//! Reads either a 16- or 32-bit data word from an C28 control system address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the C28 address to read from
//! \param usLength designates 16- or 32-bit read (1 = 16-bit, 2 = 32-bit)
//! \param ulStatusFlag indicates the M to C Flag number mask used to report
//! the status of the command sent back from the control  system.
//!
//! This function will allow the M3 master system to read 16/32-bit data from
//! the C28 control system into the MTOCIPCDATAR register. After calling this
//! function, a call to \e IPCLiteMtoCGetResult() will read the data value in
//! the MTOCIPCDATAR register into a 16- or 32-bit variable in the M3
//! application. The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32. The \e
//! ulStatusFlag parameter accepts any other one of the flag values \b
//! IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//! The function returns \b STATUS_PASS if the command is successful or \b
//! STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCLiteMtoCDataRead(unsigned long ulFlag, unsigned long ulAddress,
                    unsigned short usLength, unsigned long ulStatusFlag)
{
    unsigned short returnStatus;
    // Return false if IPC M to C request or status flags are not available.
    if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & (ulFlag | ulStatusFlag))
    {
        returnStatus = STATUS_FAIL;
    }
    else
    {
        // Set up read command, address, and word length.
        if (usLength == IPC_LENGTH_16_BITS)
        {
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_DATA_READ_16;
        }
        else if (usLength == IPC_LENGTH_32_BITS)
        {
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_DATA_READ_32;
        }
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;

        // Force IPC event on selected request task and enable status-checking.
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulFlag | ulStatusFlag);

        returnStatus = STATUS_PASS;    
    }
    
    return returnStatus;
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit data word at an C28 control system
// address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the C28 address to write to.
//! \param ulMask specifies the 16/32-bit mask for bits which should be set at
//! C28 ulAddress. (For 16-bit mask, only the lower 16-bits of ulMask are
//! considered.
//! \param usLength specifies the length of the \e ulMask (1 = 16-bit, 2 =
//! 32-bit).
//! \param ulStatusFlag indicates the M to C Flag number mask used to report
//! the status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to set bits specified by the
//! \e usMask variable in a 16/32-bit word on the C28 control system. The data
//! word at /e ulAddress after the set bits command is then read into the
//! MTOCIPCDATAR register. After calling this function, a call to \e
//! IPCLiteMtoCGetResult() will read the data value in the MTOCIPCDATAR
//! register into a 16/32-bit variable in the M3 application.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32. The \e
//! ulStatusFlag parameter accepts any other one of the flag values \b
//! IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//! The function returns \b STATUS_PASS if the command is successful or \b
//! STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCLiteMtoCSetBits(unsigned long ulFlag, unsigned long ulAddress,
                   unsigned long ulMask, unsigned short usLength,
                   unsigned long ulStatusFlag)
{
    unsigned short returnStatus;
    
    // Return false if IPC M to C request or status flags are not available.
    if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & (ulFlag | ulStatusFlag))
    {
        returnStatus = STATUS_FAIL;
    }
    else
    {
        if (usLength == IPC_LENGTH_16_BITS)
        {
            // Set up 16-bit set bits command, address, and mask.
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_SET_BITS_16;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulMask & (0x0000FFFF);
        }
        else if (usLength == IPC_LENGTH_32_BITS)
        {
            // Set up 32-bit set bits command, address, and mask.
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_SET_BITS_32;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulMask;
        }
        // Force IPC event on selected request task and enable status-checking.
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulFlag | ulStatusFlag);

        returnStatus = STATUS_PASS;    
    }
    
    return returnStatus;
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit write-protected data word at an
//! C28 control system address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the C28 write-protected address to write to.
//! \param ulMask specifies the 16/32-bit mask for bits which should be set at
//! C28 ulAddress. (For 16-bit mask, only the lower 16-bits of ulMask are
//! considered.
//! \param usLength specifies the length of the \e ulMask (1 = 16-bit, 2 =
//! 32-bit).
//! \param ulStatusFlag indicates the M to C Flag number mask used to report
//! the status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to set bits specified by the
//! \e usMask variable in a write-protected 16/32-bit word on the C28 control
//! system. The data word at /e ulAddress after the set bits command is then
//! read into the MTOCIPCDATAR register. After calling this function, a call
//! to \e IPCLiteMtoCGetResult() will read the data value in the
//! MTOCIPCDATAR register into a 16/32-bit variable in the M3 application.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32. The \e
//! ulStatusFlag parameter accepts any other one of the flag values \b
//! IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//! The function returns \b STATUS_PASS if the command is successful or \b
//! STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCLiteMtoCSetBits_Protected (unsigned long ulFlag, unsigned long ulAddress,
                              unsigned long ulMask, unsigned short usLength,
                              unsigned long ulStatusFlag)
{
    unsigned short returnStatus;
    
    // Return false if IPC M to C request or status flags are not available.
    if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & (ulFlag | ulStatusFlag))
    {
        returnStatus = STATUS_FAIL;
    }
    else
    {
        if (usLength == IPC_LENGTH_16_BITS)
        {
            // Set up 16-bit set bits command, address, and mask.
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_SET_BITS_16_PROTECTED;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulMask & (0x0000FFFF);
        }
        else if (usLength == IPC_LENGTH_32_BITS)
        {
            // Set up 32-bit set bits command, address, and mask.
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_SET_BITS_32_PROTECTED;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulMask;
        }

        // Force IPC event on selected request task and enable status-checking.
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulFlag | ulStatusFlag);

        returnStatus = STATUS_PASS;    
    }

    return returnStatus;
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit data word at an C28 control system
//! address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the C28 address to write to.
//! \param ulMask specifies the 16/32-bit mask for bits which should be set at
//! C28 ulAddress. (For 16-bit mask, only the lower 16-bits of ulMask are
//! considered.
//! \param usLength specifies the length of the \e ulMask (1 = 16-bit, 2 =
//! 32-bit).
//! \param ulStatusFlag indicates the M to C Flag number mask used to report
//! the status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to set bits specified by the
//! \e usMask variable in a 16/32-bit word on the C28 control system. The data
//! word at /e ulAddress after the set bits command is then read into the
//! MTOCIPCDATAR register. After calling this function, a call to \e
//! IPCLiteMtoCGetResult() will read the data value in the MTOCIPCDATAR
//! register into a 16/32-bit variable in the M3 application.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32. The \e
//! ulStatusFlag parameter accepts any other one of the flag values \b
//! IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//! The function returns \b STATUS_PASS if the command is successful or \b
//! STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCLiteMtoCClearBits(unsigned long ulFlag, unsigned long ulAddress,
                     unsigned long ulMask, unsigned short usLength,
                     unsigned long ulStatusFlag)
{
    unsigned short returnStatus;
    
    // Return false if IPC M to C request or status flags are not available.
    if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & (ulFlag | ulStatusFlag))
    {
        returnStatus = STATUS_FAIL;
    }
    else
    {
        if (usLength == IPC_LENGTH_16_BITS)
        {
            // Set up 16-bit set bits command, address, and mask.
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_CLEAR_BITS_16;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulMask & (0x0000FFFF);
        }
        else if (usLength == IPC_LENGTH_32_BITS)
        {
            // Set up 32-bit set bits command, address, and mask.
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_CLEAR_BITS_32;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulMask;
        }
        // Force IPC event on selected request task and enable status-checking.
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulFlag | ulStatusFlag);

        returnStatus = STATUS_PASS;    
    }

    return returnStatus;
}

//*****************************************************************************
//! Clears the designated bits in a 16/32-bit write-protected data word at an
//! C28 control system address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the C28 write-protected address to write to.
//! \param ulMask specifies the 16/32-bit mask for bits which should be cleared
//! at C28 ulAddress. (For 16-bit mask, only the lower 16-bits of ulMask are
//! considered.
//! \param usLength specifies the length of the \e ulMask (1 = 16-bit, 2 =
//! 32-bit).
//! \param ulStatusFlag indicates the M to C Flag number mask used to report
//! the status of the command sent back from the control system.
//!
//! This function will allow the M3 master system to clear bits specified by
//! the \e usMask variable in a write-protected 16/32-bit word on the C28
//! control system. The data word at /e ulAddress after the clear bits
//! command is then read into the MTOCIPCDATAR register. After calling this
//! function, a call to \e IPCLiteMtoCGetResult() will read the data value
//! in the MTOCIPCDATAR register into a 16/32-bit variable in the M3
//! application. The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32. The \e
//! ulStatusFlag parameter accepts any other one of the flag values \b
//! IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//! The function returns \b STATUS_PASS if the command is successful or \b
//! STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCLiteMtoCClearBits_Protected (unsigned long ulFlag, unsigned long ulAddress,
                                unsigned long ulMask, unsigned short usLength,
                                unsigned long ulStatusFlag)
{
    unsigned short returnStatus;
    
    // Return false if IPC M to C request or status flags are not available.
    if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & (ulFlag | ulStatusFlag))
    {
        returnStatus = STATUS_FAIL;
    }
    else
    {
        if (usLength == IPC_LENGTH_16_BITS)
        {
            // Set up 16-bit set bits command, address, and mask.
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_CLEAR_BITS_16_PROTECTED;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulMask & (0x0000FFFF);
        }
        else if (usLength == IPC_LENGTH_32_BITS)
        {
            // Set up 32-bit set bits command, address, and mask.
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_CLEAR_BITS_32_PROTECTED;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulMask;
        }

        // Force IPC event on selected request task and enable status-checking.
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulFlag | ulStatusFlag);

        returnStatus = STATUS_PASS;    
    }

    return returnStatus;
}

//*****************************************************************************
//! Writes a 16/32-bit data word to an C28 control system address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the C28 address to write to
//! \param ulData specifies the 16/32-bit word which will be written. For
//! 16-bit words, only the lower 16-bits of ulData will be considered by the
//! control system.
//! \param usLength is the length of the word to write (0 = 16-bits, 1 =
//! 32-bits)
//! \param ulStatusFlag indicates the M to C Flag number mask used to report
//! the status of the command sent back from the control  system.
//!
//! This function will allow the M3 master system to write a 16/32-bit word via
//! the \e ulData variable to an address on the C28 control system.
//! The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32. The \e
//! ulStatusFlag parameter accepts any other one of the flag values \b
//! IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//! The function returns \b STATUS_PASS if the command is successful or \b
//! STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCLiteMtoCDataWrite(unsigned long ulFlag, unsigned long ulAddress,
                     unsigned long ulData, unsigned short usLength,
                     unsigned long ulStatusFlag)
{
    unsigned short returnStatus;
    
    // Return false if IPC M to C request or status flags are not available.
    if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & (ulFlag | ulStatusFlag))
    {
        returnStatus = STATUS_FAIL;
    }
    else
    {
        // Set up data write command, address, and data. For 16-bit write, control
        // system will
        // look at lower 16-bits only.
        if (usLength == IPC_LENGTH_16_BITS)
        {
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_DATA_WRITE_16;
        }
        else
        {
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_DATA_WRITE_32;
        }
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulData;

        // Force IPC event on selected request task and enable status-checking
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulFlag | ulStatusFlag);

        returnStatus = STATUS_PASS;    
    }

    return returnStatus;
}

//*****************************************************************************
//! Writes a 16/32-bit data word to a protected C28 control system address
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the C28 address to write to
//! \param ulData specifies the 16/32-bit word which will be written. For
//! 16-bit words, only the lower 16-bits of ulData will be considered by the
//! control system.
//! \param usLength is the length of the word to write (0 = 16-bits, 1 =
//! 32-bits)
//! \param ulStatusFlag indicates the M to C Flag number mask used to report
//! the status of the command sent back from the control  system.
//!
//! This function will allow the M3 master system to write a 16/32-bit word via
//! the \e ulData variable to a write-protected address on the C28 control
//! system. The \e usLength parameter accepts the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e ulFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32. The \e
//! ulStatusFlag parameter accepts any other one of the flag values \b
//! IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//! The function returns \b STATUS_PASS if the command is successful or \b
//! STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCLiteMtoCDataWrite_Protected(unsigned long ulFlag, unsigned long ulAddress,
                               unsigned long ulData, unsigned short usLength,
                               unsigned long ulStatusFlag)
{
    unsigned short returnStatus;
    
    // Return false if IPC M to C request or status flags are not available.
    if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & (ulFlag | ulStatusFlag))
    {
        returnStatus = STATUS_FAIL;
    }
    else
    {
        // Set up data write command, address, and data. For 16-bit write, control
        // system will
        // look at lower 16-bits only.
        if (usLength == IPC_LENGTH_16_BITS)
        {
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_DATA_WRITE_16_PROTECTED;
        }
        else
        {
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_DATA_WRITE_32_PROTECTED;
        }
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulData;

        // Force IPC event on selected request task and enable status-checking
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulFlag | ulStatusFlag);

        returnStatus = STATUS_PASS;    
    }

    return returnStatus;
}

//*****************************************************************************
//! Calls a C28 function with 1 optional parameter and an optional return
//! value.
//!
//! \param ulFlag specifies M to C IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulAddress specifies the C28 function address
//! \param ulParam specifies the 32-bit optional parameter value
//! \param ulStatusFlag indicates the M to C Flag number mask used to report
//! the status of the command sent back from the control  system.
//!
//! This function will allow the M3 master system to call a function on the
//! C28. the \e ulParam variable is a single optional 32-bit parameter to
//! pass to the function. The \e ulFlag parameter accepts any one of the
//! flag values \b IPC_FLAG1 - \b IPC_FLAG32. The \e ulStatusFlag parameter
//! accepts any other one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32
//! and \b NO_FLAG.
//! The function returns \b STATUS_PASS if the command is successful or \b
//! STATUS_FAIL if the request or status flags are unavailable.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCLiteMtoCFunctionCall(unsigned long ulFlag, unsigned long ulAddress,
                        unsigned long ulParam, unsigned long ulStatusFlag)
{
    unsigned short returnStatus;
    
    // Return false if IPC M to C request or status flags are not available.
    if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & (ulFlag | ulStatusFlag))
    {
        returnStatus = STATUS_FAIL;
    }
    else
    {
        // Set up function call command, address, and parameter.
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_FUNC_CALL;
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) = ulParam;

        // Force IPC event on selected request task and enable status-checking
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulFlag | ulStatusFlag);

        returnStatus = STATUS_PASS;    
    }

    return returnStatus;
}

//*****************************************************************************
//! Requests C28 boot ROM to branch to a specific address.
//!
//! \param ulAddress specifies the C28 address the boot ROM should branch to.
//!
//! This function will allow the M3 master system to request the C28 to exit
//! the boot ROM and subsequently branch to \e ulAddress.  IPC_FLAG1 and
//! IPC_FLAG32 are used for communication with the C28 boot ROM during the C28
//! boot process.
//! \note This function should only be called during the C28 boot process
//! before the C28 has started running application code.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCLiteMtoCBootBranch(unsigned long ulAddress)
{
    unsigned short returnStatus;
    
    // Return false if IPC M to C request or status flags are not available.
    if (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & (IPC_FLAG1 | IPC_FLAG32))
    {
        returnStatus = STATUS_FAIL;
    }
    else
    {
        // Set up function call command, address, and parameter.
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_BRANCH;
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) = ulAddress;

        // Force IPC event on selected request task and enable status-checking
        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (IPC_FLAG1 | IPC_FLAG32);

        returnStatus = STATUS_PASS;    
    }

    return returnStatus;
}

//*****************************************************************************
//! Reads either a 16- or 32-bit data word from an M3 master system address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the C to M Flag number mask used to report
//! the status of the command sent back from the master  system.
//!
//! This function will allow the C28 control system to read 16/32-bit data from
//! the M3 master system. The \e ulFlag parameter accepts any one of the
//! flag values \b IPC_FLAG1 - \b IPC_FLAG32, and the \e ulStatusFlag parameter
//! accepts any other one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and
//! \b NO_FLAG.
//*****************************************************************************
void
IPCLiteCtoMDataRead(unsigned long ulFlag, unsigned long ulStatusFlag)
{

    unsigned long* pulRAddress;
    unsigned short* pusRAddress;

    // Wait until IPC C to M request task is flagged
    while (!(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & ulFlag))
    {
    }

    // If the command and data length are correct for this function:
    // Then read from requested address and write 16/32-bit data
    // to CTOMIPCDATAR. Acknowledge the status flag
    // and the task flag.
    if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_DATA_READ_16)
    {
        // Perform 16-bit read
        pusRAddress = (unsigned short *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = (unsigned long)(*pusRAddress);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }
    else if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_DATA_READ_32)
    {
        pulRAddress = (unsigned long *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = *pulRAddress;
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to C28 there was
    // an error)
    else
    {
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulFlag);
    }
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit data word at an M3 master system
//! address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the C to M Flag number mask used to report
//! the status of the command sent back from the master system.
//!
//! This function will allow the C28 control system to set bits specified by a
//! mask variable in a 16/32-bit word on the M3 master system, and then read
//! back the word into the CTOMIPCDATAR register. The \e ulFlag parameter
//! accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32, and the
//! \e ulStatusFlag parameter accepts any other one of the flag values \b
//! IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************
void
IPCLiteCtoMSetBits(unsigned long ulFlag, unsigned long ulStatusFlag)
{

    unsigned short* pusAddress;
    unsigned long* pulAddress;

    // Wait until IPC C to M request task is flagged
    while (!(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then set the mask bits at the requested address
    // and write back the 16/32-bit data to CTOMIPCDATAR.
    // Acknowledge the status flag and the task flag.
    if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_SET_BITS_16)
    {
        pusAddress = (unsigned short *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pusAddress |= (unsigned short)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = (unsigned long)*pusAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }
    else if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_SET_BITS_32)
    {
        pulAddress = (unsigned long *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pulAddress |= (unsigned long)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = *pulAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to C28 there was
    // an error)
    else
    {
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulFlag);
    }
}

//*****************************************************************************
//! Sets the designated bits in a 16-bit data word at an M3 master system
//! write-protected address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the C to M Flag number mask used to report
//! the status of the command sent back from the master system.
//!
//! This function will allow the C28 control system to set bits specified by a
//! mask variable in a write-protected 16/32-bit word on the M3 master system,
//! and then read back the word into the CTOMIPCDATAR register. The \e ulFlag
//! parameter accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32,
//! and the \e ulStatusFlag parameter accepts any other one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************
void
IPCLiteCtoMSetBits_Protected (unsigned long ulFlag, unsigned long ulStatusFlag)
{

    unsigned short* pusAddress;
    unsigned long* pulAddress;
    unsigned long ulWRALLOWtemp;

    // Wait until IPC C to M request task is flagged
    while (!(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then enable write access with MWRALLOW and
    // set the mask bits at the requested address.
    // Write back the 16-bit data to CTOMIPCDATAR.
    // Restore the status of the MWRALLOW register.
    // Acknowledge the status flag and the task flag.
    ulWRALLOWtemp = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_SET_BITS_16_PROTECTED)
    {
        pusAddress = (unsigned short *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pusAddress |= (unsigned short)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = (unsigned long)*pusAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }
    else if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) ==
             IPC_SET_BITS_32_PROTECTED)
    {
        pulAddress = (unsigned long *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pulAddress |= (unsigned long)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = *pulAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to C28 there was
    // an error)
    else
    {
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulFlag);
    }

    // Restore status of MWRALLOW register.
    HWREG(SYSCTL_MWRALLOW) = ulWRALLOWtemp;
}

//*****************************************************************************
//! Clears the designated bits in a 16/32-bit data word at an M3 master system
//! address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the C to M Flag number mask used to report
//! the status of the command sent back from the master system.
//!
//! This function will allow the C28 control system to clear bits specified by
//! a mask variable in a 16/32-bit word on the M3 master system, and then
//! read back the word into the CTOMIPCDATAR register. The \e ulFlag
//! parameter accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32,
//! and the \e ulStatusFlag parameter accepts any other one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************
void
IPCLiteCtoMClearBits(unsigned long ulFlag, unsigned long ulStatusFlag)
{

    unsigned short* pusAddress;
    unsigned long* pulAddress;

    // Wait until IPC C to M request task is flagged
    while (!(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then clear the mask bits at the requested address
    // and write back the 16/32-bit data to CTOMIPCDATAR.
    // Acknowledge the status flag and the task flag.
    if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_CLEAR_BITS_16)
    {
        pusAddress = (unsigned short *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pusAddress &=
            ~((unsigned short)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW));
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = (unsigned long)*pusAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }
    else if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_CLEAR_BITS_32)
    {
        pulAddress = (unsigned long *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pulAddress &= ~((unsigned long)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW));
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = *pulAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to C28 there was
    // an error)
    else
    {
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulFlag);
    }
}

//*****************************************************************************
//! Clears the designated bits in a 16/32-bit data word at an M3 master system
//! write-protected address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the C to M Flag number mask used to report
//! the status of the command sent back from the master system.
//!
//! This function will allow the C28 control system to clear bits specified by
//! a mask variable in a 16/32-bit word on the M3 master system, and then
//! read back the word into the CTOMIPCDATAR register. The \e ulFlag
//! parameter accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32,
//! and the \e ulStatusFlag parameter accepts any other one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************
void
IPCLiteCtoMClearBits_Protected (unsigned long ulFlag,
                                unsigned long ulStatusFlag)
{

    unsigned short* pusAddress;
    unsigned long* pulAddress;
    unsigned long ulWRALLOWtemp;

    // Wait until IPC C to M request task is flagged
    while (!(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then enable write access with MWRALLOW and
    // clear the mask bits at the requested address.
    // Write back the 16/32-bit data to CTOMIPCDATAR.
    // Restore the status of the MWRALLOW register.
    // Acknowledge the status flag and the task flag.
    ulWRALLOWtemp = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_CLEAR_BITS_16_PROTECTED)
    {
        pusAddress = (unsigned short *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pusAddress &=
            ~((unsigned short)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW));
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = (unsigned long)*pusAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }
    else if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) ==
             IPC_CLEAR_BITS_32_PROTECTED)
    {
        pulAddress = (unsigned long *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pulAddress &= ~((unsigned long)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW));
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = (unsigned long)*pulAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to C28 there was
    // an error)
    else
    {
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulFlag);
    }

    // Restore status of MWRALLOW register.
    HWREG(SYSCTL_MWRALLOW) = ulWRALLOWtemp;
}

//*****************************************************************************
//! Writes a 16/32-bit data word to an M3 master system address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the C to M Flag number mask used to report
//! the status of the command sent back from the master system.
//!
//! This function will allow the C28 control system to write a 16/32-bit word
//!  to an address on the M3 master system. The \e ulFlag
//! parameter accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32,
//! and the \e ulStatusFlag parameter accepts any other one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************
void
IPCLiteCtoMDataWrite(unsigned long ulFlag, unsigned long ulStatusFlag)
{
    unsigned long* pulAddress;
    unsigned short* pusAddress;

    // Wait until IPC C to M request task is flagged
    while (!(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then write the 16/32-bit data to the requested address
    // and write back the 16/32-bit data to CTOMIPCDATAR.
    // Acknowledge the status flag and the task flag.

    if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_DATA_WRITE_16)
    {
        pusAddress = (unsigned short *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pusAddress = (unsigned short)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = (unsigned long)*pusAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }
    else if ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_DATA_WRITE_32))
    {
        pulAddress = (unsigned long *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pulAddress = HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = *pulAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);

    }
    // Otherwise, only acknowledge the task flag. (Indicates to C28 there was
    // an error)
    else
    {
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulFlag);
    }
}

//*****************************************************************************
//! Writes a 16/32-bit data word to a write-protected M3 master system address
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the C to M Flag number mask used to report
//! the status of the command sent back from the master system.
//!
//! This function will allow the C28 control system to write a 16/32-bit word
//! to an address on the M3 master system. The \e ulFlag
//! parameter accepts any one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32,
//! and the \e ulStatusFlag parameter accepts any other one of the flag values
//! \b IPC_FLAG1 - \b IPC_FLAG32 and \b NO_FLAG.
//*****************************************************************************

void
IPCLiteCtoMDataWrite_Protected(unsigned long ulFlag, unsigned long ulStatusFlag)
{
    unsigned long* pulAddress;
    unsigned short* pusAddress;
    unsigned long ulWRALLOWtemp;

    // Wait until IPC C to M request task is flagged
    while (!(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then enable write access with MWRALLOW and
    // write the 16/32-bit data to the requested address
    // and write back the 16/32-bit data to CTOMIPCDATAR.
    // Acknowledge the status flag and the task flag.
    ulWRALLOWtemp = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_DATA_WRITE_16_PROTECTED)
    {
        pusAddress = (unsigned short *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pusAddress = (unsigned short)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = (unsigned long)*pusAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }
    else if ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) ==
              IPC_DATA_WRITE_32_PROTECTED))
    {
        pulAddress = (unsigned long *)HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        *pulAddress = HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW);
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAR) = *pulAddress;

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);

    }
    // Otherwise, only acknowledge the task flag. (Indicates to C28 there was
    // an error)
    else
    {
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulFlag);
    }

    // Restore the status of MWRALLOW register.
    HWREG(SYSCTL_MWRALLOW) = ulWRALLOWtemp;
}

//*****************************************************************************
//! Calls an M3 function with a single optional parameter and return value.
//!
//! \param ulFlag specifies C to M IPC Flag number mask used to indicate a
//! command is being sent.
//! \param ulStatusFlag indicates the C to M Flag number mask used to report
//! the status of the command sent back from the control system.
//!
//! This function will allow the C28 control system to call an M3 function with
//! a single optional parameter and places an optional return value in the
//! CTOMIPCDATAR register. The \e ulFlag parameter accepts any one of the flag
//! values \b IPC_FLAG1 - \b IPC_FLAG32, and  the \e ulStatusFlag parameter
//! accepts any other one of the flag values \b IPC_FLAG1 - \b IPC_FLAG32 and
//! \b NO_FLAG.
//*****************************************************************************
void
IPCLiteCtoMFunctionCall(unsigned long ulFlag, unsigned long ulStatusFlag)
{
    // Wait until IPC C to M request task is flagged
    while (!(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & ulFlag))
    {
    }

    // If the command is correct for this function:
    // Then call function at requested address
    // and if there is a return value, insert into
    // MTOCIPCDATAR register.
    // Acknowledge the status flag and the task flag.
    if (HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCCOM) == IPC_FUNC_CALL)
    {
        tfIpcFuncCall func_call = (tfIpcFuncCall)HWREG(
            MTOCIPC_BASE + IPC_O_CTOMIPCADDR);
        HWREG(MTOCIPC_BASE +
              IPC_O_CTOMIPCDATAR) =
            func_call(HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCDATAW));

        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulStatusFlag | ulFlag);
    }

    // Otherwise, only acknowledge the task flag. (Indicates to M3 there was an
    // error)
    else
    {
        HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (ulFlag);
    }
}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************




