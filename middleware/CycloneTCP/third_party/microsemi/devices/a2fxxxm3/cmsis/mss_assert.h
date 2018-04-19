/*******************************************************************************
 * (c) Copyright 2009 Actel Corporation.  All rights reserved.
 * 
 * Assertion implementation.
 *
 * This file provides the implementation of the ASSERT macro. This file can be
 * modified to cater for project specific requirements regarding the way
 * assertions are handled.
 *
 * SVN $Revision: 2454 $
 * SVN $Date: 2010-03-16 15:20:09 +0000 (Tue, 16 Mar 2010) $
 */
#ifndef __MSS_ASSERT_H_
#define __MSS_ASSERT_H_

#include <assert.h>

#if defined ( __GNUC__   )

#if defined(NDEBUG)

#define ASSERT(CHECK)

#else   /* NDEBUG */
/*
 * SoftConsole assertion handling
 */
#define ASSERT(CHECK)  \
    do { \
        if (!(CHECK)) \
        { \
            __asm volatile ("BKPT\n\t"); \
        } \
    } while (0);
    
#endif  /* NDEBUG */

#elif defined ( __ICCARM__ )
/*
 * IAR Embedded Workbench assertion handling.
 * Call C library assert function which should result in error message
 * displayed in debugger.
 */
#define ASSERT(X)   assert(X)

#else
/*
 * Keil assertion handling.
 * Call C library assert function which should result in error message
 * displayed in debugger.
 */

#ifndef __MICROLIB
  #define ASSERT(X)   assert(X)
#else
  #define ASSERT(X)
#endif

#endif

#endif  /* __MSS_ASSERT_H_ */
