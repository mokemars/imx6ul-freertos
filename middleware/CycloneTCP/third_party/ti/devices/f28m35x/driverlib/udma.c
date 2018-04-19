//###########################################################################
// FILE:   udma.c
// TITLE:  Driver for the micro-DMA controller.
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
//! \addtogroup udma_api
//! @{
//*****************************************************************************

#include "inc/hw_types.h"
#include "inc/hw_udma.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/udma.h"

//*****************************************************************************
//! Enables the uDMA controller for use.
//!
//! This function enables the uDMA controller.  The uDMA controller must be
//! enabled before it can be configured and used.
//!
//! \return None.
//*****************************************************************************
void
uDMAEnable(void)
{
    // Set the master enable bit in the config register.
    HWREG(UDMA_CFG) = UDMA_CFG_MASTEN;
}

//*****************************************************************************
//! Disables the uDMA controller for use.
//!
//! This function disables the uDMA controller.  Once disabled, the uDMA
//! controller will not operate until re-enabled with uDMAEnable().
//!
//! \return None.
//*****************************************************************************
void
uDMADisable(void)
{
    // Clear the master enable bit in the config register.
    HWREG(UDMA_CFG) = 0;
}

//*****************************************************************************
//! Gets the uDMA error status.
//!
//! This function returns the uDMA error status.  It should be called from
//! within the uDMA error interrupt handler to determine if a uDMA error
//! occurred.
//!
//! \return Returns non-zero if a uDMA error is pending.
//*****************************************************************************
unsigned long
uDMAErrorStatusGet(void)
{
    // Return the uDMA error status.
    return(HWREG(UDMA_ERRCLR));
}

//*****************************************************************************
//! Clears the uDMA error interrupt.
//!
//! This function clears a pending uDMA error interrupt.  It should be called
//! from within the uDMA error interrupt handler to clear the interrupt.
//!
//! \return None.
//*****************************************************************************
void
uDMAErrorStatusClear(void)
{
    // Clear the uDMA error interrupt.
    HWREG(UDMA_ERRCLR) = 1;
}

//*****************************************************************************
//! Enables a uDMA channel for operation.
//!
//! \param ulChannelNum is the channel number to enable.
//!
//! This function enables a specific uDMA channel for use.  This function must
//! be used to enable a channel before it can be used to perform a uDMA
//! transfer.
//!
//! When a uDMA transfer is completed, the channel will be automatically
//! disabled by the uDMA controller.  Therefore, this function should be called
//! prior to starting up any new transfer.
//!
//! \return None.
//*****************************************************************************
void
uDMAChannelEnable(unsigned long ulChannelNum)
{
    // Check the arguments.
    ASSERT(ulChannelNum < 32);

    // Set the bit for this channel in the enable set register.
    HWREG(UDMA_ENASET) = 1 << ulChannelNum;
}

//*****************************************************************************
//! Disables a uDMA channel for operation.
//!
//! \param ulChannelNum is the channel number to disable.
//!
//! This function disables a specific uDMA channel.  Once disabled, a channel
//! will not respond to uDMA transfer requests until re-enabled via
//! uDMAChannelEnable().
//!
//! \return None.
//*****************************************************************************
void
uDMAChannelDisable(unsigned long ulChannelNum)
{
    // Check the arguments.
    ASSERT(ulChannelNum < 32);

    // Set the bit for this channel in the enable clear register.
    HWREG(UDMA_ENACLR) = 1 << ulChannelNum;
}

//*****************************************************************************
//! Checks if a uDMA channel is enabled for operation.
//!
//! \param ulChannelNum is the channel number to check.
//!
//! This function checks to see if a specific uDMA channel is enabled.  This
//! can be used to check the status of a transfer, since the channel will
//! be automatically disabled at the end of a transfer.
//!
//! \return Returns \b true if the channel is enabled, \b false if disabled.
//*****************************************************************************
tBoolean
uDMAChannelIsEnabled(unsigned long ulChannelNum)
{
    // Check the arguments.
    ASSERT(ulChannelNum < 32);

    // AND the specified channel bit with the enable register, and return the
    // result.
    return((HWREG(UDMA_ENASET) & (1 << ulChannelNum)) ? true : false);
}

//*****************************************************************************
//! Sets the base address for the channel control table.
//!
//! \param pControlTable is a pointer to the 1024 byte aligned base address
//! of the uDMA channel control table.
//!
//! This function sets the base address of the channel control table.  This
//! table resides in system memory and holds control information for each uDMA
//! channel.  The table must be aligned on a 1024 byte boundary.  The base
//! address must be set before any of the channel functions can be used.
//!
//! The size of the channel control table depends on the number of uDMA
//! channels, and which transfer modes are used.  Refer to the introductory
//! text and the microcontroller datasheet for more information about the
//! channel control table.
//!
//! \return None.
//*****************************************************************************
void
uDMAControlBaseSet(void *pControlTable)
{
    // Check the arguments.
    ASSERT(((unsigned long)pControlTable & ~0x3FF) ==
           (unsigned long)pControlTable);
    ASSERT((unsigned long)pControlTable >= 0x20000000);

    // Program the base address into the register.
    HWREG(UDMA_CTLBASE) = (unsigned long)pControlTable;
}

//*****************************************************************************
//! Gets the base address for the channel control table.
//!
//! This function gets the base address of the channel control table.  This
//! table resides in system memory and holds control information for each uDMA
//! channel.
//!
//! \return Returns a pointer to the base address of the channel control table.
//*****************************************************************************
void *
uDMAControlBaseGet(void)
{
    // Read the current value of the control base register, and return it to
    // the caller.
    return((void *)HWREG(UDMA_CTLBASE));
}

