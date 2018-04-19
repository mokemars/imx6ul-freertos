/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_common.h"
#include "board.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const mmu_attribute_t s_mmuDevAttr = {.type = MMU_MemoryDevice,
                                             .domain = 0U,
                                             .accessPerm = MMU_AccessRWNA,
                                             .shareable = 0U,
                                             .notSecure = 0U,
                                             .notGlob = 0U,
                                             .notExec = 1U};

static const mmu_attribute_t s_mmuRomAttr = {.type = MMU_MemoryWriteBackWriteAllocate,
                                             .domain = 0U,
                                             .accessPerm = MMU_AccessRORO,
                                             .shareable = 0U,
                                             .notSecure = 0U,
                                             .notGlob = 0U,
                                             .notExec = 0U};

static const mmu_attribute_t s_mmuRamAttr = {.type = MMU_MemoryWriteBackWriteAllocate,
                                             .domain = 0U,
                                             .accessPerm = MMU_AccessRWRW,
                                             .shareable = 0U,
                                             .notSecure = 0U,
                                             .notGlob = 0U,
                                             .notExec = 0U};

static const mmu_attribute_t s_mmuBufferAttr = {.type = MMU_MemoryNonCacheable,
                                                .domain = 0U,
                                                .accessPerm = MMU_AccessRWRW,
                                                .shareable = 0U,
                                                .notSecure = 0U,
                                                .notGlob = 0U,
                                                .notExec = 0U};

#if defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = 16384
static uint32_t MMU_L1Table[4096] @"OcramData";
#elif defined(__GNUC__)
static uint32_t MMU_L1Table[4096] __attribute__((section("OcramData"), aligned(16384)));
#else
#error Not supported compiler type
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Initialize memory system (MMU). */
void BOARD_InitMemory(void)
{
    uint32_t i;

    MMU_Init(MMU_L1Table);

    MMU_ConfigSection(MMU_L1Table, (const void *)0x00000000U, 0x00000000U, &s_mmuRomAttr); /* ROM */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00900000U, 0x00900000U, &s_mmuRamAttr); /* OCRAM */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00A00000U, 0x00A00000U, &s_mmuDevAttr); /* GIC */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00B00000U, 0x00B00000U, &s_mmuDevAttr); /* GPV_0 PL301 */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00C00000U, 0x00C00000U, &s_mmuDevAttr); /* GPV_1 PL301 */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00D00000U, 0x00D00000U, &s_mmuDevAttr); /* cpu */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x00E00000U, 0x00E00000U, &s_mmuDevAttr); /* per_m */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x01800000U, 0x01800000U, &s_mmuDevAttr); /* APBH DMA */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x02000000U, 0x02000000U, &s_mmuDevAttr); /* AIPS-1 */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x02100000U, 0x02100000U, &s_mmuDevAttr); /* AIPS-2 */
    MMU_ConfigSection(MMU_L1Table, (const void *)0x02200000U, 0x02200000U, &s_mmuDevAttr); /* AIPS-3 */

    for (i = 0; i < 32; i++)
    {
        MMU_ConfigSection(MMU_L1Table, (const void *)(0x0C000000U + (i << 20)), (0x0C000000U + (i << 20)),
                          &s_mmuDevAttr); /* QSPI Rx Buf */
    }

    for (i = 0; i < 256; i++)
    {
        MMU_ConfigSection(MMU_L1Table, (const void *)(0x50000000U + (i << 20)), (0x50000000U + (i << 20)),
                          &s_mmuRamAttr); /* EIM */
    }

    for (i = 0; i < 256; i++)
    {
        MMU_ConfigSection(MMU_L1Table, (const void *)(0x60000000U + (i << 20)), (0x60000000U + (i << 20)),
                          &s_mmuRomAttr); /* QSPI */
    }

    for (i = 0; i < 2048; i++)
    {
        MMU_ConfigSection(MMU_L1Table, (const void *)(0x80000000U + (i << 20)), (0x80000000U + (i << 20)),
                          &s_mmuRamAttr); /* DDR */
    }

/* You can place global or static variables in NonCacheable section to make it uncacheable.*/
#if defined(__ICCARM__)
#pragma section = "NonCacheable"
    uint32_t ncahceStart = (uint32_t)__section_begin("NonCacheable");
    uint32_t size = (uint32_t)__section_size("NonCacheable");
#elif defined(__GNUC__)
    extern uint32_t __noncachedata_start__[];
    extern uint32_t __noncachedata_end__[];
    uint32_t ncahceStart = (uint32_t)__noncachedata_start__;
    uint32_t size = (uint32_t)__noncachedata_end__ - (uint32_t)__noncachedata_start__;
#else
#error Not supported compiler type
#endif
    size = (size + 0xFFFFFU) & (~0xFFFFFU);

    for (i = 0; i < ((size) >> 20); i++)
    {
        MMU_ConfigSection(MMU_L1Table, (const void *)(ncahceStart + (i << 20)), (ncahceStart + (i << 20)),
                          &s_mmuBufferAttr); /* Frame buffer */
    }

    MMU_Enable();
}

void Delay(uint32_t ticks)
{
    while (ticks--)
    {
        __NOP();
    }
}
