//###########################################################################
// FILE:   gpio.h
// TITLE:  Defines and Macros for GPIO API.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright:
// Copyright (C) 2011-2015 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

#ifndef __GPIO_H__
#define __GPIO_H__

//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
// The following values define the bit field for the ucPins argument to several
// of the APIs.
//*****************************************************************************
#define GPIO_PIN_0                0x00000001    // GPIO pin 0
#define GPIO_PIN_1                0x00000002    // GPIO pin 1
#define GPIO_PIN_2                0x00000004    // GPIO pin 2
#define GPIO_PIN_3                0x00000008    // GPIO pin 3
#define GPIO_PIN_4                0x00000010    // GPIO pin 4
#define GPIO_PIN_5                0x00000020    // GPIO pin 5
#define GPIO_PIN_6                0x00000040    // GPIO pin 6
#define GPIO_PIN_7                0x00000080    // GPIO pin 7

//*****************************************************************************
// Values that can be passed to GPIODirModeSet as the ulPinIO parameter, and
// returned from GPIODirModeGet.
//*****************************************************************************
#define GPIO_DIR_MODE_IN          0x00000000    // Pin is a GPIO input
#define GPIO_DIR_MODE_OUT         0x00000001    // Pin is a GPIO output
#define GPIO_DIR_MODE_HW          0x00000002    // Pin is a peripheral function

//*****************************************************************************
// Values that can be passed to GPIOIntTypeSet as the ulIntType parameter, and
// returned from GPIOIntTypeGet.
//*****************************************************************************
#define GPIO_FALLING_EDGE         0x00000000    // Interrupt on falling edge
#define GPIO_RISING_EDGE          0x00000004    // Interrupt on rising edge
#define GPIO_BOTH_EDGES           0x00000001    // Interrupt on both edges
#define GPIO_LOW_LEVEL            0x00000002    // Interrupt on low level
#define GPIO_HIGH_LEVEL           0x00000007    // Interrupt on high level

//*****************************************************************************
// Values that can be passed to GPIOPadConfigSet as the ulPadType parameter,
// and returned by GPIOPadConfigGet in the *pulPadType parameter.
//*****************************************************************************
#define GPIO_PIN_TYPE_STD         0x00000008    // Push-pull
#define GPIO_PIN_TYPE_STD_WPU     0x0000000A    // Push-pull with weak pull-up
#define GPIO_PIN_TYPE_OD          0x00000009    // Open-drain
#define GPIO_PIN_TYPE_OD_WPU      0x0000000B    // Open-drain with weak pull-up
#define GPIO_PIN_TYPE_ANALOG      0x00000000    // Analog Function

//******************************************************************************
//Values that can be passed to
//******************************************************************************
#define GPIO_PIN_C_CORE_SELECT    0x01          // GPIO pin assigned to control
                                                // system
#define GPIO_PIN_M_CORE_SELECT    0x00          // GPIO pin assigned to Master
                                                // system - default on reset


//*****************************************************************************
// Values that can be passed to GPIOPinConfigure as the ulPinConfig parameter.
// ulPinConfig := bit 31 to bit 24 -> not used
//                bit 23 to bit 16 -> base address of GPIO port
//                bit 15 to bit 8 -> shift factor for PMCx absolute value
//                                  (given by bits 15-8)
//                bit 7 to bit 4 -> Alternate port select register value
//                                  (this register or alternate port vs
//                                   primary port is introduced in F28M35x)
//                bit 3 to bit 0 -> PMCx value of GPIOPCTL register value
//*****************************************************************************
// GPIO pin A0
#define GPIO_PA0_U0RX             0x00000001
#define GPIO_PA0_I2C1SCL          0x00000008
#define GPIO_PA0_U1RX             0x00000009

// GPIO pin A1
#define GPIO_PA1_U0TX             0x00000401
#define GPIO_PA1_I2C1SDA          0x00000408
#define GPIO_PA1_U1TX             0x00000409
#define GPIO_PA1_SSI1FSS          0x0000041f