//*****************************************************************************
//! Gets the base address for the channel control table alternate structures.
//!
//! This function gets the base address of the second half of the channel
//! control table that holds the alternate control structures for each channel.
//!
//! \return Returns a pointer to the base address of the second half of the
//! channel control table.
//*****************************************************************************
void *
uDMAControlAlternateBaseGet(void)
{
    // Read the current value of the control base register, and return it to
    // the caller.
    return((void *)HWREG(UDMA_ALTBASE));
}

//*****************************************************************************
//! Requests a uDMA channel to start a transfer.
//!
//! \param ulChannelNum is the channel number on which to request a uDMA
//! transfer.
//!
//! This function allows software to request a uDMA channel to begin a
//! transfer.  This could be used for performing a memory to memory transfer,
//! or if for some reason a transfer needs to be initiated by software instead
//! of the peripheral associated with that channel.
//!
//! \note If the channel is \b UDMA_CHANNEL_SW and interrupts are used, then
//! the completion will be signaled on the uDMA dedicated interrupt.  If a
//! peripheral channel is used, then the completion will be signaled on the
//! peripheral's interrupt.
//!
//! \return None.
//*****************************************************************************
void
uDMAChannelRequest(unsigned long ulChannelNum)
{
    // Check the arguments.
    ASSERT(ulChannelNum < 32);

    // Set the bit for this channel in the software uDMA request register.
    HWREG(UDMA_SWREQ) = 1 << (ulChannelNum & 0x1f);
}

//*****************************************************************************
//! Enables attributes of a uDMA channel.
//!
//! \param ulChannelNum is the channel to configure.
//! \param ulAttr is a combination of attributes for the channel.
//!
//! The \e ulAttr parameter is the logical OR of any of the following:
//!
//! - \b UDMA_ATTR_USEBURST is used to restrict transfers to use only a burst
//!   mode.
//! - \b UDMA_ATTR_ALTSELECT is used to select the alternate control structure
//!   for this channel (it is very unlikely that this flag should be used)
//! - \b UDMA_ATTR_HIGH_PRIORITY is used to set this channel to high priority
//! - \b UDMA_ATTR_REQMASK is used to mask the hardware request signal from the
//!   peripheral for this channel.
//!
//! \return None.
//*****************************************************************************
void
uDMAChannelAttributeEnable(unsigned long ulChannelNum, unsigned long ulAttr)
{
    // Check the arguments.
    ASSERT(ulChannelNum < 32);
    ASSERT((ulAttr & ~(UDMA_ATTR_USEBURST | UDMA_ATTR_ALTSELECT |
                       UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK)) == 0);

    // Set the useburst bit for this channel if set in ulConfig.
    if(ulAttr & UDMA_ATTR_USEBURST)
    {
        HWREG(UDMA_USEBURSTSET) = 1 << ulChannelNum;
    }

    // Set the alternate control select bit for this channel,
    // if set in ulConfig.
    if(ulAttr & UDMA_ATTR_ALTSELECT)
    {
        HWREG(UDMA_ALTSET) = 1 << ulChannelNum;
    }

    // Set the high priority bit for this channel, if set in ulConfig.
    if(ulAttr & UDMA_ATTR_HIGH_PRIORITY)
    {
        HWREG(UDMA_PRIOSET) = 1 << ulChannelNum;
    }

    // Set the request mask bit for this channel, if set in ulConfig.
    if(ulAttr & UDMA_ATTR_REQMASK)
    {
        HWREG(UDMA_REQMASKSET) = 1 << ulChannelNum;
    }
}

//*****************************************************************************
//! Disables attributes of a uDMA channel.
//!
//! \param ulChannelNum is the channel to configure.
//! \param ulAttr is a combination of attributes for the channel.
//!
//! This function is used to disable attributes of a uDMA channel.
//!
//! The \e ulAttr parameter is the logical OR of any of the following:
//!
//! - \b UDMA_ATTR_USEBURST is used to restrict transfers to use only a burst
//!   mode.
//! - \b UDMA_ATTR_ALTSELECT is used to select the alternate control structure
//!   for this channel.
//! - \b UDMA_ATTR_HIGH_PRIORITY is used to set this channel to high priority.
//! - \b UDMA_ATTR_REQMASK is used to mask the hardware request signal from the
//!   peripheral for this channel.
//!
//! \return None.
//*****************************************************************************
void
uDMAChannelAttributeDisable(unsigned long ulChannelNum, unsigned long ulAttr)
{
    // Check the arguments.
    ASSERT(ulChannelNum < 32);
    ASSERT((ulAttr & ~(UDMA_ATTR_USEBURST | UDMA_ATTR_ALTSELECT |
                       UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK)) == 0);

    // Clear the useburst bit for this channel if set in ulConfig.
    if(ulAttr & UDMA_ATTR_USEBURST)
    {
        HWREG(UDMA_USEBURSTCLR) = 1 << ulChannelNum;
    }

    // Clear the alternate control select bit for this channel, if set in
    // ulConfig.
    if(ulAttr & UDMA_ATTR_ALTSELECT)
    {
        HWREG(UDMA_ALTCLR) = 1 << ulChannelNum;
    }

    // Clear the high priority bit for this channel, if set in ulConfig.
    if(ulAttr & UDMA_ATTR_HIGH_PRIORITY)
    {
        HWREG(UDMA_PRIOCLR) = 1 << ulChannelNum;
    }

    // Clear the request mask bit for this channel, if set in ulConfig.
    if(ulAttr & UDMA_ATTR_REQMASK)
    {
        HWREG(UDMA_REQMASKCLR) = 1 << ulChannelNum;
    }
}

