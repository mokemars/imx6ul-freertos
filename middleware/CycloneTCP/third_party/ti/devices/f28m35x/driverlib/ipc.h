//###########################################################################
// FILE:   ipc.h
// TITLE:  Defines and Macros for the IPC controller.
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

#ifndef __IPC_H__
#define __IPC_H__

//*****************************************************************************
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
// The following are values that are used to define the maximum size of the
// global circular buffer variables g_asIPCCtoMBuffers and g_asIPCMtoCBuffers.
// They are also used in the IpcPut() and IpcGet() functions.
// IPC_BUFFER_SIZE and NUM_IPC_INTERRUPTS are user-configurable.
//*****************************************************************************
#define IPC_BUFFER_SIZE                           4 // # of tIpcMessage
                                                    // messages in circular
                                                    // buffer (must be interval
                                                    // of 2)
#define NUM_IPC_INTERRUPTS                        4 // # of IPC interrupts
                                                    // using circular buffer
                                                    // (must be same number on
                                                    // both C28 and M3)
#define MAX_BUFFER_INDEX                          IPC_BUFFER_SIZE - 1

//*****************************************************************************
// The following is used by IPCMInitialize() to determine the starting address
// of the Put and Get Buffers, and also the IPCMtoCSharedRamConvert() function
// to
// calculate the M3 to C28 conversion address for MSGRAM vs Sx memory.
//*****************************************************************************
#define M3_CTOMMSGRAM_START                       0x2007F000
#define M3_MTOCMSGRAM_START                       0x2007F800

//*****************************************************************************
// The following are values that can be passed to IPCMInitialize()
// as the usMIpcInterrupt and usCIpcInterrupt parameters
//*****************************************************************************
#define IPC_INT1                                  0x0001
#define IPC_INT2                                  0x0002
#define IPC_INT3                                  0x0003
#define IPC_INT4                                  0x0004

//*****************************************************************************
// The following are values that can be passed to IPCMtoCTaskBusy() as the
// ulFlags parameter.
// IPC_FLAG17 - IPC_FLAG32 can also be passed to IPCMtoCDataRead() and
// IPCMtoCReadBlock() as the ulResponseFlag parameter.
//*****************************************************************************
#define NO_FLAG                                   0x00000000 // NO FLAG
#define IPC_FLAG1                                 0x00000001 // IPC FLAG 1
#define IPC_FLAG2                                 0x00000002 // IPC FLAG 2
#define IPC_FLAG3                                 0x00000004 // IPC FLAG 3
#define IPC_FLAG4                                 0x00000008 // IPC FLAG 4
#define IPC_FLAG5                                 0x00000010 // IPC FLAG 5
#define IPC_FLAG6                                 0x00000020 // IPC FLAG 6
#define IPC_FLAG7                                 0x00000040 // IPC FLAG 7
#define IPC_FLAG8                                 0x00000080 // IPC FLAG 8
#define IPC_FLAG9                                 0x00000100 // IPC FLAG 9
#define IPC_FLAG10                                0x00000200 // IPC FLAG 10
#define IPC_FLAG11                                0x00000400 // IPC FLAG 11
#define IPC_FLAG12                                0x00000800 // IPC FLAG 12
#define IPC_FLAG13                                0x00001000 // IPC FLAG 13
#define IPC_FLAG14                                0x00002000 // IPC FLAG 14
#define IPC_FLAG15                                0x00004000 // IPC FLAG 15
#define IPC_FLAG16                                0x00008000 // IPC FLAG 16
#define IPC_FLAG17                                0x00010000 // IPC FLAG 17
#define IPC_FLAG18                                0x00020000 // IPC FLAG 18
#define IPC_FLAG19                                0x00040000 // IPC FLAG 19
#define IPC_FLAG20                                0x00080000 // IPC FLAG 20
#define IPC_FLAG21                                0x00100000 // IPC FLAG 21
#define IPC_FLAG22                                0x00200000 // IPC FLAG 22
#define IPC_FLAG23                                0x00400000 // IPC FLAG 23
#define IPC_FLAG24                                0x00800000 // IPC FLAG 24
#define IPC_FLAG25                                0x01000000 // IPC FLAG 25
#define IPC_FLAG26                                0x02000000 // IPC FLAG 26
#define IPC_FLAG27                                0x04000000 // IPC FLAG 27
#define IPC_FLAG28                                0x08000000 // IPC FLAG 28
#define IPC_FLAG29                                0x10000000 // IPC FLAG 29
#define IPC_FLAG30                                0x20000000 // IPC FLAG 30
#define IPC_FLAG31                                0x40000000 // IPC FLAG 31
#define IPC_FLAG32                                0x80000000 // IPC FLAG 32

