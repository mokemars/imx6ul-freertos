/*******************************************************************************
 * (c) Copyright 2009-2013 Microsemi SoC Products Group.  All rights reserved.
 * 
 * SmartFusion microcontroller subsystem I2C bare metal software driver 
 * implementation.
 * 
 * SVN $Revision: 5631 $
 * SVN $Date: 2013-04-06 13:04:00 +0100 (Sat, 06 Apr 2013) $
 */
#include "mss_i2c.h"
#include "../../CMSIS/mss_assert.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif 

/*------------------------------------------------------------------------------
 * I2C transaction direction.
 */
#define WRITE_DIR    0u
#define READ_DIR     1u

/* -- TRANSACTIONS TYPES -- */
#define NO_TRANSACTION                      0u
#define MASTER_WRITE_TRANSACTION            1u
#define MASTER_READ_TRANSACTION             2u
#define MASTER_RANDOM_READ_TRANSACTION      3u
#define WRITE_SLAVE_TRANSACTION             4u
#define READ_SLAVE_TRANSACTION              5u

/* -- SMBUS H/W STATES -- */
/* -- MASTER STATES -- */
#define ST_START            0x08u           /* start condition sent */
#define ST_RESTART          0x10u           /* repeated start */
#define ST_SLAW_ACK         0x18u           /* SLA+W sent, ack received */
#define ST_SLAW_NACK        0x20u           /* SLA+W sent, nack received */
#define ST_TX_DATA_ACK      0x28u           /* Data sent, ACK'ed */
#define ST_TX_DATA_NACK     0x30u           /* Data sent, NACK'ed */
#define ST_LOST_ARB         0x38u           /* Master lost arbitration */
#define ST_SLAR_ACK         0x40u           /* SLA+R sent, ACK'ed */
#define ST_SLAR_NACK        0x48u           /* SLA+R sent, NACK'ed */
#define ST_RX_DATA_ACK      0x50u           /* Data received, ACK sent */
#define ST_RX_DATA_NACK     0x58u           /* Data received, NACK sent */
#define ST_RESET_ACTIVATED  0xD0u           /* Master reset is activated */

/* -- SLAVE STATES -- */
#define ST_SLAVE_SLAW       0x60u           /* SLA+W received */
#define ST_SLAVE_SLAR_ACK   0xA8u           /* SLA+R received, ACK returned */
#define ST_SLV_LA           0x68u           /* Slave lost arbitration */
#define ST_GCA              0x70u           /* GCA received */
#define ST_GCA_LA           0x78u           /* GCA lost arbitration */
#define ST_RDATA            0x80u           /* Data received */
#define ST_SLA_NACK         0x88u           /* Slave addressed, NACK returned */
#define ST_GCA_ACK          0x90u           /* Previously addresses with GCA, data ACKed */
#define ST_GCA_NACK         0x98u           /* GCA addressed, NACK returned */
#define ST_RSTOP            0xA0u           /* Stop received */
#define ST_SLARW_LA         0xB0u           /* Arbitration lost */
#define ST_RACK             0xB8u           /* Byte sent, ACK received */
#define ST_SLAVE_RNACK      0xC0u           /* Byte sent, NACK received */
#define ST_FINAL            0xC8u           /* Final byte sent, ACK received */
#define ST_SLV_RST          0xD8u           /* Slave reset state */

/* SMBUS related Masks */
#define SMBUS_IRQ_MASK               0x03u
#define SMBUS_ENABLE_MASK            0x04u
#define SMBUS_SMBUS_ALERT_MASK       0x10u
#define SMBUS_SUSPEND_MASK           0x40u
#define SMBUS_RESET_MASK             0x80u

/*
 * Maximum address offset length in slave write-read transactions.
 * A maximum of two bytes will be interpreted as address offset within the slave
 * tx buffer.
 */
#define MAX_OFFSET_LENGTH       2u

/*------------------------------------------------------------------------------
 *
 */
static uint32_t disable_interrupts( void );
static void restore_interrupts( uint32_t primask );
static void mss_i2c_isr( mss_i2c_instance_t * this_i2c );
static void enable_slave_if_required(mss_i2c_instance_t * this_i2c);

#if defined(__GNUC__)
__attribute__((__interrupt__)) void I2C0_IRQHandler(void);
#else
void I2C0_IRQHandler(void);
#endif

#if defined(__GNUC__)
__attribute__((__interrupt__)) void I2C1_IRQHandler( void );
#else
void I2C1_IRQHandler( void );
#endif

/*------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * 
 */
mss_i2c_instance_t g_mss_i2c0;
mss_i2c_instance_t g_mss_i2c1;

