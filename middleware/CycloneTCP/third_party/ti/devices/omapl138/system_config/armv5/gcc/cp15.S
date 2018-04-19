@******************************************************************************
@
@ cp15.S - CP15 coprocesser configuration APIs
@
@******************************************************************************
@
@ Copyright (C) 2010 Texas InSTRuments Incorporated - http://www.ti.com/
@
@
@  RediSTRibution and use in source and binary forms, with or without
@  modification, are permitted provided that the following conditions
@  are met:
@
@    RediSTRibutions of source code must retain the above copyright
@    notice, this list of conditions and the following disclaimer.
@
@    RediSTRibutions in binary form must reproduce the above copyright
@    notice, this list of conditions and the following disclaimer in the
@    documentation and/or other materials provided with the
@    diSTRibution.
@
@    Neither the name of Texas InSTRuments Incorporated nor the names of
@    its contributors may be used to endorse or promote products derived
@    from this software without specific prior written permission.
@
@  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
@  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
@  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
@  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
@  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
@  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
@  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
@  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
@  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
@  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
@
@******************************************************************************

@****************************** Global Symbols*********************************
        .global CP15ICacheDisable
        .global CP15DCacheDisable
        .global CP15ICacheEnable
        .global CP15DCacheEnable
        .global CP15ICacheFlush
        .global CP15DCacheCleanFlush
        .global CP15DCacheClean
        .global CP15DCacheFlush
        .global CP15DCacheCleanBuff
        .global CP15DCacheCleanFlushBuff
        .global CP15DCacheFlushBuff
        .global CP15ICacheFlushBuff
        .global CP15TtbSet
        .global CP15TlbInvalidate
        .global CP15MMUDisable
        .global CP15MMUEnable

@**************************** Code section ************************************
        .text

        @ This code is assembled for ARM instructions
        .code 32

@*****************************************************************************
@ This API disable the InSTRuction cache.
@*****************************************************************************
CP15ICacheDisable:
    MRC     p15, #0, r0, c1, c0, #0
    BIC     r0,  r0, #0x00001000
    MCR     p15, #0, r0, c1, c0, #0
    BX      lr

@*****************************************************************************
@ This API disable the Data cache.
@*****************************************************************************
CP15DCacheDisable:
    PUSH    {r4-r11, lr}
    MRC     p15, #0, r0, c1, c0, #0
    BIC     r0,  r0, #0x00000004
    MCR     p15, #0, r0, c1, c0, #0
    BL      CP15DCacheCleanFlush
    POP     {r4-r11, lr}
    BX      lr

@*****************************************************************************
@ This API enables I-cache
@*****************************************************************************
CP15ICacheEnable:
    MRC     p15, #0, r0, c1, c0, #0
    ORR     r0,  r0, #0x00001000
    MCR     p15, #0, r0, c1, c0, #0
    BX      lr

@*****************************************************************************
@ This API enable the Data cache.
@*****************************************************************************
CP15DCacheEnable:
    MRC     p15, #0, r0, c1, c0, #0
    ORR     r0,  r0, #0x00000004
    MCR     p15, #0, r0, c1, c0, #0
    BX      lr

@*****************************************************************************
@ This API Invalidates the entire Data/Unified Cache
@*****************************************************************************
CP15DCacheFlush:
    MOV     r0, #0
    MCR     p15, #0, r0, c7, c6, #0
    BX      lr

@*****************************************************************************
@ This API cleans the entire D Cache to PoC
@*****************************************************************************
CP15DCacheClean:
    MRC     p15, #0, r15, c7, c10, #3
    BNE     CP15DCacheClean
    BX      lr

@*****************************************************************************
@ This API cleans and invalidates the entire D Cache to PoC
@*****************************************************************************
CP15DCacheCleanFlush:
    MRC     p15, #0, r15, c7, c14, #3
    BNE     CP15DCacheClean
    BX      lr

@*****************************************************************************
@ This API invalidates entire I Cache
@*****************************************************************************
CP15ICacheFlush:
    MOV     r0, #0
    MCR     p15, #0, r0, c7, c5, #0
    BX      lr

@*****************************************************************************
@ This API cleans the D-cache/Unified  lines corresponding to the buffer
@ pointer upto the specified length to PoC.
@ r0 - Start Address
@ r1 - Number of bytes to be cleaned
@*****************************************************************************
CP15DCacheCleanBuff:
    PUSH    {r14}
    ADD     r14, r0, r1               @ Calculate the end address
    MOV     r2, #32                   @ Calculate the line size
    SUB     r3, r2, #1                @ Calculate the mask
    BIC     r0, r0, r3                @ Align to cache line boundary
    TST     r3, r14
    BIC     r14, r14, r3
    MCRNE   p15, #0, r14, c7, c10, #1 @ Clean D Cache

