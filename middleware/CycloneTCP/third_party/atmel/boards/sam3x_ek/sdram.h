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
#define SDRAM_BASE 0x70000000
//SDRAM size in bytes
#define SDRAM_SIZE (32 * 1024 * 1024)

//SDRAM pins (PIOC)
#define SDRAM_PIOC_MASK (PIO_PC30A_A9 | \
   PIO_PC29A_A8 | PIO_PC28A_A7 | PIO_PC27A_A6 | PIO_PC26A_A5 | \
   PIO_PC25A_A4 | PIO_PC24A_A3 | PIO_PC23A_A2 | PIO_PC21A_NBS0 | \
   PIO_PC17A_D15 | PIO_PC16A_D14 | PIO_PC15A_D13 | PIO_PC14A_D12 | \
   PIO_PC13A_D11 | PIO_PC12A_D10 | PIO_PC11A_D9 | PIO_PC10A_D8 | \
   PIO_PC9A_D7 | PIO_PC8A_D6 | PIO_PC7A_D5 | PIO_PC6A_D4 | \
   PIO_PC5A_D3 | PIO_PC4A_D2 | PIO_PC3A_D1 | PIO_PC2A_D0)

//SDRAM pins (PIOD)
#define SDRAM_PIOD_MASK (PIO_PD25A_A13 | \
   PIO_PD23A_A11 | PIO_PD22A_A10 | PIO_PD16A_CAS | PIO_PD15A_RAS | \
   PIO_PD14A_SDWE | PIO_PD13A_SDCKE | PIO_PD12A_SDCS | PIO_PD11A_SDA10 | \
   PIO_PD10A_NBS1 | PIO_PD7A_BA1 | PIO_PD6A_BA0 | PIO_PD4A_A14)

void sdramInit(uint32_t coreClockFrequency);
error_t sdramTest(void);

#endif