//*****************************************************************************
// The following are values that are returned from all of the IPCMtoC command
// functions to determine whether the command was successfully sent or not.
//*****************************************************************************
#define STATUS_FAIL                               0x0001
#define STATUS_PASS                               0x0000

//*****************************************************************************
// The following are values that can be passed to IPCMtoCDataRead(),
// IPCMtoCSetBits(),IPCMtoCSetBits_Protected(), IPCMtoCClearBits(),
// IPCMtoCClearBits_Protected(), IPCMtoCDataWrite(),
// IPCMtoCDataWrite_Protected()
// as usLength parameter to determine whether command applies to 16- or 32-bit
// data word.
//*****************************************************************************
#define IPC_LENGTH_16_BITS                        0x00000001
#define IPC_LENGTH_32_BITS                        0x00000002

//*****************************************************************************
// The following are values that can be passed to all IPCMtoC command functions
// as bBlock parameter to determine whether to wait/block until a slot in
// PutBuffer is available if it is full, or to exit with a failure status.
//*****************************************************************************
#define ENABLE_BLOCKING                           0x0001
#define DISABLE_BLOCKING                          0x0000

//*****************************************************************************
// The following are values that are used by all command functions and passed
// between processors in tIpcMessage.ulmessage or in the xTOyIPCCOM register
// to determine what command is requested by the sending processor.
//*****************************************************************************
#define IPC_SET_BITS_16                           0x00000001 // Used for
                                                             // IPC-Lite
#define IPC_SET_BITS_32                           0x00000002 // Used for
                                                             // IPC-Lite
#define IPC_CLEAR_BITS_16                         0x00000003 // Used for
                                                             // IPC-Lite
#define IPC_CLEAR_BITS_32                         0x00000004 // Used for
                                                             // IPC-Lite
#define IPC_DATA_WRITE_16                         0x00000005 // Used for
                                                             // IPC-Lite
#define IPC_DATA_WRITE_32                         0x00000006 // Used for
                                                             // IPC-Lite
#define IPC_DATA_READ_16                          0x00000007 // Used for C28
                                                             // Boot ROM
#define IPC_DATA_READ_32                          0x00000008 // Used for C28
                                                             // Boot ROM
#define IPC_DATA_READ                             0x00000008
#define IPC_SET_BITS_16_PROTECTED                 0x00000009 // Used for
                                                             // IPC-Lite
#define IPC_SET_BITS_32_PROTECTED                 0x0000000A // Used for
                                                             // IPC-Lite
#define IPC_CLEAR_BITS_16_PROTECTED               0x0000000B // Used for
                                                             // IPC-Lite
#define IPC_CLEAR_BITS_32_PROTECTED               0x0000000C // Used for
                                                             // IPC-Lite
#define IPC_DATA_WRITE_16_PROTECTED               0x0000000D // Used for
                                                             // IPC-Lite
#define IPC_DATA_WRITE_32_PROTECTED               0x0000000E // Used for
                                                             // IPC-Lite
// 0x0000000F and 0x0000010 are reserved by C28x boot ROM
#define IPC_BRANCH                                0x00000011
#define IPC_FUNC_CALL                             0x00000012
#define IPC_MTOC_EXECUTE_BOOTMODE_CMD             0x00000013

#define IPC_SET_BITS                              0x00010001
#define IPC_CLEAR_BITS                            0x00010002
#define IPC_DATA_WRITE                            0x00010003
#define IPC_BLOCK_READ                            0x00010004
#define IPC_BLOCK_WRITE                           0x00010005
#define IPC_DATA_READ_PROTECTED                   0x00010007
#define IPC_SET_BITS_PROTECTED                    0x00010008
#define IPC_CLEAR_BITS_PROTECTED                  0x00010009
#define IPC_DATA_WRITE_PROTECTED                  0x0001000A
#define IPC_BLOCK_WRITE_PROTECTED                 0x0001000B
#define IPC_MEM_ACCESS_REQ                        0x0001000C


