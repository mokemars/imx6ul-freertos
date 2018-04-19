/*******************************************************************************
 * (c) Copyright 2007 - 2010 Actel Corporation.  All rights reserved.
 *
 * SmartFusion Microcontroller Subsystem UART bare metal software driver public API.
 *
 * SVN $Revision: 4178 $
 * SVN $Date: 2012-03-05 14:10:01 +0530 (Mon, 05 Mar 2012) $
 */
/*=========================================================================*//**
  @mainpage SmartFusion MSS UART Bare Metal Driver.

  @section intro_sec Introduction
  The SmartFusion™ microcontroller subsystem (MSS) includes two UART peripherals
  for serial communication. This driver provides a set of functions for
  controlling the MSS UARTs as part of a bare metal system where no operating
  system is available. These drivers can be adapted for use as part of an
  operating system, but the implementation of the adaptation layer between
  this driver and the operating system's driver model is outside the scope
  of this driver.

  @section hw_dependencies Hardware Flow Dependencies
  The configuration of all features of the MSS UARTs is covered by this driver
  with the exception of the SmartFusion IOMUX configuration. SmartFusion allows
  multiple non-concurrent uses of some external pins through IOMUX configuration.
  This feature allows optimization of external pin usage by assigning external
  pins for use by either the microcontroller subsystem or the FPGA fabric.
  The MSS UARTs serial signals are routed through IOMUXes to the SmartFusion
  device external pins. These IOMUXes are configured automatically by the
  MSS configurator tool in the hardware flow correctly when the MSS UARTs are
  enabled in that tool. You must ensure that the MSS UARTs are enabled by the
  MSS configurator tool in the hardware flow; otherwise the serial inputs and
  outputs will not be connected to the chip's external pins. For more information
  on IOMUX, refer to the IOMUX section of the SmartFusion Datasheet.
  The base address, register addresses, and interrupt number assignment for the
  MSS UART blocks are defined as constants in the SmartFusion CMSIS-PAL You must
  ensure that the SmartFusion CMSIS-PAL is either included in the software tool
  chain used to build your project or is included in your project.

  @section theory_op Theory of Operation
  The MSS UART driver functions are logically grouped into three groups:
    - Initialization and configuration functions
    - Polled transmit and receive functions
    - Interrupt driven transmit and receive functions

  Initialization and Configuration
  The MSS UART driver is initialized through a call to the MSS_UART_init()
  function. This function takes the UART’s configuration as parameters.
  The MSS_UART_init() function must be called before any other MSS UART
  driver functions can be called. The first parameter of the MSS_UART_init()
  function is a pointer to one of two global data structures used to store
  state information for each MSS UART. A pointer to these data structures
  is also used as the first parameter to any of the driver functions to
  identify which MSS UART will be used by the called function. The names
  of these two data structures are g_mss_uart0 and g_mss_uart1. Therefore,
  any call to an MSS UART function should be of the form
  MSS_UART_function_name( &g_mss_uart0, ... ) or
  MSS_UART_function_name( &g_mss_uart1, ... ).

  Polled Transmit and Receive Operations
  The driver can be used to transmit and receive data once initialized.
  Polled operations where the driver constantly polls the state of the UART
  registers in order to control data transmit or data receive are performed
  using these functions:
  • MSS_UART_polled_tx()
  • MSS_UART_polled_tx_string
  • MSS_UART_fill_tx_fifo()
  • MSS_UART_get_rx()
  Data is transmitted using the MSS_UART_polled_tx() function. This function
  is blocking, meaning that it will only return once the data passed to the
  function has been sent to the MSS UART hardware transmitter. Data received
  by the MSS UART hardware receiver can be read by the MSS_UART_get_rx() function.
  The MSS_UART_polled_tx_string() function is provided to transmit a NULL (‘\0’)
  terminated string in polled mode. This function is blocking, meaning that it
  will only return once the data passed to the function has been sent to the
  MSS UART hardware transmitter.
  The MSS_UART_fill_tx_fifo() function fills the MSS UART hardware transmit FIFO
  with data from a buffer passed as a parameter and returns the number of bytes
  transferred to the FIFO. If the transmit FIFO is not empty when the
  MSS_UART_fill_tx_fifo() function is called it returns immediately without
  transferring any data to the FIFO.

  Interrupt Driven Operations
  The driver can also transmit or receive data under interrupt control, freeing
  your application to perform other tasks until an interrupt occurs indicating
  that the driver’s attention is required. Interrupt controlled UART operations
  are performed using these functions:
  • MSS_UART_irq_tx()
  • MSS_UART_tx_complete()
  • MSS_UART_set_tx_handler()
  • MSS_UART_set_rx_handler()
  • MSS_UART_set_rxstatus_handler()
  • MSS_UART_set_modemstatus_handler()
  • MSS_UART_enable_irq()
  • MSS_UART_disable_irq()

  Interrupt Handlers
  The MSS UART driver supports all types of interrupt triggered by the MSS UART.
  The driver’s internal top level interrupt handler identifies the source of the
  MSS UART interrupt and calls the corresponding lower level handler function
  that you previously registered with the driver through calls to the
  MSS_UART_set_rx_handler(), MSS_UART_set_tx_handler(),
  MSS_UART_set_rxstatus_handler(), and MSS_UART_set_modemstatus_handler() functions.
  You are responsible for creating these lower level interrupt handlers as part of
  your application program and registering them with the driver.
  Note:     The SmartFusion CMSIS-PAL defines the UART0_IRQHandler() and
  UART1_IRQHandler() functions (with weak linkage) and assigns them as the
  interrupt service routines (ISR) for the MSS UART interrupt inputs to the
  Cortex-M1 NVIC. The MSS UART driver provides the implementation functions for
  both of these ISRs from which it calls its own internal top level, interrupt
  handler function. The ISRs also clear the MSS UART interrupt in the NVIC before
  returning to prevent the same interrupt event retriggering a call to the ISR.
  The MSS_UART_enable_irq() and MSS_UART_disable_irq() functions are used to
  enable or disable the received line status, received data available/character
  timeout, transmit holding register empty and modem status interrupts at the
  MSS UART level. The MSS_UART_enable_irq() function also enables the MSS UART
  instance interrupt at the Cortex-M3 level.

  Transmitting Data
  Interrupt-driven transmit is initiated by a call to MSS_UART_irq_tx(),
  specifying the block of data to transmit. Your application is then free to
  perform other tasks and inquire later whether transmit has completed by calling
  the MSS_UART_tx_complete() function. The MSS_UART_irq_tx() function enables
  the UART’s transmit holding register empty (THRE) interrupt and then, when
  the interrupt goes active, the driver’s default THRE interrupt handler transfers
  the data block to the UART until the entire block is transmitted.

  Note:     You can use the MSS_UART_set_tx_handler() function to assign an
  alternative handler to the THRE interrupt. In this case, you must not use
  the MSS_UART_irq_tx() function to initiate the transmit, as this will
  re-assign the driver’s default THRE interrupt handler to the THRE interrupt.
  Instead, your alternative THRE interrupt handler must include a call to
  the MSS_UART_fill_tx_fifo() function to transfer the data to the UART.

  Receiving Data
  Interrupt-driven receive is performed by first calling MSS_UART_set_rx_handler()
  to register a receive handler function that will be called by the driver
  whenever receive data is available. You must provide this receive handler
  function which must include a call to the MSS_UART_get_rx() function to
  actually read the received data.

  UART Status
  The function MSS_UART_get_rx_status() is used to read the receiver error status.
  This function returns the overrun, parity, framing, break, and FIFO error status
  of the receiver.
  The function MSS_UART_get_tx_status() is used to read the transmitter status.
  This function returns the transmit empty (TEMT) and transmit holding register
  empty (THRE) status of the transmitter.
  The function MSS_UART_get_modem_status() is used to read the modem status flags.
  This function returns the current value of the modem status register.

  Loopback
  The MSS_UART_set_loopback() function can be used to locally loopback the Tx and
  Rx lines of a UART. This is not to be confused with the loopback of UART0 to
  UART1, which can be achieved through the microcontroller subsystem’s system
  registers.
 *//*=========================================================================*/
