//###########################################################################
// FILE:   dcsm.c
// TITLE:  M3 Driver for the DCSM security module.
//         
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
//! \addtogroup dcsm_api
//! @{
//*****************************************************************************

#include "inc/hw_dcsm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/dcsm.h"
#include "driverlib/debug.h"
//*****************************************************************************
//! Allows access to flash sectors by reading zone select locations.
//!
//! This function reads zone 1 and zone 2 GRABSECTCR registers, allowing access
//! to flash memory sectors.
//!
//! \return None
//
//*****************************************************************************
void DCSMGrabFlashSectors(void)
{
    volatile unsigned long temp;
    temp = HWREG(FLASH_BASE + ZONE1_GRABSECT_OFFSET);
    temp = HWREG(FLASH_BASE + ZONE2_GRABSECT_OFFSET);
}

//*****************************************************************************
//! Allows access to RAM blocks by reading zone select locations.
//!
//! This function reads zone 1 and zone 2 GRABRAMR flash locations , allowing 
//! access to RAM memory blocks.
//!
//! \return None
//
//*****************************************************************************
void DCSMGrabRAMBlocks(void)
{
    volatile unsigned long temp;
    temp = HWREG(FLASH_BASE + ZONE1_GRABRAM_OFFSET);
    temp = HWREG(FLASH_BASE + ZONE2_GRABRAM_OFFSET);
}

//*****************************************************************************
//! Loads M3-Z1-EXEONLY register
//!
//! This function reads M3_Z1_EXEONLY flash location transfering the contents
//! of this location to M3_Z1_EXEONLY register
//! 
//! \return None
//
//*****************************************************************************
void DCSMReadZone1EXEONLY(void)
{
    volatile unsigned long temp;
    temp = HWREG(FLASH_BASE + ZONE1_EXEONLY_OFFSET);
    
}

//*****************************************************************************
//! Loads M3-Z2-EXEONLY register
//!
//! This function reads M3_Z2_EXEONLY flash location transfering the contents
//! of this location to M3_Z2_EXEONLY register
//! 
//! \return None
//
//*****************************************************************************
void DCSMReadZone2EXEONLY(void)
{
    volatile unsigned long temp;
    temp = HWREG(FLASH_BASE + ZONE2_EXEONLY_OFFSET);
    
}

//*****************************************************************************
//! Unlocks Zone 1 CSM.
//!
//! \param psCMDKey is a pointer to the CSMPSWDKEY struct that has the CSM 
//!  password for zone 1.
//!
//! This function unlocks the CSM password. It first reads the
//! four password locations in the flash. If any of the password values is 
//! different from 0xFFFFFFFF, it unlocks the device by writing the provided 
//! passwords into CSM Key registers
//!
//!  \return None.
//
//*****************************************************************************
void DCSMUnlockZone1CSM(CSMPSWDKEY *psCMDKey)
{
    
    if((HWREG(FLASH_BASE)  &
       HWREG(FLASH_BASE + ZONE1_CSMPSWD1_OFFSET) &
       HWREG(FLASH_BASE + ZONE1_CSMPSWD2_OFFSET) &
       HWREG(FLASH_BASE + ZONE1_CSMPSWD3_OFFSET)) != 0xFFFFFFFF)
    {
        HWREG(CSM_BASE) = psCMDKey->csmkey0;
        HWREG(CSM_BASE + DCSM_O_Z1_CSMKEY1) = psCMDKey->csmkey1;
        HWREG(CSM_BASE + DCSM_O_Z1_CSMKEY2) = psCMDKey->csmkey2;
        HWREG(CSM_BASE + DCSM_O_Z1_CSMKEY3) = psCMDKey->csmkey3;    
       
    }

}

//*****************************************************************************
//! Unlocks Zone 1 ECSL.
//!
//! \param psECSLKey is a pointer to the ESCLPSWDKEY struct that has 
//! the ECSL password for zone 1.
//!
//! This function unlocks the ECSL password. It first reads 
//! the two password locations in the flash. If any of the password values 
//! is different from 0xFFFFFFFF, it unlocks the device by writing the 
//! provided passwords into ECSL Key registers
//!
//!    \return None.
//
//*****************************************************************************
void DCSMUnlockZone1ECSL(ECSLPSWDKEY *psECSLKey)
{
        
    if((HWREG(FLASH_BASE + ZONE1_ECSLPSWD0_OFFSET) &
        HWREG(FLASH_BASE + ZONE1_ECSLPSWD1_OFFSET)) != 0xFFFFFFFF)
    {
        HWREG(CSM_BASE + DCSM_O_Z1_ECSLKEY0 ) = psECSLKey->ecslkey0;
        HWREG(CSM_BASE + DCSM_O_Z1_ECSLKEY1 ) = psECSLKey->ecslkey1;
    }
    
}

