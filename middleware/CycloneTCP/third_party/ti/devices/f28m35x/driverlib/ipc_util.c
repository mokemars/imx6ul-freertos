//###########################################################################
// FILE:   ipc_util.c
// TITLE:  Driver for the IPC Module Utility Functions.
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
//! \addtogroup ipc_util_api_m3
//! @{
//*****************************************************************************

#include "inc/hw_ipc.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ram.h"
#include "inc/hw_sysctl.h"
#include "driverlib/ipc.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/ram.h"
#include "driverlib/sysctl.h"

//*****************************************************************************
//! M3 Acknowledges CtoM IPC Flag.
//!
//! \param ulFlags specifies the IPC flag mask for flags being acknowledged.
//!
//! This function will allow the M3 master system to acknowledge/clear the IPC
//! flag set by the C28 control system. The \e ulFlags parameter can be any of
//! the IPC flag values: \b IPC_FLAG1 - \b IPC_FLAG32.
//!
//! \return None.
//*****************************************************************************
void
IPCCtoMFlagAcknowledge (unsigned long ulFlags)
{
    HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= ulFlags;
}

//*****************************************************************************
//! Determines whether the given CtoM IPC flags are busy or not.
//!
//! \param ulFlags specifies CtoM IPC Flag number masks to check the status of.
//!
//! Allows the caller to determine whether the designated IPC flags are
//! pending. The \e ulFlags parameter can be any of the IPC flag values:
//! \b IPC_FLAG1 - \b IPC_FLAG32.
//!
//! \return Returns \b 1 if the IPC flags are busy or \b 0 if designated
//! IPC flags are free.
//*****************************************************************************
unsigned short
IPCCtoMFlagBusy (unsigned long ulFlags)
{
    unsigned short returnStatus;
    
    if ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & ulFlags)==0)
    {
        returnStatus = 0U;
    }
    else
    {
        returnStatus = 1U;
    }
    
    return returnStatus;
}

//*****************************************************************************
//! Determines whether the given MtoC IPC flags are busy or not.
//!
//! \param ulFlags specifies MtoC IPC Flag number masks to check the status of.
//!
//! Allows the caller to determine whether the designated IPC flags are
//! available for further master to control system communication. If \b 0 is
//! returned, then all designated tasks have completed and are available.
//! The \e ulFlags parameter can be any of the IPC flag values:
//! \b IPC_FLAG1 - \b IPC_FLAG32.
//!
//! \return Returns \b 1 if the IPC flags are busy or \b 0 if designated
//! IPC flags are free.
//*****************************************************************************
unsigned short
IPCMtoCFlagBusy (unsigned long ulFlags)
{
    unsigned short returnStatus;
    
    if ((HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & ulFlags)==0)
    {
        returnStatus = 0U;        
    }
    else
    {
        returnStatus = 1U;        
    }
    
    return returnStatus;
}

//*****************************************************************************
//! M3 Sets MtoC IPC Flag
//!
//! \param ulFlags specifies the IPC flag mask for flags being set.
//!
//! This function will allow the M3 master system to set the designated IPC
//! flags to send to the C28 control system. The \e ulFlags parameter can be
//! any of the IPC flag values: \b IPC_FLAG1 - \b IPC_FLAG32.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCFlagSet (unsigned long ulFlags)
{
    HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= ulFlags;
}

//*****************************************************************************
//! M3 Clears MtoC IPC Flag
//!
//! \param ulFlags specifies the IPC flag mask for flags being set.
//!
//! This function will allow the M3 master system to set the designated IPC
//! flags to send to the C28 control system.  The \e ulFlags parameter can be
//! any of the IPC flag values: \b IPC_FLAG1 - \b IPC_FLAG32.
//!
//! \return None.
//*****************************************************************************
void
IPCMtoCFlagClear (unsigned long ulFlags)
{
    HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCLR) |= ulFlags;
}

//*****************************************************************************
//! Converts a M3 shared RAM address to its C28 equivalent address
//!
//! \param ulShareAddress specifies the M3 address in shared RAM (Sx RAM
//! or MSG RAM).
//!
//! This function will convert a shared RAM address location on the M3 master
//! system to its equivalent C28 control system address (applies to MSG RAM's
//! and Sx shared RAM blocks).
//!
//! \return C28 equivalent address.
//*****************************************************************************
unsigned long
IPCMtoCSharedRamConvert (unsigned long ulShareAddress)
{
    unsigned long returnStatus;
    
    // MSG RAM address conversion
    if (ulShareAddress >= M3_CTOMMSGRAM_START)
    {
        returnStatus = ((ulShareAddress - 0x20000000) >> 1);
    }
    // Sx RAM address conversion
    else
    {
        returnStatus = ((ulShareAddress - 0x1FFF0000) >> 1);
    }
    
    return returnStatus;
}