#ifndef __MSS_UART_H_
#define __MSS_UART_H_ 1

#include "../../CMSIS/a2fxxxm3.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
  Baud rates
  The following definitions are used to specify standard baud rates as a
  parameter to the MSS_UART_init() function.
 */
#define MSS_UART_110_BAUD       110
#define MSS_UART_300_BAUD       300
#define MSS_UART_1200_BAUD      1200
#define MSS_UART_2400_BAUD      2400
#define MSS_UART_4800_BAUD      4800
#define MSS_UART_9600_BAUD      9600
#define MSS_UART_19200_BAUD     19200
#define MSS_UART_38400_BAUD     38400
#define MSS_UART_57600_BAUD     57600
#define MSS_UART_115200_BAUD    115200
#define MSS_UART_230400_BAUD    230400
#define MSS_UART_460800_BAUD    460800
#define MSS_UART_921600_BAUD    921600

/***************************************************************************//**
  Data Bits Length
  The following defines are used to build the value of the MSS_UART_init()
  function line_config parameter.
 */
#define MSS_UART_DATA_5_BITS     ( (uint8_t) 0x00 )
#define MSS_UART_DATA_6_BITS     ( (uint8_t) 0x01 )
#define MSS_UART_DATA_7_BITS     ( (uint8_t) 0x02 )
#define MSS_UART_DATA_8_BITS     ( (uint8_t) 0x03 )

/***************************************************************************//**
  Parity
  The following defines are used to build the value of the MSS_UART_init()
  function line_config parameter.
 */
#define MSS_UART_NO_PARITY           ( (uint8_t) 0x00 )
#define MSS_UART_ODD_PARITY          ( (uint8_t) 0x08 )
#define MSS_UART_EVEN_PARITY         ( (uint8_t) 0x18 )
#define MSS_UART_STICK_PARITY_0      ( (uint8_t) 0x38 )
#define MSS_UART_STICK_PARITY_1      ( (uint8_t) 0x28 )

/***************************************************************************//**
  Number of Stop Bits
  The following defines are used to build the value of the MSS_UART_init()
  function line_config parameter.
 */
#define MSS_UART_ONE_STOP_BIT        ( (uint8_t) 0x00 )
#define MSS_UART_ONEHALF_STOP_BIT    ( (uint8_t) 0x04 )
#define MSS_UART_TWO_STOP_BITS       ( (uint8_t) 0x04 )

/***************************************************************************//**
  Receiver Error Status
  The following defines are used to determine the UART receiver error type.
  These bit mask constants are used with the return value of the
  MSS_UART_get_rx_status() function to find out if any errors occurred while
  receiving data.
 */
#define MSS_UART_INVALID_PARAM    ( (uint8_t)0xFF )
#define MSS_UART_NO_ERROR         ( (uint8_t)0x00 )
#define MSS_UART_OVERUN_ERROR     ( (uint8_t)0x02 )
#define MSS_UART_PARITY_ERROR     ( (uint8_t)0x04 )
#define MSS_UART_FRAMING_ERROR    ( (uint8_t)0x08 )
#define MSS_UART_BREAK_ERROR      ( (uint8_t)0x10 )
#define MSS_UART_FIFO_ERROR       ( (uint8_t)0x80 )

/***************************************************************************//**
  Transmitter Status
  The following definitions are used to determine the UART transmitter status.
  These bit mask constants are used with the return value of the
  MSS_UART_get_tx_status() function to find out the status of the transmitter.
 */
#define MSS_UART_TX_BUSY          ( (uint8_t) 0x00 )
#define MSS_UART_THRE             ( (uint8_t) 0x20 )
#define MSS_UART_TEMT             ( (uint8_t) 0x40 )

/***************************************************************************//**
  Modem Status
  The following defines are used to determine the modem status. These bit
  mask constants are used with the return value of the
  MSS_UART_get_modem_status() function to find out the modem status of
  the UART.
 */
#define MSS_UART_DCTS             ( (uint8_t) 0x01 )
#define MSS_UART_DDSR             ( (uint8_t) 0x02 )
#define MSS_UART_TERI             ( (uint8_t) 0x04 )
#define MSS_UART_DDCD             ( (uint8_t) 0x08 )
#define MSS_UART_CTS              ( (uint8_t) 0x10 )
#define MSS_UART_DSR              ( (uint8_t) 0x20 )
#define MSS_UART_RI               ( (uint8_t) 0x40 )
#define MSS_UART_DCD              ( (uint8_t) 0x80 )

