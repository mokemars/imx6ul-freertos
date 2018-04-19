//###########################################################################
// FILE:   ipc.c
// TITLE:  M3 Driver for the IPC module.
//         The IPC functions require the usage of the CtoM and MtoC MSG
//         RAM's to store the circular ring buffer and indexes. Commands
//         can be queued up in order on a single IPC interrupt channel.
//         For those IPC commands which are not interdependent,
//         multiple IPC interrupt channels may be used.
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
//! \addtogroup ipc_api_m3 M3 IPC API Drivers
//! @{
//*****************************************************************************

#include "inc/hw_ipc.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/ipc.h"

// Map MtoC Put circular buffers to MTOC MSGRAM
#pragma DATA_SECTION(g_asIPCMtoCBuffers, "PUTBUFFER");
#pragma DATA_SECTION(g_usPutWriteIndexes, "PUTWRITEIDX");
#pragma DATA_SECTION(g_usGetReadIndexes, "GETREADIDX");

// Map CtoM Get circular buffers to CTOM MSGRAM
#pragma DATA_SECTION(g_asIPCCtoMBuffers, "GETBUFFER");
#pragma DATA_SECTION(g_usGetWriteIndexes, "GETWRITEIDX");
#pragma DATA_SECTION(g_usPutReadIndexes, "PUTREADIDX");

// Circular Buffer Definitions (Reserves beginning of each MSGRAM block)
tIpcMessage g_asIPCMtoCBuffers[NUM_IPC_INTERRUPTS][IPC_BUFFER_SIZE];
tIpcMessage g_asIPCCtoMBuffers[NUM_IPC_INTERRUPTS][IPC_BUFFER_SIZE];

// Circular Buffer Index Definitions (Reserves beginning of each MSGRAM block)
unsigned short g_usPutWriteIndexes[NUM_IPC_INTERRUPTS];
unsigned short g_usPutReadIndexes[NUM_IPC_INTERRUPTS];
unsigned short g_usGetWriteIndexes[NUM_IPC_INTERRUPTS];
unsigned short g_usGetReadIndexes[NUM_IPC_INTERRUPTS];

//*****************************************************************************
//! Initializes M3 master system IPC Driver controller
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param usMIpcInterrupt specifies the M3 IPC interrupt number used by
//! psController.
//! \param usCIpcInterrupt specifies the C28 IPC interrupt number used by
//! psController.
//!
//! This function initializes the IPC driver controller with circular buffer
//! and index addresses for an IPC interrupt pair (M3-C28).  The \e
//! usCIpcInterrupt and \e usMIpcInterrupt parameters can be 1 of the
//! following values: \b IPC_INT1, \b IPC_INT2, \b IPC_INT3, \b IPC_INT4.
//!
//! \note If an interrupt is currently in use by an \e tIpcController instance,
//! that particular interrupt should not be tied to a second \e
//! tIpcController instance.
//!
//! \note For a particular usMIpcInterrupt - usCIpcInterrupt pair, there must
//! be an instance of tIpcController defined and initialized on both the M3
//! and the C28 systems.
//!
//! \return None.
//*****************************************************************************
void
IPCMInitialize (volatile tIpcController *psController,
                unsigned short usMIpcInterrupt, unsigned short usCIpcInterrupt)
{
    // Check arguments
    ASSERT(usCIpcInterrupt <= NUM_IPC_INTERRUPTS);
    ASSERT(usMIpcInterrupt <= NUM_IPC_INTERRUPTS);

    // CtoMPutBuffer and Index Initialization
    psController->psPutBuffer = &g_asIPCMtoCBuffers[usCIpcInterrupt-1][0];
    psController->pusPutWriteIndex = &g_usPutWriteIndexes[usCIpcInterrupt-1];
    psController->pusPutReadIndex = &g_usPutReadIndexes[usCIpcInterrupt-1];
    psController->ulPutFlag = (unsigned long)(1 << (usCIpcInterrupt - 1));

    // MtoCGetBuffer and Index Initialization
    psController->psGetBuffer = &g_asIPCCtoMBuffers[usMIpcInterrupt-1][0];
    psController->pusGetWriteIndex = &g_usGetWriteIndexes[usMIpcInterrupt-1];
    psController->pusGetReadIndex = &g_usGetReadIndexes[usMIpcInterrupt-1];

    // Initialize PutBuffer WriteIndex = 0 and GetBuffer ReadIndex = 0
    *(psController->pusPutWriteIndex) = 0;
    *(psController->pusGetReadIndex) = 0;
}

//*****************************************************************************
//! Writes a message into the PutBuffer.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param psMessage specifies the address of the \e tIpcMessage instance to be
//! written to PutBuffer.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a free slot (1= wait until free spot available, 0 = exit with
//! STATUS_FAIL if no free slot).
//!
//! This function checks if there is a free slot in the PutBuffer. If so, it
//! puts the message pointed to by \e psMessage into the free slot and
//! increments the WriteIndex.
//! Then it sets the appropriate IPC interrupt flag specified by
//! \e psController->usPutFlag.  The \e bBlock parameter can be one of the
//! following values: \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return 1 if PutBuffer is full. 0 if Put occurs successfully.
//*****************************************************************************
unsigned short
IpcPut (volatile tIpcController *psController, tIpcMessage *psMessage,
        unsigned short bBlock)
{
    unsigned short writeIndex;
    unsigned short readIndex;
    unsigned short returnStatus = STATUS_PASS;

    writeIndex = *(psController->pusPutWriteIndex);
    readIndex = *(psController->pusPutReadIndex);

    // Wait until Put Buffer slot is free
    while (((writeIndex + 1) & MAX_BUFFER_INDEX) == readIndex)
    {
        // If designated as a "Blocking" function, and Put buffer is full,
        // return immediately with fail status.
        if (!bBlock)
        {
            returnStatus = STATUS_FAIL;
            break;
        }

        readIndex = *(psController->pusPutReadIndex);
    }

    if (returnStatus != STATUS_FAIL)
    {
        // When slot is free, Write Message to PutBuffer, update PutWriteIndex,
        // and set M3 IPC INT Flag
        psController->psPutBuffer[writeIndex] = *psMessage;

        writeIndex = (writeIndex + 1) & MAX_BUFFER_INDEX;
        *(psController->pusPutWriteIndex) = writeIndex;

        HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= psController->ulPutFlag;    
    }

    return returnStatus;
}

