/**
 * @file evk1105.h
 * @brief EVK1105 evaluation kit
 *
 * @section License
 *
 * Copyright (C) 2010-2017 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Open.
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

#ifndef _EVK1105_H
#define _EVK1105_H

//Dependencies
#include <avr32/io.h>

//OSC0 frequency (12MHz)
#define OSC0_FREQ 12000000
//OSC1 frequency (11.2896MHz)
#define OSC1_FREQ 11289600
//OSC32 frequency (32kHz)
#define OSC32_FREQ 32768

//Main clock frequency (66MHz)
#define MCLK_FREQ (12000000 * 11 / 2)
//CPU clock frequency (66MHz)
#define CPU_FREQ (MCLK_FREQ / 1)
//HSB clock frequency (66MHz)
#define HSB_FREQ (MCLK_FREQ / 1)
//PBA clock frequency (66MHz)
#define PBA_FREQ (MCLK_FREQ / 1)
//PBB clock frequency (66MHz)
#define PBB_FREQ (MCLK_FREQ / 1)

//LED0 (PB27)
#define LED0_PORT 1
#define LED0_POS  27

//LED1 (PB28)
#define LED1_PORT 1
#define LED1_POS  28

//LED2 (PA5)
#define LED2_PORT 0
#define LED2_POS  5

//LED3 (PA6)
#define LED3_PORT 0
#define LED3_POS  6

//SDRAM memory parameters
#define SDRAM_PART_HDR  "mt48lc16m16a2tg7e.h"
//SDRAM data bus width
#define SDRAM_DBW 16

#endif