//*****************************************************************************
//! Unlocks Zone 2 CSM.
//!
//! \param psCMDKey is a pointer to the CSMPSWDKEY that has the CSM 
//!  password for zone 2.
//!
//! This function unlocks the CSM password. It first reads 
//! the four password locations in the flash. If any of the password values 
//! is different from 0xFFFFFFFF, it unlocks the device by writing the 
//! provided passwords into CSM Key registers
//!
//! \return None.
//
//*****************************************************************************
void DCSMUnlockZone2CSM(CSMPSWDKEY *psCMDKey)
{
    
    // Read the CSM password locations to load password from flash to registers
    if((HWREG(FLASH_BASE + ZONE2_CSMPSWD0_OFFSET) &
        HWREG(FLASH_BASE + ZONE2_CSMPSWD1_OFFSET) &
        HWREG(FLASH_BASE + ZONE2_CSMPSWD2_OFFSET) &
        HWREG(FLASH_BASE + ZONE2_CSMPSWD3_OFFSET)) != 0xFFFFFFFF)    
    {
        // If password differs from 0xFFFFFFFF, unlock by writing the correct
        // password
        HWREG(CSM_BASE + DCSM_O_Z2_CSMKEY0) = psCMDKey->csmkey0;
        HWREG(CSM_BASE + DCSM_O_Z2_CSMKEY1) = psCMDKey->csmkey1;
        HWREG(CSM_BASE + DCSM_O_Z2_CSMKEY2) = psCMDKey->csmkey2;
        HWREG(CSM_BASE + DCSM_O_Z2_CSMKEY3) = psCMDKey->csmkey3;    
    }
    
}

//*****************************************************************************
//! Unlocks Zone 2 ECSL.
//!
//! \param psECSLKey is a pointer to the ESCLPSWDKEY struct that has 
//! the ECSL password for zone 2.
//!
//! This function unlocks the ECSL password. It first reads 
//! the two password locations in the flash. If any of the password values 
//! is different from 0xFFFFFFFF, it unlocks the device by writing the 
//! provided passwords into ECSL Key registers
//!
//! \return None. 
//
//*****************************************************************************
void DCSMUnlockZone2ECSL(ECSLPSWDKEY *psECSLKey)
{
    
    // Read the ECSL password locations to load password from flash to
    // registers
    if((HWREG(FLASH_BASE + ZONE2_ECSLPSWD0_OFFSET) &
        HWREG(FLASH_BASE + ZONE2_ECSLPSWD1_OFFSET)) != 0xFFFFFFFF)
    {
        // If password differs from 0xFFFFFFFF, unock by writing the correct
        // password
        HWREG(CSM_BASE + DCSM_O_Z2_ECSLKEY0 ) = psECSLKey->ecslkey0;
        HWREG(CSM_BASE + DCSM_O_Z2_ECSLKEY1 ) = psECSLKey->ecslkey1;
    }
    
}

//*****************************************************************************
//! Secures zone 1 by setting the FORCESEC bit of M3_Z1_CR register
//!
//! This function locks and enables reloading of CSM and ECSL lock registers 
//! by setting the FORCESEC bit (bit 15 of M3_Z1_CR register). 
//!
//!  \return None
//
//*****************************************************************************
void DCSMSecureZone1(void)
{
    HWREG(CSM_BASE + DCSM_O_Z1_CR)|= 0x00008000;
}

//*****************************************************************************
//! Secures zone 2 by setting the FORCESEC bit of M3_Z2_CR register
//!
//! This function locks and enables reloading of CSM and ECSL lock registers 
//! by setting the FORCESEC bit (bit 15 of M3_Z2_CR register). 
//! 
//!  \return None
//
//*****************************************************************************
void DCSMSecureZone2(void)
{
    HWREG(CSM_BASE + DCSM_O_Z2_CR)|= 0x00008000;
}