//*****************************************************************************
//! Reads a message from the GetBuffer.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param psMessage specifies the address of the \e tIpcMessage instance where
//! the message from GetBuffer should be written to.
//! \param bBlock specifies whether to allow function to block until GetBuffer
//! has a message (1= wait until message available, 0 = exit with
//! STATUS_FAIL if no message).
//!
//! This function checks if there is a message in the GetBuffer. If so, it get
//! the message in the GetBuffer pointed to by the ReadIndex and writes it
//! to the address pointed to by \e psMessage. The \e bBlock parameter can
//! be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//!
//! \return 1 if GetBuffer is empty. 0 if Get occurs successfully.
//*****************************************************************************
unsigned short
IpcGet (volatile tIpcController *psController, tIpcMessage *psMessage,
        unsigned short bBlock)
{
    unsigned short writeIndex;
    unsigned short readIndex;
    unsigned short returnStatus = STATUS_PASS;

    writeIndex = *(psController->pusGetWriteIndex);
    readIndex = *(psController->pusGetReadIndex);

    // Loop while GetBuffer is empty
    while (writeIndex == readIndex)
    {
        // If designated as a "Blocking" function, and Get buffer is empty,
        // return immediately with fail status.
        if (!bBlock)
        {
            returnStatus = STATUS_FAIL;
            break;
        }

        writeIndex = *(psController->pusGetWriteIndex);
    }

    if (returnStatus != STATUS_FAIL)
    {
        // If there is a message in GetBuffer, Read Message and update the
        // ReadIndex
        *psMessage = psController->psGetBuffer[readIndex];

        readIndex = (readIndex + 1) & MAX_BUFFER_INDEX;
        *(psController->pusGetReadIndex) = readIndex;
    }

    return returnStatus;
}

//*****************************************************************************
//! Sends the command to read either a 16- or 32-bit data word from a
//! C28 control system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the C28 address to read from
//! \param pvData is a pointer to the 16/32-bit variable where read data will
//! be stored.
//! \param usLength designates 16- or 32-bit read (1 = 16-bit, 2 = 32-bit)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//! \param ulResponseFlag indicates the MtoC Flag number mask used to report
//! when the read data is available at pvData. (\e ulResponseFlag MUST use IPC
//! flags 17-32, and not 1-16).
//!
//! This function will allow the M3 master system to send a 16/32-bit data read
//! command to the C28 control system and set a ResponseFlag to track the
//! status of the read.
//! The C28 control system will respond with a DataWrite command which will
//! place the data in the M3 address pointed to by \e pvData. When the M3
//! receives the DataWrite command and writes the read data into \e *pvData
//! location, it will clear the ResponseFlag, indicating to the rest of the
//! system that the data is ready. The \e usLength parameter can be one of
//! the following values: \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS.
//! The \e bBlock parameter can be one of the following values: \b
//! ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32or \b NO_FLAG.
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
//! was not sent)
//*****************************************************************************
unsigned short
IPCMtoCDataRead (volatile tIpcController *psController, unsigned long ulAddress,
                 void *pvData, unsigned short usLength, unsigned short bBlock,
                 unsigned long ulResponseFlag)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up read command, address, dataw1 = ResponseFlag | word length,
    // dataw2 = address where word
    // should be written to when returned.
    sMessage.ulcommand = IPC_DATA_READ;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (ulResponseFlag & 0xFFFF0000)|(unsigned long)usLength;
    sMessage.uldataw2 = (unsigned long)pvData;

    // Set ResponseFlag (cleared once data is read into address at pvData)
    // Put Message into PutBuffer and set IPC INT flag
    HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulResponseFlag & 0xFFFF0000);
    usStatus = IpcPut (psController, &sMessage, bBlock);

    return usStatus;
    /* Note: Read Response will have sMessage.ulcommand = IPC_DATA_WRITE
     *                               sMessage.uladdress = (unsigned long)
     *pvData
     *                               sMessage.uldataw1  = ulStatusFlag |
     *(unsigned long) usLength;
     *                               sMessage.uldataw2  = word to be read into
     *pvData address.
     */
}