// GPIO pin A2
#define GPIO_PA2_SSI0CLK          0x00000801
#define GPIO_PA2_MIITXD2          0x00000803
#define GPIO_PA2_U1CTS            0x0000081e

// GPIO pin A3
#define GPIO_PA3_SSI0FSS          0x00000c01
#define GPIO_PA3_MIITXD1          0x00000c03
#define GPIO_PA3_U1DCD            0x00000c1e
#define GPIO_PA3_SSI1CLK          0x00000c1f

// GPIO pin A4
#define GPIO_PA4_SSI0RX           0x00001001
#define GPIO_PA4_MIITXD0          0x00001003
#define GPIO_PA4_CAN0RX           0x00001005
#define GPIO_PA4_U1DSR            0x0000101e

// GPIO pin A5
#define GPIO_PA5_SSI0TX           0x00001401
#define GPIO_PA5_MIIRXDV          0x00001403
#define GPIO_PA5_CAN0TX           0x00001405
#define GPIO_PA5_U1RTS            0x0000141e

// GPIO pin A6
#define GPIO_PA6_I2C1SCL          0x00001801
#define GPIO_PA6_CCP1             0x00001802
#define GPIO_PA6_MIIRXCK          0x00001803
#define GPIO_PA6_CAN0RX           0x00001806
#define GPIO_PA6_USB0EPEN         0x00001808
#define GPIO_PA6_U1CTS            0x00001809
#define GPIO_PA6_U1DTR            0x0000181e

// GPIO pin A7
#define GPIO_PA7_I2C1SDA          0x00001c01
#define GPIO_PA7_CCP4             0x00001c02
#define GPIO_PA7_MIIRXER          0x00001c03
#define GPIO_PA7_CAN0TX           0x00001c06
#define GPIO_PA7_CCP3             0x00001c07
#define GPIO_PA7_USB0PFLT         0x00001c08
#define GPIO_PA7_U1DCD            0x00001c09
#define GPIO_PA7_MIIRXD1          0x00001c1c
#define GPIO_PA7_U1RI             0x00001c1e

// GPIO pin B0
#define GPIO_PB0_CCP0             0x00010001
#define GPIO_PB0_U1RX             0x00010005
#define GPIO_PB0_SSI2TX           0x0001001d
#define GPIO_PB0_CAN1TX           0x0001001e
#define GPIO_PB0_U4TX             0x0001001f

// GPIO pin B1
#define GPIO_PB1_CCP2             0x00010401
#define GPIO_PB1_CCP1             0x00010404
#define GPIO_PB1_U1TX             0x00010405
#define GPIO_PB1_SSI2RX           0x0001041d

// GPIO pin B2
#define GPIO_PB2_I2C0SCL          0x00010801
#define GPIO_PB2_CCP3             0x00010804
#define GPIO_PB2_CCP0             0x00010805
#define GPIO_PB2_USB0EPEN         0x00010808
#define GPIO_PB2_SSI2CLK          0x0001081d
#define GPIO_PB2_CAN1RX           0x0001081e
#define GPIO_PB2_U4RX             0x0001081f

// GPIO pin B3
#define GPIO_PB3_I2C0SDA          0x00010c01
#define GPIO_PB3_USB0PFLT         0x00010c08
#define GPIO_PB3_SSI2FSS          0x00010c1d
#define GPIO_PB3_U1RX             0x00010c1e

// GPIO pin B4
#define GPIO_PB4_U2RX             0x00011004
#define GPIO_PB4_CAN0RX           0x00011005
#define GPIO_PB4_U1RX             0x00011007
#define GPIO_PB4_EPI0S23          0x00011008
#define GPIO_PB4_CAN1TX           0x0001101e
#define GPIO_PB4_SSI1TX           0x0001101f

// GPIO pin B5
#define GPIO_PB5_CCP5             0x00011402
#define GPIO_PB5_CCP6             0x00011403
#define GPIO_PB5_CCP0             0x00011404
#define GPIO_PB5_CAN0TX           0x00011405
#define GPIO_PB5_CCP2             0x00011406
#define GPIO_PB5_U1TX             0x00011407
#define GPIO_PB5_EPI0S22          0x00011408
#define GPIO_PB5_CAN1RX           0x0001141e
#define GPIO_PB5_SSI1RX           0x0001141f

