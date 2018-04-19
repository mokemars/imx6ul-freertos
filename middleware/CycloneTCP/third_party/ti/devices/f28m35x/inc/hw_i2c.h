//###########################################################################
// FILE:   hw_i2c.h
// TITLE:  Macros used when accessing the I2C master and slave hardware.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_I2C_H__
#define __HW_I2C_H__

//*****************************************************************************
// The following are defines for the I2C register offsets.
//*****************************************************************************
#define I2C_O_MSA            0x00000000     // I2C Master Slave Address
#define I2C_O_SOAR           0x00000000     // I2C Slave Own Address
#define I2C_O_SCSR           0x00000004     // I2C Slave Control/Status
#define I2C_O_MCS            0x00000004     // I2C Master Control/Status
#define I2C_O_SDR            0x00000008     // I2C Slave Data
#define I2C_O_MDR            0x00000008     // I2C Master Data
#define I2C_O_MTPR           0x0000000C     // I2C Master Timer Period
#define I2C_O_SIMR           0x0000000C     // I2C Slave Interrupt Mask
#define I2C_O_SRIS           0x00000010     // I2C Slave Raw Interrupt Status
#define I2C_O_MIMR           0x00000010     // I2C Master Interrupt Mask
#define I2C_O_MRIS           0x00000014     // I2C Master Raw Interrupt Status
#define I2C_O_SMIS           0x00000014     // I2C Slave Masked Interrupt
                                            // Status
#define I2C_O_SICR           0x00000018     // I2C Slave Interrupt Clear
#define I2C_O_MMIS           0x00000018     // I2C Master Masked Interrupt
                                            // Status
#define I2C_O_MICR           0x0000001C     // I2C Master Interrupt Clear
#define I2C_O_MCR            0x00000020     // I2C Master Configuration

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_MSA register.
//*****************************************************************************
#define I2C_MSA_SA_M         0x000000FE     // I2C Slave Address
#define I2C_MSA_RS           0x00000001     // Receive not send
#define I2C_MSA_SA_S         1

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_SOAR register.
//*****************************************************************************
#define I2C_SOAR_OAR_M       0x0000007F     // I2C Slave Own Address
#define I2C_SOAR_OAR_S       0

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_SCSR register.
//*****************************************************************************
#define I2C_SCSR_FBR         0x00000004     // First Byte Received
#define I2C_SCSR_TREQ        0x00000002     // Transmit Request
#define I2C_SCSR_DA          0x00000001     // Device Active
#define I2C_SCSR_RREQ        0x00000001     // Receive Request

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_MCS register.
//*****************************************************************************
#define I2C_MCS_BUSBSY       0x00000040     // Bus Busy
#define I2C_MCS_IDLE         0x00000020     // I2C Idle
#define I2C_MCS_ARBLST       0x00000010     // Arbitration Lost
#define I2C_MCS_ACK          0x00000008     // Data Acknowledge Enable
#define I2C_MCS_DATACK       0x00000008     // Acknowledge Data
#define I2C_MCS_ADRACK       0x00000004     // Acknowledge Address
#define I2C_MCS_STOP         0x00000004     // Generate STOP
#define I2C_MCS_START        0x00000002     // Generate START
#define I2C_MCS_ERROR        0x00000002     // Error
#define I2C_MCS_RUN          0x00000001     // I2C Master Enable
#define I2C_MCS_BUSY         0x00000001     // I2C Busy

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_SDR register.
//*****************************************************************************
#define I2C_SDR_DATA_M       0x000000FF     // Data for Transfer
#define I2C_SDR_DATA_S       0

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_MDR register.
//*****************************************************************************
#define I2C_MDR_DATA_M       0x000000FF     // Data Transferred
#define I2C_MDR_DATA_S       0

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_MTPR register.
//*****************************************************************************
#define I2C_MTPR_TPR_M       0x000000FF     // SCL Clock Period
#define I2C_MTPR_TPR_S       0

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_SIMR register.
//*****************************************************************************
#define I2C_SIMR_STOPIM      0x00000004     // Stop Condition Interrupt Mask
#define I2C_SIMR_STARTIM     0x00000002     // Start Condition Interrupt Mask
#define I2C_SIMR_DATAIM      0x00000001     // Data Interrupt Mask

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_SRIS register.
//*****************************************************************************
#define I2C_SRIS_STOPRIS     0x00000004     // Stop Condition Raw Interrupt
                                            // Status
#define I2C_SRIS_STARTRIS    0x00000002     // Start Condition Raw Interrupt
                                            // Status
#define I2C_SRIS_DATARIS     0x00000001     // Data Raw Interrupt Status

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_MIMR register.
//*****************************************************************************
#define I2C_MIMR_IM          0x00000001     // Interrupt Mask

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_MRIS register.
//*****************************************************************************
#define I2C_MRIS_RIS         0x00000001     // Raw Interrupt Status

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_SMIS register.
//*****************************************************************************
#define I2C_SMIS_STOPMIS     0x00000004     // Stop Condition Masked Interrupt
                                            // Status
#define I2C_SMIS_STARTMIS    0x00000002     // Start Condition Masked Interrupt
                                            // Status
#define I2C_SMIS_DATAMIS     0x00000001     // Data Masked Interrupt Status

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_SICR register.
//*****************************************************************************
#define I2C_SICR_STOPIC      0x00000004     // Stop Condition Interrupt Clear
#define I2C_SICR_STARTIC     0x00000002     // Start Condition Interrupt Clear
#define I2C_SICR_DATAIC      0x00000001     // Data Interrupt Clear

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_MMIS register.
//*****************************************************************************
#define I2C_MMIS_MIS         0x00000001     // Masked Interrupt Status

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_MICR register.
//*****************************************************************************
#define I2C_MICR_IC          0x00000001     // Interrupt Clear

//*****************************************************************************
// The following are defines for the bit fields in the I2C_O_MCR register.
//*****************************************************************************
#define I2C_MCR_SFE          0x00000020     // I2C Slave Function Enable
#define I2C_MCR_MFE          0x00000010     // I2C Master Function Enable
#define I2C_MCR_LPBK         0x00000001     // I2C Loopback


#endif // __HW_I2C_H__



