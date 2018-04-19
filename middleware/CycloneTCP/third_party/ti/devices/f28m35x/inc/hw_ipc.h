//###########################################################################
// FILE:   hw_ipc.h
// TITLE:  Macros for use in accessing the IPC registers.
//###########################################################################
// $TI Release: F28M35x Support Library v206 $
// $Release Date: Thu Mar  5 10:10:30 CST 2015 $
// $Copyright: Copyright (C) 2011-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_IPC_H__
#define __HW_IPC_H__

//*****************************************************************************
// The following are defines for the M to C Inter Processor Communication
// register offsets.
//*****************************************************************************
#define IPC_O_CTOMIPCACK         0x000000000 // C28 to M3 Interrupt Request
                                             // Clear
#define IPC_O_CTOMIPCSTS         0x000000004 // C28 to M3 Interrupt Status
#define IPC_O_MTOCIPCSET         0x000000008 // M3 to C28 Interrupt Request Set
#define IPC_O_MTOCIPCCLR         0x00000000C // M3 to C28 Interrupt Request
                                             // Clear
#define IPC_O_MTOCIPCFLG         0x000000010 // M3 to C28 Interrupt Status Flag
#define IPC_O_MIPCCOUNTERL       0x000000018 // IPC Counter Low Register
#define IPC_O_MIPCCOUNTERH       0x00000001C // IPC Counter High Register
#define IPC_O_CTOMIPCCOM         0x000000020 // C28 to M3 Command
#define IPC_O_CTOMIPCADDR        0x000000024 // C28 to M3 Address
#define IPC_O_CTOMIPCDATAW       0x000000028 // C28 to M3 Data Write
#define IPC_O_CTOMIPCDATAR       0x00000002C // C28 to M3 Data Read
#define IPC_O_MTOCIPCCOM         0x000000030 // M3 to C28 Command Register
#define IPC_O_MTOCIPCADDR        0x000000034 // M3 to C28 Address
#define IPC_O_MTOCIPCDATAW       0x000000038 // M3 to C28 Data Write
#define IPC_O_MTOCIPCDATAR       0x00000003C // M3 to C28 Data Read
#define IPC_O_CTOMIPCBOOTSTS     0x000000040 // IPC Boot Status
#define IPC_O_MTOCIPCBOOTMODE    0x000000044 // IPC Boot Mode
#define IPC_O_MPUMPREQUEST       0x000000048 // Flash Programming Semaphore
                                             // Pump Request
#define IPC_O_MCLKREQUEST        0x00000004C // Clock Configuratioin Semaphore
                                             // M3 Request


//*****************************************************************************
// The following are defines for the bit fields in the CTOMIPCACK register.
//*****************************************************************************
#define IPC_CTOMIPCACK_IPC1     0x00000001  // C28 to M3 IPC 1 Ack
#define IPC_CTOMIPCACK_IPC2     0x00000002  // C28 to M3 IPC 2 Ack
#define IPC_CTOMIPCACK_IPC3     0x00000004  // C28 to M3 IPC 3 Ack
#define IPC_CTOMIPCACK_IPC4     0x00000008  // C28 to M3 IPC 4 Ack
#define IPC_CTOMIPCACK_IPC5     0x00000010  // C28 to M3 IPC 5 Ack
#define IPC_CTOMIPCACK_IPC6     0x00000020  // C28 to M3 IPC 6 Ack
#define IPC_CTOMIPCACK_IPC7     0x00000040  // C28 to M3 IPC 7 Ack
#define IPC_CTOMIPCACK_IPC8     0x00000080  // C28 to M3 IPC 8 Ack
#define IPC_CTOMIPCACK_IPC9     0x00000100  // C28 to M3 IPC 9 Ack
#define IPC_CTOMIPCACK_IPC10    0x00000200  // C28 to M3 IPC 10 Ack
#define IPC_CTOMIPCACK_IPC11    0x00000400  // C28 to M3 IPC 11 Ack
#define IPC_CTOMIPCACK_IPC12    0x00000800  // C28 to M3 IPC 12 Ack
#define IPC_CTOMIPCACK_IPC13    0x00001000  // C28 to M3 IPC 13 Ack
#define IPC_CTOMIPCACK_IPC14    0x00002000  // C28 to M3 IPC 14 Ack
#define IPC_CTOMIPCACK_IPC15    0x00004000  // C28 to M3 IPC 15 Ack
#define IPC_CTOMIPCACK_IPC16    0x00008000  // C28 to M3 IPC 16 Ack
#define IPC_CTOMIPCACK_IPC17    0x00010000  // C28 to M3 IPC 17 Ack
#define IPC_CTOMIPCACK_IPC18    0x00020000  // C28 to M3 IPC 18 Ack
#define IPC_CTOMIPCACK_IPC19    0x00040000  // C28 to M3 IPC 19 Ack
#define IPC_CTOMIPCACK_IPC20    0x00080000  // C28 to M3 IPC 20 Ack
#define IPC_CTOMIPCACK_IPC21    0x00100000  // C28 to M3 IPC 21 Ack
#define IPC_CTOMIPCACK_IPC22    0x00200000  // C28 to M3 IPC 22 Ack
#define IPC_CTOMIPCACK_IPC23    0x00400000  // C28 to M3 IPC 23 Ack
#define IPC_CTOMIPCACK_IPC24    0x00800000  // C28 to M3 IPC 24 Ack
#define IPC_CTOMIPCACK_IPC25    0x01000000  // C28 to M3 IPC 25 Ack
#define IPC_CTOMIPCACK_IPC26    0x02000000  // C28 to M3 IPC 26 Ack
#define IPC_CTOMIPCACK_IPC27    0x04000000  // C28 to M3 IPC 27 Ack
#define IPC_CTOMIPCACK_IPC28    0x08000000  // C28 to M3 IPC 28 Ack
#define IPC_CTOMIPCACK_IPC29    0x10000000  // C28 to M3 IPC 29 Ack
#define IPC_CTOMIPCACK_IPC30    0x20000000  // C28 to M3 IPC 30 Ack
#define IPC_CTOMIPCACK_IPC31    0x40000000  // C28 to M3 IPC 31 Ack
#define IPC_CTOMIPCACK_IPC32    0x80000000  // C28 to M3 IPC 32 Ack