//*****************************************************************************
//! Executes a C28 control system bootloader.
//!
//! \param ulBootMode specifies which C28 control system boot mode to execute.
//!
//! This function will allow the M3 master system to boot the C28 control
//! system via the following modes: Boot to RAM, Boot to Flash, Boot via SPI,
//! SCI, I2C, or parallel I/O. Unlike other IPCLite driver functions, this
//! function blocks and waits until the control system boot ROM is configured
//! and ready to receive MTOCIPC INT1 interrupts. It then blocks and waits
//! until IPC INT1 and IPC FLAG32 are available in the C28 boot ROM prior to
//! sending the command to execute the selected bootloader. The \e
//! ulBootMode parameter accepts one of the following values: \b
//! CBROM_MTOC_BOOTMODE_COMMAND_ILLEGAL, \b
//! CBROM_MTOC_BOOTMODE_BOOT_FROM_RAM, \b
//! CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH, \b
//! CBROM_MTOC_BOOTMODE_BOOT_FROM_SCI, \b CBROM_MTOC_BOOTMODE_BOOT_FROM_SPI,
//! \b CBROM_MTOC_BOOTMODE_BOOT_FROM_I2C, \b
//! CBROM_MTOC_BOOTMODE_BOOT_FROM_PARALLEL.
//!
//! \return 0 (success) if command is sent, or 1 (failure) if boot mode is
//! invalid and command was not sent.
//*****************************************************************************
unsigned short
IPCMtoCBootControlSystem(unsigned long ulBootMode)
{
    unsigned short returnStatus = STATUS_PASS;
    
    // Wait until C28 control system boot ROM is ready to receive MTOCIPC INT1
    // interrupts
    while ((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCBOOTSTS) &
            CBROM_BOOTSTS_CTOM_CONTROL_SYSTEM_READY)!=
           CBROM_BOOTSTS_CTOM_CONTROL_SYSTEM_READY)
    {
    }

    //If the device is F28M35x
    if(((HWREG(SYSCTL_DID0) & SYSCTL_DID0_CLASS_M) >>  SYSCTL_DID0_CLASS_S) == 0x40)
    {
    	// and it is a rev 0 device
    	if((HWREG(SYSCTL_DID0) & SYSCTL_DID0_REVID_M) == 0)
    	{
			//Initialize Control Subsystem memories
			if(RAMControlInitM1MsgRam())
            {
				returnStatus = STATUS_FAIL;
            }
			if(RAMControlInitL0L3Ram())
            {
				returnStatus = STATUS_FAIL;
            }
    	}
    }
    
    // Continue only if initialization of control subsystem memories 
    // was successful
    if (returnStatus != STATUS_FAIL)
    {
        // Loop until C28 control system IPC flags 1 and 32 are available
        while (HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & (IPC_FLAG1 | IPC_FLAG32))
        {
        }

        if (ulBootMode >= CBROM_MTOC_BOOTMODE_BOOT_COMMAND_MAX_SUPPORT_VALUE)
        {
            returnStatus = STATUS_FAIL;
        }
        else
        {
            // Based on boot mode, enable pull-ups on peripheral pins and give GPIO pin
            // control to C28x control system.
            switch (ulBootMode)
            {
                case CBROM_MTOC_BOOTMODE_BOOT_FROM_SCI:
                // SCITXDA ->    PF3_GPIO35
                // SCIRXDA ->    PF4_GPIO36
                    HWREG(GPIO_PORTF_BASE + GPIO_O_PUR) |= (GPIO_PIN_3 | GPIO_PIN_4);
                    HWREG(GPIO_PORTF_BASE + GPIO_O_CSEL) |= (GPIO_PIN_3 | GPIO_PIN_4);
                    break;
                case CBROM_MTOC_BOOTMODE_BOOT_FROM_LOOP:
                    ulBootMode = CBROM_MTOC_BOOTMODE_BOOT_FROM_SCI;
                    break;
                case CBROM_MTOC_BOOTMODE_BOOT_FROM_SPI:
                // SPISIMOA ->    PD0_GPIO16
                // SPISOMIA ->    PD1_GPIO17
                // SPICLKA ->    PD2_GPIO18
                // SPISTEA    ->  PD3_GPIO19
                    HWREG(GPIO_PORTD_BASE + GPIO_O_PUR) |=
                         (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
                    HWREG(GPIO_PORTD_BASE + GPIO_O_CSEL) |=
                         (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
                    break;
                case CBROM_MTOC_BOOTMODE_BOOT_FROM_I2C:
                // SDAA ->    PF0_GPIO32
                // SCLA ->    PF1_GPIO33
                    HWREG(GPIO_PORTF_BASE + GPIO_O_PUR) |= (GPIO_PIN_0 | GPIO_PIN_1);
                    HWREG(GPIO_PORTF_BASE + GPIO_O_CSEL) |= (GPIO_PIN_0 | GPIO_PIN_1);
                    break;
                case CBROM_MTOC_BOOTMODE_BOOT_FROM_PARALLEL:
                    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
                    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
                    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

                    // GPIO[7:6] ->    PB1:0_GPIO1:0
                    // GPIO[5:0] ->    PA5:0_GPIO5:0
                    HWREG(GPIO_PORTA_BASE + GPIO_O_PUR) |=
                        (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5);
                    HWREG(GPIO_PORTB_BASE + GPIO_O_PUR) |= (GPIO_PIN_0 | GPIO_PIN_1);
                    HWREG(GPIO_PORTA_BASE + GPIO_O_CSEL) |=
                        (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5);
                    HWREG(GPIO_PORTB_BASE + GPIO_O_CSEL) |= (GPIO_PIN_0 | GPIO_PIN_1);
                    // DSP_CTL ->  PE2_GPIO26
                    // HOST_CTL -> PE3_GPIO27
                    HWREG(GPIO_PORTE_BASE + GPIO_O_PUR) |= (GPIO_PIN_2 | GPIO_PIN_3);
                    HWREG(GPIO_PORTE_BASE + GPIO_O_CSEL) |= (GPIO_PIN_2 | GPIO_PIN_3);
                    break;
            }

            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) = IPC_MTOC_EXECUTE_BOOTMODE_CMD;
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCBOOTMODE) = ulBootMode;

            // Force IPC event on selected request task and enable status-checking.
            HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= (IPC_FLAG1 | IPC_FLAG32); 
        }
           
    }

    return returnStatus;
}

