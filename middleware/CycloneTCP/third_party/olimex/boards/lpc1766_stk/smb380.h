/**
 * @file smb380.h
 * @brief SMB380 triaxial accelerator
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

#ifndef _SMB380_H
#define _SMB380_H

//Dependencies
#include "os_port.h"

//I2C clock frequency
#define I2C_CLOCK_FREQUENCY 100000

//I2C address
#define SMB380_ADDR 0x38

//SMB380 registers
#define SMB380_REG_CHIP_ID          0x00
#define SMB380_REG_VERSION          0x01
#define SMB380_REG_ACC_X_LSB        0x02
#define SMB380_REG_ACC_X_MSB        0x03
#define SMB380_REG_ACC_Y_LSB        0x04
#define SMB380_REG_ACC_Y_MSB        0x05
#define SMB380_REG_ACC_Z_LSB        0x06
#define SMB380_REG_ACC_Z_MSB        0x07
#define SMB380_REG_TEMP             0x08
#define SMB380_REG_STATUS           0x09
#define SMB380_REG_CTRL1            0x0A
#define SMB380_REG_CTRL2            0x0B
#define SMB380_REG_LG_THRES         0x0C
#define SMB380_REG_LG_DUR           0x0D
#define SMB380_REG_HG_THRES         0x0E
#define SMB380_REG_HG_DUR           0x0F
#define SMB380_REG_ANY_MOTION_THRES 0x10
#define SMB380_REG_HYST             0x11
#define SMB380_REG_CUSTOMER1        0x12
#define SMB380_REG_CUSTOMER2        0x13
#define SMB380_REG_CTRL3            0x14
#define SMB380_REG_CTRL4            0x15
#define SMB380_REG_OFFET_GAIN_X     0x16
#define SMB380_REG_OFFET_GAIN_Y     0x17
#define SMB380_REG_OFFET_GAIN_Z     0x18
#define SMB380_REG_OFFET_GAIN_T     0x19
#define SMB380_REG_OFFET_X          0x1A
#define SMB380_REG_OFFET_Y          0x1B
#define SMB380_REG_OFFET_Z          0x1C
#define SMB380_REG_OFFET_T          0x1D

//CHIP_ID register
#define CHIP_ID_MASK                0x07
#define CHIP_ID_VERSION_2           0x02

//CTRL3 register
#define CTRL3_RANGE_MASK            0x18
#define CTRL3_RANGE_2G              0x00
#define CTRL3_RANGE_4G              0x08
#define CTRL3_RANGE_8G              0x10

#define CTRL3_BANDWIDTH_MASK        0x07
#define CTRL3_BANDWIDTH_25HZ        0x00
#define CTRL3_BANDWIDTH_50HZ        0x01
#define CTRL3_BANDWIDTH_100HZ       0x02
#define CTRL3_BANDWIDTH_190HZ       0x03
#define CTRL3_BANDWIDTH_375HZ       0x04
#define CTRL3_BANDWIDTH_750HZ       0x05
#define CTRL3_BANDWIDTH_1500HZ      0x06

//SMB380 related functions
error_t smb380Init(void);
error_t smb380GetAcc(int16_t *ax, int16_t *ay, int16_t *az);
error_t smb380WriteReg(uint8_t address, uint8_t data);
error_t smb380ReadReg(uint8_t address, uint8_t *data);

//I2C related functions
void i2cInit(void);
error_t i2cWrite(uint8_t address, const uint8_t *data, size_t length);
error_t i2cRead(uint8_t address, uint8_t *data, size_t length);

#endif