//*****************************************************************************
//! Sends the command to read either a 16- or 32-bit data word from a
//! C28 control system address into a write-protected M3 address.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the C28 address to read from
//! \param pvData is a pointer to the 16/32-bit write-protected variable where
//! read data will be stored.
//! \param usLength designates 16- or 32-bit read (1 = 16-bit, 2 = 32-bit)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//! \param ulResponseFlag indicates the MtoC Flag number mask used to report
//! when the read data is available at pvData. (\e ulResponseFlag MUST use IPC
//! flags 17-32, and not 1-16)
//!
//! This function will allow the M3 master system to send a 16/32-bit data read
//! command to the C28 control system and set a ResponseFlag to track the
//! status of the read.
//! The C28 control system will respond with a DataWrite command which will
//! place the data in the M3 address pointed to by \e pvData. When the M3
//! receives the DataWrite command and writes the read data into \e *pvData
//! location, it will clear the ResponseFlag, indicating to the rest of the
//! system that the data is ready. The \e usLength parameter can be one of
//! the following values: \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS.
//! The \e bBlock parameter can be one of the following values: \b
//! ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
//! was not sent)
//*****************************************************************************
unsigned short
IPCMtoCDataRead_Protected (volatile tIpcController *psController,
                           unsigned long ulAddress, void *pvData,
                           unsigned short usLength, unsigned short bBlock,
                           unsigned long ulResponseFlag)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up read command, address, dataw1 = ResponseFlag | word length,
    // dataw2 = address where word
    // should be written to when returned.
    sMessage.ulcommand = IPC_DATA_READ_PROTECTED;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (ulResponseFlag & 0xFFFF0000)|(unsigned long)usLength;
    sMessage.uldataw2 = (unsigned long)pvData;

    // Set ResponseFlag (cleared once data is read into address at pvData)
    // Put Message into PutBuffer and set IPC INT flag
    HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulResponseFlag & 0xFFFF0000);
    usStatus = IpcPut (psController, &sMessage, bBlock);

    return usStatus;
    /* Note: Read Response will have sMessage.ulcommand = IPC_DATA_WRITE
     *                               sMessage.uladdress = (unsigned long)
     *pvData
     *                               sMessage.uldataw1  = ulStatusFlag |
     *(unsigned long) usLength;
     *                               sMessage.uldataw2  = word to be read into
     *pvData address.
     */
}

//*****************************************************************************
//! Sets the designated bits in a 16-bit data word at an C28 control system
//! address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the C28 address to write to
//! \param ulMask specifies the 16/32-bit mask for bits which should be set at
//! \e ulAddress.
//! 16-bit masks should fill the lower 16-bits (upper 16-bits will be all
//! 0x0000).
//! \param usLength specifies the length of the bit mask (1=16-bits, 2=32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//! This function will allow the M3 master system to set bits specified by the
//! \e ulMask variable in a 16/32-bit word on the C28 control system. The \e
//! usLength parameter can be one of the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter can
//! be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
//! was not sent)
//*****************************************************************************
unsigned short
IPCMtoCSetBits(volatile tIpcController *psController, unsigned long ulAddress,
               unsigned long ulMask, unsigned short usLength,
               unsigned short bBlock)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up set bits command, address, dataw1 = word length, dataw2 =
    // 16/32-bit mask
    sMessage.ulcommand = IPC_SET_BITS;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (unsigned long)usLength;
    sMessage.uldataw2 = ulMask;

    // Put Message into PutBuffer and set IPC INT flag
    usStatus = IpcPut (psController, &sMessage, bBlock);
    return usStatus;
}

//*****************************************************************************
//! Sets the designated bits in a 16-bit write-protected data word at a C28
//! control system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the C28 address to write to
//! \param ulMask specifies the 16/32-bit mask for bits which should be set at
//! \e ulAddress. 16-bit masks should fill the lower 16-bits (upper 16-bits
//! will be all 0x0000).
//! \param usLength specifies the length of the bit mask (1=16-bits, 2=32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the M3 master system to set bits specified by the
//! \e ulMask variable in a write-protected 16/32-bit word on the C28 control
//! system. The \e usLength parameter can be one of the  following values:
//! \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter
//! can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
//! was not sent)
//*****************************************************************************
unsigned short
IPCMtoCSetBits_Protected(volatile tIpcController *psController,
                         unsigned long ulAddress, unsigned long ulMask,
                         unsigned short usLength, unsigned short bBlock)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up set bits command, address, dataw1 = word length, dataw2 =
    // 16/32-bit mask
    sMessage.ulcommand = IPC_SET_BITS_PROTECTED;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (unsigned long)usLength;
    sMessage.uldataw2 = ulMask;

    // Put Message into PutBuffer and set IPC INT flag
    usStatus = IpcPut (psController, &sMessage, bBlock);
    return usStatus;
}

//*****************************************************************************
//! Clears the designated bits in a 16-bit data word at a C28 control system
//! address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the C28 address to write to
//! \param ulMask specifies the 16/32-bit mask for bits which should be cleared
//! at \e ulAddress. 16-bit masks should fill the lower 16-bits (upper 16-bits
//! will be all 0x0000).
//! \param usLength specifies the length of the bit mask (1=16-bits, 2=32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no slot).
//!
//! This function will allow the M3 master system to clear bits specified by
//! the \e ulMask variable in a 16/32-bit word on the C28 control system.  The
//! \e usLength parameter can be one of the following values: \b
//! IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter can
//! be one of the following values: \b ENABLE_BLOCKING or
//! \b DISABLE_BLOCKING.
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
//! was not sent)
//*****************************************************************************
unsigned short
IPCMtoCClearBits(volatile tIpcController *psController, unsigned long ulAddress,
                 unsigned long ulMask, unsigned short usLength,
                 unsigned short bBlock)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up clear bits command, address, dataw1 = word length, dataw2 =
    // 16/32-bit mask
    sMessage.ulcommand = IPC_CLEAR_BITS;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (unsigned long)usLength;
    sMessage.uldataw2 = ulMask;

    // Put Message into PutBuffer and set IPC INT flag
    usStatus = IpcPut (psController, &sMessage, bBlock);
    return usStatus;
}

