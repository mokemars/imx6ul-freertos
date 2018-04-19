//###########################################################################
// FILE:   pbist.c
// TITLE:  Driver for the PBIST controller
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

#include "inc/hw_pbist.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pbist.h"


//*****************************************************************************
//!
//! Enables the clocking to the PBIST controller,
//! enabling access to PBIST Control registers.
//!
//*****************************************************************************
void PBISTInit(void)
{
	// Enable clock to PBIST, Turning it on
	 HWREG(PBIST_BASE + PBIST_PACT) = 0x1;

	// Even if you plan to override ALGO and RINFO
	// PBIST_OVER must be set to 0x9 first before setting to 1
	HWREG(PBIST_BASE + PBIST_OVER) = 0x9;
}


//*****************************************************************************
//!
//! Enables the clocking to the PBIST controller,
//! enabling access to PBIST Control registers.
//!
//*****************************************************************************
void PBISTDisable(void)
{
	// Disable clock to PBIST, Turning it off
	 HWREG(PBIST_BASE + PBIST_PACT) = 0x0;
}

//*****************************************************************************
//! Selects the Algorthim(s) to be run on the device.
//!
//! \param ulALGO specifies the the Algorithms to be run
//! 	Valid values are found inc/pbist.h
//!
//! \return None.
//*****************************************************************************
void PBISTAlgoSelect(unsigned long ulALGO)
{
	HWREG(PBIST_BASE + PBIST_ALGO) = ulALGO;
}

//*****************************************************************************
//! Selects the Memory Groups which the Algorithm will run on.
//!
//! \param ulRINFOL specifies the the desired memory groups.
//! 	Valid values are found inc/pbist.h
//!
//! \return None.
//*****************************************************************************
void PBISTRinfoLSelect(unsigned long ulRINFOL)
{
	HWREG(PBIST_BASE + PBIST_RINFOL) = ulRINFOL;
}

//*****************************************************************************
//! Selects the Memory Groups which the Algorithm will run on.
//!
//! \param ulRINFOU specifies the the desired memory groups.
//! 	Valid values are found inc/pbist.h
//!
//! \return None.
//*****************************************************************************
void PBISTRinfoUSelect(unsigned long ulRINFOU)
{
	HWREG(PBIST_BASE + PBIST_RINFOU) = ulRINFOU;
}

//*****************************************************************************
//! Overrides the current ALGO and RINFOL/RINFOU registers
//!
//! \param bOVER specifies if the ALGO and RINFOL?RINFU must be overwritten
//! 	1 = Overwrite both ALGO and RINFOL/RINFOU
//! 	0 = Overwrite ONLY ALGO
//!
//! \return None.
//*****************************************************************************
void PBISTOverride(tBoolean bOVER)
{
	if(bOVER)
		HWREG(PBIST_BASE + PBIST_OVER)= 0x0;
	else
		HWREG(PBIST_BASE + PBIST_OVER) = 0x1;
}

//*****************************************************************************
//! Starts the PBIST tests
//!
//! \return None.
//*****************************************************************************
void PBISTStart(void)
{
	// Write to the DLR register to kick off the PBIST execution
	HWREG(PBIST_BASE+PBIST_DLR)  = 0x21C;
}


//*****************************************************************************
//! Check if the PBIST is stopped due to a failure
//!
//! \return
//! 	true: There was a failure
//!		false: There are no failures
//*****************************************************************************
tBoolean PBISTFailureStatus(void)
{
	// check the Fail Statuse bits for both Port 0 and Port 1
	// if one or the other is not 0, there was a failure.
	if((HWREG(PBIST_BASE + PBIST_FSRF0) != 0) ||
			(HWREG(PBIST_BASE+PBIST_FSRF1) !=0))
		return true;
	else
		return false;
}

//*****************************************************************************
//! Stop the PBIST where it is. Since the PBIST pauses wherever it finds
//! a falilure, you may choose to stop testing for other failures.
//!
//! \return: None
//*****************************************************************************
void PBISTStop(void)
{
	// Stop the PBIST if where it is stopped.
	HWREG(PBIST_BASE + PBIST_STR) = 0x4;
	HWREG(PBIST_BASE+PBIST_DLR)  = 0x218;
}
//*****************************************************************************
//!	Continue the PBIST testing. Since the PBIST pauses wherever it finds
//! a falilure, you may choose to continue testing for other failures
//!
//! \return: None
//*****************************************************************************
void PBISTResume(void)
{
	// Check the Fail Statuse bits for both Port 0 and Port 1
	// if one or the other is not 0, there was a failure.
	HWREG(PBIST_BASE + PBIST_STR) = 0x2;
}
