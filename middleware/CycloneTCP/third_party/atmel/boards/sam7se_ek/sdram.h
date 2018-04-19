/**
 * @file sdram.h
 * @brief SDRAM configuration
 *
 * @section License
 *
 * Copyright (C) 2010-2017 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.0
 **/

#ifndef _SDRAM_H
#define _SDRAM_H

//Dependencies
#include "error.h"

//SDRAM base address
#define AT91C_SDRAM_BASE ((volatile uint32_t *) 0x20000000)
//SDRAM size (32 Mega bytes)
#define AT91C_SDRAM_SIZE (32 * 1024 * 1024)

//SDRAM control signals
#define AT91C_SDRAM_CTRL_MASK (AT91C_PA29_RAS | AT91C_PA28_CAS | AT91C_PA27_SDWE | AT91C_PA26_NCS1_SDCS | \
                               AT91C_PA25_SDCKE | AT91C_PA24_SDA10 | AT91C_PA23_NWR1_NBS1_CFIOR_NUB)
//SDRAM address bus (A1/NBS2, A12 and A15 not used)
#define AT91C_SDRAM_ADDR_MASK (AT91C_PB17_A17_BA1 | AT91C_PB16_A16_BA0 | AT91C_PB14_A14 | AT91C_PB13_A13 | \
                               AT91C_PB11_A11 | AT91C_PB10_A10 | AT91C_PB9_A9 | AT91C_PB8_A8 | \
                               AT91C_PB7_A7 | AT91C_PB6_A6 | AT91C_PB5_A5 | AT91C_PB4_A4 | \
                               AT91C_PB3_A3 | AT91C_PB2_A2 | AT91C_PB0_A0_NBS0)

//SDRAM data bus
#define AT91C_SDRAM_DATA_MASK (AT91C_PC15_D15 | AT91C_PC14_D14 | AT91C_PC13_D13 | AT91C_PC12_D12 | \
                               AT91C_PC11_D11 | AT91C_PC10_D10 | AT91C_PC9_D9 | AT91C_PC8_D8 | \
                               AT91C_PC7_D7 | AT91C_PC6_D6 | AT91C_PC5_D5 | AT91C_PC4_D4 | \
                               AT91C_PC3_D3 | AT91C_PC2_D2 | AT91C_PC1_D1 | AT91C_PC0_D0)

//SDRAM characteristics
#define AT91C_SDRC_TWR_2  (2 << 7)
#define AT91C_SDRC_TRC_4  (4 << 11)
#define AT91C_SDRC_TRP_4  (4 << 15)
#define AT91C_SDRC_TRCD_2 (2 << 19)
#define AT91C_SDRC_TRAS_3 (3 << 23)
#define AT91C_SDRC_TXSR_4 (4 << 27)

//Refresh period (64ms for 8192 rows)
#define AT91C_SDRC_TR_TIME (AT91B_MCK / 1000 * 64 / 8192)

void sdramInit(void);
error_t sdramTest(void);

#endif