//*****************************************************************************
//! Gets the enabled attributes of a uDMA channel.
//!
//! \param ulChannelNum is the channel to configure.
//!
//! This function returns a combination of flags representing the attributes of
//! the uDMA channel.
//!
//! \return Returns the logical OR of the attributes of the uDMA channel, which
//! can be any of the following:
//! - \b UDMA_ATTR_USEBURST is used to restrict transfers to use only a burst
//!   mode.
//! - \b UDMA_ATTR_ALTSELECT is used to select the alternate control structure
//!   for this channel.
//! - \b UDMA_ATTR_HIGH_PRIORITY is used to set this channel to high priority.
//! - \b UDMA_ATTR_REQMASK is used to mask the hardware request signal from the
//!   peripheral for this channel.
//*****************************************************************************
unsigned long
uDMAChannelAttributeGet(unsigned long ulChannelNum)
{
    unsigned long ulAttr = 0;

    // Check the arguments.
    ASSERT(ulChannelNum < 32);

    // Check to see if useburst bit is set for this channel.
    if(HWREG(UDMA_USEBURSTSET) & (1 << ulChannelNum))
    {
        ulAttr |= UDMA_ATTR_USEBURST;
    }

    // Check to see if the alternate control bit is set for this channel.
    if(HWREG(UDMA_ALTSET) & (1 << ulChannelNum))
    {
        ulAttr |= UDMA_ATTR_ALTSELECT;
    }

    // Check to see if the high priority bit is set for this channel.
    if(HWREG(UDMA_PRIOSET) & (1 << ulChannelNum))
    {
        ulAttr |= UDMA_ATTR_HIGH_PRIORITY;
    }

    // Check to see if the request mask bit is set for this channel.
    if(HWREG(UDMA_REQMASKSET) & (1 << ulChannelNum))
    {
        ulAttr |= UDMA_ATTR_REQMASK;
    }

    // Return the configuration flags.
    return(ulAttr);
}

//*****************************************************************************
//! Sets the control parameters for a uDMA channel control structure.
//!
//! \param ulChannelStructIndex is the logical OR of the uDMA channel number
//! with \b UDMA_PRI_SELECT or \b UDMA_ALT_SELECT.
//! \param ulControl is logical OR of several control values to set the control
//! parameters for the channel.
//!
//! This function is used to set control parameters for a uDMA transfer.  These
//! are typically parameters that are not changed often.
//!
//! The \e ulChannelStructIndex parameter should be the logical OR of the
//! channel number with one of \b UDMA_PRI_SELECT or \b UDMA_ALT_SELECT to
//! choose whether the primary or alternate data structure is used.
//!
//! The \e ulControl parameter is the logical OR of five values: the data size,
//! the source address increment, the destination address increment, the
//! arbitration size, and the use burst flag.  The choices available for each
//! of these values is described below.
//!
//! Choose the data size from one of \b UDMA_SIZE_8, \b UDMA_SIZE_16, or
//! \b UDMA_SIZE_32 to select a data size of 8, 16, or 32 bits.
//!
//! Choose the source address increment from one of \b UDMA_SRC_INC_8,
//! \b UDMA_SRC_INC_16, \b UDMA_SRC_INC_32, or \b UDMA_SRC_INC_NONE to select
//! an address increment of 8-bit bytes, 16-bit halfwords, 32-bit words, or
//! to select non-incrementing.
//!
//! Choose the destination address increment from one of \b UDMA_DST_INC_8,
//! \b UDMA_DST_INC_16, \b UDMA_DST_INC_32, or \b UDMA_DST_INC_NONE to select
//! an address increment of 8-bit bytes, 16-bit halfwords, 32-bit words, or
//! to select non-incrementing.
//!
//! The arbitration size determines how many items are transferred before
//! the uDMA controller re-arbitrates for the bus.  Choose the arbitration size
//! from one of \b UDMA_ARB_1, \b UDMA_ARB_2, \b UDMA_ARB_4, \b UDMA_ARB_8,
//! through \b UDMA_ARB_1024 to select the arbitration size from 1 to 1024
//! items, in powers of 2.
//!
//! The value \b UDMA_NEXT_USEBURST is used to force the channel to only
//! respond to burst requests at the tail end of a scatter-gather transfer.
//!
//! \note The address increment cannot be smaller than the data size.
//!
//! \return None.
//*****************************************************************************
void
uDMAChannelControlSet(unsigned long ulChannelStructIndex,
                      unsigned long ulControl)
{
    tDMAControlTable *pCtl;

    // Check the arguments.
    ASSERT(ulChannelStructIndex < 64);
    ASSERT(HWREG(UDMA_CTLBASE) != 0);

    // Get the base address of the control table.
    pCtl = (tDMAControlTable *)HWREG(UDMA_CTLBASE);

    // Get the current control word value and mask off the fields to be
    // changed, then OR in the new settings.
    pCtl[ulChannelStructIndex].ulControl =
        ((pCtl[ulChannelStructIndex].ulControl &
          ~(UDMA_CHCTL_DSTINC_M |
            UDMA_CHCTL_DSTSIZE_M |
            UDMA_CHCTL_SRCINC_M |
            UDMA_CHCTL_SRCSIZE_M |
            UDMA_CHCTL_ARBSIZE_M |
            UDMA_CHCTL_NXTUSEBURST)) |
         ulControl);
}

