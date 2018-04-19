/*******************************************************************************
 * (c) Copyright 2009 Actel Corporation.  All rights reserved.
 *
 * SVN $Revision:$
 * SVN $Date:$
 */
#ifndef MSS_IAP_H_
#define MSS_IAP_H_

void enable_IAP(void);
void disable_IAP(void);
void IAP_Wait(void);
void IAP_IRSCAN_in(void);
void IAP_IRSCAN_out(unsigned char* outbuf);
void IAP_do_DRSCAN_in(unsigned int bits_to_shift,unsigned long value);
unsigned long IAP_do_DRSCAN_out(unsigned int bits_to_shift,unsigned long value);
unsigned char IAP_do_IRSCAN_out(void);
void IAP_goto_jtag_state( unsigned char jtag_state, unsigned int cycles );
#endif /*MSS_IAP_H_*/