/*------------------------------------------------------------------------------
 * MSS_I2C_init()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_init
(
    mss_i2c_instance_t * this_i2c,
    uint8_t ser_address,
    mss_i2c_clock_divider_t ser_clock_speed
)
{
    uint_fast16_t clock_speed = (uint_fast16_t)ser_clock_speed;
    
    ASSERT( (this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1) );
    
    /*
     * Initialize all items of the this_i2c data structure to zero. This
     * initializes all state variables to their init value. It relies on
     * the fact that NO_TRANSACTION, I2C_SUCCESS and I2C_RELEASE_BUS all
     * have an actual value of zero.
     */
    memset(this_i2c, 0, sizeof(mss_i2c_instance_t));
    
    if ( this_i2c == &g_mss_i2c0 )
    {
        this_i2c->irqn = I2C0_IRQn;
        this_i2c->hw_reg = I2C0;
        this_i2c->hw_reg_bit = I2C0_BITBAND;
        
        /* reset I2C0 */
        SYSREG->SOFT_RST_CR |= SYSREG_I2C0_SOFTRESET_MASK;
        /* Clear any previously pended I2C0 interrupt */
        NVIC_ClearPendingIRQ( I2C0_IRQn );
        /* Take I2C0 out of reset. */
        SYSREG->SOFT_RST_CR &= ~SYSREG_I2C0_SOFTRESET_MASK;
    }
    else
    {
        this_i2c->irqn = I2C1_IRQn;
        this_i2c->hw_reg = I2C1;
        this_i2c->hw_reg_bit = I2C1_BITBAND;
        
        /* reset I2C1 */
        SYSREG->SOFT_RST_CR |= SYSREG_I2C1_SOFTRESET_MASK;
        /* Clear any previously pended I2C1 interrupt */
        NVIC_ClearPendingIRQ( I2C1_IRQn );
        /* Take I2C1 out of reset. */
        SYSREG->SOFT_RST_CR &= ~SYSREG_I2C1_SOFTRESET_MASK;
    }
    
    /* Update Serial address of the device */
    this_i2c->ser_address = (uint_fast8_t)ser_address << 1u;

    this_i2c->hw_reg_bit->CTRL_ENS1 = 0x01u; /* set enable bit */
    this_i2c->hw_reg_bit->CTRL_CR2 = (uint32_t)((clock_speed >> 2u) & 0x01u);
    this_i2c->hw_reg_bit->CTRL_CR1 = (uint32_t)((clock_speed >> 1u) & 0x01u);
    this_i2c->hw_reg_bit->CTRL_CR0 = (uint32_t)(clock_speed & 0x01u);
    this_i2c->hw_reg->ADDR = (uint8_t)this_i2c->ser_address;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_write()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_write
(
    mss_i2c_instance_t * this_i2c,
    uint8_t serial_addr,
    const uint8_t * write_buffer,
    uint16_t write_size,
    uint8_t options
)
{
    uint32_t primask;
    volatile uint8_t stat_ctrl;

    ASSERT((this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1));

    primask = disable_interrupts();

    /* Update the transaction only when there is no transaction going on I2C */
    if( this_i2c->transaction == NO_TRANSACTION)
    {
      this_i2c->transaction = MASTER_WRITE_TRANSACTION;
    }

    /* Update the Pending transaction information so that transaction can restarted */
    this_i2c->pending_transaction = MASTER_WRITE_TRANSACTION ;

    /* Update target address */
    this_i2c->target_addr = (uint_fast8_t)serial_addr << 1u;

    this_i2c->dir = WRITE_DIR;

    this_i2c->master_tx_buffer = write_buffer;
    this_i2c->master_tx_size = write_size;
    this_i2c->master_tx_idx = 0u;

    /* Set I2C status in progress */
    this_i2c->master_status = MSS_I2C_IN_PROGRESS;
    this_i2c->options = options;

    if(MSS_I2C_IN_PROGRESS == this_i2c->slave_status)
    {
        this_i2c->is_transaction_pending = 1u;
    }
    else
    {
        this_i2c->hw_reg_bit->CTRL_STA = 0x01u;
    }

    /*
     * Clear interrupts if required (depends on repeated starts).
     * Since the Bus is on hold, only then prior status needs to
     * be cleared.
     */
    if ( MSS_I2C_HOLD_BUS == this_i2c->bus_status )
    {
        this_i2c->hw_reg_bit->CTRL_SI = 0u;
        stat_ctrl = this_i2c->hw_reg->STATUS;
        stat_ctrl = stat_ctrl;  /* Avoids Lint warning */
        NVIC_ClearPendingIRQ( this_i2c->irqn );
    }

    /* Enable the interrupt. ( Re-enable) */
    NVIC_EnableIRQ( this_i2c->irqn );

    restore_interrupts( primask );
}