/***************************************************************************//**
  MSS UART Interrupts
  The following defines are used to enable and disable MSS UART interrupts.
  They are used to build the value of the irq_mask parameter for the
  MSS_UART_enable_irq() and MSS_UART_disable_irq() functions. A bitwise OR
  of these constants is used to enable or disable multiple interrupts.
 */
#define MSS_UART_RBF_IRQ          ( (uint8_t) 0x01 )
#define MSS_UART_TBE_IRQ          ( (uint8_t) 0x02 )
#define MSS_UART_LS_IRQ           ( (uint8_t) 0x04 )
#define MSS_UART_MS_IRQ           ( (uint8_t) 0x08 )

/***************************************************************************//**
  RX FIFO trigger sizes
  This enumeration specifies the receiver FIFO trigger level. This is the
  number of bytes that must be received before the UART generates a receive
  data available interrupt. It provides the allowed values for the
  MSS_UART_set_rx_handler() function trigger_level parameter.
 */
typedef enum mss_uart_rx_trig_level_t {
    MSS_UART_FIFO_SINGLE_BYTE    = 0x00,
    MSS_UART_FIFO_FOUR_BYTES     = 0x40,
    MSS_UART_FIFO_EIGHT_BYTES    = 0x80,
    MSS_UART_FIFO_FOURTEEN_BYTES = 0xC0,
    MSS_UART_FIFO_INVALID_TRIG_LEVEL
} mss_uart_rx_trig_level_t;

/***************************************************************************//**
  Loopback.
  This enumeration specifies the loopback configuration of the UART. It
  provides the allowed values for the MSS_UART_set_loopback() function’s
  loopback parameter. Use MSS_UART_LOOPBACK_ON to set up the UART to locally
  loopback its Tx and Rx lines.
 */
typedef enum mss_uart_loopback_t {
    MSS_UART_LOOPBACK_OFF   = 0,
    MSS_UART_LOOPBACK_ON    = 1
} mss_uart_loopback_t;


/***************************************************************************//**
  MSS UART instance type.
  This is type definition for MSS UART instance. You need to create and
  maintain a record of this type. This holds all data regarding the MSS UART
  instance
 */
typedef struct  mss_uart_instance mss_uart_instance_t;

/***************************************************************************//**
  Interrupt handler prototype.
  This typedef specifies the function prototype for MSS UART interrupt handlers.
  All interrupt handlers registered with the MSS UART driver must be of this type.
  The interrupt handlers are registered with the driver through the
  MSS_UART_set_rx_handler(), MSS_UART_set_tx_handler(),
  MSS_UART_set_rxstatus_handler(), and MSS_UART_set_modemstatus_handler()
  functions.
  The this_uart parameter is a pointer to either g_mss_uart0 or g_mss_uart1 to
  identify the MSS UART to associate with the handler function.
 */
typedef void (*mss_uart_irq_handler_t)( mss_uart_instance_t * this_uart );

/***************************************************************************//**
  mss_uart_instance.
  There is one instance of this structure for each instance of the
  microcontroller subsystem’s UARTs. Instances of this structure are used to
  identify a specific UART. A pointer to an initialized instance of the
  mss_uart_instance_t structure is passed as the first parameter to
  MSS UART driver functions to identify which UART should perform the
  requested operation.
 */
struct mss_uart_instance{
    /* CMSIS related defines identifying the UART hardware. */
    UART_TypeDef *          hw_reg;     /*!< Pointer to UART registers. */
    UART_BitBand_TypeDef *  hw_reg_bit; /*!< Pointer to UART registers bit band area. */
    IRQn_Type               irqn;       /*!< UART's Cortex-M3 NVIC interrupt number. */
    uint8_t                 status;     /*!< Sticky line status. */

    /* transmit related info (used with interrupt driven transmit): */
    const uint8_t * tx_buffer;          /*!< Pointer to transmit buffer. */
    uint32_t        tx_buff_size;       /*!< Transmit buffer size. */
    uint32_t        tx_idx;             /*!< Index within transmit buffer of next byte to transmit.*/

    /* line status interrupt handler:*/
    mss_uart_irq_handler_t linests_handler;   /*!< Pointer to user registered line status handler. */
    /* receive interrupt handler:*/
    mss_uart_irq_handler_t rx_handler;        /*!< Pointer to user registered receiver handler. */
    /* transmit interrupt handler:*/
    mss_uart_irq_handler_t tx_handler;        /*!< Pointer to user registered transmit handler. */
    /* modem status interrupt handler:*/
    mss_uart_irq_handler_t modemsts_handler;  /*!< Pointer to user registered modem status handler. */
};

/***************************************************************************//**
  This instance of mss_uart_instance_t holds all data related to the operations
  performed by UART0. The function MSS_UART_init() initializes this structure.
  A pointer to g_mss_uart0 is passed as the first parameter to MSS UART driver
  functions to indicate that UART0 should perform the requested operation.
 */
extern mss_uart_instance_t g_mss_uart0;

/***************************************************************************//**
  This instance of mss_uart_instance_t holds all data related to the operations
  performed by UART1. The function MSS_UART_init() initializes this structure.
  A pointer to g_mss_uart1 is passed as the first parameter to MSS UART driver
  functions to indicate that UART1 should perform the requested operation.
 */
extern mss_uart_instance_t g_mss_uart1;

