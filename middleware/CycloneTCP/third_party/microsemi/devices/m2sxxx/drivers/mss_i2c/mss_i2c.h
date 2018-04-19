/*******************************************************************************
* (c) Copyright 2009-2013 Microsemi SoC Products Group.  All rights reserved.
 * 
 * SmartFusion2 microcontroller subsystem I2C bare metal software driver public
 * API.
 *
 * SVN $Revision: 5631 $
 * SVN $Date: 2013-04-06 13:04:00 +0100 (Sat, 06 Apr 2013) $
 */
/*=========================================================================*//**
  @mainpage SmartFusion2 MSS I2C Bare Metal Driver.

  @section intro_sec Introduction
  The SmartFusion2 microcontroller subsystem (MSS) includes two I2C peripherals
  for serial communication. This driver provides a set of functions for
  controlling the MSS I2Cs as part of a bare metal system where no
  operating system is available.
  These drivers can be adapted for use as part of an operating system, but the
  implementation of the adaptation layer between this driver and the operating
  system's driver model is outside the scope of this driver.
  
  @section hw_dependencies Hardware Flow Dependencies
  The configuration of all features of the MSS I2C peripherals is covered by
  this driver with the exception of the SmartFusion2 IOMUX configuration.
  SmartFusion2 allows multiple non-concurrent uses of some external pins through
  IOMUX configuration. This feature allows optimization of external pin usage by
  assigning external pins for use by either the microcontroller subsystem or the
  FPGA fabric. The MSS I2C serial signals are routed through IOMUXs to the
  SmartFusion2 device external pins. The MSS I2C serial signals may also be
  routed through IOMUXs to the SmartFusion2 FPGA fabric.
  The IOMUXs are configured using the SmartFusion2 MSS configurator tool. You
  must ensure that the MSS I2C peripherals are enabled and configured in the
  SmartFusion2 MSS configurator if you wish to use them. For more information on
  IOMUXs, refer to the IOMUX section of the SmartFusion2 Microcontroller
  Subsystem (MSS) User’s Guide.
  The base address and register addresses and interrupt number assignment for
  the MSS I2C peripherals are defined as constants in the SmartFusion2 CMSIS
  HAL. You must ensure that the latest SmartFusion2 CMSIS HAL is included in the
  project settings of the software tool chain used to build your project and
  that it is generated into your project.
  
  @section theory_op Theory of Operation
  The MSS I2C driver functions are grouped into the following categories:
    - Initialization and configuration functions
    - Interrupt control
    - I2C slave address configuration
    - I2C master operations – functions to handle write, read and write-read
                              transactions
    - I2C slave operations – functions to handle write, read and write-read
                             transactions
    - Mixed master and slave operations
    - SMBus interface configuration and control

    
  Initialization and Configuration
    The MSS I2C driver is initialized through a call to the MSS_I2C_init() function.
    This function takes the MSS I2C's configuration as parameters. The
    MSS_I2C_init() function must be called before any other MSS I2C driver functions
    can be called. The first parameter of the MSS_I2C_init() function is a pointer
    to one of two global data structures used by the driver to store state
    information for each MSS I2C. A pointer to these data structures is also used
    as first parameter to any of the driver functions to identify which MSS I2C
    will be used by the called function. The names of these two data structures
    are g_mss_i2c0 and g_mss_i2c1. Therefore any call to an MSS I2C driver function
    should be of the form MSS_I2C_function_name( &g_mss_i2c0, ... ) or
    MSS_I2C_function_name( &g_mss_i2c1, ... ).
    The MSS_I2C_init() function call for each MSS I2C also takes the I2C serial
    address assigned to the MSS I2C and the serial clock divider to be used to
    generate its I2C clock as configuration parameters.
    
  Interrupt Control
    The MSS I2C driver is interrupt driven and it enables and disables the
    generation of INT interrupts by MSS I2C at various times when it is
    operating. The driver automatically handles MSS I2C interrupts internally,
    including enabling disabling and clearing MSS I2C interrupts in the
    Cortex-M3 interrupt controller when required.
    The function MSS_I2C_register_write_handler() is used to register a write
    handler function with the MSS I2C driver that it calls on completion of an
    I2C write transaction by the MSS I2C slave. It is your responsibility to
    create and register the implementation of this handler function that
    processes or trigger the processing of the received data.
    The SMBSUS and SMBALERT interrupts are related to the SMBus interface and
    are enabled and disabled through I2C_enable_smbus_irq(),
    I2C_disable_smbus_irq() respectively. It is your responsibility to create
    interrupt handler functions in your application to get the desired response
    for the SMBus interrupts.


  I2C Slave Address Configuration
    The SmartFusion MSS I2C can respond to two slave addresses:
      - Slave address 0 - This is the primary address that is used for I2C 
                          transactions. You must configure the slave address
                          during MSS_I2C_init() function call, if the MSS I2C is
                          to operate in slave mode.
      - General call address - The MSS I2C slave can be configured to respond to
                          a broadcast command by a master transmitting the
                          general call address 0x00. Use the MSS_I2C_set_gca()
                          function to enable the slave to respond to the general
                          call address. If the I2C slave is not required to
                          respond to the general call address, disable this
                          address by calling MSS_I2C_clear_gca().
                          
  Transaction Types
    The MSS I2C driver is designed to handle three types of I2C transactions:
      Write transactions
      Read transactions
      Write-read transactions
 
    Write transaction
      The master I2C device initiates a write transaction by sending a START bit as
      soon as the bus becomes free. The START bit is followed by the 7-bit serial
      address of the target slave device followed by the read/write bit indicating
      the direction of the transaction. The slave acknowledges the receipt of its
      address with an acknowledge bit. The master sends data one byte at a time to
      the slave, which must acknowledge the receipt of each byte for the next byte
      to be sent. The master sends a STOP bit to complete the transaction.
      The slave can abort the transaction by replying with a non-acknowledge bit
      instead of an acknowledge bit.
      The application programmer can choose not to send a STOP bit at the end of
      the transaction causing the next transaction to begin with a repeated START bit.
      
    Read transaction
      The master I2C device initiates a read transaction by sending a START bit as
      soon as the bus becomes free. The START bit is followed by the 7-bit serial
      address of the target slave device followed by the read/write bit indicating
      the direction of the transaction. The slave acknowledges the receipt of its
      slave address with an acknowledge bit. The slave sends data one byte at a
      time to the master, which must acknowledge receipt of each byte for the
      next byte to be sent. The master sends a non-acknowledge bit following the
      last byte it wishes to read followed by a STOP bit.
      The application programmer can choose not to send a STOP bit at the end of
      the transaction causing the next transaction to begin with a repeated START bit.
 
    Write-read transaction
      The write-read transaction is a combination of a write transaction immediately
      followed by a read transaction. There is no STOP bit between the write and
      read phases of a write-read transaction. A repeated START bit is sent between
      the write and read phases.
      The write-read transaction is typically used to send a command or offset in the
      write transaction specifying the logical data to be transferred during the read
      phase.
      The application programmer can choose not to send a STOP bit at the end of
      the transaction causing the next transaction to begin with a repeated START bit.

  Master Operations
    The application can use the MSS_I2C_write(), MSS_I2C_read() and
    MSS_I2C_write_read() functions to initiate an I2C bus transaction. The
    application can then wait for the transaction to complete using th
    MSS_I2C_wait_complete() function or poll the status of the I2C transaction
    using the MSS_I2C_get_status() function until it returns a value different
    from MSS_I2C_IN_PROGRESS. The MSS_I2C_system_tick() function can be used to
    set a time base for the MSS_I2C_wait_complete() function’s time out delay.

  Slave Operations
    The configuration of the MSS I2C driver to operate as an I2C slave requires
    the use of the following functions:
       MSS_I2C_set_slave_tx_buffer()
       MSS_I2C_set_slave_rx_buffer()
       MSS_I2C_set_slave_mem_offset_length()
       MSS_I2C_register_write_handler()
       MSS_I2C_enable_slave()
    Use of all functions is not required if the slave I2C does not need to support
    all types of I2C read transactions. The subsequent sections list the functions
    that must be used to support each transaction type. 
    
    Responding to read transactions
      The following functions are used to configure the MSS I2C driver to respond
      to I2C read transactions:
        MSS_I2C_set_slave_tx_buffer()
        MSS_I2C_enable_slave()
      The function MSS_I2C_set_slave_tx_buffer() specifies the data buffer that
      will be transmitted when the I2C slave is the target of an I2C read
      transaction. It is then up to the application to manage the content of that
      buffer to control the data that will be transmitted to the I2C master as a
      result of the read transaction.
      The function MSS_I2C_enable_slave() enables the MSS I2C hardware instance
      to respond to I2C transactions. It must be called after the MSS I2C driver
      has been configured to respond to the required transaction types.

    Responding to write transactions
      The following functions are used to configure the MSS I2C driver to
      respond to I2C write transactions:
        - MSS_I2C_set_slave_rx_buffer()
        - MSS_I2C_register_write_handler()
        - MSS_I2C_enable_slave()
      The function MSS_I2C_set_slave_rx_buffer() specifies the data buffer that
      will be used to store the data received by the I2C slave when it is the
      target an I2C  write transaction.
      The function MSS_I2C_register_write_handler() specifies the handler
      function that must be called on completion of the I2C write transaction.
      It is this handler function that will process or trigger the processing of
      the received data.
      The function MSS_I2C_enable_slave() enables the MSS I2C hardware instance
      to respond to I2C transactions. It must be called after the MSS I2C driver
      has been configured to respond to the required transaction types.
      
    Responding to write-read transactions
      The following functions are used to configure the MSS I2C driver to respond
      to write-read transactions:
        - MSS_I2C_set_slave_tx_buffer()
        - MSS_I2C_set_slave_rx_buffer()
        - MSS_I2C_register_write_handler()
        - MSS_I2C_set_slave_mem_offset_length()
        - MSS_I2C_enable_slave()
      The function MSS_I2C_set_slave_mem_offset_length() specifies the number of
      bytes expected by the I2C slave during the write phase of the write-read
      transaction.
      The function MSS_I2C_set_slave_tx_buffer() specifies the data that will be
      transmitted to the I2C master during the read phase of the write-read
      transaction. The value received by the I2C slave during the write phase of
      the transaction will be used as an index into the transmit buffer
      specified by this function to decide which part of the transmit buffer
      will be transmitted to the I2C master as part of the read phase of the
      write-read transaction.
      The function MSS_I2C_set_slave_rx_buffer() specifies the data buffer that
      will be used to store the data received by the I2C slave during the write
      phase of the write-read transaction. This buffer must be at least large
      enough to accommodate the number of bytes specified through the
      MSS_I2C_set_slave_mem_offset_length() function.
      The function MSS_I2C_register_write_handler() can optionally be used to
      specify a handler function that is called on completion of the write phase
      of the I2C write-read transaction. If a handler function is registered, it
      is responsible for processing the received data in the slave receive
      buffer and populating the slave transmit buffer with the data that will be
      transmitted to the I2C master as part of the read phase of the write-read
      transaction.
      The function MSS_I2C_enable_slave() enables the MSS I2C hardware instance
      to respond to I2C transactions. It must be called after the MSS I2C driver
      has been configured to respond to the required transaction types.

  Mixed Master and Slave Operations
      The MSS I2C device supports mixed master and slave operations. If the MSS
      I2C slave has a transaction in progress and your application attempts to
      begin a master mode transaction, the MSS I2C driver queues the master mode
      transaction until the bus is released and the MSS I2C can switch to master
      mode and acquire the bus. The MSS I2C master then starts the previously
      pended transaction.
      
  SMBus Interface Configuration and Control
    The MSS I2C driver initializes the MSS I2C peripheral’s SMBus interface
    using the MSS_I2C_smbus_init() function. The MSS_I2C_suspend_smbus_slave()
    function is used by the MSS I2C, as an SMBus master, to force slave devices
    on the SMBus to enter their power-down/suspend mode. The
    MSS_I2C_reset_smbus() function is used by the MSS I2C, as an SMBus master,
    to force all devices on the SMBus to reset. The MSS_I2C_set_smsbus_alert()
    function is used by the MSS I2C, as an SMBus slave, to force communication
    with the SMBus master. The I2C_enable_smbus_irq() and
    I2C_disable_smbus_irq() functions are used to enable and disable the MSS
    I2C’s SMBSUS and SMBALERT SMBus interrupts.
    
 *//*=========================================================================*/