//*****************************************************************************
//! Clears the designated bits in a 16-bit write-protected data word at a C28
//! control system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the C28 address to write to
//! \param ulMask specifies the 16/32-bit mask for bits which should be cleared
//! at \e ulAddress. 16-bit masks should fill the lower 16-bits (upper
//! 16-bits will be all 0x0000).
//! \param usLength specifies the length of the bit mask (1=16-bits, 2=32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//!
//! This function will allow the M3 master system to set bits specified by the
//! \e ulMask variable in a write-protected 16/32-bit word on the C28
//! control system. The \e usLength parameter can be  one of the following
//! values: \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock
//! parameter can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
//! was not sent)
//*****************************************************************************
unsigned short
IPCMtoCClearBits_Protected(volatile tIpcController *psController,
                           unsigned long ulAddress, unsigned long ulMask,
                           unsigned short usLength, unsigned short bBlock)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up clear bits command, address, dataw1 = word length, dataw2 =
    // 16/32-bit mask
    sMessage.ulcommand = IPC_CLEAR_BITS_PROTECTED;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (unsigned long)usLength;
    sMessage.uldataw2 = ulMask;

    // Put Message into PutBuffer and set IPC INT flag
    usStatus = IpcPut (psController, &sMessage, bBlock);
    return usStatus;
}

//*****************************************************************************
//! Writes a 16/32-bit data word to a C28 control system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the C28 address to write to
//! \param ulData specifies the 16/32-bit word which will be written. For
//! 16-bit words, only the lower 16-bits of ulData will be considered by the
//! master system.
//! \param usLength is the length of the word to write (1 = 16-bits, 2 =
//! 32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//! \param ulResponseFlag is used to pass the \e ulResponseFlag back to the C28
//! only when this function is called in response to \e IPCCtoMDataRead().
//! Otherwise, set to 0.
//!
//! This function will allow the M3 master system to write a 16/32-bit word via
//! the \e ulData variable to an address on the C28 master system.
//! The \e usLength parameter can be one of the following values:
//!  \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter
//! can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCMtoCDataWrite(volatile tIpcController *psController, unsigned long ulAddress,
                 unsigned long ulData, unsigned short usLength,
                 unsigned short bBlock, unsigned long ulResponseFlag)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up write command, address, dataw1 = ResponseFlag | word length,
    // dataw2 = data to write
    sMessage.ulcommand = IPC_DATA_WRITE;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = ulResponseFlag |(unsigned long)usLength;
    sMessage.uldataw2 = ulData;

    // Put Message into PutBuffer and set IPC INT flag
    usStatus = IpcPut (psController, &sMessage, bBlock);
    return usStatus;
}

//*****************************************************************************
//! Writes a 16/32-bit data word to a write-protected C28 control system
//! address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the write-protected C28 address to write to
//! \param ulData specifies the 16/32-bit word which will be written. For
//! 16-bit words, only the lower 16-bits of ulData will be considered by the
//! master system.
//! \param usLength is the length of the word to write (1 = 16-bits, 2 =
//! 32-bits)
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//! \param ulResponseFlag is used to pass the \e ulResponseFlag back to the C28
//! only when this function is called in response to \e IPCCtoMDataRead().
//! Otherwise, set to 0.
//!
//! This function will allow the M3 master system to write a 16/32-bit word via
//! the \e ulData variable to a write-protected address on the C28 control
//! system.
//! The \e usLength parameter can be one of the following values:
//!  \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter
//! can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \return status of command (0=success, 1=error)
//*****************************************************************************
unsigned short
IPCMtoCDataWrite_Protected(volatile tIpcController *psController,
                           unsigned long ulAddress, unsigned long ulData,
                           unsigned short usLength, unsigned short bBlock,
                           unsigned long ulResponseFlag)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up write command, address, dataw1 = ResponseFlag | word length,
    // dataw2 = data to write
    sMessage.ulcommand = IPC_DATA_WRITE_PROTECTED;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = ulResponseFlag |(unsigned long)usLength;
    sMessage.uldataw2 = ulData;

    // Put Message into PutBuffer and set IPC INT flag

    usStatus = IpcPut (psController, &sMessage, bBlock);
    return usStatus;
}

//*****************************************************************************
//! Sends the command to read a block of data from an C28 control system
//! address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the C28 memory block starting address to read
//! from.
//! \param ulShareAddress specifies the M3 shared memory address the read block
//! will read to.
//! \param usLength designates the block size in 16-bit words.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//! \param ulResponseFlag indicates the MtoC Flag number mask used to report
//! when the read block data is available starting at /e ulShareAddress. (\e
//! ulResponseFlag MUST use IPC flags 17-32, and not 1-16)
//!
//! This function will allow the M3 master system to send a read block command
//! to the C28 master system and set a ResponseFlag to track the status of
//! the read. The C28 master system will process the read and place the data
//! in shared memory at the location specified in the \e ulShareAddress
//! parameter and then clear the ResponseFlag, indicating that the block is
//! ready. The \e bBlock parameter can be one of the following values: \b
//! ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
//! was not sent)
//*****************************************************************************
unsigned short
IPCMtoCBlockRead(volatile tIpcController *psController, unsigned long ulAddress,
                 unsigned long ulShareAddress, unsigned short usLength,
                 unsigned short bBlock, unsigned long ulResponseFlag)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up block read command, address, dataw1 = ResponseFlag | block
    // length, dataw2 = C28 address in shared memory
    // where block data should be read to (corresponding to M3 ulShareAddress).
    sMessage.ulcommand = IPC_BLOCK_READ;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = (ulResponseFlag & 0xFFFF0000) |(unsigned long)usLength;
    sMessage.uldataw2 = IPCMtoCSharedRamConvert(ulShareAddress);

    // Set ResponseFlag (cleared once data is read into Share Address location)
    // Put Message into PutBuffer and set IPC INT flag
    HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (ulResponseFlag & 0xFFFF0000);
    usStatus = IpcPut (psController, &sMessage, bBlock);

    return usStatus;
    /* Note: Read Block Response will occur in processing of ReadBlock (since
       C28 has access to shared memory)
     */
}

