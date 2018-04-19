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

//SDRAM base address
#define SDRAM_BASE 0x70000000
//SDRAM size in bytes
#define SDRAM_SIZE (2 * 1024 * 1024)

//SDRAM signals (PIOA pins assigned to peripheral A)
#define SDRAM_PIOA_PERIPH_A_MASK (PIO_PA16A_D15 | PIO_PA15A_D14)

//SDRAM signals (PIOA pins assigned to peripheral C)
#define SDRAM_PIOA_PERIPH_C_MASK (PIO_PA20C_BA0)

//SDRAM signals (PIOC pins assigned to peripheral A)
#define SDRAM_PIOC_PERIPH_A_MASK ( \
   PIO_PC29A_A11 | PIO_PC28A_A10 | PIO_PC27A_A9 | PIO_PC26A_A8 | \
   PIO_PC25A_A7 | PIO_PC24A_A6 | PIO_PC23A_A5 | PIO_PC22A_A4 | \
   PIO_PC21A_A3 | PIO_PC20A_A2 | PIO_PC18A_NBS0 | PIO_PC15A_SDCS | \
   PIO_PC7A_D7 | PIO_PC6A_D6 | PIO_PC5A_D5 | PIO_PC4A_D4 | \
   PIO_PC3A_D3 | PIO_PC2A_D2 | PIO_PC1A_D1 | PIO_PC0A_D0)

//SDRAM signals (PIOD pins assigned to peripheral C)
#define SDRAM_PIOD_PERIPH_C_MASK (PIO_PD29C_SDWE | PIO_PD23C_SDCK | PIO_PD17C_CAS | \
   PIO_PD16C_RAS | PIO_PD14C_SDCKE | PIO_PD13C_SDA10| PIO_PD15C_NBS1)

//SDRAM signals (PIOE pins assigned to peripheral A)
#define SDRAM_PIOE_PERIPH_A_MASK (PIO_PE5A_D13 | PIO_PE4A_D12 | \
   PIO_PE3A_D11 | PIO_PE2A_D10 | PIO_PE1A_D9 | PIO_PE0A_D8)

//SDRAM related functions
void sdramInit(void);

#endif