// GPIO pin B6
#define GPIO_PB6_CCP1             0x00011801
#define GPIO_PB6_CCP7             0x00011802
#define GPIO_PB6_CCP5             0x00011806
#define GPIO_PB6_MIICRS           0x0001181c
#define GPIO_PB6_I2C0SDA          0x0001181d
#define GPIO_PB6_U1TX             0x0001181e
#define GPIO_PB6_SSI1CLK          0x0001181f

// GPIO pin B7
#define GPIO_PB7_NMI              0x00011c04
#define GPIO_PB7_MIIRXD1          0x00011c07
#define GPIO_PB7_I2C0SCL          0x00011c1d
#define GPIO_PB7_U1RX             0x00011c1e
#define GPIO_PB7_SSI1FSS          0x00011c1f

// GPIO pin C4
#define GPIO_PC4_CCP5             0x00021001
#define GPIO_PC4_MIITXD3          0x00021003
#define GPIO_PC4_CCP2             0x00021005
#define GPIO_PC4_CCP4             0x00021006
#define GPIO_PC4_EPI0S2           0x00021008
#define GPIO_PC4_CCP1             0x00021009

// GPIO pin C5
#define GPIO_PC5_CCP1             0x00021401
#define GPIO_PC5_CCP3             0x00021405
#define GPIO_PC5_USB0EPEN         0x00021406
#define GPIO_PC5_EPI0S3           0x00021408

// GPIO pin C6
#define GPIO_PC6_CCP3             0x00021801
#define GPIO_PC6_U1RX             0x00021805
#define GPIO_PC6_CCP0             0x00021806
#define GPIO_PC6_USB0PFLT         0x00021807
#define GPIO_PC6_EPI0S4           0x00021808

// GPIO pin C7
#define GPIO_PC7_CCP4             0x00021c01
#define GPIO_PC7_CCP0             0x00021c04
#define GPIO_PC7_U1TX             0x00021c05
#define GPIO_PC7_USB0PFLT         0x00021c06
#define GPIO_PC7_EPI0S5           0x00021c08

// GPIO pin D0
#define GPIO_PD0_CAN0RX           0x00030002
#define GPIO_PD0_U2RX             0x00030004
#define GPIO_PD0_U1RX             0x00030005
#define GPIO_PD0_CCP6             0x00030006
#define GPIO_PD0_MIIRXDV          0x00030007
#define GPIO_PD0_U1CTS            0x00030009
#define GPIO_PD0_MIIRXD2          0x0003001c
#define GPIO_PD0_SSI0TX           0x0003001d
#define GPIO_PD0_CAN1TX           0x0003001e
#define GPIO_PD0_USB0EPEN         0x0003001f

// GPIO pin D1
#define GPIO_PD1_CAN0TX           0x00030402
#define GPIO_PD1_U2TX             0x00030404
#define GPIO_PD1_U1TX             0x00030405
#define GPIO_PD1_CCP7             0x00030406
#define GPIO_PD1_MIITXER          0x00030407
#define GPIO_PD1_U1DCD            0x00030409
#define GPIO_PD1_CCP2             0x0003040a
#define GPIO_PD1_MIICOL           0x0003041c
#define GPIO_PD1_SSI0RX           0x0003041d
#define GPIO_PD1_CAN1RX           0x0003041e
#define GPIO_PD1_USB0PFLT         0x0003041f

// GPIO pin D2
#define GPIO_PD2_U1RX             0x00030801
#define GPIO_PD2_CCP6             0x00030802
#define GPIO_PD2_CCP5             0x00030804
#define GPIO_PD2_MIIRXDV          0x00030807
#define GPIO_PD2_EPI0S20          0x00030808
#define GPIO_PD2_SSI0CLK          0x0003081d
#define GPIO_PD2_U1TX             0x0003081e
#define GPIO_PD2_CAN0RX           0x0003081f