//*****************************************************************************
// Function to synchronize the CPUs
//*****************************************************************************
void IpcSync(volatile unsigned int flag)
{
	//Synchronize the two CPUs. Neither CPU will return from this function
	//call before the other one enters it. Must be called with the same
	//flag number on both CPUs.

	//	Set MTOC IPC Flag
	HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= flag;

	//	Wait for CTOM IPC Flag
	while((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & flag) == 0) {}

	//	Ack CTOM IPC Flag
	HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= flag;

	//	Wait for MTOC IPC Flag Ack
	while(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & flag) {}
}

//*****************************************************************************
// Function to send command to C28 from M3
//*****************************************************************************
void SendIpcCommand(unsigned long int command, unsigned long int address, 
                    unsigned long int data, unsigned long int flag)
{
    HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCCOM) =  command;
    HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCADDR) =  address;   
    HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAW) =  data; 
    
    if (flag != NO_FLAG)
    {
        SendIpcFlag(flag);
    }
}

//*****************************************************************************
// Function to ACK CTOM IPC Flag
//*****************************************************************************
void AckIpcFlag(unsigned long int flag)
{
	HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= flag;
	return;
}

//*****************************************************************************
// Function to poll for CTOMIPCSTS from C28
//*****************************************************************************
void WaitForIpcFlag(unsigned long int flag)
{
	while((HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCSTS) & flag) == 0) {}
	HWREG(MTOCIPC_BASE + IPC_O_CTOMIPCACK) |= flag;
	return;
}

//*****************************************************************************
// Function to poll for MTOCIPCFLG
//*****************************************************************************
void WaitForIpcAck(unsigned long int flag)
{
	while((HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & flag) == 0) {}
	return;
}

//*****************************************************************************
// Function to set MTOC_IPCSET register
//*****************************************************************************
void SendIpcFlag(unsigned long int flag)
{
	HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCSET) |= flag;
	return;
}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************