cleanloop:
    MCR     p15, #0, r0 , c7, c10, #1 @ Clean D Cache
    ADDS    r0, r0, r2                @ Go to next line
    CMP     r0, r14
    BLT     cleanloop

    POP     {r14}
    BX      lr

@*****************************************************************************
@ This API cleans and invalidates the D-cache/Unified  lines corresponding to
@ the buffer pointer upto the specified length to PoC.
@ r0 - Start Address
@ r1 - Number of bytes to be cleaned and flushed
@*****************************************************************************
CP15DCacheCleanFlushBuff:
    PUSH    {r14}
    ADD     r14, r0, r1               @ Calculate the end address
    MOV     r2, #32                   @ Calculate the line size
    SUB     r3, r2, #1                @ Calculate the mask
    BIC     r0, r0, r3                @ Align to cache line boundary
    TST     r3, r14
    BIC     r14, r14, r3
    MCRNE   p15, #0, r14, c7, c14, #1 @ Clean and Flush D Cache

cleanflushloop:
    MCR     p15, #0, r0 , c7, c14, #1 @ Clean and Flush D Cache
    ADDS    r0, r0, r2                @ Go to next line
    CMP     r0, r14
    BLT     cleanflushloop

    POP     {r14}
    BX      lr

@*****************************************************************************
@ This API invalidates the D-cache/Unified  lines corresponding to
@ the buffer pointer upto the specified length to PoC.
@ r0 - Start Address
@ r1 - Number of bytes to be flushed
@*****************************************************************************
CP15DCacheFlushBuff:
    PUSH    {r14}
    ADD     r14, r0, r1               @ Calculate the end address
    MOV     r2, #32                   @ Calculate the line size
    SUB     r3, r2, #1                @ Calculate the mask
    TST     r3, r0
    BIC     r0, r0, r3                @ Align to cache line boundary
    MCRNE   p15, #0, r0, c7, c14, #1  @ Clean and Flush D/U line to PoC
    ADDNE   r0, r0, r2
    TST     r3, r14
    BIC     r14, r14, r3
    MCRNE   p15, #0, r14, c7, c14, #1 @ Clean and Flush D/U line to PoC
    B       dflushcmp

dflushloop:
    MCR     p15, #0, r0 , c7, c6, #1  @ Flush D/U line to PoC
    ADDS    r0, r0, r2                @ Go to next line

dflushcmp:
    CMP     r0, r14
    BLT     dflushloop
    POP     {r14}
    BX      lr

@*****************************************************************************
@ This API invlidates I-cache lines from the star address till the length
@ specified to PoU.
@ r0 - Start Address
@ r1 - Number of bytes to be cleaned
@*****************************************************************************
CP15ICacheFlushBuff:
    PUSH    {r14}
    ADD     r14, r0, r1               @ Calculate the end address
    MOV     r2, #32                   @ Calculate the line size
    SUB     r3, r2, #1                @ Calculate the mask
    BIC     r0, r0, r3                @ Align to cache line boundary
    TST     r3, r14
    BIC     r14, r14, r3
    MCRNE   p15, #0, r14, c7, c5, #1  @ Invalidate by MVA to PoU

iflushloop:
    MCR     p15, #0, r0, c7, c5, #1   @ Invalidate by MVA to PoU
    ADDS    r0, r0, r2                @ Go to next line
    CMP     r0, r14
    BLT     iflushloop

    POP     {r14}
    BX      lr

@*****************************************************************************
@ Sets TTB Register
@ Note that this API sets the domain access to client
@ r0 - Translation Table Base Address
@*****************************************************************************
CP15TtbSet:
    MOV     r1, #0
    MCR     p15, #0, r1, c8, c7, #0
    LDR     r1, _CLIENTD
    MCR     p15, #0, r1, c3, c0, #0   @ Set the domain access
    MCR     p15, #0, r0, c2, c0, #0    @ Set the TTB register
    BX      lr

@*****************************************************************************
@ This API Invalidates the TLB
@*****************************************************************************
CP15TlbInvalidate:
    MCR     p15, #0, r0, c8, c7, #0    @ r0 value will be ignored
    BX      lr

@*****************************************************************************
@ This API Disables MMU.
@*****************************************************************************
CP15MMUDisable:
    MCR     p15, #0, r0, c8, c7, #0    @ Invalidate TLB
    MRC     p15, #0, r0, c1, c0, #0
    BIC     r0, r0, #1
    MCR     p15, #0, r0, c1, c0, #0    @ Clear MMU bit
    BX      lr

@*****************************************************************************
@ This API Enables MMU.
@*****************************************************************************
CP15MMUEnable:
    MRC     p15, #0, r0, c1, c0, #0
    ORR     r0, r0, #0x001
    MCR     p15, #0, r0, c1, c0, #0    @ Set MMU Enable bit
    BX      lr

_CLIENTD:
   .word  0x55555555

@
@ End of the file
@
    .end

