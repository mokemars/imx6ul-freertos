/**
 * @file smb380.c
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

//Dependencies
#include "lpc_1766stk.h"
#include "smb380.h"
#include "error.h"
#include "debug.h"


/**
 * @brief SMB380 initialization
 * @return Error code (see #ErrorCode enumeration)
 **/

error_t smb380Init(void)
{
   error_t error;
   uint8_t value;

   //Debug message
   TRACE_INFO("Initializing SMB380...\r\n");

   //I2C initialization
   i2cInit();

   //Read chip ID
   error = smb380ReadReg(SMB380_REG_CHIP_ID, &value);
   //Any error to report?
   if(error)
      return error;

   //Verify chip identifier
   if((value & CHIP_ID_MASK) != CHIP_ID_VERSION_2)
      return ERROR_INVALID_VERSION;

   //Read version register
   error = smb380ReadReg(SMB380_REG_VERSION, &value);
   //Any error to report?
   if(error)
      return error;

   //Select +/-2g range and 25Hz bandwidth
   value = CTRL3_RANGE_2G | CTRL3_BANDWIDTH_25HZ;
   //Write configuration to SMB380 device
   error = smb380WriteReg(SMB380_REG_CTRL3, value);
   //Any error to report?
   if(error)
      return error;

   //Successful initialization
   return NO_ERROR;
}


/**
 * @brief Get acceleration data
 * @param[out] ax Acceleration value (X axis)
 * @param[out] ay Acceleration value (Y axis)
 * @param[out] az Acceleration value (Z axis)
 * @param[in] data Register value
 * @return Error code (see #ErrorCode enumeration)
 **/