/***************************************************************************//**
  The MSS_UART_init() function initializes and configures one of the SmartFusion 
  MSS UARTs with the configuration passed as a parameter. The configuration 
  parameters are the baud_rate which is used to generate the baud value and 
  the line_config which is used to specify the line configuration(bit length, 
  stop bits and parity).

  Note: 	The calculated baud value is limited to a non-zero 16-bit value. 
  If the baud value exceeds 16-bits, the MSS_UART_init() function will set the 
  baud value to its reset value of 0x0001. This will give a baud rate 
  of (PCLK ÷ 16) and the MSS UART may not perform as expected.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t structure
    identifying the MSS UART hardware block to be initialized. There are two
    such data structures, g_mss_uart0 and g_mss_uart1, associated with MSS
    UART0 and MSS UART1 respectively. This parameter must point to either
    the g_mss_uart0 or g_mss_uart1 global data structure defined within
    the UART driver..

  @param baud_rate
    The baud_rate parameter specifies the baud rate. It can be specified for
    common baud rates’ using the following defines:
    •   MSS_UART_110_BAUD
    •   MSS_UART_300_BAUD
    •   MSS_UART_1200_BAUD
    •   MSS_UART_2400_BAUD
    •   MSS_UART_4800_BAUD
    •   MSS_UART_9600_BAUD
    •   MSS_UART_19200_BAUD
    •   MSS_UART_38400_BAUD
    •   MSS_UART_57600_BAUD
    •   MSS_UART_115200_BAUD
    •   MSS_UART_230400_BAUD
    •   MSS_UART_460800_BAUD
    •   MSS_UART_921600_BAUD
    Alternatively, any nonstandard baud rate can be specified by simply passing
    the actual required baud rate as the value for this parameter.

  @param line_config
    The line_config parameter is the line configuration specifying the bit length,
    number of stop bits and parity settings. This is a bitwise OR of one value
    from each of the following groups of allowed values:
    •   One of the following to specify the transmit/receive data bit length:
        MSS_UART_DATA_5_BITS
        MSS_UART_DATA_6_BITS,
        MSS_UART_DATA_7_BITS
        MSS_UART_DATA_8_BITS
    •   One of the following to specify the parity setting:
        MSS_UART_NO_PARITY
        MSS_UART_EVEN_PARITY
        MSS_UART_ODD_PARITY
        MSS_UART_STICK_PARITY_0
        MSS_UART_STICK_PARITY_1
    •   One of the following to specify the number of stop bits:
        MSS_UART_ONE_STOP_BIT
        MSS_UART_ONEHALF_STOP_BIT
        MSS_UART_TWO_STOP_BITS

  @return
    This function does not return a value.

  Example:
  @code
  #include "mss_uart.h"

  int main(void)
  {
     MSS_UART_init
     (
         &g_mss_uart0,
         MSS_UART_57600_BAUD,
         MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
     );
     return(0);
  }
  @endcode
 */
void
MSS_UART_init
(
    mss_uart_instance_t* this_uart,
    uint32_t baud_rate,
    uint8_t line_config
);

/***************************************************************************//**
  The function MSS_UART_polled_tx() is used to transmit data. It transfers the
  contents of the transmitter data buffer, passed as a function parameter, into
  the UART’s hardware transmitter FIFO. It returns when the full content of the
  transmit data buffer has been transferred to the UART’s transmit FIFO. It is
  safe to release or reuse the memory used as the transmitter data buffer once
  this function returns.

  Note:     This function reads the UART’s line status register (LSR) to poll
  for the active state of the transmitter holding register empty (THRE) bit
  before transferring data from the data buffer to the transmitter FIFO. It
  transfers data to the transmitter FIFO in blocks of 16 bytes or less and
  allows the FIFO to empty before transferring the next block of data.

  Note:     The actual transmission over the serial connection will still be
  in progress when this function returns. Use the MSS_UART_get_tx_status()
  function if you need to know when the transmitter is empty.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @param pbuff
    The pbuff parameter is a pointer to a buffer containing the data to
    be transmitted.

  @param tx_size
    The tx_size parameter specifies the size, in bytes, of the data to
    be transmitted.

  @return
    This function does not return a value.

  Example:
  @code
  #include "mss_uart.h"

  int main(void)
  {
     uint8_t message[12] = "Hello World";

     MSS_UART_init( &g_mss_uart0, MSS_UART_57600_BAUD,
                    MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT );
     MSS_UART_polled_tx( &g_mss_uart0, message, sizeof(message) );
     return(0);
  }
  @endcode
 */
void
MSS_UART_polled_tx
(
    mss_uart_instance_t * this_uart,
    const uint8_t * pbuff,
    uint32_t tx_size
);

/***************************************************************************//**
  The function MSS_UART_polled_tx_string() is used to transmit a NULL ('\0')
  terminated string. It transfers the text string, from the buffer starting at
  the address pointed to by p_sz_string into the UART’s hardware transmitter
  FIFO. It returns when the complete string has been transferred to the UART's
  transmit FIFO. It is safe to release or reuse the memory used as the string
  buffer once this function returns.

  Note:     This function reads the UART’s line status register (LSR) to poll
  for the active state of the transmitter holding register empty (THRE) bit
  before transferring data from the data buffer to the transmitter FIFO. It
  transfers data to the transmitter FIFO in blocks of 16 bytes or less and
  allows the FIFO to empty before transferring the next block of data.

  Note:     The actual transmission over the serial connection will still be
  in progress when this function returns. Use the MSS_UART_get_tx_status()
  function if you need to know when the transmitter is empty.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @param p_sz_string
    The p_sz_string parameter is a pointer to a buffer containing the NULL
    ('\0') terminated string to be transmitted.

  @return
    This function does not return a value.

  Example:
  @code
  #include "mss_uart.h"

  int main(void)
  {
     uint8_t message[12] = "Hello World";

     MSS_UART_init( &g_mss_uart0, MSS_UART_57600_BAUD,
                    MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT );
     MSS_UART_polled_tx_string( &g_mss_uart0, message );
     return(0);
  }
  @endcode

 */
void
MSS_UART_polled_tx_string
(
    mss_uart_instance_t * this_uart,
    const uint8_t * p_sz_string
);


/***************************************************************************//**
  The function MSS_UART_irq_tx() is used to initiate an interrupt-driven
  transmit. It returns immediately after making a note of the transmit buffer
  location and enabling transmit interrupts both at the UART and Cortex-M3 NVIC
  level. This function takes a pointer via the pbuff parameter to a memory
  buffer containing the data to transmit. The memory buffer specified through
  this pointer must remain allocated and contain the data to transmit until
  the transmit completion has been detected through calls to function
  MSS_UART_tx_complete(). The actual transmission over the serial connection
  is still in progress until calls to the MSS_UART_tx_complete() function
  indicate transmit completion.

  Note:     The MSS_UART_irq_tx() function enables both the transmit holding
  register empty (THRE) interrupt in the UART and the MSS UART instance
  interrupt in the Cortex-M3 NVIC as part of its implementation.

  Note:     The MSS_UART_irq_tx() function assigns an internal default transmit
  interrupt handler function to the UART’s THRE interrupt. This interrupt
  handler overrides any custom interrupt handler that you may have previously
  registered using the MSS_UART_set_tx_handler() function.

  Note:     The MSS_UART_irq_tx() function’s default transmit interrupt
  handler disables the UART’s THRE interrupt when all of the data has
  been transferred to the UART's transmit FIFO.


  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @param pbuff
    The pbuff parameter is a pointer to a buffer containing the data
    to be transmitted.

  @param tx_size
    The tx_size parameter specifies the size, in bytes, of the data
    to be transmitted.

  @return
    This function does not return a value.

  Example:
  @code
  #include "mss_uart.h"

  int main(void)
  {
     uint8_t tx_buff[10] = "abcdefghi";
     MSS_UART_init
     (
         &g_mss_uart0,
         MSS_UART_57600_BAUD,
         MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
     );
     MSS_UART_irq_tx( &g_mss_uart0, tx_buff, sizeof(tx_buff));
     while ( 0 == MSS_UART_tx_complete( &g_mss_uart0 ) )
     {
         ;
     }
     return(0);
  }
  @endcode
 */