//*****************************************************************************
// The following are defines for the bit fields in the CTOMIPCSTS register.
//*****************************************************************************
#define IPC_CTOMIPCSTS_IPC1     0x00000001  // C28 to M3 IPC 1 Status Flag
#define IPC_CTOMIPCSTS_IPC2     0x00000002  // C28 to M3 IPC 2 Status Flag
#define IPC_CTOMIPCSTS_IPC3     0x00000004  // C28 to M3 IPC 3 Status Flag
#define IPC_CTOMIPCSTS_IPC4     0x00000008  // C28 to M3 IPC 4 Status Flag
#define IPC_CTOMIPCSTS_IPC5     0x00000010  // C28 to M3 IPC 5 Status Flag
#define IPC_CTOMIPCSTS_IPC6     0x00000020  // C28 to M3 IPC 6 Status Flag
#define IPC_CTOMIPCSTS_IPC7     0x00000040  // C28 to M3 IPC 7 Status Flag
#define IPC_CTOMIPCSTS_IPC8     0x00000080  // C28 to M3 IPC 8 Status Flag
#define IPC_CTOMIPCSTS_IPC9     0x00000100  // C28 to M3 IPC 9 Status Flag
#define IPC_CTOMIPCSTS_IPC10    0x00000200  // C28 to M3 IPC 10 Status Flag
#define IPC_CTOMIPCSTS_IPC11    0x00000400  // C28 to M3 IPC 11 Status Flag
#define IPC_CTOMIPCSTS_IPC12    0x00000800  // C28 to M3 IPC 12 Status Flag
#define IPC_CTOMIPCSTS_IPC13    0x00001000  // C28 to M3 IPC 13 Status Flag
#define IPC_CTOMIPCSTS_IPC14    0x00002000  // C28 to M3 IPC 14 Status Flag
#define IPC_CTOMIPCSTS_IPC15    0x00004000  // C28 to M3 IPC 15 Status Flag
#define IPC_CTOMIPCSTS_IPC16    0x00008000  // C28 to M3 IPC 16 Status Flag
#define IPC_CTOMIPCSTS_IPC17    0x00010000  // C28 to M3 IPC 17 Status Flag
#define IPC_CTOMIPCSTS_IPC18    0x00020000  // C28 to M3 IPC 18 Status Flag
#define IPC_CTOMIPCSTS_IPC19    0x00040000  // C28 to M3 IPC 19 Status Flag
#define IPC_CTOMIPCSTS_IPC20    0x00080000  // C28 to M3 IPC 20 Status Flag
#define IPC_CTOMIPCSTS_IPC21    0x00100000  // C28 to M3 IPC 21 Status Flag
#define IPC_CTOMIPCSTS_IPC22    0x00200000  // C28 to M3 IPC 22 Status Flag
#define IPC_CTOMIPCSTS_IPC23    0x00400000  // C28 to M3 IPC 23 Status Flag
#define IPC_CTOMIPCSTS_IPC24    0x00800000  // C28 to M3 IPC 24 Status Flag
#define IPC_CTOMIPCSTS_IPC25    0x01000000  // C28 to M3 IPC 25 Status Flag
#define IPC_CTOMIPCSTS_IPC26    0x02000000  // C28 to M3 IPC 26 Status Flag
#define IPC_CTOMIPCSTS_IPC27    0x04000000  // C28 to M3 IPC 27 Status Flag
#define IPC_CTOMIPCSTS_IPC28    0x08000000  // C28 to M3 IPC 28 Status Flag
#define IPC_CTOMIPCSTS_IPC29    0x10000000  // C28 to M3 IPC 29 Status Flag
#define IPC_CTOMIPCSTS_IPC30    0x20000000  // C28 to M3 IPC 30 Status Flag
#define IPC_CTOMIPCSTS_IPC31    0x40000000  // C28 to M3 IPC 31 Status Flag
#define IPC_CTOMIPCSTS_IPC32    0x80000000  // C28 to M3 IPC 32 Status Flag