#ifndef MSS_I2C_H_
#define MSS_I2C_H_

#include "../../CMSIS/m2sxxx.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*-------------------------------------------------------------------------*//**
  The mss_i2c_clock_divider_t type is used to specify the divider to be applied
  to the MSS I2C PCLK or BCLK signal in order to generate the I2C clock.
 */
typedef enum mss_i2c_clock_divider {
    MSS_I2C_PCLK_DIV_256 = 0u,
    MSS_I2C_PCLK_DIV_224,
    MSS_I2C_PCLK_DIV_192,
    MSS_I2C_PCLK_DIV_160,
    MSS_I2C_PCLK_DIV_960,
    MSS_I2C_PCLK_DIV_120,
    MSS_I2C_PCLK_DIV_60,
    MSS_I2C_BCLK_DIV_8
} mss_i2c_clock_divider_t;

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_RELEASE_BUS constant is used to specify the options parameter to
  functions MSS_I2C_read(), MSS_I2C_write() and MSS_I2C_write_read() to indicate
  that a STOP bit must be generated at the end of the I2C transaction to release
  the bus.
 */
#define MSS_I2C_RELEASE_BUS     0x00u

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_HOLD_BUS constant is used to specify the options parameter to
  functions MSS_I2C_read(), MSS_I2C_write() and MSS_I2C_write_read() to
  indicate that a STOP bit must not be generated at the end of the I2C
  transaction in order to retain the bus ownership. This causes the next
  transaction to begin with a repeated START bit and no STOP bit between the
  transactions.
 */
