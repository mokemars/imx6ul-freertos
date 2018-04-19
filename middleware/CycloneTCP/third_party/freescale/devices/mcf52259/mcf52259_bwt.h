/********************************************************************/
/* Coldfire C Header File
 *
 *     Date      : 2009/04/30
 *     Revision  : 0.8
 *
 *     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 *     http      : www.freescale.com
 *     mail      : support@freescale.com
 */

#ifndef __MCF52259_BWT_H__
#define __MCF52259_BWT_H__


/*********************************************************************
*
* Backup Watchdog Timer Module (BWT)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_BWT_WCR                          (*(vuint16*)(0x40140000))
#define MCF_BWT_WMR                          (*(vuint16*)(0x40140002))
#define MCF_BWT_WCNTR                        (*(vuint16*)(0x40140004))
#define MCF_BWT_WSR                          (*(vuint16*)(0x40140006))


/* Bit definitions and macros for MCF_BWT_WCR */
#define MCF_BWT_WCR_EN                       (0x1)
#define MCF_BWT_WCR_DOZE                     (0x4)
#define MCF_BWT_WCR_WAIT                     (0x8)
#define MCF_BWT_WCR_STOP                     (0x10)

/* Bit definitions and macros for MCF_BWT_WMR */
#define MCF_BWT_WMR_WM(x)                    (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_BWT_WCNTR */
#define MCF_BWT_WCNTR_WC(x)                  (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_BWT_WSR */
#define MCF_BWT_WSR_WS(x)                    (((x)&0xFFFF)<<0)


#endif /* __MCF52259_BWT_H__ */