//*****************************************************************************
//! Writes a block of data to a C28 control system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the C28 memory block starting address to write
//! to.
//! \param ulShareAddress specifies the M3 shared memory address where data to
//! write from resides.
//! \param usLength designates the block size in 16- or 32-bit words (depends
//! on \e usWordLength).
//! \param usWordLength designates the word size (16-bits = 1 or 32-bits = 2).
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//!
//! This function will allow the M3 master system to write a block of data to
//! the C28 control system starting from the location specified by the
//! \e ulAdress parameter. Prior to calling this function, the M3 master system
//! code should place the data to write in shared memory starting at /e
//! ulShareAddress.
//! The \e usWordLength parameter can be one of the following values:
//! \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter
//! can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \note If the Sx SARAM blocks are used to pass the RAM block between the
//! processors, the MSXMSEL register must be configured for M3 master access
//! prior to the M3 writing the block to \e ulShareAddress in order to give
//! the M3 write access to the Sx memory block(s) used for passing data.
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
// was not sent)
//*****************************************************************************
unsigned short
IPCMtoCBlockWrite(volatile tIpcController *psController,
                  unsigned long ulAddress, unsigned long ulShareAddress,
                  unsigned short usLength, unsigned short usWordLength,
                  unsigned short bBlock)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up block write command, address, dataw1 = block length and size of
    // each word in block,
    // dataw2 = C28 shared mem address where write data resides
    sMessage.ulcommand = IPC_BLOCK_WRITE;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 =
        ((unsigned long)(usWordLength)<<16) + (unsigned long)usLength;
    sMessage.uldataw2 = IPCMtoCSharedRamConvert(ulShareAddress);

    // Put Message into PutBuffer and set IPC INT flag
    usStatus = IpcPut (psController, &sMessage, bBlock);
    return usStatus;
}

//*****************************************************************************
//! Writes a block of data to a write-protected C28 control system address
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the write-protected C28 memory block starting
//! address to write to.
//! \param ulShareAddress specifies the M3 shared memory address where data to
//! write from resides.
//! \param usLength designates the block size in 16- or 32-bit words (depends
//! on \e usWordLength).
//! \param usWordLength designates the word size (16-bits = 1 or 32-bits = 2).
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//!
//! This function will allow the M3 master system to write a block of data to
//! a write-protected region on the C28 control system starting from the
//! location specified by the \e ulAdress parameter. Prior to calling this
//! function, the M3 master system code should place the data to write in
//! shared memory starting at /e ulShareAddress.
//! The \e usWordLength parameter can be one of the following values:
//! \b IPC_LENGTH_16_BITS or \b IPC_LENGTH_32_BITS. The \e bBlock parameter
//! can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//! The \e ulResponseFlag parameter can be any single one of the flags \b
//! IPC_FLAG17 - \b IPC_FLAG32 or \b NO_FLAG.
//!
//! \note If the Sx SARAM blocks are used to pass the RAM block between the
//! processors, the MSXMSEL register must be configured for M3 master access
//! prior to the M3 writing the block to /e ulShareAddress in order to give
//! the M3 write access to the Sx memory block(s) used for passing data.
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
//! was not sent)
//*****************************************************************************
unsigned short
IPCMtoCBlockWrite_Protected(volatile tIpcController *psController,
                            unsigned long ulAddress,
                            unsigned long ulShareAddress,
                            unsigned short usLength,
                            unsigned short usWordLength,
                            unsigned short bBlock)
{
    unsigned short usStatus;
    tIpcMessage sMessage;

    // Set up block write command, address, dataw1 = block length and size of
    // each word in block,
    // dataw2 = C28 shared mem address where write data resides
    sMessage.ulcommand = IPC_BLOCK_WRITE_PROTECTED;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 =
        ((unsigned long)(usWordLength)<<16) + (unsigned long)usLength;
    sMessage.uldataw2 = IPCMtoCSharedRamConvert(ulShareAddress);

    // Put Message into PutBuffer and set IPC INT flag
    usStatus = IpcPut (psController, &sMessage, bBlock);
    return usStatus;
}

//*****************************************************************************
//! Calls a C28 function with 1 optional parameter.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulAddress specifies the C28 function address
//! \param ulParam specifies the 32-bit optional parameter value. If not used,
//! this can be a dummy value.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//!
//! This function will allow the M3 control system to call a function on the
//! C28. The \e ulParam variable is a single optional 32-bit parameter to
//! pass to the function. The \e bBlock parameter can be one of the
//! following values: \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return status of command (\b STATUS_PASS =success, \b STATUS_FAIL = error
//! because PutBuffer was full, command was not sent)
//*****************************************************************************
unsigned short
IPCMtoCFunctionCall(volatile tIpcController *psController,
                    unsigned long ulAddress, unsigned long ulParam,
                    unsigned short bBlock)
{
    unsigned short status;
    tIpcMessage sMessage;

    // Set up function call command, address, dataw1 = 32-bit parameter
    sMessage.ulcommand = IPC_FUNC_CALL;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = ulParam;

    // Put Message into PutBuffer and set IPC INT flag
    status = IpcPut (psController, &sMessage, bBlock);
    return status;
}