void
MSS_UART_irq_tx
(
    mss_uart_instance_t * this_uart,
    const uint8_t * pbuff,
    uint32_t tx_size
);

/***************************************************************************//**
  The MSS_UART_tx_complete() function is used to find out if the interrupt-driven
  transmit previously initiated through a call to MSS_UART_irq_tx() is complete.
  This is typically used to find out when it is safe to reuse or release the
  memory buffer holding transmit data.

  Note:     The transfer of all of the data from the memory buffer to the UART’s
  transmit FIFO and the actual transmission over the serial connection are both
  complete when a call to the MSS_UART_tx_complete() function indicates transmit
  completion.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @return
    This function return a non-zero value if transmit has completed, otherwise
    it returns zero.

  Example:
    See the MSS_UART_irq_tx() function for an example that uses the
    MSS_UART_tx_complete() function.
 */
int8_t
MSS_UART_tx_complete
(
   mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_get_rx() function reads the content of the UART receiver’s FIFO
  and stores it in the receive buffer that is passed via the rx_buff function
  parameter. It copies either the full contents of the FIFO into the receive
  buffer, or just enough data from the FIFO to fill the receive buffer,
  dependent upon the size of the receive buffer passed by the buff_size
  parameter. The MSS_UART_get_rx() function returns the number of bytes copied
  into the receive buffer .This function is non-blocking and will return 0
  immediately if no data has been received.

  Note:     The MSS_UART_get_rx() function reads and accumulates the receiver
  status of the MSS UART instance before reading each byte from the receiver's
  data register/FIFO. This allows the driver to maintain a sticky record of any
  receiver errors that occur as the UART receives each data byte; receiver
  errors would otherwise be lost after each read from the receiver's data register.
  A call to the MSS_UART_get_rx_status() function returns any receiver errors
  accumulated during the execution of the MSS_UART_get_rx() function.

  Note:     If you need to read the error status for each byte received, set
  the buff_size to 1 and read the receive line error status for each byte
  using the MSS_UART_get_rx_status() function.
  The MSS_UART_get_rx() function can be used in polled mode, where it is called
  at regular intervals to find out if any data has been received, or in interrupt
  driven-mode, where it is called as part of a receive handler that is called
  by the driver as a result of data being received.

  Note:     In interrupt driven mode you should call the MSS_UART_get_rx()
  function as part of the receive handler function that you register with
  the MSS UART driver through a call to MSS_UART_set_rx_handler().

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @param rx_buff
    The rx_buff parameter is a pointer to a buffer where the received
    data is copied.

  @param buff_size
    The buff_size parameter specifies the size of the receive buffer in bytes.

  @return
    This function returns the number of bytes that were copied into the
    rx_buff buffer. It returns 0 if no data has been received.

  Polled mode example:
  @code
   int main( void )
   {
      uint8_t rx_buff[RX_BUFF_SIZE];
      uint32_t rx_idx  = 0;

      MSS_UART_init
      (
         &g_mss_uart0,
         MSS_UART_57600_BAUD,
         MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
      );
      while( 1 )
      {
          rx_size = MSS_UART_get_rx( &g_mss_uart0, rx_buff, sizeof(rx_buff) );
          if (rx_size > 0)
          {
              process_rx_data( rx_buff, rx_size );
          }
          task_a();
          task_b();
      }
      return 0;
   }
  @endcode

  Interrupt driven example:
  @code
   int main( void )
   {
      MSS_UART_init
      (
          &g_mss_uart1,
          MSS_UART_57600_BAUD,
          MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
      );
      MSS_UART_set_rx_handler( &g_mss_uart1, uart1_rx_handler,
                               MSS_UART_FIFO_SINGLE_BYTE );
      while( 1 )
      {
          task_a();
          task_b();
      }
      return 0;
   }

   void uart1_rx_handler( mss_uart_instance_t * this_uart )
   {
      uint8_t rx_buff[RX_BUFF_SIZE];
      uint32_t rx_idx  = 0;
      rx_size = MSS_UART_get_rx( this_uart, rx_buff, sizeof(rx_buff) );
      process_rx_data( rx_buff, rx_size );
   }
  @endcode
 */
size_t
MSS_UART_get_rx
(
   mss_uart_instance_t * this_uart,
   uint8_t * rx_buff,
   size_t buff_size
);

/***************************************************************************//**
  The MSS_UART_set_rx_handler() function is used to register a receive handler
  function that is called by the driver when a UART receive data available (RDA)
  interrupt occurs. You must create and register the receive handler function
  to suit your application and it must include a call to the MSS_UART_get_rx()
  function to actually read the received data.

  Note:     The MSS_UART_set_rx_handler() function enables both the RDA
  interrupt in the UART and the MSS UART instance interrupt in the Cortex-M3
  NVIC as part
  of its implementation.

  Note:     You can disable the RDA interrupt once the data is received by
  calling the MSS_UART_disable_irq() function. This is your choice and is
  dependent upon your application.


  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @param handler
    The handler parameter is a pointer to a receive interrupt handler function
    provided by your application that will be called as a result of a UART RDA
    interrupt. This handler function must be of type mss_uart_irq_handler_t.

  @param trigger_level
    The trigger_level parameter is the receive FIFO trigger level. This
    specifies the number of bytes that must be received before the UART
    triggers an RDA interrupt.

  @return
    This function does not return a value.

  Example:
  @code
  #include "mss_uart.h"

  #define RX_BUFF_SIZE    64

  uint8_t g_rx_buff[RX_BUFF_SIZE];

  void uart0_rx_handler( mss_uart_instance_t * this_uart )
  {
      MSS_UART_get_rx( this_uart, &g_rx_buff[g_rx_idx], sizeof(g_rx_buff) );
  }

  int main(void)
  {
      MSS_UART_init
      (
          &g_mss_uart0,
          MSS_UART_57600_BAUD,
          MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
      );
      MSS_UART_set_rx_handler( &g_mss_uart0,
                               uart0_rx_handler,
                               MSS_UART_FIFO_SINGLE_BYTE );
      while ( 1 )
      {
         ;
      }
      return(0);
   }
  @endcode
 */
void
MSS_UART_set_rx_handler
(
    mss_uart_instance_t *       this_uart,
    mss_uart_irq_handler_t          handler,
    mss_uart_rx_trig_level_t    trigger_level
);

/***************************************************************************//**
  The MSS_UART_set_loopback() function is used to locally loopback the Tx and
  Rx lines of a UART. This is not to be confused with the loopback of UART0
  to UART1, which can be achieved through the microcontroller subsystem’s
  system registers.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @param loopback
    The loopback parameter indicates whether or not the UART’s transmit
    and receive lines should be looped back. Allowed values are as follows:
    •   MSS_UART_LOOPBACK_ON
    •   MSS_UART_LOOPBACK_OFF

  @return
    This function does not return a value.

  Example:
  @code
      MSS_UART_set_loopback( &g_mss_uart0, MSS_UART_LOOPBACK_OFF );
  @endcode
 */
void
MSS_UART_set_loopback
(
    mss_uart_instance_t *   this_uart,
    mss_uart_loopback_t     loopback
);

/***************************************************************************//**
  The MSS_UART_set_rxstatus_handler() function is used to register a receiver
  status handler function that is called by the driver when a UART receiver
  line status (RLS) interrupt occurs. You must create and register the handler
  function to suit your application.

  Note:     The MSS_UART_set_rxstatus_handler() function enables both the RLS
  interrupt in the UART and the MSS UART instance interrupt in the Cortex-M3
  NVIC as part of its implementation.

  Note:     You can disable the RLS interrupt when required by calling the
  MSS_UART_disable_irq() function. This is your choice and is dependent upon
  your application.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @param handler
     The handler parameter is a pointer to a receiver line status interrupt
     handler function provided by your application that will be called as a
     result of a UART RLS interrupt. This handler function must be of type
     mss_uart_irq_handler_t.

  @return
     This function does not return a value.

  Example:
  @code
  #include "mss_uart.h"

  void uart_rxsts_handler( mss_uart_instance_t * this_uart )
  {
    uint8_t status;
    status = MSS_UART_get_rx_status( this_uart );
    if( status & MSS_UART_OVERUN_ERROR )
    {
        discard_rx_data();
    }
  }

  int main(void)
  {
    MSS_UART_init( &g_mss_uart0,
                   MSS_UART_57600_BAUD,
                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY |
                                       MSS_UART_ONE_STOP_BIT );
    MSS_UART_set_rxstatus_handler( &g_mss_uart0, uart_rxsts_handler);

    while ( 1 )
    {
        ;
    }
    return(0);
  }
  @endcode
 */
void
MSS_UART_set_rxstatus_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_set_tx_handler() function is used to register a transmit handler
  function that is called by the driver when a UART transmit holding register
  empty (THRE) interrupt occurs. You must create and register the transmit
  handler function to suit your application. You can use the
  MSS_UART_fill_tx_fifo() function in your transmit handler function to
  write data to the transmitter.

  Note:     The MSS_UART_set_tx_handler() function enables both the THRE
  interrupt in the UART and the MSS UART instance interrupt in the Cortex-M3
  NVIC as part of its implementation.

  Note:     You can disable the THRE interrupt when required by calling the
  MSS_UART_disable_irq() function. This is your choice and is dependent upon
  your application.

  Note:     The MSS_UART_irq_tx() function does not use the transmit handler
  function that you register with the MSS_UART_set_tx_handler() function.
  It uses its own internal THRE interrupt handler function that overrides
  any custom interrupt handler that you register using the
  MSS_UART_set_tx_handler() function.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

   @param handler
     The handler parameter is a pointer to a transmit interrupt handler
     function provided by your application that will be called as a result
     of a UART THRE interrupt. This handler function must be of type
     mss_uart_irq_handler_t.

   @return
     This function does not return a value.

   Example:
   @code
    #include "mss_uart.h"

    uint8_t * g_tx_buffer;
    size_t g_tx_size = 0;

    void uart_tx_handler( mss_uart_instance_t * this_uart )
    {
        size_t size_in_fifo;
        size_in_fifo = MSS_UART_fill_tx_fifo( this_uart,
                                              (const uint8_t *)g_tx_buffer,
                                              g_tx_size );
        if ( size_in_fifo  ==  g_tx_size )
        {
            g_tx_size = 0;
            MSS_UART_disable_irq( this_uart, MSS_UART_TBE_IRQ );
        }
        else
        {
            g_tx_buffer = &g_tx_buffer[size_in_fifo];
            g_tx_size = g_tx_size - size_in_fifo;
        }
    }

    int main(void)
    {
        uint8_t message[12] = "Hello world";

        MSS_UART_init( &g_mss_uart0,
                       MSS_UART_57600_BAUD,
                       MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY |
                                           MSS_UART_ONE_STOP_BIT );

        g_tx_buffer = message;
        g_tx_size = sizeof(message);

        MSS_UART_set_tx_handler( &g_mss_uart0, uart_tx_handler);

        while ( 1 )
        {
            ;
        }
        return(0);
    }
   @endcode
 */
void
MSS_UART_set_tx_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_set_modemstatus_handler() function is used to register a modem
  status handler function that is called by the driver when a UART modem status
  (MS) interrupt occurs. You must create and register the handler function to
  suit your application.

  Note:     The MSS_UART_set_modemstatus_handler() function enables both the MS
  interrupt in the UART and the MSS UART instance interrupt in the Cortex-M3 NVIC
  as part of its implementation.

  Note:     You can disable the MS interrupt when required by calling the
  MSS_UART_disable_irq() function. This is your choice and is dependent
  upon your application.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

   @param handler
     The handler parameter is a pointer to a modem status interrupt handler
     function provided by your application that will be called as a result
     of a UART MS interrupt. This handler function must be of type
     mss_uart_irq_handler_t.

   @return
     This function does not return a value.

   Example:
   @code
   #include "mss_uart.h"

   void uart_modem_handler( mss_uart_instance_t * this_uart )
   {
     uint8_t status;
     status = MSS_UART_get_modem_status( this_uart );
     if( status & MSS_UART_CTS )
     {
        uart_cts_handler();
     }
   }

   int main(void)
   {
      MSS_UART_init( &g_mss_uart0,
                     MSS_UART_57600_BAUD,
                     MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY |
                                       MSS_UART_ONE_STOP_BIT );
      MSS_UART_set_modemstatus_handler( &g_mss_uart0, uart_modem_handler);

      while ( 1 )
      {
        ;
      }
      return(0);
   }
   @endcode
 */

void
MSS_UART_set_modemstatus_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
);