//*****************************************************************************
//! Returns the EXE-ONLY status of zone 1 for a flash sector
//!
//! \param sector is the flash sector value. Valid ranges are 0 for sector 0 
//!     through 12 for sector 12.
//!
//! This function takes in a valid sector value and returns the status of EXE 
//! ONLY security protection for the sector.
//! 
//! \return Returns 0 if the sector is not EXE-ONLY protected ,1 if the 
//!         sector is EXE-ONLY protected.
//
//*****************************************************************************
unsigned long DCSMGetZone1EXEstatus(unsigned long sector)
{
    
    ASSERT((sector >= 0) && (sector < 13 ));
    
    DCSMReadZone1EXEONLY();
    if(((HWREG(CSM_BASE + DCSM_O_Z1_EXEONLYR) >> sector) & 0x00000001) == 1)
    {
        return(1);
    }
    else
    {
        return(0);
    }
    
}

//*****************************************************************************
//! Returns the EXE-ONLY status of zone 2 for a flash sector
//!
//! \param sector is the flash sector value. Valid ranges are 1 for sector 1 
//!     through 13 for sector 13.
//!
//! This function takes in a valid sector value and returns the status of EXE 
//! ONLY security protection for the sector.
//! 
//! \return Returns 0 if the sector is not EXE-ONLY protected ,1 if the 
//!         sector is EXE-ONLY protected.
//
//*****************************************************************************
unsigned long DCSMGetZone2EXEstatus(unsigned long sector)
{
    
    ASSERT((sector > 0) && (sector <= 13 ));
    
    DCSMReadZone2EXEONLY();
    if(((HWREG(CSM_BASE + DCSM_O_Z2_EXEONLYR) >> sector) & 0x00000001) == 1)
    {
        return(1);
    }
    else
    {
        return(0);
    }
    
}

//*****************************************************************************
//! Returns the CSM security status of zone 1
//!
//! This function returns the security status of zone 1 CSM
//! 
//! \return Returns 0 if zone is not CSM secure and 1 if zone is secure.
//
//*****************************************************************************
unsigned long DCSMZone1CSMSecurityStatus(void)
{
    unsigned long status;
        
    status = HWREG(CSM_BASE + DCSM_O_Z1_CR);
    
    // if ARMED bit is set and MATCH bit or ALLONE bit or both MATCH and
    // ALLONEbits are set then CSM is unsecure. Else it is secure.
    if((status & 0x0800) && ((status & 0x0200)||(status & 0x0080)))
    {
        return(0);        
    }
    else
    {
        return(1);
    }
}

//*****************************************************************************
//! Returns the CSM security status of zone 2
//!
//! This function returns the security status of zone 2 CSM
//! 
//! \return Returns 0 if zone is not CSM secure, 1 if zone is secure and 2 if  
//!         both CSM-ALLZERO and CSM-ALLONE bits are set( invalid combination).
//
//*****************************************************************************
unsigned long DCSMZone2CSMSecurityStatus(void)
{
    unsigned long status;
        
    status = HWREG(CSM_BASE + DCSM_O_Z2_CR);
     
    // if ARMED bit is set and MATCH bit or ALLONE bit or both MATCH and
    // ALLONE bits are set then CSM is unsecure. Else it is secure.
    if((status & 0x0800) && ((status & 0x0200)||(status & 0x0080)))
    {
        return(0);        
    }
    else
    {
        return(1);
    }
}

//*****************************************************************************
//! Returns the ECSL security status of zone 1
//!
//! This function returns the security status of zone 1 ECSL 
//! 
//! \return Returns 0 if zone 2 is not ECSL secure and 1 if zone is ECSL secure 
//
//*****************************************************************************
unsigned long DCSMZone1ECSLSecurityStatus(void)
{
    unsigned long status;
    
    status = HWREG(CSM_BASE + DCSM_O_Z1_CR);
    
    // if ARMED bit is set and MATCH bit or ALLONE bit or both MATCH and
    // ALLONE bits are set then ECSL is unsecure. Else it is secure.
    if((status & 0x1000) && ((status & 0x0400)||(status & 0x0100)))
    {
        return(0);        
    }
    else
    {
        return(1);
    }
}

