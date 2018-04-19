/**
 * \file  dspcache.h
 *
 * \brief Contains DSP cache API declarations and parameter definitions.
 */

/*
* Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/ 
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CACHE_H_
#define CACHE_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
** 							INTERNAL MACRO DEFINITIONS
******************************************************************************/
/* Defines used for 'memCfg' defines -- make 'memCfg' defines unique */
#define L1P_MEM_UNIQUE			(0x1 << 0)
#define L1D_MEM_UNIQUE			(0x1 << 1)
#define L2_MEM_UNIQUE			(0x1 << 2)


/******************************************************************************
** 							EXTERNAL MACRO DEFINITIONS
******************************************************************************/
/*
 * API Parameter: memCfg
 *
 * Brief: Used to select L1P, L1D, and/or L2 cache configuration.
 *
 * Functions: TBD
 *
 * NOTE: User can perform a logical OR using the below parameters to configure
 *		 L1P, L1D, and L2 with one call. 
 *		 Ex: (L1PCFG_L1PMODE_4K | L1DCFG_L1DMODE_16K | L2CFG_L2MODE_256K) */
/* L1P (Choose 1) */
#define L1PCFG_L1PMODE_4K		(0x1 << ((L1P_MEM_UNIQUE >> 1) * 4))
#define L1PCFG_L1PMODE_8K		(0x2 << ((L1P_MEM_UNIQUE >> 1) * 4))
#define L1PCFG_L1PMODE_16K		(0x3 << ((L1P_MEM_UNIQUE >> 1) * 4))
#define L1PCFG_L1PMODE_32K		(0x4 << ((L1P_MEM_UNIQUE >> 1) * 4))
/* OR L1D (Choose 1) */
#define L1DCFG_L1DMODE_4K		(0x1 << ((L1D_MEM_UNIQUE >> 1) * 4))
#define L1DCFG_L1DMODE_8K		(0x2 << ((L1D_MEM_UNIQUE >> 1) * 4))
#define L1DCFG_L1DMODE_16K		(0x3 << ((L1D_MEM_UNIQUE >> 1) * 4))
#define L1DCFG_L1DMODE_32K		(0x4 << ((L1D_MEM_UNIQUE >> 1) * 4))
/* OR L2 (Choose 1) */
#define L2CFG_L2MODE_32K		(0x1 << ((L2_MEM_UNIQUE >> 1) * 4))
#define L2CFG_L2MODE_64K		(0x2 << ((L2_MEM_UNIQUE >> 1) * 4))
#define L2CFG_L2MODE_128K		(0x3 << ((L2_MEM_UNIQUE >> 1) * 4))
#define L2CFG_L2MODE_256K		(0x4 << ((L2_MEM_UNIQUE >> 1) * 4))


/******************************************************************************
** 							FUNCTION DEFINITIONS
******************************************************************************/
extern void CacheEnable (unsigned int memCfg);
extern void CacheEnableMAR (unsigned int baseAddr, unsigned int byteSize);
extern void CacheDisableMAR (unsigned int baseAddr, unsigned int byteSize);
extern void CacheInvL1pAll (void);
extern void CacheWBAll (void);
extern void CacheWBInvAll (void);
extern void CacheInv (unsigned int baseAddr, unsigned int byteSize);
extern void CacheWB (unsigned int baseAddr, unsigned int byteSize);
extern void CacheWBInv (unsigned int baseAddr, unsigned int byteSize);

#ifdef __cplusplus
}
#endif

#endif