/*------------------------------------------------------------------------------
 * MSS_I2C_read()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_read
(
    mss_i2c_instance_t * this_i2c,
    uint8_t serial_addr,
    uint8_t * read_buffer,
    uint16_t read_size,
    uint8_t options
)
{
    uint32_t primask;
    volatile uint8_t stat_ctrl;

    ASSERT((this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1));

    primask = disable_interrupts();
    
    /* Update the transaction only when there is no transaction going on I2C */
    if( this_i2c->transaction == NO_TRANSACTION)
    {
      this_i2c->transaction = MASTER_READ_TRANSACTION;
    }

    /* Update the Pending transaction information so that transaction can restarted */
    this_i2c->pending_transaction = MASTER_READ_TRANSACTION ;

    /* Update target address */
    this_i2c->target_addr = (uint_fast8_t)serial_addr << 1u;

    this_i2c->dir = READ_DIR;

    this_i2c->master_rx_buffer = read_buffer;
    this_i2c->master_rx_size = read_size;
    this_i2c->master_rx_idx = 0u;
    
    /* Set I2C status in progress */
    this_i2c->master_status = MSS_I2C_IN_PROGRESS;
    this_i2c->options = options;

    if(MSS_I2C_IN_PROGRESS == this_i2c->slave_status)
    {
        this_i2c->is_transaction_pending = 1u;
    }
    else
    {
        this_i2c->hw_reg_bit->CTRL_STA = 0x01u;
    }

    /*
     * Clear interrupts if required (depends on repeated starts).
     * Since the Bus is on hold, only then prior status needs to
     * be cleared.
     */
    if ( MSS_I2C_HOLD_BUS == this_i2c->bus_status )
    {
        this_i2c->hw_reg_bit->CTRL_SI = 0u;
        stat_ctrl = this_i2c->hw_reg->STATUS;
        stat_ctrl = stat_ctrl;  /* Avoids Lint warning */
        NVIC_ClearPendingIRQ( this_i2c->irqn );
    }
    
    /* Enable the interrupt. ( Re-enable) */
    NVIC_EnableIRQ( this_i2c->irqn );
    restore_interrupts( primask );
}

/*------------------------------------------------------------------------------
 * MSS_I2C_write_read()
 * See "mss_i2c.h" for details of how to use this function.
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
)
{
    ASSERT((this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1));
    ASSERT(offset_size > 0u);
    ASSERT(addr_offset != (const uint8_t *)0);
    ASSERT(read_size > 0u);
    ASSERT(read_buffer != (uint8_t *)0);

    if((read_size > 0u) && (offset_size > 0u))
    {
        uint32_t primask;
        volatile uint8_t stat_ctrl;

        primask = disable_interrupts();

        /* Update the transaction only when there is no transaction going on I2C */
        if( this_i2c->transaction == NO_TRANSACTION)
        {
            this_i2c->transaction = MASTER_RANDOM_READ_TRANSACTION;
        }

        /* Update the Pending transaction information so that transaction can restarted */
        this_i2c->pending_transaction = MASTER_RANDOM_READ_TRANSACTION ;

        /* Update target address */
        this_i2c->target_addr = (uint_fast8_t)serial_addr << 1u;

        this_i2c->dir = WRITE_DIR;
        this_i2c->master_tx_buffer = addr_offset;
        this_i2c->master_tx_size = offset_size;
        this_i2c->master_tx_idx = 0u;

        this_i2c->master_rx_buffer = read_buffer;
        this_i2c->master_rx_size = read_size;
        this_i2c->master_rx_idx = 0u;

        /* Set I2C status in progress */
        this_i2c->master_status = MSS_I2C_IN_PROGRESS;
        this_i2c->options = options;

        if(MSS_I2C_IN_PROGRESS == this_i2c->slave_status)
        {
            this_i2c->is_transaction_pending = 1u;
        }
        else
        {
            this_i2c->hw_reg_bit->CTRL_STA = 0x01u;
        }

        /*
         * Clear interrupts if required (depends on repeated starts).
         * Since the Bus is on hold, only then prior status needs to
         * be cleared.
         */
        if ( MSS_I2C_HOLD_BUS == this_i2c->bus_status )
        {
            this_i2c->hw_reg_bit->CTRL_SI = 0u;
            stat_ctrl = this_i2c->hw_reg->STATUS;
            stat_ctrl = stat_ctrl;  /* Avoids Lint warning */
            NVIC_ClearPendingIRQ( this_i2c->irqn );
        }

        /* Enable the interrupt. ( Re-enable) */
        NVIC_EnableIRQ( this_i2c->irqn );

        restore_interrupts( primask );
    }
}

/*------------------------------------------------------------------------------
 * MSS_I2C_get_status()
 * See "mss_i2c.h" for details of how to use this function.
 */