//*****************************************************************************
//! Sets the transfer parameters for a uDMA channel control structure.
//!
//! \param ulChannelStructIndex is the logical OR of the uDMA channel number
//! with either \b UDMA_PRI_SELECT or \b UDMA_ALT_SELECT.
//! \param ulMode is the type of uDMA transfer.
//! \param pvSrcAddr is the source address for the transfer.
//! \param pvDstAddr is the destination address for the transfer.
//! \param ulTransferSize is the number of data items to transfer.
//!
//! This function is used to set the parameters for a uDMA transfer.  These are
//! typically parameters that are changed often.  The function
//! uDMAChannelControlSet() MUST be called at least once for this channel prior
//! to calling this function.
//!
//! The \e ulChannelStructIndex parameter should be the logical OR of the
//! channel number with one of \b UDMA_PRI_SELECT or \b UDMA_ALT_SELECT to
//! choose whether the primary or alternate data structure is used.
//!
//! The \e ulMode parameter should be one of the following values:
//!
//! - \b UDMA_MODE_STOP stops the uDMA transfer.  The controller sets the mode
//!   to this value at the end of a transfer.
//! - \b UDMA_MODE_BASIC to perform a basic transfer based on request.
//! - \b UDMA_MODE_AUTO to perform a transfer that will always complete once
//!   started even if request is removed.
//! - \b UDMA_MODE_PINGPONG to set up a transfer that switches between the
//!   primary and alternate control structures for the channel.  This allows
//!   use of ping-pong buffering for uDMA transfers.
//! - \b UDMA_MODE_MEM_SCATTER_GATHER to set up a memory scatter-gather
//!   transfer.
//! - \b UDMA_MODE_PER_SCATTER_GATHER to set up a peripheral scatter-gather
//!   transfer.
//!
//! The \e pvSrcAddr and \e pvDstAddr parameters are pointers to the first
//! location of the data to be transferred.  These addresses should be aligned
//! according to the item size.  The compiler will take care of this if the
//! pointers are pointing to storage of the appropriate data type.
//!
//! The \e ulTransferSize parameter is the number of data items, not the number
//! of bytes.
//!
//! The two scatter/gather modes, memory and peripheral, are actually different
//! depending on whether the primary or alternate control structure is
//! selected.  This function will look for the \b UDMA_PRI_SELECT and
//! \b UDMA_ALT_SELECT flag along with the channel number and will set the
//! scatter/gather mode as appropriate for the primary or alternate control
//! structure.
//!
//! The channel must also be enabled using uDMAChannelEnable() after calling
//! this function.  The transfer will not begin until the channel has been set
//! up and enabled.  Note that the channel is automatically disabled after the
//! transfer is completed, meaning that uDMAChannelEnable() must be called
//! again after setting up the next transfer.
//!
//! \note Great care must be taken to not modify a channel control structure
//! that is in use or else the results will be unpredictable, including the
//! possibility of undesired data transfers to or from memory or peripherals.
//! For BASIC and AUTO modes, it is safe to make changes when the channel is
//! disabled, or the uDMAChannelModeGet() returns \b UDMA_MODE_STOP.  For
//! PINGPONG or one of the SCATTER_GATHER modes, it is safe to modify the
//! primary or alternate control structure only when the other is being used.
//! The uDMAChannelModeGet() function will return \b UDMA_MODE_STOP when a
//! channel control structure is inactive and safe to modify.
//!
//! \return None.
//*****************************************************************************
void
uDMAChannelTransferSet(unsigned long ulChannelStructIndex,
                       unsigned long ulMode, void *pvSrcAddr, void *pvDstAddr,
                       unsigned long ulTransferSize)
{
    tDMAControlTable *pControlTable;
    unsigned long ulControl;
    unsigned long ulInc;
    unsigned long ulBufferBytes;

    // Check the arguments.
    ASSERT(ulChannelStructIndex < 64);
    ASSERT(HWREG(UDMA_CTLBASE) != 0);
    ASSERT(ulMode <= UDMA_MODE_PER_SCATTER_GATHER);
    ASSERT((unsigned long)pvSrcAddr >= 0x20000000);
    ASSERT((unsigned long)pvDstAddr >= 0x20000000);
    ASSERT((ulTransferSize != 0) && (ulTransferSize <= 1024));

    // Get the base address of the control table.
    pControlTable = (tDMAControlTable *)HWREG(UDMA_CTLBASE);

    // Get the current control word value and mask off the mode and size
    // fields.
    ulControl = (pControlTable[ulChannelStructIndex].ulControl &
                 ~(UDMA_CHCTL_XFERSIZE_M | UDMA_CHCTL_XFERMODE_M));

    // Adjust the mode if the alt control structure is selected.
    if(ulChannelStructIndex & UDMA_ALT_SELECT)
    {
        if((ulMode == UDMA_MODE_MEM_SCATTER_GATHER) ||
           (ulMode == UDMA_MODE_PER_SCATTER_GATHER))
        {
            ulMode |= UDMA_MODE_ALT_SELECT;
        }
    }

    // Set the transfer size and mode in the control word (but don't write the
    // control word yet as it could kick off a transfer).
    ulControl |= ulMode | ((ulTransferSize - 1) << 4);

    // Get the address increment value for the source, from the control word.
    ulInc = (ulControl & UDMA_CHCTL_SRCINC_M);

    // Compute the ending source address of the transfer.  If the source
    // increment is set to none, then the ending address is the same as the
    // beginning.
    if(ulInc != UDMA_SRC_INC_NONE)
    {
        ulInc = ulInc >> 26;
        ulBufferBytes = ulTransferSize << ulInc;
        pvSrcAddr = (void *)((unsigned long)pvSrcAddr + ulBufferBytes - 1);
    }

    // Load the source ending address into the control block.
    pControlTable[ulChannelStructIndex].pvSrcEndAddr = pvSrcAddr;

    // Get the address increment value for the destination, from the control
    // word.
    ulInc = ulControl & UDMA_CHCTL_DSTINC_M;

    // Compute the ending destination address of the transfer.  If the
    // destination increment is set to none, then the ending address is the
    // same as the beginning.
    if(ulInc != UDMA_DST_INC_NONE)
    {
        // There is a special case if this is setting up a scatter-gather
        // transfer.  The destination pointer needs to point to the end of
        // the alternate structure for this channel instead of calculating
        // the end of the buffer in the normal way.
        if((ulMode == UDMA_MODE_MEM_SCATTER_GATHER) ||
           (ulMode == UDMA_MODE_PER_SCATTER_GATHER))
        {
            pvDstAddr =
                (void *)&pControlTable[ulChannelStructIndex |
                                       UDMA_ALT_SELECT].ulSpare;
        }
        // Not a scatter-gather transfer, calculate end pointer normally.
        else
        {
            ulInc = ulInc >> 30;
            ulBufferBytes = ulTransferSize << ulInc;
            pvDstAddr = (void *)((unsigned long)pvDstAddr + ulBufferBytes - 1);
        }
    }

    // Load the destination ending address into the control block.
    pControlTable[ulChannelStructIndex].pvDstEndAddr = pvDstAddr;

    // Write the new control word value.
    pControlTable[ulChannelStructIndex].ulControl = ulControl;
}