#define MSS_I2C_HOLD_BUS        0x01u

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_SMBALERT_IRQ constant is used with the MSS_I2C_enable_smbus_irq()
  and MSS_I2C_disable_smbus_irq() functions to enable or disable the SMBus
  SMBALERT interrupt.
 */
#define MSS_I2C_SMBALERT_IRQ       0x01u

/*-------------------------------------------------------------------------*//**
 The MSS_I2C_SMBSUS_IRQ constant is used with the MSS_I2C_enable_smbus_irq() and
 MSS_I2C_disable_smbus_irq() functions to enable or disable the SMBus
 SMBSUS interrupt.
 */
#define MSS_I2C_SMBSUS_IRQ         0x02u

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_NO_TIMEOUT constant is used to specify the timeout_ms parameter to
  the MSS_I2C_wait_complete() function to indicate that the function must not
  time out while waiting for the I2C transaction to complete.
 */
#define MSS_I2C_NO_TIMEOUT  0u

/*-------------------------------------------------------------------------*//**
  The mss_i2c_status_t type is used to report the status of I2C transactions.
 */
typedef enum mss_i2c_status
{
    MSS_I2C_SUCCESS = 0u,
    MSS_I2C_IN_PROGRESS,
    MSS_I2C_FAILED,
    MSS_I2C_TIMED_OUT
} mss_i2c_status_t;

/*-------------------------------------------------------------------------*//**
  The mss_i2c_slave_handler_ret_t type is used by slave write handler functions
  to indicate whether or not the received data buffer should be released.
 */