//*****************************************************************************
// The following are defines for the bit fields in the MTOCIPCSET register.
//*****************************************************************************
#define IPC_MTOCIPCSET_IPC1     0x00000001  // M3 to C28 IPC 1 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC2     0x00000002  // M3 to C28 IPC 2 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC3     0x00000004  // M3 to C28 IPC 3 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC4     0x00000008  // M3 to C28 IPC 4 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC5     0x00000010  // M3 to C28 IPC 5 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC6     0x00000020  // M3 to C28 IPC 6 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC7     0x00000040  // M3 to C28 IPC 7 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC8     0x00000080  // M3 to C28 IPC 8 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC9     0x00000100  // M3 to C28 IPC 9 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC10    0x00000200  // M3 to C28 IPC 10 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC11    0x00000400  // M3 to C28 IPC 11 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC12    0x00000800  // M3 to C28 IPC 12 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC13    0x00001000  // M3 to C28 IPC 13 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC14    0x00002000  // M3 to C28 IPC 14 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC15    0x00004000  // M3 to C28 IPC 15 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC16    0x00008000  // M3 to C28 IPC 16 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC17    0x00010000  // M3 to C28 IPC 17 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC18    0x00020000  // M3 to C28 IPC 18 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC19    0x00040000  // M3 to C28 IPC 19 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC20    0x00080000  // M3 to C28 IPC 20 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC21    0x00100000  // M3 to C28 IPC 21 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC22    0x00200000  // M3 to C28 IPC 22 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC23    0x00400000  // M3 to C28 IPC 23 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC24    0x00800000  // M3 to C28 IPC 24 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC25    0x01000000  // M3 to C28 IPC 25 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC26    0x02000000  // M3 to C28 IPC 26 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC27    0x04000000  // M3 to C28 IPC 27 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC28    0x08000000  // M3 to C28 IPC 28 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC29    0x10000000  // M3 to C28 IPC 29 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC30    0x20000000  // M3 to C28 IPC 30 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC31    0x40000000  // M3 to C28 IPC 31 Interrupt
                                            // Request Set
#define IPC_MTOCIPCSET_IPC32    0x80000000  // M3 to C28 IPC 32 Interrupt
                                            // Request Set

//*****************************************************************************
// The following are defines for the bit fields in the MTOCIPCCLR register.
//*****************************************************************************
#define IPC_MTOCIPCCLR_IPC1     0x00000001  // M3 to C28 IPC 1 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC2     0x00000002  // M3 to C28 IPC 2 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC3     0x00000004  // M3 to C28 IPC 3 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC4     0x00000008  // M3 to C28 IPC 4 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC5     0x00000010  // M3 to C28 IPC 5 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC6     0x00000020  // M3 to C28 IPC 6 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC7     0x00000040  // M3 to C28 IPC 7 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC8     0x00000080  // M3 to C28 IPC 8 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC9     0x00000100  // M3 to C28 IPC 9 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC10    0x00000200  // M3 to C28 IPC 10 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC11    0x00000400  // M3 to C28 IPC 11 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC12    0x00000800  // M3 to C28 IPC 12 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC13    0x00001000  // M3 to C28 IPC 13 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC14    0x00002000  // M3 to C28 IPC 14 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC15    0x00004000  // M3 to C28 IPC 15 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC16    0x00008000  // M3 to C28 IPC 16 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC17    0x00010000  // M3 to C28 IPC 17 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC18    0x00020000  // M3 to C28 IPC 18 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC19    0x00040000  // M3 to C28 IPC 19 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC20    0x00080000  // M3 to C28 IPC 20 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC21    0x00100000  // M3 to C28 IPC 21 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC22    0x00200000  // M3 to C28 IPC 22 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC23    0x00400000  // M3 to C28 IPC 23 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC24    0x00800000  // M3 to C28 IPC 24 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC25    0x01000000  // M3 to C28 IPC 25 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC26    0x02000000  // M3 to C28 IPC 26 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC27    0x04000000  // M3 to C28 IPC 27 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC28    0x08000000  // M3 to C28 IPC 28 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC29    0x10000000  // M3 to C28 IPC 29 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC30    0x20000000  // M3 to C28 IPC 30 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC31    0x40000000  // M3 to C28 IPC 31 Interrupt
                                            // Request Clear