//*****************************************************************************
//! Sends generic message to C28 control system
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param ulCommand specifies 32-bit command word to insert into message.
//! \param ulAddress specifies 32-bit address word to insert into message.
//! \param ulDataW1 specifies 1st 32-bit data word to insert into message.
//! \param ulDataW2 specifies 2nd 32-bit data word to insert into message.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//!
//! This function will allow the M3 master system to send a generic message to
//! the C28 control system. Note that the user should create a corresponding
//! function on the C28 side to interpret/use the message or fill parameters
//! in such a way that the existing IPC drivers can recognize the command
//! and properly process the message.
//! The \e bBlock parameter can be one of the following values: \b
//! ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return status of command (0=success, 1=error - PutBuffer was full, command
//! was not sent)
//*****************************************************************************
unsigned short
IPCMtoCSendMessage(volatile tIpcController *psController,
                   unsigned long ulCommand, unsigned long ulAddress,
                   unsigned long ulDataW1, unsigned long ulDataW2,
                   unsigned short bBlock)
{

    unsigned short usStatus;
    tIpcMessage sMessage;

    // Package message to send
    sMessage.ulcommand = ulCommand;
    sMessage.uladdress = ulAddress;
    sMessage.uldataw1 = ulDataW1;
    sMessage.uldataw2 = ulDataW2;

    // Put Message into PutBuffer and set IPC INT flag
    usStatus = IpcPut (psController, &sMessage, bBlock);
    return usStatus;
}

//*****************************************************************************
//! Responds to 16/32-bit data word write command from C28 control system
//!
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//!
//! This function will allow the M3 master system to write a 16/32-bit word
//! received from the C28 control system to the address indicated in \e
//! *psMessage. In the event that the IPC_DATA_WRITE command was received as
//! a result of an IPC_DATA_READ command, this function will also clear the
//! IPC response flag tracking the read so other threads in the M3 system
//! will be aware that the data is ready.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMDataWrite(tIpcMessage *psMessage)
{
    // Data word length = dataw1 (15:0), responseFlag = valid only for IPC
    // flags 17-32
    unsigned short usLength = (unsigned short) psMessage->uldataw1;
    unsigned long ulResponseFlag = (psMessage->uldataw1) & 0xFFFF0000;

    // Write 16/32-bit word to address
    if (usLength == IPC_LENGTH_16_BITS)
    {
        *(volatile unsigned short *)(psMessage->uladdress) =
            (unsigned short)psMessage->uldataw2;
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        *(volatile unsigned long *)(psMessage->uladdress) = psMessage->uldataw2;
    }

    // If data write command is in response to a data read command from C28
    // to M3, clear ResponseFlag, indicating read data from M3 is ready.
    HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCLR) |= ulResponseFlag;
}

//*****************************************************************************
//! Responds to 16/32-bit write-protected data word write command from
//! C28 control system
//!
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//!
//! This function will allow the M3 master system to write a 16/32-bit word
//! received from the C28 control system to the write-protected address
//! indicated in \e *psMessage. In the event that the
//! IPC_DATA_WRITE_PROTECTED command was received as a result of an
//! IPC_DATA_READ_PROTECTED command, this function will also clear the IPC
//! response flag tracking the read so other threads in the M3 system will be
//! aware that the data is ready.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMDataWrite_Protected(tIpcMessage *psMessage)
{

    // Data word length = dataw1 (15:0), responseFlag = valid only for IPC
    // flags 17-32
    unsigned short usLength = (unsigned short) psMessage->uldataw1;
    unsigned long ulResponseFlag = (psMessage->uldataw1) & 0xFFFF0000;

    // Save current status of MWRALLOW register then configure to
    // allow access to MWRALLOW-protected memory addresses.
    unsigned long ulWRALLOWtemp = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    // Write 16/32-bit word to address
    if (usLength == IPC_LENGTH_16_BITS)
    {
        *(volatile unsigned short *)(psMessage->uladdress) =
            (short)psMessage->uldataw2;
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        *(volatile unsigned long *)(psMessage->uladdress) = psMessage->uldataw2;
    }

    // Restore MWRALLOW register to original state.
    HWREG(SYSCTL_MWRALLOW) = ulWRALLOWtemp;

    // If data write command is in response to a data read command from C28
    // to M3, clear ResponseFlag, indicating read data from M3 is ready.
    HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCLR) |= ulResponseFlag;
}

//*****************************************************************************
//! Responds to 16/32-bit data word read command from C28 control system
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//!
//! This function will allow the C28 control system to read a 16/32-bit data
//! word at the M3 address specified in /e psMessage, and send a Write command
//! with the read data back to the C28 control system. It will also send the
//! Response Flag used to track the read back to the C28. The \e bBlock
//! parameter can be one of the following values: \b ENABLE_BLOCKING or \b
//! DISABLE_BLOCKING.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMDataRead(volatile tIpcController *psController, tIpcMessage *psMessage,
                unsigned short bBlock)
{
    unsigned long ulReaddata;
    unsigned short usLength;

    // If data word length = 16-bits, read the 16-bit value at the given
    // address and cast as
    // 32-bit word to send back to C28.
    // If data word length = 32-bits, read the 32-bit value at the given
    // address.
    usLength = (unsigned short)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        ulReaddata =
            (unsigned long)(*(volatile unsigned short *)psMessage->uladdress);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        ulReaddata = *(unsigned long *)psMessage->uladdress;
    }

    // Send a Write command to write the requested data to the C28 read into
    // address.
    // psMessage->uldataw2 contains C28 address where readdata will be written.
    // psMessage->uldataw1 contains the read response flag in IPC flag 17-32.
    IPCMtoCDataWrite(psController, psMessage->uldataw2, ulReaddata, usLength,
                     bBlock, (psMessage->uldataw1 & 0xFFFF0000));
}