// GPIO pin D3
#define GPIO_PD3_U1TX             0x00030c01
#define GPIO_PD3_CCP7             0x00030c02
#define GPIO_PD3_CCP0             0x00030c04
#define GPIO_PD3_EPI0S21          0x00030c08
#define GPIO_PD3_SSI0FSS          0x00030c1d
#define GPIO_PD3_U1RX             0x00030c1e
#define GPIO_PD3_CAN0TX           0x00030c1f

// GPIO pin D4
#define GPIO_PD4_CCP0             0x00031001
#define GPIO_PD4_CCP3             0x00031002
#define GPIO_PD4_MIITXD3          0x00031004
#define GPIO_PD4_U1RI             0x00031009
#define GPIO_PD4_EPI0S19          0x0003100a
#define GPIO_PD4_U3TX             0x0003101e
#define GPIO_PD4_CAN1TX           0x0003101f

// GPIO pin D5
#define GPIO_PD5_CCP2             0x00031401
#define GPIO_PD5_CCP4             0x00031402
#define GPIO_PD5_MIITXD2          0x00031404
#define GPIO_PD5_U2RX             0x00031409
#define GPIO_PD5_EPI0S28          0x0003140a
#define GPIO_PD5_U3RX             0x0003141e
#define GPIO_PD5_CAN1RX           0x0003141f

// GPIO pin D6
#define GPIO_PD6_MIITXD1          0x00031804
#define GPIO_PD6_U2TX             0x00031809
#define GPIO_PD6_EPI0S29          0x0003180a
#define GPIO_PD6_I2C1SDA          0x0003181e
#define GPIO_PD6_U1TX             0x0003181f

// GPIO pin D7
#define GPIO_PD7_CCP1             0x00031c03
#define GPIO_PD7_MIITXD0          0x00031c04
#define GPIO_PD7_U1DTR            0x00031c09
#define GPIO_PD7_EPI0S30          0x00031c0a
#define GPIO_PD7_I2C1SCL          0x00031c1e
#define GPIO_PD7_U1RX             0x00031c1f

// GPIO pin E0
#define GPIO_PE0_SSI1CLK          0x00040002
#define GPIO_PE0_CCP3             0x00040003
#define GPIO_PE0_EPI0S8           0x00040008
#define GPIO_PE0_USB0PFLT         0x00040009
#define GPIO_PE0_SSI3TX           0x0004001d
#define GPIO_PE0_CAN0RX           0x0004001e
#define GPIO_PE0_SSI1TX           0x0004001f

// GPIO pin E1
#define GPIO_PE1_SSI1FSS          0x00040402
#define GPIO_PE1_CCP2             0x00040404
#define GPIO_PE1_CCP6             0x00040405
#define GPIO_PE1_EPI0S9           0x00040408
#define GPIO_PE1_SSI3RX           0x0004041d
#define GPIO_PE1_CAN0TX           0x0004041e
#define GPIO_PE1_SSI1RX           0x0004041f

// GPIO pin E2
#define GPIO_PE2_CCP4             0x00040801
#define GPIO_PE2_SSI1RX           0x00040802
#define GPIO_PE2_CCP2             0x00040805
#define GPIO_PE2_EPI0S24          0x00040808
#define GPIO_PE2_SSI3CLK          0x0004081d
#define GPIO_PE2_U2RX             0x0004081e
#define GPIO_PE2_SSI1CLK          0x0004081f

// GPIO pin E3
#define GPIO_PE3_CCP1             0x00040c01
#define GPIO_PE3_SSI1TX           0x00040c02
#define GPIO_PE3_CCP7             0x00040c05
#define GPIO_PE3_EPI0S25          0x00040c08
#define GPIO_PE3_SSI3FSS          0x00040c1d
#define GPIO_PE3_U2TX             0x00040c1e
#define GPIO_PE3_SSI1FSS          0x00040c1f

// GPIO pin E4
#define GPIO_PE4_CCP3             0x00041001
#define GPIO_PE4_U2TX             0x00041005
#define GPIO_PE4_CCP2             0x00041006
#define GPIO_PE4_MIIRXD0          0x00041007
#define GPIO_PE4_U0RX             0x0004101d
#define GPIO_PE4_USB0EPEN         0x0004101f

