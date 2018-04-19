/*******************************************************************************
 * (c) Copyright 2008 Actel Corporation.  All rights reserved.
 *
 * SVN $Revision: 761 $
 * SVN $Date: 2009-03-24 16:39:51 +0000 (Tue, 24 Mar 2009) $
 */
#include "a2fxxxm3.h"
#include "iap_hw_interface.h"
#include "dpuser.h"
#include "dpscan.h"
#include "dpalg.h"
#include "dpcom.h"

extern unsigned char opcode;
/*------------------------------------------------------------------------------
 * IAP_STATUS_IDLE_MASK:
 *   IDLE field of IAP STATUS register.
 *------------------------------------------------------------------------------
 * IDLE indicates the IAP state machine is IDLE and is ready to process another 
 * command. If asserted then an APB write transaction is guaranteed not to 
 * generate wait states.
 */
#define IAP_STATUS_IDLE_MASK     0x00000004
#define IAP_BITBAND          ((IAP_BitBandTypeDef *) BITBAND_ADDRESS(IAP_BASE))
typedef struct
{
    __IO uint32_t IAP_IR[32];
    __IO uint32_t IAP_DR2[32];
    __IO uint32_t IAP_DR3[32];
    __IO uint32_t IAP_DR5[32];
    __IO uint32_t IAP_DR26[32];
    __IO uint32_t IAP_DR32[32];
    __IO uint32_t IAP_DR[32];
    __IO uint32_t IAP_DR_LENGTH[32];
    __IO uint32_t IAP_TAP_NEW_STATE[32];
    
    __IO uint32_t IAP_TAP_CTRL_TMS;
    __IO uint32_t IAP_TAP_CTRL_TDI;
    __IO uint32_t IAP_TAP_CTRL_TCK;
    __IO uint32_t IAP_TAP_CTRL_TRSTB;
    __IO uint32_t IAP_TAP_CTRL_TDO;
    __IO uint32_t IAP_TAP_CTRL_IAP_ENABLE;
    __IO uint32_t IAP_TAP_CTRL_CURSTATE[4];
    __IO uint32_t IAP_TAP_CTRL_M3DONE;
    __IO uint32_t IAP_TAP_CTRL_M3ACK;
    __IO uint32_t IAP_TAP_CONTROL[20];
    
    __I  uint32_t IAP_STATUS_M3_INTR;
    __I  uint32_t IAP_STATUS_BUSY;
    __I  uint32_t IAP_STATUS_IDLE;
    __I  uint32_t IAP_STATUS_READY;
    __I  uint32_t IAP_STATUS[28];
    
} IAP_BitBandTypeDef;

void enable_IAP(void)
{
    // Enable IAP
    SYSREG->SOFT_RST_CR &= ~SYSREG_IAP_SOFTRESET_MASK;
    
    /* Set IAP FCFG to 7. */
    SYSREG->IAP_CR = 7;
    IAP_BITBAND->IAP_TAP_CTRL_IAP_ENABLE = 1;

  return;
}

void disable_IAP(void)
{
  IAP_BITBAND->IAP_TAP_CTRL_IAP_ENABLE = 0;
  return;
}


void IAP_Wait(void)
{
    uint32_t idle;
    /* Wait for read to complete. */
    do {
      idle = IAP->IAP_STATUS;      
      idle &= IAP_STATUS_IDLE_MASK;
    } while ( idle == 0);
}

void IAP_IRSCAN_in(void)
{
    IAP_Wait();
    IAP->IAP_IR = opcode;
    /* Wait for command to go through. */
}

void IAP_IRSCAN_out(unsigned char* outbuf)
{
    IAP_Wait();
    IAP->IAP_IR = opcode;
    IAP_Wait();
    *outbuf = IAP->IAP_IR;
}

unsigned char IAP_do_IRSCAN_out(void)
{
      unsigned char return_value;
      
      IAP_Wait();
      IAP->IAP_IR = opcode;
      return_value = IAP->IAP_IR;

      return return_value;
}

void IAP_do_DRSCAN_in(unsigned int bits_to_shift,unsigned long value)
{
    IAP_Wait();
    if (bits_to_shift == 32)
    {
        IAP->IAP_DR32 = value;
    }
    else if (bits_to_shift == 26)
    {
        IAP->IAP_DR26 = value;
    }
    else if (bits_to_shift == 5)
    {
        IAP->IAP_DR5 = value;
    }
    else if (bits_to_shift == 3)
    {
        IAP->IAP_DR3 = value;
    }
    else if (bits_to_shift == 2)
    {
        IAP->IAP_DR2 = value;
    }
    else 
    {
        IAP->IAP_DR_LENGTH = bits_to_shift;
        IAP->IAP_DR = value;
    }
    return;
}

unsigned long IAP_do_DRSCAN_out(unsigned int bits_to_shift,unsigned long value)
{
      DPULONG return_value;
      IAP_Wait();
      if (bits_to_shift == 32)
      {
          IAP->IAP_DR32 = value;
          IAP_Wait();
          return_value = IAP->IAP_DR32;
      }
      else if (bits_to_shift == 26)
      {
          IAP->IAP_DR26 = value;
          IAP_Wait();
          return_value = IAP->IAP_DR26;
      }
      else if (bits_to_shift == 5)
      {
          IAP->IAP_DR5 = value;
          IAP_Wait();
          return_value = IAP->IAP_DR5;
      }
      else if (bits_to_shift == 3)
      {
          IAP->IAP_DR3 = value;
          IAP_Wait();
          return_value = IAP->IAP_DR3;
      }
      else if (bits_to_shift == 2)
      {
          IAP->IAP_DR2 = value;
          IAP_Wait();
          return_value = IAP->IAP_DR2;
      }
      else 
      {
          IAP->IAP_DR_LENGTH = bits_to_shift;
          IAP->IAP_DR = value;
          IAP_Wait();
          return_value = IAP->IAP_DR;
      }
      return return_value;
}


void IAP_goto_jtag_state( unsigned char jtag_state, unsigned int cycles )
{
      uint32_t register_value;
      IAP_Wait();	
      register_value = jtag_state | (cycles << 4);
      IAP->IAP_TAP_NEW_STATE = register_value;
}