//*****************************************************************************
//! Responds to 16/32-bit data word read command from C28 control system
//! to read into a write-protected word on the C28 system.
//!
//! \param psController specifies the address of a \e tIpcController instance
//! used to store information about the "Put" and "Get" circular buffers and
//! their respective indexes.
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//! \param bBlock specifies whether to allow function to block until PutBuffer
//! has a slot (1= wait until slot free, 0 = exit with STATUS_FAIL if no
//! slot).
//!
//! This function will allow the C28 control system to read a 16/32-bit data
//! word at the M3 address specified in /e psMessage, and send a Write
//! Protected command with the read data back to the C28 control system at a
//! write protected address. It will also send the Response Flag used to
//! track the read back to the C28. The \e bBlock parameter can be one of
//! the following values: \b ENABLE_BLOCKING or \b DISABLE_BLOCKING.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMDataRead_Protected(volatile tIpcController *psController,
                          tIpcMessage *            psMessage,
                          unsigned short bBlock)
{
    unsigned long ulReaddata;
    unsigned short usLength;

    // If data word length = 16-bits, read the 16-bit value at the given
    // address and cast as
    // 32-bit word to send back to C28.
    // If data word length = 32-bits, read the 32-bit value at the given
    // address.
    usLength = (unsigned short)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        ulReaddata =
            (unsigned long)(*(volatile unsigned short *)psMessage->uladdress);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        ulReaddata = *(unsigned long *)psMessage->uladdress;
    }

    // Send a Write command to write the requested data to the C28 read into
    // address.
    // psMessage->uldataw2 contains C28 address where readdata will be written.
    // psMessage->uldataw1 contains the read response flag in IPC flag 17-32.
    IPCMtoCDataWrite_Protected(psController, psMessage->uldataw2, ulReaddata,
                               usLength, bBlock,
                               (psMessage->uldataw1 & 0xFFFF0000));
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit data word at an M3 master system
//! address
//!
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//!
//! This function will allow the C28 control system to set the bits in a
//! 16/32-bit word on the M3 master system via an M3 address and mask passed
//! in via the \e psMessage.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMSetBits(tIpcMessage *psMessage)
{
    unsigned short usLength;

    // Determine length of word at psMessage->uladdress and then set bits based
    // on
    // either the 16-bit or 32-bit bit-mask in psMessage->uldataw2.
    // (16-bit length ignores upper 16-bits of psMessage->uldataw2)
    usLength = (unsigned short)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        *(volatile unsigned short*)psMessage->uladdress |=
            (unsigned short) psMessage->uldataw2;
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        *(volatile unsigned long *)psMessage->uladdress |=  psMessage->uldataw2;
    }
}

//*****************************************************************************
//! Sets the designated bits in a 16/32-bit write-protected data word at an
//! M3 master system address
//!
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//!
//! This function will allow the C28 control system to set the bits in a write-
//! protected 16/32-bit word on the M3 master system via an M3 address and mask
//! passed in via the \e psMessage.
//!
//! \return None
//*****************************************************************************
void
IPCCtoMSetBits_Protected(tIpcMessage *psMessage)
{
    unsigned long ulWRALLOWtemp;
    unsigned short usLength;

    // Save current status of MWRALLOW register then configure to
    // allow access to MWRALLOW-protected memory addresses.
    ulWRALLOWtemp = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    // Determine length of word at psMessage->uladdress and then set bits based
    // on
    // either the 16-bit or 32-bit bit-mask in psMessage->uldataw2.
    // (16-bit length ignores upper 16-bits of psMessage->uldataw2)
    usLength = (unsigned short)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        *(volatile unsigned short*)psMessage->uladdress |=
            (unsigned short) psMessage->uldataw2;
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        *(volatile unsigned long *)psMessage->uladdress |= psMessage->uldataw2;
    }

    // Restore MWRALLOW register to original state.
    HWREG(SYSCTL_MWRALLOW) = ulWRALLOWtemp;
}

//*****************************************************************************
//! Clears the designated bits in a 32-bit data word at an M3 master system
//! address
//!
//! \param psMessage specifies the pointer to the message received from the
//! C28 control system.
//!
//! This function will allow the C28 control system to clear the bits in a
//! 16/32-bit word on the M3 master system via an M3 address and mask passed
//! in via the \e psMessage.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMClearBits(tIpcMessage *psMessage)
{

    unsigned short usLength;

    // Determine length of word at psMessage->uladdress and then clear bits
    // based on
    // either the 16-bit or 32-bit bit-mask in psMessage->uldataw2.
    // (16-bit length ignores upper 16-bits of psMessage->uldataw2)
    usLength = (unsigned short)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        *(volatile unsigned short*)psMessage->uladdress &=
            ~((unsigned short) psMessage->uldataw2);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        *(volatile unsigned long *)psMessage->uladdress &=
            ~(psMessage->uldataw2);
    }
}

//*****************************************************************************
//! Clears the designated bits in a write-protected 16/32-bit data word at an
//! M3 master system address
//!
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//!
//! This function will allow the C28 control system to clear the bits in a
//! 16/32-bit write-protected  word on the M3 master system via an M3
//! address and mask passed in via the \e psMessage.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMClearBits_Protected(tIpcMessage *psMessage)
{
    unsigned long ulWRALLOWtemp;
    unsigned short usLength;

    // Save current status of MWRALLOW register then configure to
    // allow access to MWRALLOW-protected memory addresses.
    ulWRALLOWtemp = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    // Determine length of word at psMessage->uladdress and then clear bits
    // based on
    // either the 16-bit or 32-bit bit-mask in psMessage->uldataw2.
    // (16-bit length ignores upper 16-bits of psMessage->uldataw2)
    usLength = (unsigned short)psMessage->uldataw1;

    if (usLength == IPC_LENGTH_16_BITS)
    {
        *(volatile unsigned short*)psMessage->uladdress &=
            ~((unsigned short) psMessage->uldataw2);
    }
    else if (usLength == IPC_LENGTH_32_BITS)
    {
        *(volatile unsigned long *)psMessage->uladdress &=
            ~(psMessage->uldataw2);
    }

    // Restore MWRALLOW register to original state.
    HWREG(SYSCTL_MWRALLOW) = ulWRALLOWtemp;
}