//*****************************************************************************
//! Configures a uDMA channel for scatter-gather mode.
//!
//! \param ulChannelNum is the uDMA channel number.
//! \param ulTaskCount is the number of scatter-gather tasks to execute.
//! \param pvTaskList is a pointer to the beginning of the scatter-gather
//! task list.
//! \param ulIsPeriphSG is a flag to indicate it is a peripheral scatter-gather
//! transfer (else it will be memory scatter-gather transfer)
//!
//! This function is used to configure a channel for scatter-gather mode.
//! The caller must have already set up a task list, and pass a pointer to
//! the start of the task list as the \e pvTaskList parameter.  The
//! \e ulTaskCount parameter is the count of tasks in the task list, not the
//! size of the task list.  The flag \e bIsPeriphSG should be used to indicate
//! if the scatter-gather should be configured for a peripheral or memory
//! scatter-gather operation.
//!
//! \sa uDMATaskStructEntry
//!
//! \return None.
//*****************************************************************************
void
uDMAChannelScatterGatherSet(unsigned long ulChannelNum, unsigned ulTaskCount,
                            void *pvTaskList, unsigned long ulIsPeriphSG)
{
    tDMAControlTable *pControlTable;
    tDMAControlTable *pTaskTable;

    // Check the parameters
    ASSERT(ulChannelNum < 32);
    ASSERT(HWREG(UDMA_CTLBASE) != 0);
    ASSERT(pvTaskList != 0);
    ASSERT(ulTaskCount <= 1024);
    ASSERT(ulTaskCount != 0);

    // Get the base address of the control table.
    pControlTable = (tDMAControlTable *)HWREG(UDMA_CTLBASE);

    // Get a handy pointer to the task list
    pTaskTable = (tDMAControlTable *)pvTaskList;

    // Compute the ending address for the source pointer.  This will be the
    // last element of the last task in the task table
    pControlTable[ulChannelNum].pvSrcEndAddr =
        &pTaskTable[ulTaskCount - 1].ulSpare;

    // Compute the ending address for the destination pointer.  This will be
    // the end of the alternate structure for this channel.
    pControlTable[ulChannelNum].pvDstEndAddr =
        &pControlTable[ulChannelNum | UDMA_ALT_SELECT].ulSpare;

    // Compute the control word.  Most configurable items are fixed for
    // scatter-gather.  Item and increment sizes are all 32-bit and arb
    // size must be 4.  The count is the number of items in the task list
    // times 4 (4 words per task).
    pControlTable[ulChannelNum].ulControl =
        (UDMA_CHCTL_DSTINC_32 | UDMA_CHCTL_DSTSIZE_32 |
         UDMA_CHCTL_SRCINC_32 | UDMA_CHCTL_SRCSIZE_32 |
         UDMA_CHCTL_ARBSIZE_4 |
         (((ulTaskCount * 4) - 1) << UDMA_CHCTL_XFERSIZE_S) |
         (ulIsPeriphSG ? UDMA_CHCTL_XFERMODE_PER_SG :
          UDMA_CHCTL_XFERMODE_MEM_SG));
}

//*****************************************************************************
//! Gets the current transfer size for a uDMA channel control structure.
//!
//! \param ulChannelStructIndex is the logical OR of the uDMA channel number
//! with either \b UDMA_PRI_SELECT or \b UDMA_ALT_SELECT.
//!
//! This function is used to get the uDMA transfer size for a channel.  The
//! transfer size is the number of items to transfer, where the size of an item
//! might be 8, 16, or 32 bits.  If a partial transfer has already occurred,
//! then the number of remaining items will be returned.  If the transfer is
//! complete, then 0 will be returned.
//!
//! \return Returns the number of items remaining to transfer.
//*****************************************************************************
unsigned long
uDMAChannelSizeGet(unsigned long ulChannelStructIndex)
{
    tDMAControlTable *pControlTable;
    unsigned long ulControl;

    // Check the arguments.
    ASSERT(ulChannelStructIndex < 64);
    ASSERT(HWREG(UDMA_CTLBASE) != 0);

    // Get the base address of the control table.
    pControlTable = (tDMAControlTable *)HWREG(UDMA_CTLBASE);

    // Get the current control word value and mask off all but the size field
    // and the mode field.
    ulControl = (pControlTable[ulChannelStructIndex].ulControl &
                 (UDMA_CHCTL_XFERSIZE_M | UDMA_CHCTL_XFERMODE_M));

    // If the size field and mode field are 0 then the transfer is finished
    // and there are no more items to transfer
    if(ulControl == 0)
    {
        return(0);
    }

    // Otherwise, if either the size field or more field is non-zero, then
    // not all the items have been transferred.
    else
    {
        // Shift the size field and add one, then return to user.
        return((ulControl >> 4) + 1);
    }
}