//*****************************************************************************
// The following are values that can be passed into the
// IPCMtoCBootControlSystem() function in the ulBootMode parameter.
//*****************************************************************************
#define CBROM_MTOC_BOOTMODE_COMMAND_ILLEGAL       0x00000000
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_RAM         0x00000001
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_FLASH       0x00000002
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_SCI         0x00000003
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_SPI         0x00000004
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_I2C         0x00000005
#define CBROM_MTOC_BOOTMODE_BOOT_FROM_PARALLEL    0x00000006

#define CBROM_MTOC_BOOTMODE_BOOT_FROM_LOOP        0x00000007
//*****************************************************************************
// The following value is used by the
// IPCMtoCBootControlSystem() function to limit the allowed boot mode values.
//*****************************************************************************
#define CBROM_MTOC_BOOTMODE_BOOT_COMMAND_MAX_SUPPORT_VALUE       0x00000008


//*****************************************************************************
// The following values report on the IPC command status for IPC commands sent
// to the C28 boot ROM. These values will reside in MTOCIPCDATAW[15:0]
// when C28 boot ROM sends an IPC CtoM INT1 to the M3 master system with a NAK
// status (CBROM_BOOTSTS_CTOM_BOOT_CMD_NAK_STATUS_NOT_SUPPORTED or
// CBROM_BOOTSTS_CTOM_BOOT_CMD_NAK_STATUS_BUSY_WITH_BOOT) in
// CTOMIPCBOOTSTS[15:0].
//*****************************************************************************
#define CBROM_NAK_STATUS_INVALID_VALUE                           0x00000000 \
//invalid value or value when not set

#define CBROM_NAK_STATUS_CMD_NOT_SUPPORTED                       0x00000001 \
//NAK STATUS command not supported

#define CBROM_NAK_STATUS_CMD_NOT_SET_PROPERLY                    0x00000002 \
//NAK Status command not set properly , for ex: IPC1 set but IPC32 not set

#define CBROM_NAK_STATUS_ALREADY_BUSY_WITH_YOUR_CMD              0x00000003 \
//NAK status when Cortex M3 is trying to send a second command before first 
//one could complete

#define CBROM_NAK_STATUS_CMD_RESULTED_IN_ERROR                   0x00000004 \
//NAK status command execution resulted in an error

#define CBROM_NAK_STATUS_CMD_CANNOT_BE_EXECUTED_NOW              0x00000005 \
//NAK status - command cannot be executed now in the current state of bootROM

#define CBROM_NAK_STATUS_MAX_SUPPORT_VALUE                       0x00000006 \
//one less than this value are valid values

//*****************************************************************************
// The following values report on the health status of the C28 boot ROM and
// will reside in MTOCIPCDATAW[31:16] and CTOMBOOTSTS[31:16]
//*****************************************************************************
#define C_BOOTROM_START_BOOT                                     0x00010000 \
//set during the initialization phase of A-bootROM

#define C_BOOTROM_DETECT_ANALOG_INTOSC1_MCLK_STS_ON              0x00020000 \
//set if C-BootROM detects that Analog system has INTOSC1 missing

#define C_BOOTROM_DETECT_ANALOG_SYSTEM_IN_ITRAP                  0x00040000 \
//set if C-BootROM detects that Analog system is in iTRAP

#define C_BOOTROM_DETECT_ANALOG_SYSTEM_ADCCLK_MISSING            0x00080000 \
//set if C-BootROM detects that analog system has ADCCLK missing

#define C_BOOTROM_IN_PIE_VECTOR_MISMATCH                         0x00100000 \
//set if C-BootROM detects PIE VECTOR MISMATCH error

#define C_BOOTROM_GOT_AN_ITRAP                                   0x00200000 \
//set if C-BootROM detects an iTRAP

#define C_BOOTROM_GOT_AN_AERR_NMI                                0x00400000 \
//set if C-BootROM detects an AERR

#define C_BOOTROM_GOT_C28XBISTERR_NMI                            0x00800000 \
//set if C-BootROM detects a C28xBISTERR

#define C_BOOTROM_GOT_M3BISTERR_NMI                              0x01000000 \
//set if C-BootROM detects M3 BIST ERR

#define C_BOOTROM_GOT_FLUNCERR_NMI                               0x02000000 \
//set if C-BootROM detects FLASH UNCORRECTABLE ERR

#define C_BOOTROM_GOT_RAMUNCERR_NMI                              0x04000000 \
//set if C-BootROM detectes RAM UNCORRECTABLE ERR