/***************************************************************************//**
  The MSS_UART_fill_tx_fifo() function fills the UART's hardware transmitter
  FIFO with the data found in the transmitter buffer that is passed via the
  tx_buffer function parameter. If the transmitter FIFO is not empty when
  the function is called, the function returns immediately without transferring
  any data to the FIFO; otherwise, the function transfers data from the
  transmitter buffer to the FIFO until it is full or until the complete
  contents of the transmitter buffer have been copied into the FIFO. The
  function returns the number of bytes copied into the UART's transmitter FIFO.

  Note:     This function reads the UART’s line status register (LSR) to check
  for the active state of the transmitter holding register empty (THRE) bit
  before transferring data from the data buffer to the transmitter FIFO. If
  THRE is 0, the function returns immediately, without transferring any data
  to the FIFO. If THRE is 1, the function transfers up to 16 bytes of data
  to the FIFO and then returns.

  Note:     The actual transmission over the serial connection will still be
  in progress when this function returns. Use the MSS_UART_get_tx_status()
  function if you need to know when the transmitter is empty.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @param tx_buffer
    The tx_buffer parameter is a pointer to a buffer containing the data
    to be transmitted.

  @param tx_size
    The tx_size parameter is the size in bytes, of the data to be transmitted.

  @return
    This function returns the number of bytes copied into the UART's
    transmitter FIFO.

  Example:
  @code
     void send_using_interrupt( uint8_t * pbuff, size_t tx_size )
     {
        size_t size_in_fifo;
        size_in_fifo = MSS_UART_fill_tx_fifo( &g_mss_uart0, pbuff, tx_size );
     }
  @endcode
 */