typedef enum mss_i2c_slave_handler_ret {
    MSS_I2C_REENABLE_SLAVE_RX = 0u,
    MSS_I2C_PAUSE_SLAVE_RX = 1u
} mss_i2c_slave_handler_ret_t;

typedef struct mss_i2c_instance mss_i2c_instance_t ;

/*-------------------------------------------------------------------------*//**
  Slave write handler functions prototype.
  ------------------------------------------------------------------------------ 
  This defines the function prototype that must be followed by MSS I2C slave
  write handler functions. These functions are registered with the MSS I2C driver
  through the MSS_I2C_register_write_handler() function.
  
  Declaring and Implementing Slave Write Handler Functions:
    Slave write handler functions should follow the following prototype:
    mss_i2c_slave_handler_ret_t write_handler
    (
        mss_i2c_instance_t *instance, uint8_t * data, uint16_t size
    );
    The instance parameter is a pointer to mss_i2c_instance_t for which this
    slave write handler has been declared.
    The data parameter is a pointer to a buffer (received data buffer) holding
    the data written to the MSS I2C slave.
    The size parameter is the number of bytes held in the received data buffer.
    Handler functions must return one of the following values:
        MSS_I2C_REENABLE_SLAVE_RX
        MSS_I2C_PAUSE_SLAVE_RX.
    If the handler function returns MSS_I2C_REENABLE_SLAVE_RX, the driver
    releases the received data buffer and allows further I2C write transactions
    to the MSS I2C slave to take place.
    If the handler function returns MSS_I2C_PAUSE_SLAVE_RX, the MSS I2C slave
    responds to subsequent write requests with a non-acknowledge bit (NACK),
    until the received data buffer content has been processed by some other part
    of the software application.
    A call to MSS_I2C_enable_slave() is required at some point after
    returning MSS_I2C_PAUSE_SLAVE_RX in order to release the received data
    buffer so it can be used to store data received by subsequent I2C write
    transactions.
 */
typedef mss_i2c_slave_handler_ret_t (*mss_i2c_slave_wr_handler_t)( mss_i2c_instance_t *instance, uint8_t *, uint16_t );

/*-------------------------------------------------------------------------*//**
  mss_i2c_instance_t
  ------------------------------------------------------------------------------
  There is one instance of this structure for each of the MSS I2Cs. Instances
  of this structure are used to identify a specific MSS I2C. A pointer to an
  instance of the mss_i2c_instance_t structure is passed as the first parameter
  to MSS I2C driver functions to identify which MSS I2C should perform the
  requested operation.
 */
struct mss_i2c_instance
{
    uint_fast8_t ser_address;

    /* Transmit related info:*/
    uint_fast8_t target_addr;
    
    /* Current transaction type (WRITE, READ, RANDOM_READ)*/
    uint8_t transaction;
    
    uint_fast16_t random_read_addr;

    uint8_t options;
    
    /* I2C hardware instance identification */
    IRQn_Type  irqn;
    I2C_TypeDef * hw_reg;
    I2C_BitBand_TypeDef * hw_reg_bit;
    
    /* Master TX INFO: */
    const uint8_t * master_tx_buffer;
    uint_fast16_t master_tx_size;
    uint_fast16_t master_tx_idx;
    uint_fast8_t dir;
    
    /* Master RX INFO: */
    uint8_t * master_rx_buffer;
    uint_fast16_t master_rx_size;
    uint_fast16_t master_rx_idx;

    /* Master Status */
    volatile mss_i2c_status_t master_status;
    uint32_t master_timeout_ms;

    /* Slave TX INFO */
    const uint8_t * slave_tx_buffer;
    uint_fast16_t slave_tx_size;
    uint_fast16_t slave_tx_idx;
    
    /* Slave RX INFO */
    uint8_t * slave_rx_buffer;
    uint_fast16_t slave_rx_size;
    uint_fast16_t slave_rx_idx;

    /* Slave Status */
    volatile mss_i2c_status_t slave_status;
    
    /* Slave data: */
    uint_fast8_t slave_mem_offset_length;
    mss_i2c_slave_wr_handler_t slave_write_handler;
    uint8_t is_slave_enabled;

    /* User  specific data */
    void *p_user_data ;

    /* I2C bus status */
    uint8_t bus_status;

    /* Is transaction pending flag */
    uint8_t is_transaction_pending;

    /* I2C Pending transaction */
    uint8_t pending_transaction;
};

/*-------------------------------------------------------------------------*//**
  This instance of mss_i2c_instance_t holds all data related to the operations
  performed by MSS I2C 0. A pointer to g_mss_i2c0 is passed as the first
  parameter to MSS I2C driver functions to indicate that MSS I2C 0 should
  perform the requested operation.
 */
extern mss_i2c_instance_t g_mss_i2c0;

/*-------------------------------------------------------------------------*//**
  This instance of mss_i2c_instance_t holds all data related to the operations
  performed by MSS I2C 1. A pointer to g_mss_i2c1 is passed as the first
  parameter to MSS I2C driver functions to indicate that MSS I2C 1 should
  perform the requested operation.
 */
extern mss_i2c_instance_t g_mss_i2c1;