//*****************************************************************************
//! Gets the transfer mode for a uDMA channel control structure.
//!
//! \param ulChannelStructIndex is the logical OR of the uDMA channel number
//! with either \b UDMA_PRI_SELECT or \b UDMA_ALT_SELECT.
//!
//! This function is used to get the transfer mode for the uDMA channel.  It
//! can be used to query the status of a transfer on a channel.  When the
//! transfer is complete the mode will be \b UDMA_MODE_STOP.
//!
//! \return Returns the transfer mode of the specified channel and control
//! structure, which will be one of the following values: \b UDMA_MODE_STOP,
//! \b UDMA_MODE_BASIC, \b UDMA_MODE_AUTO, \b UDMA_MODE_PINGPONG,
//! \b UDMA_MODE_MEM_SCATTER_GATHER, or \b UDMA_MODE_PER_SCATTER_GATHER.
//*****************************************************************************
unsigned long
uDMAChannelModeGet(unsigned long ulChannelStructIndex)
{
    tDMAControlTable *pControlTable;
    unsigned long ulControl;

    // Check the arguments.
    ASSERT(ulChannelStructIndex < 64);
    ASSERT(HWREG(UDMA_CTLBASE) != 0);

    // Get the base address of the control table.
    pControlTable = (tDMAControlTable *)HWREG(UDMA_CTLBASE);

    // Get the current control word value and mask off all but the mode field.
    ulControl = (pControlTable[ulChannelStructIndex].ulControl &
                 UDMA_CHCTL_XFERMODE_M);

    // Check if scatter/gather mode, and if so, mask off the alt bit.
    if(((ulControl & ~UDMA_MODE_ALT_SELECT) == UDMA_MODE_MEM_SCATTER_GATHER) ||
       ((ulControl & ~UDMA_MODE_ALT_SELECT) == UDMA_MODE_PER_SCATTER_GATHER))
    {
        ulControl &= ~UDMA_MODE_ALT_SELECT;
    }

    // Return the mode to the caller.
    return(ulControl);
}

//*****************************************************************************
//! Selects the default peripheral for a set of uDMA channels between 0-7.
//!
//! \param ulDefPeriphs is the logical or of the uDMA channels for which to
//! use the default peripheral, instead of the secondary peripheral.
//!
//! This function is used to select the default peripheral assignment for
//! a set of uDMA channels between 0-7.
//!
//! The parameter \e ulDefPeriphs can be the logical OR of any of the
//! following macros.
//!
//! - \b UDMA_CHAN0_DEF_USBEP1RX_M
//! - \b UDMA_CHAN1_DEF_USBEP1TX_M
//! - \b UDMA_CHAN2_DEF_USBEP2RX_M
//! - \b UDMA_CHAN3_DEF_USBEP2TX_M
//! - \b UDMA_CHAN4_DEF_USBEP3RX_M
//! - \b UDMA_CHAN5_DEF_USBEP3TX_M
//! - \b UDMA_CHAN6_DEF_ETH0RX_M
//! - \b UDMA_CHAN7_DEF_ETH0TX_M
//!
//! \return None.
//*****************************************************************************
void
uDMAChannel0_7SelectDefault(unsigned long ulDefPeriphs)
{
    // Select the default peripheral for the specified channels.
    HWREG(UDMA_CHMAP0) &= ~ulDefPeriphs;
}

//*****************************************************************************
//! Selects the alternate peripheral for a set of uDMA channels between 0-7.
//!
//! \param ulAltPeriphs is the logical or of the uDMA channels for which to
//! use an alternate peripheral, instead of the default peripheral.
//!
//! This function is used to select an alternate peripheral assignment for
//! a set of uDMA channels between 0-7.  By selecting the alternate peripheral
//! assignment for a channel, the default peripheral assignment is no longer
//! available for that channel.
//!
//! The parameter \e ulAltPeriphs can be the logical OR of any of the
//! following macros. For a single channel, only 1 of the alternate selections
//! can be included in the logical OR. (i.e. cannot OR both a "_SEC_"
//! peripheral selection and a "_THRD_" peripheral selection for the
//! same DMA channel.)
//!
//! - \b UDMA_CHAN0_SEC_UART2RX
//! - \b UDMA_CHAN1_SEC_UART2TX
//! - \b UDMA_CHAN2_SEC_TMR3A
//! - \b UDMA_CHAN3_SEC_TMR3B
//! - \b UDMA_CHAN4_SEC_TMR2A
//! - \b UDMA_CHAN5_SEC_TMR2B
//! - \b UDMA_CHAN6_SEC_TMR2A
//! - \b UDMA_CHAN7_SEC_TMR2B
//!
//! \return None.
//*****************************************************************************
void
uDMAChannel0_7SelectAltMapping (unsigned long ulAltPeriphs)
{
    // Select the alternate peripheral for the specified channels.
    HWREG(UDMA_CHMAP0) |= ulAltPeriphs;
}