size_t
MSS_UART_fill_tx_fifo
(
    mss_uart_instance_t * this_uart,
    const uint8_t * tx_buffer,
    size_t tx_size
);

/***************************************************************************//**
  The MSS_UART_enable_irq() function enables the MSS UART interrupts specified
  by the irq_mask parameter. The irq_mask parameter identifies the MSS UART
  interrupts by bit position, as defined in the interrupt enable register (IER)
  of MSS UART. The MSS UART interrupts and their identifying irq_mask bit
  positions are as follows:
    •   Receive data available interrupt (RDA)              (irq_mask bit 0)
    •   Transmit holding register empty interrupt (THRE)    (irq_mask bit 1)
    •   Receiver line status interrupt (LS)                 (irq_mask bit 2)
    •   Modem status interrupt (MS)                         (irq_mask bit 3)
  When an irq_mask bit position is set to 1, this function enables the
  corresponding MSS UART interrupt in the IER register. When an irq_mask bit
  position is set to 0, the corresponding interrupt’s state remains unchanged
  in the IER register.

  Note:     The MSS_UART_enable_irq() function also enables the MSS UART
  instance interrupt in the Cortex-M3 NVIC.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @param irq_mask
    The irq_mask parameter is used to select which of the MSS UART’s interrupts
    you want to enable. The allowed value for the irq_mask parameter is one
    of the following constants or a bitwise OR of more than one:
    •   MSS_UART_RBF_IRQ        (bit mask = 0x01)
    •   MSS_UART_TBE_IRQ        (bit mask = 0x02)
    •   MSS_UART_LS_IRQ         (bit mask = 0x04)
    •   MSS_UART_MS_IRQ         (bit mask = 0x08)

  @return
     This function does not return a value.

  Example:
  @code
    MSS_UART_enable_irq( &g_mss_uart0,( MSS_UART_RBF_IRQ | MSS_UART_TBE_IRQ ) );
  @endcode
 */
void
MSS_UART_enable_irq
(
    mss_uart_instance_t * this_uart,
    uint8_t irq_mask
);

/***************************************************************************//**
  The MSS_UART_disable_irq() function disables the MSS UART interrupts specified
  by the irq_mask parameter. The irq_mask parameter identifies the MSS UART
  interrupts by bit position, as defined in the interrupt enable register (IER)
  of MSS UART. The MSS UART interrupts and their identifying bit positions are
  as follows:
    •   Receive data available interrupt (RDA)              (irq_mask bit 0)
    •   Transmit holding register empty interrupt (THRE)    (irq_mask bit 1)
    •   Receiver line status interrupt (LS)                 (irq_mask bit 2)
    •   Modem status interrupt (MS)                         (irq_mask bit 3)
  When an irq_mask bit position is set to 1, this function disables the
  corresponding MSS UART interrupt in the IER register. When an irq_mask bit
  position is set to 0, the corresponding interrupt’s state remains unchanged
  in the IER register.

  Note:     If you disable all four of the UART’s interrupts, the
  MSS_UART_disable_irq() function also disables the MSS UART instance
  interrupt in the Cortex-M3 NVIC.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @param irq_mask
     The irq_mask parameter is used to select which of the MSS UART’s interrupts
     you want to disable. The allowed value for the irq_mask parameter is one of
     the following constants or a bitwise OR of more than one:
        •   MSS_UART_RBF_IRQ        (bit mask = 0x01)
        •   MSS_UART_TBE_IRQ        (bit mask = 0x02)
        •   MSS_UART_LS_IRQ         (bit mask = 0x04)
        •   MSS_UART_MS_IRQ         (bit mask = 0x08)

  @return
     This function does not return a value.

  Example:
  @code
    MSS_UART_disable_irq( &g_mss_uart0, ( MSS_UART_RBF_IRQ | MSS_UART_TBE_IRQ ) );
  @endcode
 */