// GPIO pin E5
#define GPIO_PE5_CCP5             0x00041401
#define GPIO_PE5_MIITXER          0x0004141c
#define GPIO_PE5_U0TX             0x0004141d
#define GPIO_PE5_USB0PFLT         0x0004141f

// GPIO pin E6
#define GPIO_PE6_U1CTS            0x00041809
#define GPIO_PE6_MIIMDIO          0x0004181c
#define GPIO_PE6_CAN0RX           0x0004181d

// GPIO pin E7
#define GPIO_PE7_U1DCD            0x00041c09
#define GPIO_PE7_MIIRXD3          0x00041c1c
#define GPIO_PE7_CAN0TX           0x00041c1d

// GPIO pin F0
#define GPIO_PF0_CAN1RX           0x00050001
#define GPIO_PF0_MIIRXCK          0x00050004
#define GPIO_PF0_U1DSR            0x00050009
#define GPIO_PF0_I2C0SDA          0x0005001d
#define GPIO_PF0_TRACED2          0x0005001e

// GPIO pin F1
#define GPIO_PF1_CAN1TX           0x00050401
#define GPIO_PF1_MIIRXER          0x00050404
#define GPIO_PF1_U1RTS            0x00050409
#define GPIO_PF1_CCP3             0x0005040a
#define GPIO_PF1_I2C0SCL          0x0005041d
#define GPIO_PF1_TRACED3          0x0005041e

// GPIO pin F2
#define GPIO_PF2_MIIPHYINTRn      0x00050803
#define GPIO_PF2_SSI1CLK          0x00050809
#define GPIO_PF2_TRACECLK         0x0005081e
#define GPIO_PF2_XCLKOUT0         0x0005081f

// GPIO pin F3
#define GPIO_PF3_MIIMDC           0x00050c03
#define GPIO_PF3_SSI1FSS          0x00050c09
#define GPIO_PF3_U0TX             0x00050c1d
#define GPIO_PF3_TRACED0          0x00050c1e

// GPIO pin F4
#define GPIO_PF4_CCP0             0x00051001
#define GPIO_PF4_MIIMDIO          0x00051003
#define GPIO_PF4_EPI0S12          0x00051008
#define GPIO_PF4_SSI1RX           0x00051009
#define GPIO_PF4_U0RX             0x0005101d

// GPIO pin F5
#define GPIO_PF5_CCP2             0x00051401
#define GPIO_PF5_MIIRXD3          0x00051403
#define GPIO_PF5_EPI0S15          0x00051408
#define GPIO_PF5_SSI1TX           0x00051409

// GPIO pin F6
#define GPIO_PF6_CCP1             0x00051801
#define GPIO_PF6_MIIRXD2          0x00051803
#define GPIO_PF6_U1RTS            0x0005180a

#if 0
// GPIO pin F7 - pad but not pinned out
#define GPIO_PF7_CCP4             0x00051C01
#define GPIO_PF7_MIIRXD1          0x00051803
#define GPIO_PF7_EPIOS12          0x0005180a
#endif

// GPIO pin G0
#define GPIO_PG0_U2RX             0x00060001
#define GPIO_PG0_I2C1SCL          0x00060003
#define GPIO_PG0_USB0EPEN         0x00060007
#define GPIO_PG0_EPI0S13          0x00060008
#define GPIO_PG0_MIIRXD2          0x0006001c
#define GPIO_PG0_U4RX             0x0006001d

// GPIO pin G1
#define GPIO_PG1_U2TX             0x00060401
#define GPIO_PG1_I2C1SDA          0x00060403
#define GPIO_PG1_EPI0S14          0x00060408
#define GPIO_PG1_MIIRXD1          0x0006041c
#define GPIO_PG1_U4TX             0x0006041d

// GPIO pin G2
#define GPIO_PG2_MIICOL           0x00060803

// GPIO pin G3
#define GPIO_PG3_MIICRS           0x00060c03
#define GPIO_PG3_MIIRXDV          0x00060c1c
#define GPIO_PG3_TRACED1          0x00060c1e

#if 0