//*****************************************************************************
//! Returns the ECSL security status of zone 2
//!
//! This function returns the security status of zone 2 ECSL 
//! 
//! \return Returns 0 if zone 2 is not ECSL secure, 1 if zone is ECSL secure 
//! and 2 if both ECSL-ALLZERO and ECSL-ALLONE bits are set
//! (invalid combination).
//
//*****************************************************************************
unsigned long DCSMZone2ECSLSecurityStatus(void)
{
    unsigned long status;
        
    status = HWREG(CSM_BASE + DCSM_O_Z2_CR);
    
    // if ARMED bit is set and MATCH bit or ALLONE bit or both MATCH and
    // ALLONE bits are set then ECSL is unsecure. Else it is secure.
    if((status & 0x1000) && ((status & 0x0400)||(status & 0x0100)))
    {
        return(0);        
    }
    else
    {
        return(1);
    }
}

//*****************************************************************************
//! Returns the security zone a RAM section belongs to
//!
//! \param ramsection is the RAM section. Valid values are 0 for C0 RAM
//! and 1 for C1 RAM.
//!
//! This function returns the security zone a RAM section belongs to.
//!
//! \return Returns 0 if the section is inaccessible , 1 if the section belongs
//!  to zone 1, 2 if the section belongs to zone 2 and 3 if the section doesn't
//!  belong to any zone (or if the section is unsecure)
//
//*****************************************************************************
unsigned long DCSMGetRAMZone(unsigned long ramsection)
{
    unsigned long z1_status;
    unsigned long z2_status;
    unsigned long shift_value; 
    
        
    ASSERT((ramsection == 0) || (ramsection == 1));
    
    // read from grab ram flash location to transfer values to grab registers
    DCSMGrabRAMBlocks();
    
    shift_value = ramsection << 1; // 2*(sector - 1)
    z1_status = (HWREG(CSM_BASE + DCSM_O_Z1_GRABRAMR) >> shift_value) &
                0x00000003;
    z2_status = (HWREG(CSM_BASE + DCSM_O_Z2_GRABRAMR) >> shift_value) &
                0x00000003;
    
    // z1_status = 0x3, z2_status = 0x3, sector doesn't belong to any zone
    if((z1_status & z2_status) == 0x3)
    {
        return(3);
    }
    // z1_status = 0x3 and z2_status = 0x2 or 0x1 
    else if((z1_status == 0x3) && ((z2_status == 0x2) || (z2_status == 0x1)))
    {
        return(2);
    }
    // z2_status = 0x3 and z2_status = 0x1 or 0x2
    else if((z2_status == 0x3) && ((z1_status == 0x2) || (z1_status == 0x1)))
    {
        return(1);
    }
    else
    {
        return(0);
    }
    
}

//*****************************************************************************
//! Returns the security zone a flash sector belongs to
//!
//! \param sector is the flash sector value. Valid ranges are 1 for sector 1 
//!     through 12 for sector 12.
//!
//! This function returns the security zone a flash sector belongs to.
//!
//! \return Returns 0 if the sector is inaccessible. 1 if the sector belongs
//!  to zone 1, 2 if the sector belongs to zone 2 and 3 if the sector doesn't 
//!  belong to any zone (or if the sector is unsecure)
//
//*****************************************************************************
unsigned long DCSMGetFlashSectorZone(unsigned long sector)
{
    unsigned long z1_status;
    unsigned long z2_status;
    unsigned long shift_value; 
    
    ASSERT((sector > 0) && (sector < 13 ));
    
    // read from grab sector flash location to transfer values to grab
    // registers
    DCSMGrabRAMBlocks();
    shift_value = (sector - 1) << 1; // 2*(sector - 1)
    z1_status = (HWREG(CSM_BASE + DCSM_O_Z1_GRABSECTR) >> shift_value) &
                0x00000003;
    z2_status = (HWREG(CSM_BASE + DCSM_O_Z2_GRABSECTR) >> shift_value) &
                0x00000003;
    
    // z1_status = 0x3, z2_status = 0x3, sector doesn't belong to any zone
    if((z1_status & z2_status) == 0x3)
    {
        return(3);
    }
    // z1_status = 0x11 and z2_status = 0x01 or 0x10
    else if((z1_status == 0x3) && ((z2_status == 0x2) || (z2_status == 0x1)))
    {
        return(2);
    }
    // z2_status = 0x3 and z2_status = 0x1 or 0x2
    else if((z2_status == 0x3) && ((z1_status == 0x2) || (z1_status == 0x1)))
    {
        return(1);
    }
    else
    {
        return(0);
    }
    
}

//*****************************************************************************
// Close the Doxygen group.
//! @}
//*****************************************************************************