mss_i2c_status_t MSS_I2C_get_status
(
    mss_i2c_instance_t * this_i2c
)
{
    mss_i2c_status_t i2c_status ;

    ASSERT( (this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1) );

    i2c_status = this_i2c->master_status ;
    return i2c_status;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_wait_complete()
 * See "mss_i2c.h" for details of how to use this function.
 */
mss_i2c_status_t MSS_I2C_wait_complete
(
    mss_i2c_instance_t * this_i2c,
    uint32_t timeout_ms
)
{
    mss_i2c_status_t i2c_status;
    
    ASSERT( (this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1) );
    
    this_i2c->master_timeout_ms = timeout_ms;

    /* Run the loop until state returns I2C_FAILED  or I2C_SUCESS*/
    do {
        i2c_status = this_i2c->master_status;
    } while(MSS_I2C_IN_PROGRESS == i2c_status);
    
    return i2c_status;
}

/*------------------------------------------------------------------------------
 * I2C_system_tick()
 * See "core_i2c.h" for details of how to use this function.
 */
void MSS_I2C_system_tick
(
    mss_i2c_instance_t * this_i2c,
    uint32_t ms_since_last_tick
)
{
    if(this_i2c->master_timeout_ms != MSS_I2C_NO_TIMEOUT)
    {
        if(this_i2c->master_timeout_ms > ms_since_last_tick)
        {
            this_i2c->master_timeout_ms -= ms_since_last_tick;
        }
        else
        {
            /*
             * Mark current transaction as having timed out.
             */
            this_i2c->master_status = MSS_I2C_TIMED_OUT;
            this_i2c->transaction = NO_TRANSACTION;
            this_i2c->is_transaction_pending = 0;
            
            /*
             * Make sure we do not incorrectly signal a timeout for subsequent
             * transactions.
             */
            this_i2c->master_timeout_ms = MSS_I2C_NO_TIMEOUT;
        }
    }
}

/*------------------------------------------------------------------------------
 * MSS_I2C_set_slave_tx_buffer()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_set_slave_tx_buffer
(
    mss_i2c_instance_t * this_i2c,
    const uint8_t * tx_buffer,
    uint16_t tx_size
)
{
    uint32_t primask;
    
    ASSERT( (this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1) );

    primask = disable_interrupts();
    
    this_i2c->slave_tx_buffer = tx_buffer;
    this_i2c->slave_tx_size = tx_size;
    this_i2c->slave_tx_idx = 0u;
    
    restore_interrupts( primask );
}

/*------------------------------------------------------------------------------
 * MSS_I2C_set_slave_rx_buffer()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_set_slave_rx_buffer
(
    mss_i2c_instance_t * this_i2c,
    uint8_t * rx_buffer,
    uint16_t rx_size
)
{
    uint32_t primask;

    ASSERT( (this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1) );

    primask = disable_interrupts();
    
    this_i2c->slave_rx_buffer = rx_buffer;
    this_i2c->slave_rx_size = rx_size;
    this_i2c->slave_rx_idx = 0u;

    restore_interrupts( primask );
}

/*------------------------------------------------------------------------------
 * MSS_I2C_set_slave_mem_offset_length()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_set_slave_mem_offset_length
(
    mss_i2c_instance_t * this_i2c,
    uint8_t offset_length
)
{
    ASSERT( (this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1) );
    ASSERT(offset_length <= MAX_OFFSET_LENGTH);
    
    if(offset_length > MAX_OFFSET_LENGTH)
    {
        this_i2c->slave_mem_offset_length = MAX_OFFSET_LENGTH;
    }
    else
    {
        this_i2c->slave_mem_offset_length = offset_length;
    }
}

/*------------------------------------------------------------------------------
 * MSS_I2C_register_write_handler()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_register_write_handler
(
    mss_i2c_instance_t * this_i2c,
    mss_i2c_slave_wr_handler_t handler
)
{
    ASSERT( (this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1) );

    this_i2c->slave_write_handler = handler;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_enable_slave()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_enable_slave
(
    mss_i2c_instance_t * this_i2c
)
{
    ASSERT( (this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1) );

    /* Set the assert acknowledge bit. */
    this_i2c->hw_reg_bit->CTRL_AA = 0x01u;
    
    /* Enable slave */
    this_i2c->is_slave_enabled = 1u;

    /* Enable Interrupt */
    NVIC_EnableIRQ( this_i2c->irqn );
}

/*------------------------------------------------------------------------------
 * MSS_I2C_disable_slave()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_disable_slave
(
    mss_i2c_instance_t * this_i2c
)
{
    ASSERT( (this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1) );

    /* Reset the assert acknowledge bit. */
    this_i2c->hw_reg_bit->CTRL_AA = 0x00u;

    /* Disable slave */
    this_i2c->is_slave_enabled = 0u;
}

/*------------------------------------------------------------------------------
 * 
 */
static void enable_slave_if_required
(
    mss_i2c_instance_t * this_i2c
)
{
    if( this_i2c->is_slave_enabled )
    {
        this_i2c->hw_reg_bit->CTRL_AA = 1u;
    }
}

/*------------------------------------------------------------------------------
 * MSS I2C interrupt service routine.
 *------------------------------------------------------------------------------
 * Parameters:
 * 
 * mss_i2c_instance_t * this_i2c:
 * Pointer to the mss_i2c_instance_t data structure holding all data related to
 * the MSS I2C instance that generated the interrupt.
 */