void
MSS_UART_disable_irq
(
    mss_uart_instance_t * this_uart,
    uint8_t irq_mask
);

/***************************************************************************//**
  The MSS_UART_get_rx_status() function returns the receiver error status of the
  MSS UART instance. It reads both the current error status of the receiver from
  the UART’s line status register (LSR) and the accumulated error status from
  preceding calls to the MSS_UART_get_rx() function, and it combines them using
  a bitwise OR. It returns the cumulative overrun, parity, framing, break and
  FIFO error status of the receiver, since the previous call to
  MSS_UART_get_rx_status(), as an 8-bit encoded value.

  Note:     The MSS_UART_get_rx() function reads and accumulates the receiver
  status of the MSS UART instance before reading each byte from the receiver’s
  data register/FIFO. The driver maintains a sticky record of the cumulative
  receiver error status, which persists after the MSS_UART_get_rx() function
  returns. The MSS_UART_get_rx_status() function clears the driver’s sticky
  receiver error record before returning.

  Note:     The driver’s transmit functions also read the line status
  register (LSR) as part of their implementation. When the driver reads the
  LSR, the UART clears any active receiver error bits in the LSR. This could
  result in the driver losing receiver errors. To avoid any loss of receiver
  errors, the transmit functions also update the driver’s sticky record of the
  cumulative receiver error status whenever they read the LSR.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @return
    This function returns the UART’s receiver error status as an 8-bit unsigned
    integer. The returned value is 0 if no receiver errors occurred. The driver
    provides a set of bit mask constants that should be compared with and/or
    used to mask the returned value to determine the receiver error status.
    When the return value is compared to the following bit masks, a non-zero
    result indicates that the corresponding error occurred:
        •   MSS_UART_OVERRUN_ERROR      (bit mask = 0x02)
        •   MSS_UART_PARITY_ERROR       (bit mask = 0x04)
        •   MSS_UART_FRAMING_ERROR      (bit mask = 0x08)
        •   MSS_UART_BREAK_ERROR        (bit mask = 0x10)
        •   MSS_UART_FIFO_ERROR     (bit mask = 0x80)
    When the return value is compared to the following bit mask, a non-zero
    result indicates that no error occurred:
        •   MSS_UART_NO_ERROR       (bit mask = 0x00)
    Upon unsuccessful execution, this function returns:
        •   MSS_UART_INVALID_PARAM      (bit mask = 0xFF)

  Example:
  @code
    uint8_t rx_data[MAX_RX_DATA_SIZE];
    uint8_t err_status;
    err_status = MSS_UART_get_rx_status(&g_mss_uart0);

    if(MSS_UART_NO_ERROR == err_status )
    {
       rx_size = MSS_UART_get_rx( &g_mss_uart0, rx_data, MAX_RX_DATA_SIZE );
    }
  @endcode
 */
uint8_t
MSS_UART_get_rx_status
(
    mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_get_modem_status() function returns the modem status of the
  MSS UART instance. It reads the modem status register (MSR) and returns
  the 8 bit value. The bit encoding of the returned value is exactly the
  same as the definition of the bits in the MSR.

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.

  @return
    This function returns current state of the UART's MSR as an 8 bit
    unsigned integer. The driver provides the following set of bit mask
    constants that should be compared with and/or used to mask the
    returned value to determine the modem status:
        •   MSS_UART_DCTS   (bit mask = 0x01)
        •   MSS_UART_DDSR   (bit mask = 0x02)
        •   MSS_UART_TERI   (bit mask = 0x04)
        •   MSS_UART_DDCD   (bit mask = 0x08)
        •   MSS_UART_CTS    (bit mask = 0x10)
        •   MSS_UART_DSR    (bit mask = 0x20)
        •   MSS_UART_RI     (bit mask = 0x40)
        •   MSS_UART_DCD    (bit mask = 0x80)

  Example:
  @code
    void uart_modem_status_isr( mss_uart_instance_t * this_uart )
    {
        uint8_t status;
        status = MSS_UART_get_modem_status( this_uart );
        if( status & MSS_UART_DCTS ) {
            uart_dcts_handler();
        }
        if( status & MSS_UART_CTS ) {
            uart_cts_handler();
        }
    }
  @endcode
 */
uint8_t
MSS_UART_get_modem_status
(
    mss_uart_instance_t * this_uart
);

/***************************************************************************//**
  The MSS_UART_get_tx_status() function returns the transmitter status of the
  MSS UART instance. It reads both the UART’s line status register (LSR) and
  returns the status of the transmit holding register empty (THRE) and
  transmitter empty (TEMT) bits.*

  @param this_uart
    The this_uart parameter is a pointer to an mss_uart_instance_t
    structure identifying the MSS UART hardware block that will perform
    the requested function. There are two such data structures,
    g_mss_uart0 and g_mss_uart1, associated with MSS UART0 and MSS UART1.
    This parameter must point to either the g_mss_uart0 or g_mss_uart1
    global data structure defined within the UART driver.


  @return
    This function returns the UART’s transmitter status as an 8-bit unsigned
    integer. The returned value is 0 if the transmitter status bits are not
    set or the function execution failed. The driver provides a set of bit
    mask constants that should be compared with and/or used to mask the
    returned value to determine the transmitter status.
    When the return value is compared to the following bit mask, a non-zero
    result indicates that the corresponding transmitter status bit is set:
        •   MSS_UART_THRE       (bit mask = 0x20)
        •   MSS_UART_TEMT       (bit mask = 0x40)
    When the return value is compared to the following bit mask, a non-zero
    result indicates that the transmitter is busy or the function execution
    failed.
        •   MSS_UART_TX_BUSY        (bit mask = 0x00)

  Example:
  @code
    uint8_t tx_buff[10] = "abcdefghi";

    MSS_UART_polled_tx( &g_mss_uart0, tx_buff, sizeof(tx_buff));

    while ( ! (MSS_UART_TEMT & MSS_UART_get_tx_status( &g_mss_uart0 ) ) )
    {
        ;
    }
  @endcode
 */
uint8_t
MSS_UART_get_tx_status
(
    mss_uart_instance_t * this_uart
);


#ifdef __cplusplus
}
#endif

#endif /* __MSS_UART_H_ */
