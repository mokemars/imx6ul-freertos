/**
 * \file  cache.c
 *
 * \brief Contains DSP cache related APIs.
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

/* Debug and HW Macros */
#include "debug.h"
#include "hw_types.h"

/* DSP System Defines */
#include "hw_dspcache.h"
#include "soc_C6748.h"

/* DSP System APIs */
#include "dspcache.h"
#include "interrupt.h"


/******************************************************************************
**                        INTERNAL DEFINES
******************************************************************************/
#define DSPCACHE_LxMODE     (0x00000777)
#define DSPCACHE_MAXWC      (0xFFE0)


/******************************************************************************
**                        STATIC CACHE FUNCTIONS
******************************************************************************/
static void All (unsigned int cacheReg)
{
    unsigned int restoreVal;
    
    /* Critical section - disable interrupts */
    restoreVal = IntGlobalDisable();
    
    /* Wait for any previous cache operation to complete */
    while(HWREG(SOC_CACHE_0_REGS + DSPCACHE_L2WWC) != 0)
    {
        /* Open a window for interrupts */
        IntGlobalRestore(restoreVal);
        
        /* Disable interrupts */
        restoreVal = IntGlobalDisable();
    }
    
    /* Perform global coherence operation */
    HWREG(SOC_CACHE_0_REGS + cacheReg) = 1;
    
    /* End of critical section - restore interrupts */
    IntGlobalRestore(restoreVal);
    
    /* Wait until cache operation completes */
    while(HWREG(SOC_CACHE_0_REGS + cacheReg) & 1);
}

static void Block (unsigned int baseAddr, unsigned int byteSize, unsigned int cacheReg)
{
    unsigned int cacheWCReg, alignAddr, restoreVal;
    int wordCnt;
    
    /* Get address of cache word count register */
    cacheWCReg = cacheReg + 4;
    
    /* Word align the base address */
    alignAddr = baseAddr & ~3;
    
    /* Assign word count */
    wordCnt = (byteSize + 3 + (baseAddr - alignAddr)) >> 2;
    
    while(wordCnt > 0)
    {
        /* Critical section - disable interrupts */
        restoreVal = IntGlobalDisable();
        
        /* Wait for any previous cache operation to complete */
        while(HWREG(SOC_CACHE_0_REGS + DSPCACHE_L2WWC) != 0)
        {
            /* Open a window for interrupts */
            IntGlobalRestore(restoreVal);
        
            /* Disable interrupts */
            restoreVal = IntGlobalDisable();
        }
        
        /* Set the address and word count regs to perform coherence operation */
        HWREG(SOC_CACHE_0_REGS + cacheReg) = alignAddr;
        HWREG(SOC_CACHE_0_REGS + cacheWCReg) = 
            (wordCnt > DSPCACHE_MAXWC) ? DSPCACHE_MAXWC : wordCnt;
        
        /* End of critical section - restore interrupts */
        IntGlobalRestore(restoreVal);
        
        wordCnt -= DSPCACHE_MAXWC;
        alignAddr += (DSPCACHE_MAXWC * sizeof(unsigned int));
    }
    
    /* Wait for cache operation to complete */
    while(HWREG(SOC_CACHE_0_REGS + DSPCACHE_L2WWC) != 0);
}