//*****************************************************************************
//! Selects the default peripheral for a set of uDMA channels between 8-15.
//!
//! \param ulDefPeriphs is the logical or of the uDMA channels for which to
//! use the default peripheral, instead of the secondary peripheral.
//!
//! This function is used to select the default peripheral assignment for
//! a set of uDMA channels between 8-15.
//!
//! The parameter \e ulDefPeriphs can be the logical OR of any of the
//! following macros.
//!
//! - \b UDMA_CHAN8_DEF_UART0RX_M
//! - \b UDMA_CHAN9_DEF_UART0TX_M
//! - \b UDMA_CHAN10_DEF_SSI0RX_M
//! - \b UDMA_CHAN11_DEF_SSI0TX_M
//! - \b UDMA_CHAN12_DEF_SW_M
//! - \b UDMA_CHAN13_DEF_SW_M
//! - \b UDMA_CHAN14_DEF_RSVD_M
//! - \b UDMA_CHAN15_DEF_RSVD_M
//!
//! \return None.
//*****************************************************************************
void
uDMAChannel8_15SelectDefault(unsigned long ulDefPeriphs)
{
    // Select the default peripheral for the specified channels.
    HWREG(UDMA_CHMAP1) &= ~ulDefPeriphs;
}

//*****************************************************************************
//! Selects the alternate peripheral for a set of uDMA channels between 8-15.
//!
//! \param ulAltPeriphs is the logical or of the uDMA channels for which to
//! use an alternate peripheral, instead of the default peripheral.
//!
//! This function is used to select an alternate peripheral assignment for
//! a set of uDMA channels between 8-15.  By selecting the alternate peripheral
//! assignment for a channel, the default peripheral assignment is no longer
//! available for that channel.
//!
//! The parameter \e ulAltPeriphs can be the logical OR of any of the
//! following macros. For a single channel, only 1 of the alternate selections
//! can be included in the logical OR. (i.e. cannot OR both a "_SEC_"
//! peripheral selection and a "_THRD_" peripheral selection for the same
//! DMA channel.)
//!
//! - \b UDMA_CHAN8_SEC_UART1RX
//! - \b UDMA_CHAN9_SEC_UART1TX
//! - \b UDMA_CHAN10_SEC_SSI1RX
//! - \b UDMA_CHAN11_SEC_SSI1TX
//! - \b UDMA_CHAN12_SEC_UART2RX
//! - \b UDMA_CHAN12_THRD_SSI2RX
//! - \b UDMA_CHAN13_SEC_UART2TX
//! - \b UDMA_CHAN13_THRD_SSI2TX
//! - \b UDMA_CHAN14_SEC_TMR2A
//! - \b UDMA_CHAN14_THRD_SSI3RX
//! - \b UDMA_CHAN15_SEC_TMR2B
//! - \b UDMA_CHAN15_THRD_SSI3TX
//!
//! \return None.
//*****************************************************************************
void
uDMAChannel8_15SelectAltMapping (unsigned long ulAltPeriphs)
{
    // Select the alternate peripheral for the specified channels.
    HWREG(UDMA_CHMAP1) |= ulAltPeriphs;
}

//*****************************************************************************
//! Selects the default peripheral for a set of uDMA channels between 16-23.
//!
//! \param ulDefPeriphs is the logical or of the uDMA channels for which to
//! use the default peripheral, instead of the secondary peripheral.
//!
//! This function is used to select the default peripheral assignment for
//! a set of uDMA channels between 16-23.
//!
//! The parameter \e ulDefPeriphs can be the logical OR of any of the
//! following macros.
//!
//! - \b UDMA_CHAN16_DEF_RSVD_M
//! - \b UDMA_CHAN17_DEF_RSVD_M
//! - \b UDMA_CHAN18_DEF_TMR0A_M
//! - \b UDMA_CHAN19_DEF_TMR0B_M
//! - \b UDMA_CHAN20_DEF_TMR1A_M
//! - \b UDMA_CHAN21_DEF_TMR1B_M
//! - \b UDMA_CHAN22_DEF_UART1RX_M
//! - \b UDMA_CHAN23_DEF_UART1TX_M
//!
//! \return None.
//*****************************************************************************
void
uDMAChannel16_23SelectDefault(unsigned long ulDefPeriphs)
{
    // Select the default peripheral for the specified channels.
    HWREG(UDMA_CHMAP2) &= ~ulDefPeriphs;
}

//*****************************************************************************
//! Selects the alternate peripheral for a set of uDMA channels between 16-23.
//!
//! \param ulAltPeriphs is the logical or of the uDMA channels for which to
//! use an alternate peripheral, instead of the default peripheral.
//!
//! This function is used to select an alternate peripheral assignment for
//! a set of uDMA channels between 16-23.  By selecting the alternate
//! peripheral assignment for a channel, the default peripheral assignment
//! is no longer available for that channel.
//!
//! The parameter \e ulAltPeriphs can be the logical OR of any of the
//! following macros. For a single channel, only 1 of the alternate selections
//! can be included in the logical OR. (i.e. cannot OR both a "_SEC_"
//! peripheral selection and a "_THRD_" peripheral selection for the same
//! DMA channel.)
//!
//! - \b UDMA_CHAN16_THRD_UART3RX
//! - \b UDMA_CHAN17_THRD_UART3TX
//! - \b UDMA_CHAN18_SEC_TMR1A
//! - \b UDMA_CHAN18_THRD_UART4RX
//! - \b UDMA_CHAN19_SEC_TMR1B
//! - \b UDMA_CHAN19_THRD_UART4TX
//! - \b UDMA_CHAN20_SEC_EPI0RX
//! - \b UDMA_CHAN21_SEC_EPI0TX
//! - \b UDMA_CHAN22_SEC_SW
//! - \b UDMA_CHAN22_THRD_ADCINT1
//! - \b UDMA_CHAN23_SEC_SW
//! - \b UDMA_CHAN23_THRD_ADCINT2
//!
//! \return None.
//*****************************************************************************
void
uDMAChannel16_23SelectAltMapping (unsigned long ulAltPeriphs)
{
    // Select the alternate peripheral for the specified channels.
    HWREG(UDMA_CHMAP2) |= ulAltPeriphs;
}