static void mss_i2c_isr
(
    mss_i2c_instance_t * this_i2c
)
{
    volatile uint8_t status;
    uint8_t data;
    uint8_t hold_bus;
    uint8_t clear_irq = 1u;
    ASSERT( (this_i2c == &g_mss_i2c0) || (this_i2c == &g_mss_i2c1) );

    status = this_i2c->hw_reg->STATUS;

    switch( status )
    {
        /************** MASTER TRANSMITTER / RECEIVER *******************/
      
        case ST_START: /* start has been xmt'd */
        case ST_RESTART: /* repeated start has been xmt'd */
            this_i2c->hw_reg_bit->CTRL_STA = 0u;
            this_i2c->hw_reg->DATA = (uint8_t)this_i2c->target_addr;
            this_i2c->hw_reg_bit->DATA_DIR = this_i2c->dir;
            if ( this_i2c->dir == WRITE_DIR )
            {
                this_i2c->master_tx_idx = 0u;
            }
            else if ( this_i2c->dir == READ_DIR)
            {
                this_i2c->master_rx_idx = 0u;
            }
            
            /*
             * Clear the pending transaction. This condition will be true if the slave 
             * has acquired the bus to carry out pending master transaction which 
             * it had received during its slave transmission or reception mode. 
             */
            if(this_i2c->is_transaction_pending)
            {
                this_i2c->is_transaction_pending = 0u;
            }

            /*
             * Make sure to update proper transaction after master START
             * or RESTART
             */
            if(this_i2c->transaction != this_i2c->pending_transaction)
            {
                this_i2c->transaction = this_i2c->pending_transaction;
            }
            break;
            
        case ST_LOST_ARB:
            /* Set start bit.  Let's keep trying!  Don't give up! */
            this_i2c->hw_reg_bit->CTRL_STA = 0x01u;
            break;
            
        /******************* MASTER TRANSMITTER *************************/
        case ST_SLAW_NACK:
            /* SLA+W has been transmitted; not ACK has been received - let's stop. */
            this_i2c->hw_reg_bit->CTRL_STO = 0x01u;
            this_i2c->master_status = MSS_I2C_FAILED;
            this_i2c->transaction = NO_TRANSACTION;
            enable_slave_if_required(this_i2c);
            break;
            
        case ST_SLAW_ACK:
        case ST_TX_DATA_ACK:
            /* data byte has been xmt'd with ACK, time to send stop bit or repeated start. */
            if (this_i2c->master_tx_idx < this_i2c->master_tx_size)
            {    
                this_i2c->hw_reg->DATA = this_i2c->master_tx_buffer[this_i2c->master_tx_idx++];
            }
            else if ( this_i2c->transaction == MASTER_RANDOM_READ_TRANSACTION )
            {
                /* We are finished sending the address offset part of a random read transaction.
                 * It is is time to send a restart in order to change direction. */
                 this_i2c->dir = READ_DIR;
                 this_i2c->hw_reg_bit->CTRL_STA = 0x01u;
            }
            else /* done sending. let's stop */
            {
                /*
                 * Set the transaction back to NO_TRANSACTION to allow user to do further
                 * transaction
                 */
                this_i2c->transaction = NO_TRANSACTION;
                hold_bus = this_i2c->options & MSS_I2C_HOLD_BUS;

                /* Store the information of current I2C bus status in the bus_status*/
                this_i2c->bus_status  = hold_bus;
                if ( hold_bus == 0u )
                { 
                    this_i2c->hw_reg_bit->CTRL_STO = 0x01u; /*xmt stop condition */
                    enable_slave_if_required(this_i2c);
                }
                else
                {
                    NVIC_DisableIRQ( this_i2c->irqn );
                    clear_irq = 0u;
                }
                this_i2c->master_status = MSS_I2C_SUCCESS;
            }
            break;

          case ST_TX_DATA_NACK:
            /* data byte SENT, ACK to be received
             * In fact, this means we've received a NACK (This may not be 
             * obvious, but if we've rec'd an ACK then we would be in state 
             * 0x28!) hence, let's send a stop bit
             */
            this_i2c->hw_reg_bit->CTRL_STO = 0x01u;
            this_i2c->master_status = MSS_I2C_FAILED;

            /*
             * Set the transaction back to NO_TRANSACTION to allow user to do further
             * transaction
             */
            this_i2c->transaction = NO_TRANSACTION;
            enable_slave_if_required(this_i2c);

            break;
              
      /********************* MASTER (or slave?) RECEIVER *************************/
      
      /* STATUS codes 08H, 10H, 38H are all covered in MTX mode */
        case ST_SLAR_ACK: /* SLA+R tx'ed. */
            /* Let's make sure we ACK the first data byte received (set AA bit in CTRL) unless
             * the next byte is the last byte of the read transaction.
             */
            if(this_i2c->master_rx_size > 1u)
            {
                this_i2c->hw_reg_bit->CTRL_AA = 0x01u;
            }
            else if(1u == this_i2c->master_rx_size)
            {
                this_i2c->hw_reg_bit->CTRL_AA = 0u;
            }
            else /* this_i2c->master_rx_size == 0u */
            {
                this_i2c->hw_reg_bit->CTRL_AA = 0x01u;
                this_i2c->hw_reg_bit->CTRL_STO = 0x01u;
                this_i2c->master_status = MSS_I2C_SUCCESS;
                this_i2c->transaction = NO_TRANSACTION;
            }
            break;
            
        case ST_SLAR_NACK: /* SLA+R tx'ed; let's release the bus (send a stop condition) */
            this_i2c->hw_reg_bit->CTRL_STO = 0x01u;
            this_i2c->master_status = MSS_I2C_FAILED;

            /*
             * Set the transaction back to NO_TRANSACTION to allow user to do further
             * transaction
             */
            this_i2c->transaction = NO_TRANSACTION;
            enable_slave_if_required(this_i2c);
            break;
          
        case ST_RX_DATA_ACK: /* Data byte received, ACK returned */
            /* First, get the data */
            this_i2c->master_rx_buffer[this_i2c->master_rx_idx++] = this_i2c->hw_reg->DATA;

            if( this_i2c->master_rx_idx >= (this_i2c->master_rx_size - 1u))
            {
                /* If we're at the second last byte, let's set AA to 0 so
                 * we return a NACK at the last byte. */
                this_i2c->hw_reg_bit->CTRL_AA = 0u;
            }
            break;
            
        case ST_RX_DATA_NACK: /* Data byte received, NACK returned */
            /* Get the data, then send a stop condition */
            this_i2c->master_rx_buffer[this_i2c->master_rx_idx] = this_i2c->hw_reg->DATA;
          
            hold_bus = this_i2c->options &  MSS_I2C_HOLD_BUS; 

            /* Store the information of current I2C bus status in the bus_status*/
            this_i2c->bus_status  = hold_bus;
            if ( hold_bus == 0u )
            { 
                this_i2c->hw_reg_bit->CTRL_STO = 0x01u;  /*xmt stop condition */

                /* Bus is released, now we can start listening to bus, if it is slave */
                   enable_slave_if_required(this_i2c);
            }
            else
            {
                NVIC_DisableIRQ( this_i2c->irqn );
                clear_irq = 0u;
            }

            /*
             * Set the transaction back to NO_TRANSACTION to allow user to do further
             * transaction
             */
            this_i2c->transaction = NO_TRANSACTION;
            this_i2c->master_status = MSS_I2C_SUCCESS;
            break;
        
        /******************** SLAVE RECEIVER **************************/
        case ST_GCA_NACK: /* NACK after, GCA addressing */
        case ST_SLA_NACK: /* Re-enable AA (assert ack) bit for future transmissions */
            this_i2c->hw_reg_bit->CTRL_AA = 0x01u;
            
            this_i2c->transaction = NO_TRANSACTION;
            this_i2c->slave_status = MSS_I2C_SUCCESS;
            
            /* Check if transaction was pending. If yes, set the START bit */
            if(this_i2c->is_transaction_pending)
            {
                this_i2c->hw_reg_bit->CTRL_STA = 1u ;
            }
            break;
            
        case ST_GCA: /* General call address received, ACK returned */
        case ST_SLAVE_SLAW: /* SLA+W received, ACK returned */

            this_i2c->transaction = WRITE_SLAVE_TRANSACTION;
            this_i2c->slave_rx_idx = 0u;
            this_i2c->random_read_addr = 0u;

             /* If Start Bit is set, clear it, but store that information since it is because of
              * pending transaction
              */
            if(this_i2c->hw_reg_bit->CTRL_STA)
            {
                this_i2c->hw_reg_bit->CTRL_STA = 0u ;
                this_i2c->is_transaction_pending = 1u;
            }
            this_i2c->slave_status = MSS_I2C_IN_PROGRESS;

#ifndef INCLUDE_SLA_IN_RX_PAYLOAD
            /* Only break from this case if the slave address must NOT be included at the
             * beginning of the received write data. */
            break;
#endif            
        case ST_GCA_ACK: /* DATA received; ACK sent after GCA */
        case ST_RDATA: /* DATA received; must clear DATA register */
            if(this_i2c->slave_rx_buffer != (uint8_t *)0)
            {
                data = this_i2c->hw_reg->DATA;
                this_i2c->slave_rx_buffer[this_i2c->slave_rx_idx++] = data;
                
#ifdef INCLUDE_SLA_IN_RX_PAYLOAD
                if(status != ST_SLAVE_SLAW)
                {
                    /* Ignore the slave address byte in the random read address
                       computation in the case where INCLUDE_SLA_IN_RX_PAYLOAD
                       is defined. */
#endif
                    this_i2c->random_read_addr = (this_i2c->random_read_addr << 8) + data;
#ifdef INCLUDE_SLA_IN_RX_PAYLOAD
                }
#endif
            }
            
            if (this_i2c->slave_rx_idx >= this_i2c->slave_rx_size)
            {
                this_i2c->hw_reg_bit->CTRL_AA = 0u;   /* send a NACK when done (next reception) */
            }
            break;
            
        case ST_RSTOP:
            /* STOP or repeated START occurred. */
            /* We cannot be sure if the transaction has actually completed as
             * this hardware state reports that either a STOP or repeated START
             * condition has occurred. We assume that this is a repeated START
             * if the transaction was a write from the master to this point.*/
            if ( this_i2c->transaction == WRITE_SLAVE_TRANSACTION )
            {
                if ( this_i2c->slave_rx_idx == this_i2c->slave_mem_offset_length )
                {
                    this_i2c->slave_tx_idx = this_i2c->random_read_addr;
                }
                {
                    /* Call the slave's write transaction handler if it exists. */
                    if ( this_i2c->slave_write_handler != 0u )
                    {
                        mss_i2c_slave_handler_ret_t h_ret;
                        h_ret = this_i2c->slave_write_handler( this_i2c, this_i2c->slave_rx_buffer, (uint16_t)this_i2c->slave_rx_idx );
                        if ( MSS_I2C_REENABLE_SLAVE_RX == h_ret )
                        {
                            this_i2c->hw_reg_bit->CTRL_AA = 0x01u;
                        }
                        else
                        {
                            this_i2c->hw_reg_bit->CTRL_AA = 0u;
                        }
                    }
                    else
                    {
                        /* Re-enable address acknowledge in case we were ready to nack the next received byte. */
                        this_i2c->hw_reg_bit->CTRL_AA = 0x01u;
                    }
                }
            }

            /* Mark any previous master write transaction as complete. */
            this_i2c->slave_status = MSS_I2C_SUCCESS;

            /* Check if transaction was pending. If yes, set the START bit */
            if(this_i2c->is_transaction_pending)
            {
                this_i2c->hw_reg_bit->CTRL_STA = 1u ;
            }

            /*
             * Set the transaction back to NO_TRANSACTION to allow user to do further
             * transaction
             */
            this_i2c->transaction = NO_TRANSACTION;
            break;
            
        case ST_SLV_RST: /* SMBUS ONLY: timeout state. must clear interrupt */
        case ST_SLV_LA: /* Arbitr. lost (SLA rec'd) */
        case ST_GCA_LA: /* Arbitr. lost (GCA rec'd) */
            /* do nothing */
            this_i2c->slave_status = MSS_I2C_FAILED;
            break;
            
        /****************** SLAVE TRANSMITTER **************************/
        case ST_SLAVE_SLAR_ACK: /* SLA+R received, ACK returned */
        case ST_SLARW_LA:   /* Arbitration lost, SLA+R received, ACK returned */
        case ST_RACK: /* Data tx'ed, ACK received */
            if ( status == ST_SLAVE_SLAR_ACK )
            {
                this_i2c->transaction = READ_SLAVE_TRANSACTION;
                this_i2c->random_read_addr = 0u;

                this_i2c->slave_status = MSS_I2C_IN_PROGRESS;

                /* If Start Bit is set, clear it, but store that information since it is because of
                 * pending transaction
                 */
                if(this_i2c->hw_reg_bit->CTRL_STA)
                {
                    this_i2c->hw_reg_bit->CTRL_STA = 0u ;
                    this_i2c->is_transaction_pending = 1u;
                }
            }
            if (this_i2c->slave_tx_idx >= this_i2c->slave_tx_size)
            {
                /* Ensure 0xFF is returned to the master when the slave specifies
                 * an empty transmit buffer. */
                this_i2c->hw_reg->DATA = 0xFFu;
            }
            else
            {
                /* Load the data the data byte to be sent to the master. */
                this_i2c->hw_reg->DATA = this_i2c->slave_tx_buffer[this_i2c->slave_tx_idx++];
            }
            /* Determine if this is the last data byte to send to the master. */
            if (this_i2c->slave_tx_idx >= this_i2c->slave_tx_size) /* last byte? */
            {
                this_i2c->hw_reg_bit->CTRL_AA = 0u;
                /* Next read transaction will result in slave's transmit buffer
                 * being sent from the first byte. */
                this_i2c->slave_tx_idx = 0u;
            }
            break;
        
        case ST_SLAVE_RNACK:    /* Data byte has been transmitted; not-ACK has been received. */
        case ST_FINAL: /* Last Data byte tx'ed, ACK received */
            /* We assume that the transaction will be stopped by the master.
             * Reset slave_tx_idx so that a subsequent read will result in the slave's
             * transmit buffer being sent from the first byte. */
            this_i2c->slave_tx_idx = 0u;
            this_i2c->hw_reg_bit->CTRL_AA = 0x01u;

            /*  Mark previous state as complete */
            this_i2c->slave_status = MSS_I2C_SUCCESS;

            /* Check if transaction was pending. If yes, set the START bit */
            if(this_i2c->is_transaction_pending)
            {
                this_i2c->hw_reg_bit->CTRL_STA = 1u ;
            }

            /*
             * Set the transaction back to NO_TRANSACTION to allow user to do further
             * transaction
             */
            this_i2c->transaction = NO_TRANSACTION;
            break;

        /* Master Reset has been activated Wait 35 ms for interrupt to be set, clear interrupt and proceed to 0xF8 state. */
        case ST_RESET_ACTIVATED:
        default:
            /* Some undefined state has encountered. Clear Start bit to make sure, next good transaction happen */
            this_i2c->hw_reg_bit->CTRL_STA = 0x00u;
            this_i2c->master_status = MSS_I2C_FAILED;
            /*
             * Set the transaction back to NO_TRANSACTION to allow user to do further
             * transaction
             */
            this_i2c->transaction = NO_TRANSACTION;
            
            /* 
             * We need this because we end up in an unexpected state when we are
             * the taget of read quick command. We need to reset the slave_tx_idx
             * here to ensure the slave tx buffer is transmited from the beginning
             * in subsequent transactions.
             */
            this_i2c->slave_tx_idx = 0u;

            break;
    }
    
    if ( clear_irq )
    {
        /* clear interrupt. */
        this_i2c->hw_reg_bit->CTRL_SI = 0u;
    }
    
    /* Read the status register to ensure the last I2C registers write took place
     * in a system built around a bus making use of posted writes. */
    status = this_i2c->hw_reg->STATUS;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_smbus_init()
 * See "mss_i2c.h" for details of how to use this function.
 */
#define INIT_AND_ENABLE_SMBUS   0x54u

void MSS_I2C_smbus_init
(
    mss_i2c_instance_t * this_i2c,
    uint8_t frequency
)
{
    /* Enable SMBUS */
    this_i2c->hw_reg->SMBUS = INIT_AND_ENABLE_SMBUS;

    /* Set the frequency */
    this_i2c->hw_reg->FREQ = frequency ;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_enable_smbus_irq()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_enable_smbus_irq
(
    mss_i2c_instance_t * this_i2c,
    uint8_t  irq_type
)
{
    this_i2c->hw_reg->SMBUS |=  (uint8_t)(irq_type & SMBUS_IRQ_MASK);
}

/*------------------------------------------------------------------------------
 * MSS_I2C_disable_smbus_irq()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_disable_smbus_irq
(
    mss_i2c_instance_t * this_i2c,
    uint8_t  irq_type
)
{
    uint8_t smbus_value ;
    smbus_value = (uint8_t)(irq_type & SMBUS_IRQ_MASK) ;
    smbus_value = (uint8_t)(~smbus_value );
    this_i2c->hw_reg->SMBUS &= smbus_value;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_suspend_slave()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_suspend_smbus_slave
(
    mss_i2c_instance_t * this_i2c
)
{
    this_i2c->hw_reg->SMBUS |= SMBUS_SUSPEND_MASK;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_reset_smbus()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_reset_smbus
(
    mss_i2c_instance_t * this_i2c
)
{
    this_i2c->hw_reg->SMBUS |= SMBUS_RESET_MASK;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_set_smbus_alert()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_set_smbus_alert
(
    mss_i2c_instance_t * this_i2c
)
{
    this_i2c->hw_reg->SMBUS |= SMBUS_SMBUS_ALERT_MASK;
}

/*------------------------------------------------------------------------------
 * MSS_i2C_set_gca()
 * See "mss_i2c.h" for details of how to use this function.
 */

void MSS_I2C_set_gca
(
    mss_i2c_instance_t * this_i2c
)
{
    /* accept GC addressing. */
    this_i2c->hw_reg_bit->ADDR_GC = 0x01u;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_clear_gca()
 * See "mss_i2c.h" for details of how to use this function.
 */
void MSS_I2C_clear_gca
(
    mss_i2c_instance_t * this_i2c
)
{
    /* accept GC addressing. */
    this_i2c->hw_reg_bit->ADDR_GC = 0u;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_set_user_data()
 * See "i2c.h" for details of how to use this function.
 */
void MSS_I2C_set_user_data
(
    mss_i2c_instance_t * this_i2c,
    void * p_user_data
)
{
    this_i2c->p_user_data = p_user_data ;
}

/*------------------------------------------------------------------------------
 * MSS_I2C_get_user_data()
 * See "i2c.h" for details of how to use this function.
 */
void * MSS_I2C_get_user_data
(
    mss_i2c_instance_t * this_i2c
)
{
    return( this_i2c->p_user_data);
}
/*------------------------------------------------------------------------------
 *
 */
static uint32_t disable_interrupts( void )
{
    uint32_t primask;
    primask = __get_PRIMASK();
    __set_PRIMASK(1u);
    return primask;
}

/*------------------------------------------------------------------------------
 *
 */
static void restore_interrupts( uint32_t primask )
{
    __set_PRIMASK( primask );
}

/*------------------------------------------------------------------------------
 * I2C0_IRQHandler interrupt handler
 */
#if defined(__GNUC__)
__attribute__((__interrupt__)) void I2C0_IRQHandler( void )
#else
void I2C0_IRQHandler( void )
#endif
{
    mss_i2c_isr( &g_mss_i2c0 );
}

/*------------------------------------------------------------------------------
 * I2C1_IRQHandler interrupt handler
 */
#if defined(__GNUC__)
__attribute__((__interrupt__)) void I2C1_IRQHandler( void )
#else
void I2C1_IRQHandler( void )
#endif
{
    mss_i2c_isr( &g_mss_i2c1 );
}

#ifdef __cplusplus
}
#endif