//*****************************************************************************
//! Reads a block of data from an M3 master system address and stores into
//! shared RAM
//!
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//!
//! This function will respond to the C28 control system request to read a
//! block of data from the M3 master system, by reading the data and placing
//! that data into the shared RAM location specified in \e psMessage.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMBlockRead(tIpcMessage *psMessage)
{

    unsigned short usLength;
    volatile unsigned short* pusRAddress;
    volatile unsigned short* pusWAddress;
    unsigned short usIndex;

    pusRAddress = (volatile unsigned short *)psMessage->uladdress;
    pusWAddress = (volatile unsigned short *)psMessage->uldataw2;
    usLength = (unsigned short)psMessage->uldataw1;

    for (usIndex=0; usIndex<usLength; usIndex++)
    {
        *pusWAddress = *pusRAddress;
        pusWAddress += 1;
        pusRAddress += 1;
    }

    HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= (psMessage->uldataw1 & 0xFFFF0000);
}

//*****************************************************************************
//! Writes a block of data to an M3 master system address from shared RAM
//!
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//!
//! This function will write a block of data to an address on the M3 master
//! system. The data is first written by the C28 to shared RAM. This
//! function reads the shared RAM location, word size (16- or 32-bit), and
//! block size from \e psMessage and writes the block to the M3 address
//! specified in \e psMessage.
//!
//! \note Writes to registers accessible via the APB bus must be 32-bits wide.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMBlockWrite(tIpcMessage *psMessage)
{
    unsigned short usLength;
    unsigned short usWLength;
    unsigned short usIndex;

    usLength = (unsigned short)psMessage->uldataw1;
    usWLength = (unsigned short)((psMessage->uldataw1)>>16);

    // Determine data word access size to write to data block.
    // (Writes to registers accessible via APB bus must be 32-bits wide)
    if (usWLength == IPC_LENGTH_16_BITS)
    {
        volatile unsigned short *pusWAddress =
            (volatile unsigned short *)psMessage->uladdress;
        volatile unsigned short *pusRAddress =
            (volatile unsigned short *)psMessage->uldataw2;
        for (usIndex=0; usIndex<usLength; usIndex++)
        {
            *pusWAddress = *pusRAddress;
            pusWAddress += 1;
            pusRAddress += 1;
        }
    }
    else if (usWLength == IPC_LENGTH_32_BITS)
    {
        volatile unsigned long *pulWAddress =
            (volatile unsigned long *)psMessage->uladdress;
        volatile unsigned long *pulRAddress =
            (volatile unsigned long *)psMessage->uldataw2;

        for (usIndex=0; usIndex<usLength; usIndex++)
        {
            *pulWAddress = *pulRAddress;
            pulWAddress += 1;
            pulRAddress += 1;
        }
    }
}

//*****************************************************************************
//! Writes a block of data to an M3 master system write-protected address from
//! shared RAM
//!
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//!
//! This function will write a block of data to a write-protected group of
//! addresses on the M3 master system. The data is first written by the
//! C28 to shared RAM. This function reads the shared RAM location, word size
//! (16- or 32-bit), and block size from \e psMessage  and writes the block
//! to the M3 address specified in \e psMessage.
//!
//! \note Writes to registers accessible via the APB bus must be 32-bits wide.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMBlockWrite_Protected(tIpcMessage *psMessage)
{
    unsigned short usLength;
    unsigned short usWLength;
    unsigned short usIndex;
    unsigned long ulWRALLOWtemp;

    // Save current status of MWRALLOW register then configure to
    // allow access to MWRALLOW-protected memory addresses.
    ulWRALLOWtemp = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;

    usLength = (unsigned short)psMessage->uldataw1;
    usWLength = (unsigned short)((psMessage->uldataw1)>>16);

    // Determine data word access size to write to data block.
    // (Writes registers accessible via APB bus must be 32-bits wide)
    if (usWLength == IPC_LENGTH_16_BITS)
    {
        volatile unsigned short *pusWAddress =
            (volatile unsigned short *)psMessage->uladdress;
        volatile unsigned short *pusRAddress =
            (volatile unsigned short *)psMessage->uldataw2;
        for (usIndex=0; usIndex<usLength; usIndex++)
        {
            *pusWAddress = *pusRAddress;
            pusWAddress += 1;
            pusRAddress += 1;
        }
    }
    else if (usWLength == IPC_LENGTH_32_BITS)
    {
        volatile unsigned long *pulWAddress =
            (volatile unsigned long *)psMessage->uladdress;
        volatile unsigned long *pulRAddress =
            (volatile unsigned long *)psMessage->uldataw2;

        for (usIndex=0; usIndex<usLength; usIndex++)
        {
            *pulWAddress = *pulRAddress;
            pulWAddress += 1;
            pulRAddress += 1;
        }
    }

    // Restore MWRALLOW register to original state.
    HWREG(SYSCTL_MWRALLOW) = ulWRALLOWtemp;
}

//*****************************************************************************
//! Calls an M3 function with a single optional parameter.
//!
//! \param psMessage specifies the pointer to the message received from the C28
//! control system.
//!
//! This function will allow the C28 control system to call an M3 function with
//! a a single optional parameter. There is no return value from the
//! executed function.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMFunctionCall(tIpcMessage *psMessage)
{
    // Executes function call with parameter at given address.
    tfIpcFuncCall func_call = (tfIpcFuncCall)psMessage->uladdress;
    func_call(psMessage->uldataw1);
}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************