error_t smb380GetAcc(int16_t *ax, int16_t *ay, int16_t *az)
{
   error_t error;
   uint8_t buffer[6];

   //Point to the ACC_X register
   buffer[0] = SMB380_REG_ACC_X_LSB;

   //Set up a read operation
   error = i2cWrite(SMB380_ADDR, buffer, 1);
   //Any error to report?
   if(error)
      return error;

   //Read acceleration data
   error = i2cRead(SMB380_ADDR, buffer, 6);
   //Any error to report?
   if(error)
      return error;

   //Retrieve acceleration along the X axis
   *ax = buffer[0] | (buffer[1] << 8);
   //Retrieve acceleration along the Y axis
   *ay = buffer[2] | (buffer[3] << 8);
   //Retrieve acceleration along the Z axis
   *az = buffer[4] | (buffer[5] << 8);

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Write SMB380 register
 * @param[in] address Register address
 * @param[in] data Register value
 * @return Error code (see #ErrorCode enumeration)
 **/

error_t smb380WriteReg(uint8_t address, uint8_t data)
{
   uint8_t buffer[2];

   //Set up a write operation
   buffer[0] = address;
   buffer[1] = data;

   //Write the specified register
   return i2cWrite(SMB380_ADDR, buffer, 2);
}


/**
 * @brief Read SMB380 register
 * @param[in] address Register address
 * @param[out] data Register value
 * @return Error code (see #ErrorCode enumeration)
 **/

error_t smb380ReadReg(uint8_t address, uint8_t *data)
{
   error_t error;

   //Set up a read operation
   error = i2cWrite(SMB380_ADDR, &address, 1);
   //Any error to report?
   if(error)
      return error;

   //Read the specified register
   error = i2cRead(SMB380_ADDR, data, 1);
   //Return status code
   return error;
}


/**
 * @brief I2C initialization
 **/

void i2cInit(void)
{
   uint32_t pclk;

   //Power up I2C2 module
   LPC_SC->PCONP |= PCONP_PCI2C2;

   //Configure P0.10 (SDA2) and P0.11 (SCL2)
   LPC_PINCON->PINSEL0 &= ~(PINSEL0_P0_10_MASK | PINSEL0_P0_11_MASK);
   LPC_PINCON->PINSEL0 |= PINSEL0_P0_10_SDA2 | PINSEL0_P0_11_SCL2;

   //No pull-up, no pull-down resitors
   LPC_PINCON->PINMODE0 &= ~((3 << 10) | (3 << 11));
   LPC_PINCON->PINMODE0 |= (2 << 10) | (3 << 11);

   //Open-drain mode
   LPC_PINCON->PINMODE_OD0 = (1 << 10) | (1 << 11);

   //Check I2C2 peripheral clock
   switch(LPC_SC->PCLKSEL1 & PCLKSEL1_PCLK_I2C2)
   {
   case PCLKSEL1_PCLK_I2C2_DIV1:
      pclk = SystemCoreClock;
      break;
   case PCLKSEL1_PCLK_I2C2_DIV2:
      pclk = SystemCoreClock / 2;
      break;
   case PCLKSEL1_PCLK_I2C2_DIV4:
      pclk = SystemCoreClock / 4;
      break;
   default:
      pclk = SystemCoreClock / 8;
      break;
   }

   //Configure I2C clock period
   LPC_I2C2->I2SCLH = (pclk / I2C_CLOCK_FREQUENCY);
   LPC_I2C2->I2SCLL = (pclk / I2C_CLOCK_FREQUENCY);

   //Clear flags
   LPC_I2C2->I2CONCLR = I2CONCLR_STAC | I2CONCLR_SIC | I2CONCLR_AAC;
   //Enable I2C2
   LPC_I2C2->I2CONSET = I2CONSET_I2EN;
}


/**
 * @brief I2C write operation
 * @brief[in] address Slave address
 * @brief[in] data Buffer containing the data to send
 * @param[in] length Number of bytes to send
 * @return Error code (see #ErrorCode enumeration)
 **/

error_t i2cWrite(uint8_t address, const uint8_t *data, size_t length)
{
   error_t error;
   bool_t end;

   //Send a START condition
   LPC_I2C2->I2CONSET = I2CONSET_STA;

   //Initialize loop condition variable
   end = FALSE;

   //Main loop
   while(!end)
   {
      //Wait for the interrupt flag to be set
      while(!(LPC_I2C2->I2CONSET & I2CONSET_SI));

      //Check current state
      switch(LPC_I2C2->I2STAT & I2STAT_STATUS)
      {
      //START condition has been transmitted?
      case 0x08:
         //Write slave address (SLA+W)
         LPC_I2C2->I2DAT = (address << 1);
         //Clear STA bit
         LPC_I2C2->I2CONCLR = I2CONCLR_STAC;
         break;

      //SLA+W has been transmitted and ACK has been received?
      case 0x18:
      //Data byte has been transmitted and ACK has been received?
      case 0x28:
         //Any data to send?
         if(length > 0)
         {
            //Send the first data byte
            LPC_I2C2->I2DAT = *(data++);
            //Decrement byte counter
            length--;
         }
         else
         {
            //Send a STOP condition
            LPC_I2C2->I2CONSET = I2CONSET_STO;
            //Successful processing
            error = NO_ERROR;
            //Exit immediately
            end = TRUE;
         }
         break;

      //SLA+W has been transmitted, no ACK has been received?
      case 0x20:
      //Data byte has been transmitted, no ACK has been received?
      case 0x30:
         //Send a STOP condition
         LPC_I2C2->I2CONSET = I2CONSET_STO;
         //Report an error
         error = ERROR_NO_ACK;
         //Exit immediately
         end = TRUE;
         break;

      //Undefined error?
      default:
         //Set STO bit
         LPC_I2C2->I2CONSET = I2CONSET_STO;
         //Clear STA and AA bits
         LPC_I2C2->I2CONCLR = I2CONCLR_STAC | I2CONCLR_AAC;
         //Report an error
         error = ERROR_FAILURE;
         //Exit immediately
         end = TRUE;
         break;
      }

      //Clear interrupt flag
      LPC_I2C2->I2CONCLR = I2CONCLR_SIC;
   }

   //Return status code
   return error;
}


/**
 * @brief I2C read operation
 * @brief[in] address Slave address
 * @brief[out] data Buffer where to store the incoming data
 * @param[in] length Number of bytes to read
 * @return Error code (see #ErrorCode enumeration)
 **/

error_t i2cRead(uint8_t address, uint8_t *data, size_t length)
{
   error_t error;
   bool_t end;

   //Ensure the length is valid
   if(!length)
      return ERROR_INVALID_LENGTH;

   //Send a START condition
   LPC_I2C2->I2CONSET = I2CONSET_STA;

   //Initialize loop condition variable
   end = FALSE;

   //Main loop
   while(!end)
   {
      //Wait for the interrupt flag to be set
      while(!(LPC_I2C2->I2CONSET & I2CONSET_SI));

      //Check current state
      switch(LPC_I2C2->I2STAT & I2STAT_STATUS)
      {
      //START condition has been transmitted?
      case 0x08:
         //Write slave address (SLA+R)
         LPC_I2C2->I2DAT = (address << 1) | 0x01;
         //Clear STA bit
         LPC_I2C2->I2CONCLR = I2CONCLR_STAC;
         break;

      //SLA+R has been transmitted and ACK has been received?
      case 0x40:
         //Check the number of remaining bytes to be read
         if(length > 1)
         {
            //ACK bit will be returned for the next byte
            LPC_I2C2->I2CONSET = I2CONSET_AA;
         }
         else
         {
            //NO ACK bit will be returned for the last byte
            LPC_I2C2->I2CONCLR = I2CONCLR_AAC;
         }
         break;

      //SLA+R has been transmitted, no ACK has been received?
      case 0x48:
         //Send a STOP condition
         LPC_I2C2->I2CONSET = I2CONSET_STO;
         //Report an error
         error = ERROR_NO_ACK;
         //Exit immediately
         end = TRUE;
         break;

      //Data byte received and ACK has been returned?
      case 0x50:
         //Store incoming data
         *(data++) = LPC_I2C2->I2DAT;
         //Decrement byte counter
         length--;

         //Check the number of remaining bytes to be read
         if(length > 1)
         {
            //ACK bit will be returned for the next byte
            LPC_I2C2->I2CONSET = I2CONSET_AA;
         }
         else
         {
            //NO ACK bit will be returned for the last byte
            LPC_I2C2->I2CONCLR = I2CONCLR_AAC;
         }
         break;

      //Data byte received, no ACK has been returned?
      case 0x58:
         //Store incoming data
         *(data++) = LPC_I2C2->I2DAT;
         //Decrement byte counter
         length--;
         //Send a STOP condition
         LPC_I2C2->I2CONSET = I2CONSET_STO;
         //Successful read operation
         error = NO_ERROR;
         //Exit immediately
         end = TRUE;
         break;

      //Undefined error?
      default:
         //Set STO bit
         LPC_I2C2->I2CONSET = I2CONSET_STO;
         //Clear STA and AA bits
         LPC_I2C2->I2CONCLR = I2CONCLR_STAC | I2CONCLR_AAC;
         //Report an error
         error = ERROR_FAILURE;
         //Exit immediately
         end = TRUE;
         break;
      }

      //Clear interrupt flag
      LPC_I2C2->I2CONCLR = I2CONCLR_SIC;
   }

   //Return status code
   return error;
}