/*-------------------------------------------------------------------------*//**
  MSS I2C initialization routine.
  ------------------------------------------------------------------------------
  The MSS_I2C_init() function initializes and configures hardware and data
  structures of one of the SmartFusion2 MSS I2Cs.

  ------------------------------------------------------------------------------ 
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block to be initialized. There are two such
    data structures, g_mss_i2c0 and g_mss_i2c1, associated with MSS I2C 0 and
    MSS I2C 1 respectively. This parameter must point to either the g_mss_i2c0
    or g_mss_i2c1 global data structure defined within the I2C driver.
    
  @param ser_address:
    This parameter sets the I2C serial address being initialized. It is the I2C
    bus address to which the MSS I2C instance responds. I2C can operate in master
    or slave mode. The serial address is significant only in the case of I2C
    slave mode. In master mode, MSS I2C does not require serial address and
    providing any value to this parameter does not have any effect. If you do
    not intend to use the I2C device in slave mode, then any dummy slave address
    value can be provided to this parameter. However, in case of MSS I2C device
    switching from master to slave, it is advisable to initialize the MSS I2C
    device with configured serial slave address. You need to call the
    MSS_I2C_init() function whenever it requires to change the slave address as
    there is no separate function to set slave address of I2C device.
   
  @param ser_clock_speed:
    This parameter sets the I2C serial clock frequency. It selects the divider
    that will be used to generate the serial clock from the APB PCLK or from
    the BCLK. It can be one of the following:
        MSS_I2C_PCLK_DIV_256
        MSS_I2C_PCLK_DIV_224
        MSS_I2C_PCLK_DIV_192
        MSS_I2C_PCLK_DIV_160
        MSS_I2C_PCLK_DIV_960
        MSS_I2C_PCLK_DIV_120
        MSS_I2C_PCLK_DIV_60
        MSS_I2C_BCLK_DIV_8
 */
void MSS_I2C_init
(
    mss_i2c_instance_t * this_i2c,
    uint8_t ser_address,
    mss_i2c_clock_divider_t ser_clock_speed
);

/*******************************************************************************
 *******************************************************************************
 * 
 *                           Master specific functions
 * 
 * The following functions are only used within an I2C master's implementation.
 */
 
/*-------------------------------------------------------------------------*//**
  I2C master write function.
  ------------------------------------------------------------------------------
  This function initiates an I2C master write transaction. This function returns
  immediately after initiating the transaction. The content of the write buffer
  passed as parameter should not be modified until the write transaction
  completes. It also means that the memory allocated for the write buffer should
  not be freed or should not go out of scope before the write completes.
  You can check for the write transaction completion using the MSS_I2C_status()
  function.
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
  
  @param serial_addr:
    This parameter specifies the serial address of the target I2C device.
  
  @param write_buffer:
    This parameter is a pointer to a buffer holding the data to be written to
    the target I2C device.
    Care must be taken not to release the memory used by this buffer before the
    write transaction completes. For example, it is not appropriate to return
    from a function allocating this buffer as an array variable before the write
    transaction completes as this would result in the buffer's memory being
    de-allocated from the stack when the function returns. This memory could
    then be subsequently reused and modified causing unexpected data to be
    written to the target I2C device.
  
  @param write_size:
    Number of bytes held in the write_buffer to be written to the target I2C
    device.
 
 @param options:
    The options parameter is used to indicate if the I2C bus should be released
    on completion of the write transaction. Using the MSS_I2C_RELEASE_BUS
    constant for the options parameter causes a STOP bit to be generated at the
    end of the write transaction causing the bus to be released for other I2C
    devices to use. Using the MSS_I2C_HOLD_BUS constant as options parameter
    prevents a STOP bit from being generated at the end of the write
    transaction, preventing other I2C devices from initiating a bus transaction.
 */
void MSS_I2C_write
(
    mss_i2c_instance_t * this_i2c,
    uint8_t serial_addr,
    const uint8_t * write_buffer,
    uint16_t write_size,
    uint8_t options
);

/*-------------------------------------------------------------------------*//**
  I2C master read.
  ------------------------------------------------------------------------------
  This function initiates an I2C master read transaction. This function returns
  immediately after initiating the transaction.
  The content of the read buffer passed as the parameter should not be modified
  until the read transaction completes. It also means that the memory allocated
  for the read buffer should not be freed or should not go out of scope before
  the read completes. You can check for the read transaction completion using
  the MSS_I2C_status() function.

  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
  
  @param serial_addr:
    This parameter specifies the serial address of the target I2C device.
  
  @param read_buffer
    This is pointer to a buffer where the data received from the target device will
    be stored.
    Care must be taken not to release the memory used by this buffer before the
    read transaction completes. For example, it is not appropriate to return
    from a function allocating this buffer as an array variable before the read
    transaction completes as this would result in the buffer's memory being
    de-allocated from the stack when the function returns. This memory could
    then be subsequently reallocated resulting in the read transaction
    corrupting the newly allocated memory. 

  @param read_size:
    This parameter is the number of bytes to read from the target device. This
    size must not exceed the size of the read_buffer buffer.
 
  @param options:
    The options parameter is used to indicate if the I2C bus should be released
    on completion of the read transaction. Using the MSS_I2C_RELEASE_BUS
    constant for the options parameter causes a STOP bit to be generated at the
    end of the read transaction causing the bus to be released for other I2C
    devices to use. Using the MSS_I2C_HOLD_BUS constant as options parameter
    prevents a STOP bit from being generated at the end of the read transaction,
    preventing other I2C devices from initiating a bus transaction.
 */