// GPIO pin G4 - pad but not pinned out
#define GPIO_PG4_CCP3             0x00060c01
#define GPIO_PG4_MIIRXD0          0x00060c03
#define GPIO_PG4_EPIOS15          0x00060c08
#define GPIO_PG4_U1RI             0x00060c0a
#endif

// GPIO pin G5
#define GPIO_PG5_CCP5             0x00061401
#define GPIO_PG5_MIITXEN          0x00061403
#define GPIO_PG5_U1DTR            0x0006140a

// GPIO pin G6
#define GPIO_PG6_MIITXCLK         0x00061803
#define GPIO_PG6_U1RI             0x0006180a

// GPIO pin G7
#define GPIO_PG7_MIITXER          0x00061c03
#define GPIO_PG7_CCP5             0x00061c08
#define GPIO_PG7_EPI0S31          0x00061c09

// GPIO pin H0
#define GPIO_PH0_CCP6             0x00070001
#define GPIO_PH0_MIIPHYRSTn       0x00070003
#define GPIO_PH0_EPI0S6           0x00070008
#define GPIO_PH0_SSI3TX           0x0007001d

// GPIO pin H1
#define GPIO_PH1_CCP7             0x00070401
#define GPIO_PH1_EPI0S7           0x00070408
#define GPIO_PH1_MIIRXD0          0x0007041c
#define GPIO_PH1_SSI3RX           0x0007041d

// GPIO pin H2
#define GPIO_PH2_EPI0S1           0x00070808
#define GPIO_PH2_MIITXD3          0x00070809
#define GPIO_PH2_SSI3CLK          0x0007081d

// GPIO pin H3
#define GPIO_PH3_USB0EPEN         0x00070c04
#define GPIO_PH3_EPI0S0           0x00070c08
#define GPIO_PH3_MIITXD2          0x00070c09
#define GPIO_PH3_SSI3FSS          0x00070c1d

// GPIO pin H4
#define GPIO_PH4_USB0PFLT         0x00071004
#define GPIO_PH4_EPI0S10          0x00071008
#define GPIO_PH4_MIITXD1          0x00071009
#define GPIO_PH4_SSI1CLK          0x0007100b
#define GPIO_PH4_U3TX             0x0007101d

// GPIO pin H5
#define GPIO_PH5_EPI0S11          0x00071408
#define GPIO_PH5_MIITXD0          0x00071409
#define GPIO_PH5_SSI1FSS          0x0007140b
#define GPIO_PH5_U3RX             0x0007141d

// GPIO pin H6
#define GPIO_PH6_EPI0S26          0x00071808
#define GPIO_PH6_MIIRXDV          0x00071809
#define GPIO_PH6_SSI1RX           0x0007180b
#define GPIO_PH6_MIITXEN          0x0007181c
#define GPIO_PH6_SSI0TX           0x0007181d

// GPIO pin H7
#define GPIO_PH7_MIIRXCK          0x00071c03
#define GPIO_PH7_EPI0S27          0x00071c08
#define GPIO_PH7_SSI1TX           0x00071c0b
#define GPIO_PH7_MIITXCK          0x00071c1c
#define GPIO_PH7_SSI0RX           0x00071c1d

// GPIO pin J0
#define GPIO_PJ0_MIIRXER          0x00080003
#define GPIO_PJ0_EPI0S16          0x00080008
#define GPIO_PJ0_I2C1SCL          0x0008000b
#define GPIO_PJ0_SSI0CLK          0x0008001d

// GPIO pin J1
#define GPIO_PJ1_EPI0S17          0x00080408
#define GPIO_PJ1_USB0PFLT         0x00080409
#define GPIO_PJ1_I2C1SDA          0x0008040b
#define GPIO_PJ1_MIIRXDV          0x0008041c
#define GPIO_PJ1_SSI0FSS          0x0008041d

// GPIO pin J2
#define GPIO_PJ2_EPI0S18          0x00080808
#define GPIO_PJ2_CCP0             0x00080809
#define GPIO_PJ2_MIIRXCK          0x0008081c
#define GPIO_PJ2_SSI0CLK          0x0008081d
#define GPIO_PJ2_U0TX             0x0008081e