#define C_BOOTROM_GOT_MISSING_CLK_NMI                            0x08000000 \
//set if C-BootROM detects MISSING CLOCK NMI

#define C_BOOTROM_IN_MTOC_BRANCH                                 0x20000000 \
//set if C-BootROM is doing a mtoc branch

#define C_BOOTROM_CTOM_BOOTSTS_COMPLETE                          0x40000000 \
//set if C-BootROM is starting an application after boot


//*****************************************************************************
// The following values report on the C28 boot ROM status at all times while
// the C28 is booting, and wil reside in CTOMBOOTSTS[15:0].
//*****************************************************************************
#define CBROM_BOOTSTS_CTOM_IGNORE                                0x00000000 \
//invalid status - tells master system that the control system has not received
// a valid command.

#define CBROM_BOOTSTS_CTOM_CONTROL_SYSTEM_START_BOOT             0x00000001 \
//tells master system that control system has started to boot, but not
// completed the boot process yet

#define CBROM_BOOTSTS_CTOM_CONTROL_SYSTEM_READY                  0x00000002 \
//tells master system that control system has completed the boot and is running

#define CBROM_BOOTSTS_CTOM_BOOT_CMD_ACK                          0x00000003 \
//tells master system that C-BootROM ACKs the command in MTOCBOOTMODE register

#define CBROM_BOOTSTS_CTOM_BOOT_CMD_NAK_STATUS_NOT_SUPPORTED     0x00000004 \
//tells master system that C-BootROM doesn't support the command in
// MTOCBOOTMODE register

#define CBROM_BOOTSTS_CTOM_BOOT_CMD_NAK_STATUS_BUSY_WITH_BOOT    0x00000005 \
//tells master system that C-BootROM NAKs the current boot command in
// MTOCBOOTMODE register

#define CBROM_BOOTSTS_CTOM_MAX_SUPPORT_VALUE

//*****************************************************************************
// The following defines the Flash & Clocking Semaphore key values.
//*****************************************************************************

#define IPC_MPUMPREQUEST_KEY      0xA5937EC0 // M Flash Pump Semaphore Key Value
#define IPC_MCLKREQUEST_KEY       0xB95C8130 // M Clock Semaphore Key Value

//*****************************************************************************
//! A structure that defines an IPC message.  These fields are used by the
//! IPC drivers to determine handling of data passed between processors.
//! Although they have a defined naming scheme, they can also be used
// generically
//! to pass 32-bit data words between processors.
//*****************************************************************************
typedef struct
{
        //! The command passed between processor systems.
        unsigned long ulcommand;

        //! The receiving processor address the command is requesting action on.
        unsigned long uladdress;

        //! A 32-bit variable, the usage of which is determined by ulcommand.
    //! The most common usage is to pass length requirements
    //! with the upper 16-bits storing a Response Flag for read commands.
        unsigned long uldataw1;

        //! A 32-bit variable, the usage of which is determined by ulcommand.
    //! For block transfers, this variable is generally the address in
    //! shared memory used to pass data between processors.
        unsigned long uldataw2;
} tIpcMessage;

//*****************************************************************************
//! A structure that defines an IPC control instance.  These
//! fields are used by the IPC drivers, and normally it is not necessary for
//! user software to directly read or write fields in the table.
//*****************************************************************************
typedef struct
{
        //! The address of the PutBuffer (in MSGRAM)
        tIpcMessage *psPutBuffer;

        //! The IPC INT flag to set when sending messages
    //! for this IPC controller instance.
        unsigned long ulPutFlag;

        //! The address of the PutBuffer (in MSGRAM)
        unsigned short *pusPutWriteIndex;

        //! The address of the GetBuffer (in MSGRAM)
        unsigned short *pusPutReadIndex;


        //! The address of the GetBuffer (in MSGRAM)
        tIpcMessage *psGetBuffer;

        //! The address of the GetBuffer Write Index (in MSGRAM)
        unsigned short *pusGetWriteIndex;

        //! The address of the GetBuffer Read Index (in MSGRAM)
        unsigned short *pusGetReadIndex;

} tIpcController;

//*****************************************************************************
// A type definition for the IPC function call command.
//*****************************************************************************
typedef unsigned long (*tfIpcFuncCall)(unsigned long ulParam);

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************