/******************************************************************************
**                            API CACHE FUNCTIONS
******************************************************************************/
void CacheEnable (unsigned int memCfg)
{
    unsigned int restoreVal;
    
    /* Check for valid parameter */
    ASSERT((memCfg & DSPCACHE_LxMODE) == memCfg);
    
    /* Critical section - disable interrupts */
    restoreVal = IntGlobalDisable();
    
    /* Check to enable L1P Cache */
    if((memCfg & (DSPCACHE_L1PCFG_L1PMODE << ((L1P_MEM_UNIQUE >> 1) * 4))) != 0)
    {
        /* Enable L1P Cache */
        HWREG(SOC_CACHE_0_REGS + DSPCACHE_L1PCFG) = 
            (HWREG(SOC_CACHE_0_REGS + DSPCACHE_L1PCFG) &
            ~DSPCACHE_L1PCFG_L1PMODE) | (memCfg & DSPCACHE_L1PCFG_L1PMODE);
        
        /* Read back value to stall CPU until the mode change completes */
        HWREG(SOC_CACHE_0_REGS + DSPCACHE_L1PCFG);
    }
    
    /* Check to enable L1D Cache */
    if((memCfg & (DSPCACHE_L1DCFG_L1DMODE << ((L1D_MEM_UNIQUE >> 1) * 4))) != 0)
    {
        /* Enable L1D Cache */
        HWREG(SOC_CACHE_0_REGS + DSPCACHE_L1DCFG) =
            (HWREG(SOC_CACHE_0_REGS + DSPCACHE_L1DCFG) &
            ~DSPCACHE_L1DCFG_L1DMODE) | (memCfg & DSPCACHE_L1DCFG_L1DMODE);
        
        /* Read back value to stall CPU until the mode change completes */
        HWREG(SOC_CACHE_0_REGS + DSPCACHE_L1DCFG);
    }
    
    /* Check to enable L2 Cache */
    if((memCfg & (DSPCACHE_L2CFG_L2MODE << ((L2_MEM_UNIQUE >> 1) * 4))) != 0)
    {
        /* Enable L2 Cache */
        HWREG(SOC_CACHE_0_REGS + DSPCACHE_L2CFG) =
            (HWREG(SOC_CACHE_0_REGS + DSPCACHE_L2CFG) &
            ~DSPCACHE_L2CFG_L2MODE) | (memCfg & DSPCACHE_L2CFG_L2MODE);
        
        /* Read back value to stall CPU until the mode change completes */
        HWREG(SOC_CACHE_0_REGS + DSPCACHE_L2CFG);
    }
    
    /* End of critical section - restore interrupts */
    IntGlobalRestore(restoreVal);
}
 
void CacheEnableMAR (unsigned int baseAddr, unsigned int byteSize)
{
    unsigned int maxAddr, firstMar, lastMar, marNum;
    
    maxAddr = baseAddr + byteSize - 1;
    firstMar = baseAddr >> 24;
    lastMar = maxAddr >> 24;
    
    for(marNum = firstMar; marNum <= lastMar; marNum++)
    {
        HWREG(SOC_CACHE_0_REGS + DSPCACHE_MAR(marNum)) |= DSPCACHE_MAR_PC;
        asm(" NOP");
    }
    
    /* Read back value to stall CPU */
    HWREG(SOC_CACHE_0_REGS + DSPCACHE_MAR(firstMar));
}

void CacheDisableMAR (unsigned int baseAddr, unsigned int byteSize)
{
    unsigned int maxAddr, firstMar, lastMar, marNum;
    
    maxAddr = baseAddr + byteSize - 1;
    firstMar = baseAddr >> 24;
    lastMar = maxAddr >> 24;
    
    for(marNum = firstMar; marNum <= lastMar; marNum++)
    {
        HWREG(SOC_CACHE_0_REGS + DSPCACHE_MAR(marNum)) &= ~DSPCACHE_MAR_PC;
        asm(" NOP");
    }
    
    /* Read back value to stall CPU */
    HWREG(SOC_CACHE_0_REGS + DSPCACHE_MAR(firstMar));
}

void CacheInvL1pAll (void)
{
    All(DSPCACHE_L1PINV);
}

void CacheWBAll (void)
{
    All(DSPCACHE_L2WB);
}

void CacheWBInvAll (void)
{
    All(DSPCACHE_L2WBINV);
}

void CacheInv (unsigned int baseAddr, unsigned int byteSize)
{
    Block(baseAddr, byteSize, DSPCACHE_L2IBAR);
}

void CacheWB (unsigned int baseAddr, unsigned int byteSize)
{
    Block(baseAddr, byteSize, DSPCACHE_L2WBAR);
}

void CacheWBInv (unsigned int baseAddr, unsigned int byteSize)
{
    Block(baseAddr, byteSize, DSPCACHE_L2WIBAR);
}
