/*-----------------------------------------------------------------------------
 * Name:    I2C.h
 * Purpose: I2C definitions
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __I2C_H
#define __I2C_H

/* I2C Low level function prototypes */
extern uint32_t   I2C_Init   (void);
extern uint32_t   I2C_Start  (void);
extern uint32_t   I2C_Stop   (void);
extern uint32_t   I2C_Addr   (uint8_t  adr, uint8_t dir);
extern uint32_t   I2C_Write  (uint8_t  byte);
extern uint32_t   I2C_Read   (uint32_t ack, uint8_t *byte);

/* I2C Master data transfer function prototypes */
uint32_t I2C_WrBuf  (uint8_t addr, uint8_t *buf, uint32_t cnt);
uint32_t I2C_RdBuf  (uint8_t addr, uint8_t *buf, uint32_t cnt);

uint32_t I2C_WrData (uint8_t addr, uint8_t secByte, uint8_t *buf, uint32_t cnt);
uint32_t I2C_RdData (uint8_t addr, uint8_t secByte, uint8_t *buf, uint32_t cnt);

#endif /* __I2C_H */