// GPIO pin J3
#define GPIO_PJ3_EPI0S19          0x00080c08
#define GPIO_PJ3_U1CTS            0x00080c09
#define GPIO_PJ3_CCP6             0x00080c0a
#define GPIO_PJ3_MIIMDC           0x00080c1c
#define GPIO_PJ3_SSI0FSS          0x00080c1d
#define GPIO_PJ3_U0RX             0x00080c1e

// GPIO pin J4
#define GPIO_PJ4_EPI0S28          0x00081008
#define GPIO_PJ4_U1DCD            0x00081009
#define GPIO_PJ4_CCP4             0x0008100a
#define GPIO_PJ4_MIICOL           0x0008101c
#define GPIO_PJ4_SSI1CLK          0x0008101d

// GPIO pin J5
#define GPIO_PJ5_EPI0S29          0x00081408
#define GPIO_PJ5_U1DSR            0x00081409
#define GPIO_PJ5_CCP2             0x0008140a
#define GPIO_PJ5_MIICRS           0x0008141c
#define GPIO_PJ5_SSI1FSS          0x0008141d

// GPIO pin J6
#define GPIO_PJ6_EPI0S30          0x00081808
#define GPIO_PJ6_U1RTS            0x00081809
#define GPIO_PJ6_CCP1             0x0008180a
#define GPIO_PJ6_MIIPHYINTRn      0x0008181c
#define GPIO_PJ6_U2RX             0x0008181d

// GPIO pin J7
#define GPIO_PJ7_U1DTR            0x00081c09
#define GPIO_PJ7_CCP0             0x00081c0a
#define GPIO_PJ7_MIIPHYRSTn       0x00081c1c
#define GPIO_PJ7_U2TX             0x00081c1d




//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern void GPIODirModeSet(unsigned long ulPort, unsigned char ucPins,
                           unsigned long ulPinIO);
extern unsigned long GPIODirModeGet(unsigned long ulPort, unsigned char ucPin);
extern void GPIOIntTypeSet(unsigned long ulPort, unsigned char ucPins,
                           unsigned long ulIntType);
extern unsigned long GPIOIntTypeGet(unsigned long ulPort, unsigned char ucPin);
extern void GPIOPadConfigSet(unsigned long ulPort, unsigned char ucPins,
                             unsigned long ulPadType);
extern void GPIOPadConfigGet(unsigned long ulPort, unsigned char ucPin,
                             unsigned long *pulPadType);
extern void GPIOPinIntEnable(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinIntDisable(unsigned long ulPort, unsigned char ucPins);
extern long GPIOPinIntStatus(unsigned long ulPort, tBoolean bMasked);
extern void GPIOPinIntClear(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPortIntRegister(unsigned long ulPort,
                                void (*pfnIntHandler)(void));
extern void GPIOPortIntUnregister(unsigned long ulPort);
extern long GPIOPinRead(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinWrite(unsigned long ulPort, unsigned char ucPins,
                         unsigned char ucVal);
extern void GPIOPinConfigure(unsigned long ulPinConfig);
extern void GPIOPinTypeCAN(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeEPI(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeGPIOInput(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeGPIOOutput(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeGPIOOutputOD(unsigned long ulPort,
                                    unsigned char ucPins);
extern void GPIOPinTypeGPIOOutputLow(unsigned long ulPort, 
                                     unsigned char ucPins);
extern void GPIOPinTypeGPIOOutputHigh(unsigned long ulPort, 
                                      unsigned char ucPins);                                     
extern void GPIOPinTypeI2C(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeSSI(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeTimer(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeUART(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeUSBAnalog(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinTypeUSBDigital(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinConfigureCoreSelect(unsigned long ulPort,
                                       unsigned char ucPins,
                                       unsigned char ucCore);
extern unsigned long GPIOPinLockStatus(unsigned long ulPort,
                                       unsigned char ucPins);
extern void GPIOPinLock(unsigned long ulPort, unsigned char ucPins);
extern void GPIOPinUnlock(unsigned long ulPort, unsigned char ucPins);

//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif //  __GPIO_H__