void MSS_I2C_read
(
    mss_i2c_instance_t * this_i2c,
    uint8_t serial_addr,
    uint8_t * read_buffer,
    uint16_t read_size,
    uint8_t options
);

/*-------------------------------------------------------------------------*//**
  I2C master write-read
  ------------------------------------------------------------------------------
  This function initiates an I2C write-read transaction where data is first
  written to the target device before issuing a restart condition and changing
  the direction of the I2C transaction in order to read from the target device.
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
  
  @param serial_addr:
    This parameter specifies the serial address of the target I2C device.
  
  @param addr_offset:
    This parameter is a pointer to the buffer containing the data that will be
    sent to the slave during the write phase of the write-read transaction. This
    data is typically used to specify an address offset specifying to the I2C
    slave device what data it must return during the read phase of the
    write-read transaction.
  
  @param offset_size:
    This parameter specifies the number of offset bytes to be written during the
    write phase of the write-read transaction. This is typically the size of the
    buffer pointed to by the addr_offset parameter.
  
  @param read_buffer:
    This parameter is a pointer to the buffer where the data read from the I2C
    slave will be stored.
  
  @param read_size:
    This parameter specifies the number of bytes to read from the target I2C
    slave device. This size must not exceed the size of the buffer pointed to by
    the read_buffer parameter.
 
  @param options:
    The options parameter is used to indicate if the I2C bus should be released
    on completion of the write-read transaction. Using the MSS_I2C_RELEASE_BUS
    constant for the options parameter causes a STOP bit to be generated at the
    end of the write-read transaction causing the bus to be released for other
    I2C devices to use. Using the MSS_I2C_HOLD_BUS constant as options parameter
    prevents a STOP bit from being generated at the end of the write-read
    transaction, preventing other I2C devices from initiating a bus transaction.
 */
void MSS_I2C_write_read
(
    mss_i2c_instance_t * this_i2c,
    uint8_t serial_addr,
    const uint8_t * addr_offset,
    uint16_t offset_size,
    uint8_t * read_buffer,
    uint16_t read_size,
    uint8_t options
);
    
/*-------------------------------------------------------------------------*//**
  I2C status
  ------------------------------------------------------------------------------
  This function indicates the current state of an MSS I2C instance.
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
  ------------------------------------------------------------------------------
  @return
    The return value indicates the current state of a MSS I2C instance or the
    outcome of the previous transaction if no transaction is in progress. 
    Possible return values are:
      MSS_I2C_SUCCESS
        The last I2C transaction has completed successfully.  
      MSS_I2C_IN_PROGRESS
        There is an I2C transaction in progress.
      MSS_I2C_FAILED
        The last I2C transaction failed.
 */
mss_i2c_status_t MSS_I2C_get_status
(
    mss_i2c_instance_t * this_i2c
);

/*-------------------------------------------------------------------------*//**
  Wait for I2C transaction completion.
  ------------------------------------------------------------------------------
  This function waits for the current I2C transaction to complete. The return
  value indicates whether the last I2C transaction was successful, or is still
  in progress, or failed.
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
  @param timeout_ms:
    The timeout_ms parameter specifies the delay within which the current I2C
    transaction is expected to complete. The time out delay is given in
    milliseconds. The MSS_I2C_wait_complete() function returns MSS_I2C_TIMED_OUT
    if the current transaction does not complete before the time out delay
    expires.
    Alternatively, the timeout_ms parameter can be set to MSS_I2C_NO_TIMEOUT to
    indicate that the MSS_I2C_wait_complete() function must not time out.
    Note: If you set the timeout_ms parameter to a value other than
          MSS_I2C_NO_TIMEOUT, you must call the MSS_I2C_system_tick() function
          from an implementation of the Cortex-M3 SysTick timer interrupt
          service routine – SysTick_Handler() – in your application. Otherwise
          the time out will not take effect and the MSS_I2C_wait_complete()
          function will not time out.
          
  ------------------------------------------------------------------------------
  @return
    The return value indicates the outcome of the last I2C transaction. It can
    be one of the following: 
      MSS_I2C_SUCCESS
        The last I2C transaction has completed successfully.
      MSS_I2C_FAILED
        The last I2C transaction failed.
      MSS_I2C_TIMED_OUT
        The last transaction failed to complete within the time out delay
        specified by the timeout_ms parameter.
 */
mss_i2c_status_t MSS_I2C_wait_complete
(
    mss_i2c_instance_t * this_i2c,
    uint32_t timeout_ms
);