// IPC Driver Prototye Definitions
extern void IPCMInitialize (volatile tIpcController *psController,
                            unsigned short usMIpcInterrupt,
                            unsigned short usCIpcInterrupt);
extern unsigned short IpcPut (volatile tIpcController *psController,
                              tIpcMessage *psMessage,
                              unsigned short bBlock);
extern unsigned short IpcGet (volatile tIpcController *psController,
                              tIpcMessage *psMessage,
                              unsigned short bBlock);
extern unsigned short IPCMtoCDataRead (volatile tIpcController *psController,
                                       unsigned long ulAddress, void *pvData,
                                       unsigned short usLength,
                                       unsigned short bBlock,
                                       unsigned long ulResponseFlag);
extern unsigned short IPCMtoCDataRead_Protected (
    volatile tIpcController *psController, unsigned long ulAddress,
    void *pvData,
    unsigned short usLength, unsigned short bBlock,
    unsigned long ulResponseFlag);
extern unsigned short IPCMtoCSetBits(volatile tIpcController *psController,
                                     unsigned long ulAddress,
                                     unsigned long ulMask,
                                     unsigned short usLength,
                                     unsigned short bBlock);
extern unsigned short IPCMtoCSetBits_Protected(
    volatile tIpcController *psController, unsigned long ulAddress,
    unsigned long ulMask, unsigned short usLength, unsigned short bBlock);
extern unsigned short IPCMtoCClearBits(volatile tIpcController *psController,
                                       unsigned long ulAddress,
                                       unsigned long ulMask,
                                       unsigned short usLength,
                                       unsigned short bBlock);
extern unsigned short IPCMtoCClearBits_Protected(
    volatile tIpcController *psController, unsigned long ulAddress,
    unsigned long ulMask, unsigned short usLength, unsigned short bBlock);
extern unsigned short IPCMtoCDataWrite(volatile tIpcController *psController,
                                       unsigned long ulAddress,
                                       unsigned long ulData,
                                       unsigned short usLength,
                                       unsigned short bBlock,
                                       unsigned long ulResponseFlag);
extern unsigned short IPCMtoCDataWrite_Protected(
    volatile tIpcController *psController, unsigned long ulAddress,
    unsigned long ulData, unsigned short usLength, unsigned short bBlock,
    unsigned long ulResponseFlag);
extern unsigned short IPCMtoCBlockRead(volatile tIpcController *psController,
                                       unsigned long ulAddress,
                                       unsigned long ulShareAddress,
                                       unsigned short usLength,
                                       unsigned short bBlock,
                                       unsigned long ulResponseFlag);
extern unsigned short IPCMtoCBlockWrite(volatile tIpcController *psController,
                                        unsigned long ulAddress,
                                        unsigned long ulShareAddress,
                                        unsigned short usLength,
                                        unsigned short usWordLength,
                                        unsigned short bBlock);
extern unsigned short IPCMtoCBlockWrite_Protected(
    volatile tIpcController *psController, unsigned long ulAddress,
    unsigned long ulShareAddress, unsigned short usLength,
    unsigned short usWordLength, unsigned short bBlock);
extern unsigned short IPCMtoCFunctionCall(volatile tIpcController *psController,
                                          unsigned long ulAddress,
                                          unsigned long ulParam,
                                          unsigned short bBlock);
extern void IPCCtoMDataWrite(tIpcMessage *psMessage);
extern void IPCCtoMDataWrite_Protected(tIpcMessage *psMessage);
extern void IPCCtoMDataRead(volatile tIpcController *psController,
                            tIpcMessage *psMessage,
                            unsigned short bBlock);
extern void IPCCtoMDataRead_Protected(volatile tIpcController *psController,
                                      tIpcMessage *psMessage,
                                      unsigned short bBlock);
extern void IPCCtoMSetBits (tIpcMessage *psMessage);
extern void IPCCtoMSetBits_Protected (tIpcMessage *psMessage);
extern void IPCCtoMClearBits (tIpcMessage *psMessage);
extern void IPCCtoMClearBits_Protected (tIpcMessage *psMessage);
extern void IPCCtoMBlockRead (tIpcMessage *psMessage);
extern void IPCCtoMBlockWrite (tIpcMessage *psMessage);
extern void IPCCtoMBlockWrite_Protected (tIpcMessage *psMessage);
extern void IPCCtoMFunctionCall (tIpcMessage *psMessage);
extern void RamMReqSharedMemAccess (unsigned long ulMask,
                                    unsigned short ulMaster);