#define IPC_MTOCIPCCLR_IPC32    0x80000000  // M3 to C28 IPC 32 Interrupt
                                            // Request Clear

//*****************************************************************************
// The following are defines for the bit fields in the MTOCIPCFLG register.
//*****************************************************************************
#define IPC_MTOCIPCFLG_IPC1     0x00000001  // M3 to C28 IPC 1 Interrupt Status
                                            // Flag
#define IPC_MTOCIPCFLG_IPC2     0x00000002  // M3 to C28 IPC 2 Interrupt Status
                                            // Flag
#define IPC_MTOCIPCFLG_IPC3     0x00000004  // M3 to C28 IPC 3 Interrupt Status
                                            // Flag
#define IPC_MTOCIPCFLG_IPC4     0x00000008  // M3 to C28 IPC 4 Interrupt Status
                                            // Flag
#define IPC_MTOCIPCFLG_IPC5     0x00000010  // M3 to C28 IPC 5 Interrupt Status
                                            // Flag
#define IPC_MTOCIPCFLG_IPC6     0x00000020  // M3 to C28 IPC 6 Interrupt Status
                                            // Flag
#define IPC_MTOCIPCFLG_IPC7     0x00000040  // M3 to C28 IPC 7 Interrupt Status
                                            // Flag
#define IPC_MTOCIPCFLG_IPC8     0x00000080  // M3 to C28 IPC 8 Interrupt Status
                                            // Flag
#define IPC_MTOCIPCFLG_IPC9     0x00000100  // M3 to C28 IPC 9 Interrupt Status
                                            // Flag
#define IPC_MTOCIPCFLG_IPC10    0x00000200  // M3 to C28 IPC 10 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC11    0x00000400  // M3 to C28 IPC 11 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC12    0x00000800  // M3 to C28 IPC 12 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC13    0x00001000  // M3 to C28 IPC 13 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC14    0x00002000  // M3 to C28 IPC 14 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC15    0x00004000  // M3 to C28 IPC 15 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC16    0x00008000  // M3 to C28 IPC 16 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC17    0x00010000  // M3 to C28 IPC 17 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC18    0x00020000  // M3 to C28 IPC 18 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC19    0x00040000  // M3 to C28 IPC 19 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC20    0x00080000  // M3 to C28 IPC 20 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC21    0x00100000  // M3 to C28 IPC 21 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC22    0x00200000  // M3 to C28 IPC 22 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC23    0x00400000  // M3 to C28 IPC 23 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC24    0x00800000  // M3 to C28 IPC 24 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC25    0x01000000  // M3 to C28 IPC 25 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC26    0x02000000  // M3 to C28 IPC 26 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC27    0x04000000  // M3 to C28 IPC 27 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC28    0x08000000  // M3 to C28 IPC 28 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC29    0x10000000  // M3 to C28 IPC 29 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC30    0x20000000  // M3 to C28 IPC 30 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC31    0x40000000  // M3 to C28 IPC 31 Interrupt
                                            // Status Flag
#define IPC_MTOCIPCFLG_IPC32    0x80000000  // M3 to C28 IPC 32 Interrupt
                                            // Status Flag

//*****************************************************************************
// The following are defines for the bit fields in the MPUMPREQUEST register.
//*****************************************************************************
#define IPC_MPUMPREQUEST_SEM_S    0
#define IPC_MPUMPREQUEST_SEM_M    0x00000003 // Pump Request Semaphore from M3
#define IPC_MPUMPREQUEST_KEY_S    4
#define IPC_MPUMPREQUEST_KEY_M    0xFFFFFFF0 // Writing Value Allows Writes

//*****************************************************************************
// The following are defines for the bit fields in the MCLKREQUEST register.
//*****************************************************************************
#define IPC_MCLKREQUEST_SEM_S    0
#define IPC_MCLKREQUEST_SEM_M    0x00000003 // Clock Control Access Request
                                            // from M3
#define IPC_MCLKREQUEST_KEY_S    4
#define IPC_MCLKREQUEST_KEY_M    0xFFFFFFF0 // Writing Value Allows Writes


#endif // __HW_IPC_H__