/*-------------------------------------------------------------------------*//**
  Time out delay expiration.
  ------------------------------------------------------------------------------
  This function is used to control the expiration of the time out delay
  specified as a parameter to the I2C_wait_complete() function. It must be
  called from the interrupt service routine of a periodic interrupt source such
  as the Cortex-M3 SysTick timer interrupt. It takes the period of the interrupt
  source as its ms_since_last_tick parameter and uses it as the time base for
  the I2C_wait_complete() function’s time out delay.
  Note: This function does not need to be called if the I2C_wait_complete()
        function is called with a timeout_ms value of MSS_I2C_NO_TIMEOUT.
  Note: If this function is not called then the MSS_I2C_wait_complete() function
        will behave as if its timeout_ms was specified as MSS_I2C_NO_TIMEOUT and
        it will not time out.
        
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined within
    the I2C driver.
  @param ms_since_last_tick:
    The ms_since_last_tick parameter specifies the number of milliseconds that
    elapsed since the last call to MSS_I2C_system_tick(). This parameter would
    typically be a constant specifying the interrupt rate of a timer used to
    generate system ticks.
  ------------------------------------------------------------------------------
  @return
    none.

  Example:
    The example below shows an example of how the I2C_system_tick() function
    would be called in a Cortex-M3 based system. I2C_system_tick() is called for
    each I2C channel from the Cortex-M3 SysTick timer interrupt service routine.
    The SysTick is configured to generate an interrupt every 10 milliseconds in
    the example below.
  @code
    void SysTick_Handler(void)
    {
        MSS_I2C_system_tick(&g_mss_i2c0, 10);
        MSS_I2C_system_tick(&g_mss_i2c1, 10);
    }

  @endcode
 */
void MSS_I2C_system_tick
(
    mss_i2c_instance_t * this_i2c,
    uint32_t ms_since_last_tick
);

/*******************************************************************************
 *******************************************************************************
 * 
 *                           Slave specific functions
 * 
 * The following functions are only used within the implementation of an I2C
 * slave device.
 */

/*-------------------------------------------------------------------------*//**
  I2C slave transmit buffer configuration.
  ------------------------------------------------------------------------------
  This function specifies the memory buffer holding the data that will be sent
  to the I2C master when this MSS I2C instance is the target of an I2C read or
  write-read transaction.
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
  
  @param tx_buffer:
    This parameter is a pointer to the memory buffer holding the data to be
    returned to the I2C master when this MSS I2C instance is the target of an
    I2C read or write-read transaction.
  
  @param tx_size:
    Size of the transmit buffer pointed to by the tx_buffer parameter.
 */
void MSS_I2C_set_slave_tx_buffer
(
    mss_i2c_instance_t * this_i2c,
    const uint8_t * tx_buffer,
    uint16_t tx_size
);

/*-------------------------------------------------------------------------*//**
  I2C slave receive buffer configuration.
  ------------------------------------------------------------------------------
  This function specifies the memory buffer that will be used by the MSS I2C
  instance to receive data when it is a slave. This buffer is the memory where
  data will be stored when the MSS I2C is the target of an I2C master write
  transaction (i.e. when it is the slave).
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
  
  @param rx_buffer:
    This parameter is a pointer to the memory buffer allocated by the caller
    software to be used as a slave receive buffer.
  
  @param rx_size:
    Size of the slave receive buffer. This is the amount of memory that is
    allocated to the buffer pointed to by rx_buffer.
    Note:   This buffer size indirectly specifies the maximum I2C write
            transaction length this MSS I2C instance can be the target of.
            This is because this MSS I2C instance responds to further received
            bytes with a non-acknowledge bit (NACK) as soon as it’s receive
            buffer is full. This causes the write transaction to fail.
 */
void MSS_I2C_set_slave_rx_buffer
(
    mss_i2c_instance_t * this_i2c,
    uint8_t * rx_buffer,
    uint16_t rx_size
);

/*-------------------------------------------------------------------------*//**
  I2C slave memory offset length configuration.
  ------------------------------------------------------------------------------
  This function is used as part of the configuration of a MSS I2C instance for
  operation as a slave supporting write-read transactions. It specifies the
  number of bytes expected as part of the write phase of a write-read
  transaction. The bytes received during the write phase of a write-read
  transaction are interpreted as an offset into the slave's transmit buffer.
  This allows random access into the I2C slave transmit buffer from a remote
  I2C master.
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
  
  @param offset_length:
    The offset_length parameter configures the number of bytes to be interpreted
    by the MSS I2C slave as a memory offset value during the write phase of
    write-read transactions. The maximum value for the offset_length parameter is
    two.
 */
void MSS_I2C_set_slave_mem_offset_length
(
    mss_i2c_instance_t * this_i2c,
    uint8_t offset_length
);

/*-------------------------------------------------------------------------*//**
  I2C write handler registration. 
  ------------------------------------------------------------------------------
  Register the function that is called to process the data written to this MSS
  I2C instance when it is the slave in an I2C write transaction.
  Note: If a write handler is registered, it is called on completion of the
        write phase of a write-read transaction and responsible for processing
        the received data in the slave receive buffer and populating the slave
        transmit buffer with the data that will be transmitted to the I2C master
        as part of the read phase of the write-read transaction. If a write
        handler is not registered, the write data of a write read transaction is
        interpreted as an offset into the slave’s transmit buffer and handled by
        the driver.
        
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
  
  @param handler:
    Pointer to the function that will process the I2C write request.
 */
void MSS_I2C_register_write_handler
(
    mss_i2c_instance_t * this_i2c,
    mss_i2c_slave_wr_handler_t handler
);

/*-------------------------------------------------------------------------*//**
  I2C slave enable.
  ------------------------------------------------------------------------------
  This function enables the MSS I2C instance identified through the this_i2c
  parameter for slave mode operation. It enables the MSS I2C slave to receive
  data when it is the target of an I2C read, write or write-read transaction.
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
 */
