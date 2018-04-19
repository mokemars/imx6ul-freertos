//###########################################################################
// FILE:   ram.c
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
//! \addtogroup ram_api
//! @{
//*****************************************************************************
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_cram.h"
#include "inc/hw_ipc.h"
#include "inc/hw_ram.h"
#include "inc/hw_sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/ipc.h"
#include "driverlib/ram.h"

//*****************************************************************************
//! M3 master R/W/Exe Access to Shared SARAM.
//!
//! \param ulMask specifies the 32-bit mask for the SxMSEL RAM control register
//! to indicate which Sx SARAM blocks the C28 is requesting master access to.
//! \param usMaster specifies whether the C28 or M3 are given master access to
//! the Sx blocks.
//!
//! This function will allow the M3 master system to configure master R/W/Exe
//! access to the Sx SARAM blocks specified by the /e ulMask parameter.
//! The  \e usMaster parameter can be either: \b IPC_SX_C28MASTER
//! or \b IPC_SX_M3MASTER.
//! The \e ulMask parameter can be any of the options:
//! \b S0_ACCESS - \b S7_ACCESS.
//
//*****************************************************************************
void
RAMMReqSharedMemAccess(unsigned long ulMask, unsigned short usMaster)
{
    static unsigned long ulMWRAllow;
    ulMWRAllow = HWREG(SYSCTL_MWRALLOW);
    HWREG(SYSCTL_MWRALLOW) = 0xA5A5A5A5;
    if (usMaster == SX_M3MASTER)
    {
        HWREG(RAM_CONFIG_BASE + RAM_O_MSXMSEL) &= ~ulMask;
    }
    else if (usMaster == SX_C28MASTER)
    {
        HWREG(RAM_CONFIG_BASE + RAM_O_MSXMSEL) |= ulMask;
    }
    HWREG(SYSCTL_MWRALLOW) = ulMWRAllow;
}

//*****************************************************************************
//! Initialize Control Subsystem M1 and MsgRAM memories
//!
//! This function will zero initialize the M1 and message RAM memories of the 
//! control subsytem from the master subsystem.  This initialization should
//! occur before a boot command is sent to the C-Boot ROM.  IPC lite
//! drivers are used to do this initialization through the IPC.
//!
//! \return 1 on failure, 0 on success
//
//*****************************************************************************
unsigned char
RAMControlInitM1MsgRam(void)
{
    unsigned int ii = 0;
    //RAM INIT for M1, and CTOM MsgRAM - M0 RAM INIT is done by C-BootROM
    IPCLiteMtoCSetBits_Protected(IPC_FLAG1,
                                 CRAM_CRTESTINIT,
                                (CRTESTINIT_RAMINIT_M1 |
                                 CRTESTINIT_RAMINIT_CTOMMSGRAM),
                                 IPC_LENGTH_32_BITS, IPC_FLAG32);

    //wait until C-BootROM acks
    while(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG1) ;

    //CHECK IF pass or fail
    if(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG32)
    {   
        //still set - so command failed.
        return 1;

    }

    //READ RAM_INIT_DONE register check
    do
    {
        IPCLiteMtoCDataRead(IPC_FLAG1,
                            CRAM_CRINITDONE,
                            IPC_LENGTH_32_BITS,
                            IPC_FLAG32);

        //wait until C-BootROM acks
        while(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG1) ;

        //CHECK IF pass or fail
        if(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG32)
        {   
            //still set - so command failed.
            return 1;

        }
        else
        {
            if(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAR) & 
              (CRINITDONE_RAMINITDONE_M1 | CRINITDONE_RAMINITDONE_CTOMMSGRAM))
            {
                //RAM_INIT completed
                return 0;
            }
            else
            {
                //RAM_INIT not done yet, so wait for more time and read ram 
                //init done register.
                //GIVE some cycles delay until CCORE performs RAM INIT
                for(ii =0 ; ii < 2048; ii++) ;
                    continue;
            }
        }
    } while(1);      //do-loop-end

}

//*****************************************************************************
//! Initialize Control Subsystem L0 through L3 memories
//!
//! This function will zero initialize the L0 through L3 RAM memories of the 
//! control subsytem from the master subsystem.  This initialization should
//! occur before a boot command is sent to the C-Boot ROM.  IPC lite
//! drivers are used to do this initialization through the IPC.
//!
//! \return 1 on failure, 0 on success
//
//*****************************************************************************
unsigned char
RAMControlInitL0L3Ram(void)
{
    unsigned int ii = 0;

    //RAM INIT for L0,L1,L2,L3
    IPCLiteMtoCSetBits_Protected(IPC_FLAG1,
                                 CRAM_CLxRTESTINIT1,
                                (CLxRTESTINIT1_RAMINIT_L0| CLxRTESTINIT1_RAMINIT_L1|
                                 CLxRTESTINIT1_RAMINIT_L2| CLxRTESTINIT1_RAMINIT_L3),
                                 IPC_LENGTH_32_BITS, IPC_FLAG32);

    //wait until C-BootROM acks
    while(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG1) ;

    //CHECK IF pass or fail
    if(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG32)
    {           
        //still set - so command failed.
        return 1;
    }

    //WAIT for RAM_INIT done of above memories
    do
    {
        IPCLiteMtoCDataRead(IPC_FLAG1,
                            CRAM_CLxRINITDONE,
                            IPC_LENGTH_32_BITS,
                            IPC_FLAG32);

        //wait until C-BootROM acks
        while(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG1) ;

        //CHECK IF pass or fail
        if(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCFLG) & IPC_FLAG32)
        {                      
            //still set - so command failed.
            return 1;
        }
        else
        {
            if(HWREG(MTOCIPC_BASE + IPC_O_MTOCIPCDATAR) &
               (CLxRINITDONE_RAMINITDONE_L0| CLxRINITDONE_RAMINITDONE_L1|
                CLxRINITDONE_RAMINITDONE_L2| CLxRINITDONE_RAMINITDONE_L3 ))
            {
                //RAM_INIT completed 
                return 0;
            }
            else
            {
                //RAM_INIT not done yet, so wait for more time and read ram
                //init done register.
                //GIVE some cycles delay until CCORE performs RAM INIT
                for(ii =0 ; ii < 2048; ii++) ;
                    continue;
            }
        }
    } while(1);


}



//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************