//*****************************************************************************
//! Selects the default peripheral for a set of uDMA channels between 24-31.
//!
//! \param ulDefPeriphs is the logical or of the uDMA channels for which to
//! use the default peripheral, instead of the secondary peripheral.
//!
//! This function is used to select the default peripheral assignment for
//! a set of uDMA channels between 24-31.
//!
//! The parameter \e ulDefPeriphs can be the logical OR of any of the
//! following macros.
//!
//! - \b UDMA_CHAN24_DEF_SSI1RX_M
//! - \b UDMA_CHAN25_DEF_SSI1TX_M
//! - \b UDMA_CHAN26_DEF_SW_M
//! - \b UDMA_CHAN27_DEF_SW_M
//! - \b UDMA_CHAN28_DEF_RSVD_M
//! - \b UDMA_CHAN29_DEF_RSVD_M
//! - \b UDMA_CHAN30_DEF_SW_M
//! - \b UDMA_CHAN31_DEF_RSVD_M
//!
//! \return None.
//*****************************************************************************
void
uDMAChannel24_31SelectDefault(unsigned long ulDefPeriphs)
{
    // Select the default peripheral for the specified channels.
    HWREG(UDMA_CHMAP3) &= ~ulDefPeriphs;
}

//*****************************************************************************
//! Selects the alternate peripheral for a set of uDMA channels between 24-31.
//!
//! \param ulAltPeriphs is the logical or of the uDMA channels for which to
//! use an alternate peripheral, instead of the default peripheral.
//!
//! This function is used to select an alternate peripheral assignment for
//! a set of uDMA channels between 24-31.  By selecting the alternate
//! peripheral assignment for a channel, the default peripheral assignment
//! is no longer available for that channel.
//!
//! The parameter \e ulAltPeriphs can be the logical OR of any of the
//! following macros. For a single channel, only 1 of the alternate selections
//! can be included in the logical OR. (i.e. cannot OR both a "_SEC_"
//! peripheral selection and a "_THRD_" peripheral selection for the same
//! DMA channel.)
//!
//! - \b UDMA_CHAN26_THRD_ADCINT3
//! - \b UDMA_CHAN27_THRD_ADCINT4
//! - \b UDMA_CHAN28_SEC_SW
//! - \b UDMA_CHAN28_THRD_ADCINT5
//! - \b UDMA_CHAN29_SEC_SW
//! - \b UDMA_CHAN29_THRD_ADCINT6
//! - \b UDMA_CHAN30_SEC_SW
//! - \b UDMA_CHAN30_THRD_ADCINT7
//! - \b UDMA_CHAN31_THRD_ADCINT8
//!
//! \return None.
//*****************************************************************************
void
uDMAChannel24_31SelectAltMapping (unsigned long ulAltPeriphs)
{
    // Select the alternate peripheral for the specified channels.
    HWREG(UDMA_CHMAP3) |= ulAltPeriphs;
}

//*****************************************************************************
//! Registers an interrupt handler for the uDMA controller.
//!
//! \param ulIntChannel identifies which uDMA interrupt is to be registered.
//! \param pfnHandler is a pointer to the function to be called when the
//! interrupt is activated.
//!
//! This sets and enables the handler to be called when the uDMA controller
//! generates an interrupt.  The \e ulIntChannel parameter should be one of the
//! following:
//!
//! - \b UDMA_INT_SW to register an interrupt handler to process interrupts
//!   from the uDMA software channel (UDMA_CHANNEL_SW)
//! - \b UDMA_INT_ERR to register an interrupt handler to process uDMA error
//!   interrupts
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \note The interrupt handler for uDMA is for transfer completion when the
//! channel UDMA_CHANNEL_SW is used, and for error interrupts.  The
//! interrupts for each peripheral channel are handled through the individual
//! peripheral interrupt handlers.
//!
//! \return None.
//*****************************************************************************
void
uDMAIntRegister(unsigned long ulIntChannel, void (*pfnHandler)(void))
{
    // Check the arguments.
    ASSERT(pfnHandler);
    ASSERT((ulIntChannel == UDMA_INT_SW) || (ulIntChannel == UDMA_INT_ERR));

    // Register the interrupt handler.
    IntRegister(ulIntChannel, pfnHandler);

    // Enable the memory management fault.
    IntEnable(ulIntChannel);
}

//*****************************************************************************
//! Unregisters an interrupt handler for the uDMA controller.
//!
//! \param ulIntChannel identifies which uDMA interrupt to unregister.
//!
//! This function will disable and clear the handler to be called for the
//! specified uDMA interrupt.  The \e ulIntChannel parameter should be one of
//! \b UDMA_INT_SW or \b UDMA_INT_ERR as documented for the function
//! uDMAIntRegister().
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//*****************************************************************************
void
uDMAIntUnregister(unsigned long ulIntChannel)
{
    // Disable the interrupt.
    IntDisable(ulIntChannel);

    // Unregister the interrupt handler.
    IntUnregister(ulIntChannel);
}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************