void MSS_I2C_enable_slave
(
    mss_i2c_instance_t * this_i2c
);

/*-------------------------------------------------------------------------*//**
  I2C slave disable.
  ------------------------------------------------------------------------------
  This function disables the MSS I2C instance identified through the this_i2c
  parameter for slave mode operation. It stops the MSS I2C slave acknowledging
  I2C read, write or write-read transactions targeted at it.
  ------------------------------------------------------------------------------
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.
 */
void MSS_I2C_disable_slave
(
    mss_i2c_instance_t * this_i2c
);

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_set_gca() function is used to set the general call acknowledgment
  bit in the MSS I2C slave. This allows the MSS I2C slave to respond to a
  general call or broadcast message from the I2C master.

  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.

  @return
    none.
 */
void MSS_I2C_set_gca
(
    mss_i2c_instance_t * this_i2c
);

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_clear_gca() function is used to clear the general call
  acknowledgment bit in the MSS I2C slave. This stops the MSS I2C slave
  responding to any general call or broadcast message from the I2C master.

  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.

  @return
    none.
 */
void MSS_I2C_clear_gca
(
    mss_i2c_instance_t * this_i2c
);

/*------------------------------------------------------------------------------
                      I2C SMBUS specific APIs
 ----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_smbus_init() function enables SMBus timeouts and status logic. Set
  the frequency parameter to the MSS I2C’s PCLK frequency for 25ms SMBus
  timeouts, or to any frequency between 1 MHz and 255 MHz for
  increased/decreased timeouts.

  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.

  @param frequency
    The frequency parameter specifies the a frequency in MHz from 1 to 255. It
    can be the MSS I2C’s PCLK frequency to specify 25ms SMBus timeouts, or a
    higher or lower frequency than the PCLK for increased decreased timeouts.

  @return
    none.
 */
void MSS_I2C_smbus_init
(
    mss_i2c_instance_t * this_i2c,
    uint8_t frequency
);

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_enable_smbus_irq() function is used to enable the MSS I2C’s
  SMBSUS and SMBALERT SMBus interrupts.

  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.

  @param irq_type
    The irq_type parameter specifies which SMBus interrupt(s) to enable. The two
    possible interrupts are:
     - MSS_I2C_SMBALERT_IRQ
     - MSS_I2C_SMBSUS_IRQ
     
  @return
    none
 */
void MSS_I2C_enable_smbus_irq
(
    mss_i2c_instance_t * this_i2c,
    uint8_t  irq_type
);

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_enable_smbus_irq() function is used to disable the MSS I2C’s
  SMBSUS and SMBALERT SMBus interrupts.

  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.

  @param irq_type
    The irq_type parameter specifies which SMBus interrupt(s) to enable. The two
    possible interrupts are:
     - MSS_I2C_SMBALERT_IRQ
     - MSS_I2C_SMBSUS_IRQ.
     
  @return
    none.
 */
void MSS_I2C_disable_smbus_irq
(
    mss_i2c_instance_t * this_i2c,
    uint8_t  irq_type
);

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_suspend_smbus_slave() function is used by the MSS I2C, as an
  SMBus master, to force slave devices on the SMBus to enter their
  power-down/suspend mode. The MSS I2C asserts its I2C_X_SMBSUS_NO output
  signal.
  
  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.

  @return
    This function does not return a value.
 */
void MSS_I2C_suspend_smbus_slave
(
    mss_i2c_instance_t * this_i2c
);

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_reset_smbus() function is used by the MSS I2C, as an SMBus master,
  to force all devices on the SMBus to reset.

  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.

  @return
    This function does not return a value.
 */
void MSS_I2C_reset_smbus
(
    mss_i2c_instance_t * this_i2c
);

/*-------------------------------------------------------------------------*//**
  The MSS_I2C_set_smsbus_alert() function is used by the MSS I2C, as an SMBus
  slave, to force communication with the SMBus master. The MSS I2C asserts its
  I2C_X_SMBALERT_NO output signal.

  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.

  @return
    This function does not return a value.
 */
void MSS_I2C_set_smbus_alert
(
    mss_i2c_instance_t * this_i2c
);

/*-------------------------------------------------------------------------*//**
  The function I2C_set_user_data()sets user data.

  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.

  @param p_user_data
    The parameter p_user_data is pointer of type void. User can pass information
    using this input data.

  @return
    none.
 */
void MSS_I2C_set_user_data
(
    mss_i2c_instance_t * this_i2c,
    void * p_user_data
);

/*-------------------------------------------------------------------------*//**
  The function MSS_I2C_get_user_data() function returns user data.

  @param this_i2c:
    The this_i2c parameter is a pointer to an mss_i2c_instance_t structure
    identifying the MSS I2C hardware block that performs the requested function.
    There are two such data structures, g_mss_i2c0 and g_mss_i2c1, associated
    with MSS I2C 0 and MSS I2C 1 respectively. This parameter must point to
    either the g_mss_i2c0 or the g_mss_i2c1 global data structure defined
    within the I2C driver.

  @return
    It returns user_data which is a pointer to void type.
 */
void * MSS_I2C_get_user_data
(
    mss_i2c_instance_t * this_i2c
);

#ifdef __cplusplus
}
#endif

#endif /*MSS_I2C_H_*/