// IPC Lite Driver Prototype Definitions
extern unsigned short IPCLiteMtoCGetResult (void *pvData,
                                            unsigned short usLength,
                                            unsigned long ulStatusFlag);
extern unsigned short IPCLiteMtoCDataRead(unsigned long ulFlag,
                                          unsigned long ulAddress,
                                          unsigned short usLength,
                                          unsigned long ulStatusFlag);
extern unsigned short IPCLiteMtoCSetBits(unsigned long ulFlag,
                                         unsigned long ulAddress,
                                         unsigned long ulMask,
                                         unsigned short usLength,
                                         unsigned long ulStatusFlag);
extern unsigned short IPCLiteMtoCSetBits_Protected (unsigned long ulFlag,
                                                    unsigned long ulAddress,
                                                    unsigned long ulMask,
                                                    unsigned short usLength,
                                                    unsigned long ulStatusFlag);
extern unsigned short IPCLiteMtoCClearBits (unsigned long ulFlag,
                                            unsigned long ulAddress,
                                            unsigned long ulMask,
                                            unsigned short usLength,
                                            unsigned long ulStatusFlag);
extern unsigned short IPCLiteMtoCClearBits_Protected (
    unsigned long ulFlag, unsigned long ulAddress, unsigned long ulMask,
    unsigned short usLength, unsigned long ulStatusFlag);
extern unsigned short IPCLiteMtoCDataWrite(unsigned long ulFlag,
                                           unsigned long ulAddress,
                                           unsigned long ulData,
                                           unsigned short usLength,
                                           unsigned long ulStatusFlag);
extern unsigned short IPCLiteMtoCDataWrite_Protected(unsigned long ulFlag,
                                                     unsigned long ulAddress,
                                                     unsigned long ulData,
                                                     unsigned short usLength,
                                                     unsigned long ulStatusFlag);
extern unsigned short IPCLiteMtoCFunctionCall(unsigned long ulFlag,
                                              unsigned long ulAddress,
                                              unsigned long ulParam,
                                              unsigned long ulStatusFlag);
extern unsigned short IPCLiteMtoCBootBranch(unsigned long ulAddress);
extern void IPCLiteCtoMDataRead(unsigned long ulFlag,
                                unsigned long ulStatusFlag);
extern void IPCLiteCtoMSetBits(unsigned long ulFlag, unsigned long ulStatusFlag);
extern void IPCLiteCtoMSetBits_Protected(unsigned long ulFlag,
                                         unsigned long ulStatusFlag);
extern void IPCLiteCtoMClearBits(unsigned long ulFlag,
                                 unsigned long ulStatusFlag);
extern void IPCLiteCtoMClearBits_Protected(unsigned long ulFlag,
                                           unsigned long ulStatusFlag);
extern void IPCLiteCtoMDataWrite(unsigned long ulFlag, 
                                 unsigned long ulStatusFlag);
extern void IPCLiteCtoMDataWrite_Protected(unsigned long ulFlag,
                                           unsigned long ulStatusFlag);
extern void IPCLiteCtoMFunctionCall(unsigned long ulFlag,
                                    unsigned long ulStatusFlag);


// IPC Utility Driver Prototype Definitions
extern unsigned short IPCMtoCBootControlSystem(unsigned long ulBootMode);
extern unsigned long IPCMtoCSharedRamConvert (unsigned long ulShareAddress);
extern unsigned short IPCMtoCFlagBusy (unsigned long ulFlags);
extern void IPCMtoCFlagSet (unsigned long ulFlags);
extern void IPCMtoCFlagClear (unsigned long ulFlags);
extern void IPCCtoMFlagAcknowledge (unsigned long ulFlags);
extern unsigned short IPCCtoMFlagBusy (unsigned long ulFlags);
extern void IpcSync(volatile unsigned int flag);
extern void SendIpcCommand(unsigned long int command, 
                           unsigned long int address, 
                           unsigned long int data, 
                           unsigned long int flag);
extern void AckIpcFlag(unsigned long int flag);
extern void WaitForIpcFlag(unsigned long int flag);
extern void WaitForIpcAck(unsigned long int flag);
extern void SendIpcFlag(unsigned long int flag);                  
//*****************************************************************************
// Mark the end of the C bindings section for C++ compilers.
//*****************************************************************************
#ifdef __cplusplus
}
#endif


#endif //  __IPC_H__





