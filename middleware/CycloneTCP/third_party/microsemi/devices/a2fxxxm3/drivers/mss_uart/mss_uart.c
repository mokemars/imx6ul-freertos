/*******************************************************************************
 * (c) Copyright 2007 Actel Corporation.  All rights reserved.
 *
 * SmartFusion Microcontroller Subsystem UART bare metal software driver
 * implementation.
 *
 * SVN $Revision: 4178 $
 * SVN $Date: 2012-03-05 14:10:01 +0530 (Mon, 05 Mar 2012) $
 */
#include "mss_uart.h"
#include "../../CMSIS/mss_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * defines
 */
#define TX_COMPLETE         0U

#define TX_FIFO_SIZE        16U

#define IIRF_MASK           0x0FU

#define NULL_HANDLER        ( (mss_uart_irq_handler_t) 0)

#define INVALID_INTERRUPT   0U

#define INVALID_IRQ_HANDLER ( (mss_uart_irq_handler_t) 0 )

#define MSS_UART_DATA_READY ( (uint8_t) 0x01 )

/*******************************************************************************
 * Possible values for Interrupt Identification Register Field.
 */
#define IIRF_MODEM_STATUS   0x00U
#define IIRF_THRE           0x02U
#define IIRF_RX_DATA        0x04U
#define IIRF_RX_LINE_STATUS 0x06U
#define IIRF_DATA_TIMEOUT   0x0CU

/*******************************************************************************
 * Definitions for FIFO Control Register Fields.
 */
#define FCR_CLEAR_RX_FIFO_MASK       0x02U
#define FCR_CLEAR_TX_FIFO_MASK       0x04U
#define FCR_ENABLE_TXRDY_RXRDY_MASK  0x08U
#define FCR_TRIG_LEVEL_MASK          0xC0U

/*******************************************************************************
 * Receiver error status mask.
 */
#define STATUS_ERROR_MASK    ( MSS_UART_OVERUN_ERROR | MSS_UART_PARITY_ERROR | \
                               MSS_UART_FRAMING_ERROR  | MSS_UART_BREAK_ERROR | \
                               MSS_UART_FIFO_ERROR)

/*******************************************************************************
 * Cortex-M3 interrupt handler functions implemented as part of the MSS UART
 * driver.
 */
#if defined(__GNUC__)
__attribute__((__interrupt__)) void UART0_IRQHandler( void );
#else
void UART0_IRQHandler( void );
#endif

#if defined(__GNUC__)
__attribute__((__interrupt__)) void UART1_IRQHandler( void );
#else
void UART1_IRQHandler( void );
#endif

/*******************************************************************************
 * Local functions.
 */
static void MSS_UART_isr( mss_uart_instance_t * this_uart );
static void default_tx_handler( mss_uart_instance_t * this_uart );
/*******************************************************************************
 *
 */
mss_uart_instance_t g_mss_uart0;
mss_uart_instance_t g_mss_uart1;

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_init
(
    mss_uart_instance_t* this_uart,
    uint32_t baud_rate,
    uint8_t line_config
)
{
    uint16_t baud_value = 1U;
    uint32_t pclk_freq = 0U;
    uint32_t baud_value_l;

    /* The driver expects g_mss_uart0 and g_mss_uart1 to be the only
     * mss_uart_instance_t instances used to identify UART0 and UART1. */
    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );

    /* baud_rate should be greater than 0 */
    ASSERT( 0U < baud_rate );

    /* Force the value of the CMSIS global variables holding the various system
     * clock frequencies to be updated. */
    SystemCoreClockUpdate();

    if ( this_uart == &g_mss_uart0 )
    {
        this_uart->hw_reg = UART0;
        this_uart->hw_reg_bit = UART0_BITBAND;
        this_uart->irqn = UART0_IRQn;

        pclk_freq = g_FrequencyPCLK0;

        /* reset UART0 */
        SYSREG->SOFT_RST_CR |= SYSREG_UART0_SOFTRESET_MASK;
        /* Clear any previously pended UART0 interrupt */
        NVIC_ClearPendingIRQ( UART0_IRQn );
        /* Take UART0 out of reset. */
        SYSREG->SOFT_RST_CR &= ~SYSREG_UART0_SOFTRESET_MASK;
    }
    else
    {
        this_uart->hw_reg = UART1;
        this_uart->hw_reg_bit = UART1_BITBAND;
        this_uart->irqn = UART1_IRQn;

        pclk_freq = g_FrequencyPCLK1;

        /* Reset UART1 */
        SYSREG->SOFT_RST_CR |= SYSREG_UART1_SOFTRESET_MASK;
        /* Clear any previously pended UART1 interrupt */
        NVIC_ClearPendingIRQ( UART1_IRQn );
        /* Take UART1 out of reset. */
        SYSREG->SOFT_RST_CR &= ~SYSREG_UART1_SOFTRESET_MASK;
    }

    /* disable interrupts */
    this_uart->hw_reg->IER = 0U;

    /*
     * Compute baud value based on requested baud rate and PCLK frequency.
     * The baud value is computed using the following equation:
     *      baud_value_l = PCLK_Frequency / (baud_rate * 16)
     * The baud value is rounded up or down depending on what would be the remainder
     * of the divide by 16 operation.
     */
    if( 0U < baud_rate )
    {
        baud_value_l = (uint32_t)(pclk_freq / baud_rate);
        if( baud_value_l & 0x00000008U )
        {
            /* remainder above 0.5 */
            baud_value_l = (baud_value_l >> 4U) + 1U;        
        }
        else
        {
            /* remainder below 0.5 */
            baud_value_l = (baud_value_l >> 4U);
        }
    
        /* Check if the baud rate is supported by the PCLK */
        ASSERT( baud_value_l <= UINT16_MAX);
        if( baud_value_l <= (uint32_t)UINT16_MAX)
        {
            baud_value = (uint16_t)baud_value_l;
        }
    }
    
    /* set divisor latch */
    this_uart->hw_reg_bit->LCR_DLAB = (uint32_t)1;

    /* msb of baud value */
    this_uart->hw_reg->DMR = (uint8_t)(baud_value >> 8);
    /* lsb of baud value */
    this_uart->hw_reg->DLR = (uint8_t)baud_value;

    /* reset divisor latch */
    this_uart->hw_reg_bit->LCR_DLAB = (uint32_t)0;

    /* set the line control register (bit length, stop bits, parity) */
    this_uart->hw_reg->LCR = line_config;

    /* FIFO configuration */
    /* RX FIFO length = 1 byte, clear receiver FIFO, 
       clear transmitter FIFO, enable RXRDYN and TXRDYN pins. */
    this_uart->hw_reg->FCR = (uint8_t)MSS_UART_FIFO_SINGLE_BYTE | 
                              FCR_CLEAR_RX_FIFO_MASK | FCR_CLEAR_TX_FIFO_MASK | 
                              FCR_ENABLE_TXRDY_RXRDY_MASK;
    /* disable loopback */
    this_uart->hw_reg_bit->MCR_LOOP = (uint32_t)0;

    /* Instance setup */
    this_uart->tx_buff_size = TX_COMPLETE;
    this_uart->tx_buffer = (const uint8_t *)0;
    this_uart->tx_idx = 0U;

    /* Default handlers for MSS UART interrupts */
    this_uart->rx_handler       = NULL_HANDLER;
    this_uart->tx_handler       = default_tx_handler;
    this_uart->linests_handler  = NULL_HANDLER;
    this_uart->modemsts_handler = NULL_HANDLER;

    /* Initialize the sticky status */
    this_uart->status = 0U;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_polled_tx
(
    mss_uart_instance_t * this_uart,
    const uint8_t * pbuff,
    uint32_t tx_size
)
{
    uint32_t char_idx = 0U;
    uint32_t size_sent;
    uint8_t status;

    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );
    ASSERT( pbuff != ( (uint8_t *)0 ) );
    ASSERT( tx_size > 0U );

    if( ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) ) &&
          ( pbuff != ( (uint8_t *)0 ) ) && ( tx_size > 0U ) )
    {
         /* Remain in this loop until the entire input buffer
          * has been transferred to the UART.
          */
        do {
            /* Read the Line Status Register and update the sticky record */
            status = this_uart->hw_reg->LSR;
            this_uart->status |= status;

            /* Check if TX FIFO is empty. */
            if( status & MSS_UART_THRE )
            {
                uint32_t fill_size = TX_FIFO_SIZE;

                /* Calculate the number of bytes to transmit. */
                if ( tx_size < TX_FIFO_SIZE )
                {
                    fill_size = tx_size;
                }

                /* Fill the TX FIFO with the calculated the number of bytes. */
                for ( size_sent = 0U; size_sent < fill_size; ++size_sent )
                {
                    /* Send next character in the buffer. */
                    this_uart->hw_reg->THR = pbuff[char_idx++];
                }

                /* Calculate the number of untransmitted bytes remaining. */
                tx_size -= size_sent;
            }
        }while( tx_size );
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_polled_tx_string
(
    mss_uart_instance_t * this_uart,
    const uint8_t * p_sz_string
)
{
    uint32_t char_idx = 0U;
    uint32_t fill_size;
    uint_fast8_t data_byte;
    uint8_t status;

    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );
    ASSERT( p_sz_string != ( (uint8_t *)0 ) );

    if( ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) ) &&
          ( p_sz_string != ( (uint8_t *)0 ) ) )
    {
        /* Get the first data byte from the input buffer */
        data_byte = (uint_fast8_t)p_sz_string[char_idx];

        /* First check for the NULL terminator byte.
         * Then remain in this loop until the entire string in the input buffer
         * has been transferred to the UART.
         */
        while ( 0U != data_byte )
        {
            /* Wait until TX FIFO is empty. */
            do {
                status = this_uart->hw_reg->LSR;
                this_uart->status |= status;
            } while ( !( status & MSS_UART_THRE ) );

            /* Send bytes from the input buffer until the TX FIFO is full
             * or we reach the NULL terminator byte.
             */
            fill_size = 0U;
            while ( (0U != data_byte) && (fill_size < TX_FIFO_SIZE) )
            {
                /* Send the data byte */
                this_uart->hw_reg->THR = data_byte;
                ++fill_size;
                char_idx++;
                /* Get the next data byte from the input buffer */
                data_byte = (uint_fast8_t)p_sz_string[char_idx];
            }
        }
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_irq_tx
(
    mss_uart_instance_t * this_uart,
    const uint8_t * pbuff,
    uint32_t tx_size
)
{
    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );
    ASSERT( pbuff != ((uint8_t *)0) );
    ASSERT( tx_size > 0U );

    if ( ( tx_size > 0U ) && ( pbuff != ((uint8_t *)0) ) &&
       ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) ) )
    {
        /*Initialise the transmit info for the UART instance with the arguments.*/
        this_uart->tx_buffer = pbuff;
        this_uart->tx_buff_size = tx_size;
        this_uart->tx_idx = (uint16_t)0;

        /* Clear any previously pended interrupts */
        NVIC_ClearPendingIRQ( this_uart->irqn );

        /* assign default handler for data transfer */
        this_uart->tx_handler = default_tx_handler;

        /* enables TX interrupt */
        this_uart->hw_reg_bit->IER_ETBEI = (uint32_t)1;

        /* Enable UART instance interrupt in Cortex-M3 NVIC. */
        NVIC_EnableIRQ( this_uart->irqn );
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
int8_t
MSS_UART_tx_complete
(
    mss_uart_instance_t * this_uart
)
{
    int8_t ret_value = 0;
    uint8_t status = 0U;

    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );

    if ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) )
    {
        /* Read the Line Status Register and update the sticky record. */
        status = this_uart->hw_reg->LSR;
        this_uart->status |= status;

        if ( ( TX_COMPLETE == this_uart->tx_buff_size ) &&
             ( status & MSS_UART_TEMT ) )
        {
            ret_value = (int8_t)1;
        }
    }
    return ret_value;
}


/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
size_t
MSS_UART_get_rx
(
    mss_uart_instance_t * this_uart,
    uint8_t * rx_buff,
    size_t buff_size
)
{
    size_t rx_size = 0U;
    uint8_t status = 0U;

    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );
    ASSERT( rx_buff != ((uint8_t *)0) );
    ASSERT( buff_size > 0U );

    if( ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) ) &&
          ( rx_buff != ((uint8_t *)0) ) && ( buff_size > 0U ) )
    {
        status = this_uart->hw_reg->LSR;
        this_uart->status |= status;

        while (( (status & MSS_UART_DATA_READY) != 0U) &&
               ( rx_size < buff_size ) )
        {
            rx_buff[rx_size] = this_uart->hw_reg->RBR;
            ++rx_size;
            status = this_uart->hw_reg->LSR;
            this_uart->status |= status;
        }
    }
    return rx_size;
}
/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_enable_irq
(
    mss_uart_instance_t * this_uart,
    uint8_t irq_mask
)
{
    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );

    if( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) )
    {
        /* Clear any previously pended interrupts */
        NVIC_ClearPendingIRQ( this_uart->irqn );

        /* irq_mask encoding: 1- enable
         * bit 0 - Receive Data Available Interrupt
         * bit 1 - Transmitter Holding  Register Empty Interrupt
         * bit 2 - Receiver Line Status Interrupt
         * bit 3 - Modem Status Interrupt
         */
        this_uart->hw_reg->IER |= irq_mask;

        /* Enable UART instance interrupt in Cortex-M3 NVIC. */
        NVIC_EnableIRQ( this_uart->irqn );
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_disable_irq
(
    mss_uart_instance_t * this_uart,
    uint8_t irq_mask
)
{
    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );

    if( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) )
    {
        /* irq_mask encoding: 1 - disable
         * bit 0 - Receive Data Available Interrupt
         * bit 1 - Transmitter Holding  Register Empty Interrupt
         * bit 2 - Receiver Line Status Interrupt
         * bit 3 - Modem Status Interrupt
         */
        this_uart->hw_reg->IER &= ( (uint8_t)~irq_mask );

        /* Clear any previously pended interrupts */
        NVIC_ClearPendingIRQ( this_uart->irqn );

        if( irq_mask == IIRF_MASK )
        {
            /* Disable UART instance interrupt in Cortex-M3 NVIC. */
            NVIC_DisableIRQ( this_uart->irqn );

        }
    }
}

/***************************************************************************//**
 * Interrupt service routine triggered by any MSS UART interrupt. This routine
 * will call the handler function appropriate to the interrupt from the
 * handlers previously registered with the driver through calls to the
 * MSS_UART_set_*_handler() functions, or it will call the default_tx_handler()
 * function in response to transmit interrupts if MSS_UART_irq_tx() is used to
 * transmit data.
 */
static void
MSS_UART_isr
(
    mss_uart_instance_t * this_uart
)
{
    uint8_t iirf;

    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );

    if ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) )
    {
        iirf = this_uart->hw_reg->IIR & IIRF_MASK;

        switch ( iirf )
        {
            case IIRF_MODEM_STATUS:  /* Modem status interrupt */
            {
                ASSERT( NULL_HANDLER != this_uart->modemsts_handler );
                if( NULL_HANDLER != this_uart->modemsts_handler )
                {
                   (*(this_uart->modemsts_handler))( this_uart );
                }
            }
            break;

            case IIRF_THRE: /* Transmitter Holding Register Empty */
            {
                ASSERT( NULL_HANDLER != this_uart->tx_handler );
                if ( NULL_HANDLER != this_uart->tx_handler )
                {
                    (*(this_uart->tx_handler))( this_uart );
                }
            }
            break;

            case IIRF_RX_DATA:      /* Received Data Available */
            case IIRF_DATA_TIMEOUT: /* Received Data Timed-out */
            {
                ASSERT( NULL_HANDLER != this_uart->rx_handler );
                if ( NULL_HANDLER != this_uart->rx_handler )
                {
                    (*(this_uart->rx_handler))( this_uart );
                }
            }
            break;

            case IIRF_RX_LINE_STATUS:  /* Line Status Interrupt */
            {
                ASSERT( NULL_HANDLER != this_uart->linests_handler );
                if ( NULL_HANDLER != this_uart->linests_handler )
                {
                   (*(this_uart->linests_handler))( this_uart );
                }
            }
            break;

            default:
            {
                ASSERT( INVALID_INTERRUPT );
            }
            break;
        }
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
static void
default_tx_handler
(
    mss_uart_instance_t * this_uart
)
{
    uint8_t status;

    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );
    ASSERT( ( (uint8_t *)0 ) != this_uart->tx_buffer );
    ASSERT( 0U < this_uart->tx_buff_size );

    if( ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) ) &&
        ( ( (uint8_t *)0 ) != this_uart->tx_buffer ) &&
        ( 0U < this_uart->tx_buff_size ) )
    {
        /* Read the Line Status Register and update the sticky record. */
        status = this_uart->hw_reg->LSR;
        this_uart->status |= status;

        /*
         * This function should only be called as a result of a THRE interrupt.
         * Verify that this is true before proceeding to transmit data.
         */
        if ( status & MSS_UART_THRE )
        {
            uint32_t i;
            uint32_t fill_size = TX_FIFO_SIZE;
            uint32_t tx_remain = this_uart->tx_buff_size - this_uart->tx_idx;

            /* Calculate the number of bytes to transmit. */
            if ( tx_remain < TX_FIFO_SIZE )
            {
                fill_size = tx_remain;
            }

            /* Fill the TX FIFO with the calculated the number of bytes. */
            for ( i = 0U; i < fill_size; ++i )
            {
                /* Send next character in the buffer. */
                this_uart->hw_reg->THR = this_uart->tx_buffer[this_uart->tx_idx];
                ++this_uart->tx_idx;
            }
        }

        /* Flag Tx as complete if all data has been pushed into the Tx FIFO. */
        if ( this_uart->tx_idx == this_uart->tx_buff_size )
        {
            this_uart->tx_buff_size = TX_COMPLETE;
            /* disables TX interrupt */
            this_uart->hw_reg_bit->IER_ETBEI = 0U;
        }
    }
}
/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_rx_handler
(
    mss_uart_instance_t *       this_uart,
    mss_uart_irq_handler_t      handler,
    mss_uart_rx_trig_level_t    trigger_level
)
{
    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );
    ASSERT( handler != INVALID_IRQ_HANDLER );
    ASSERT( trigger_level < MSS_UART_FIFO_INVALID_TRIG_LEVEL );

    if( ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) ) &&
        ( handler != INVALID_IRQ_HANDLER) &&
        ( trigger_level < MSS_UART_FIFO_INVALID_TRIG_LEVEL) )
    {
        this_uart->rx_handler = handler;

        /* Set the receive interrupt trigger level. */
        /* Clear RX FIFO; Enable TXRDY and RXRDY for PDMA */
        this_uart->hw_reg->FCR = (uint8_t)(FCR_TRIG_LEVEL_MASK & (uint8_t)trigger_level) |
                              FCR_CLEAR_RX_FIFO_MASK | FCR_ENABLE_TXRDY_RXRDY_MASK;

        /* Clear any previously pended interrupts */
        NVIC_ClearPendingIRQ( this_uart->irqn );

        /* Enable receive interrupt. */
        this_uart->hw_reg_bit->IER_ERBFI = 1U;

        /* Enable UART instance interrupt in Cortex-M3 NVIC. */
        NVIC_EnableIRQ( this_uart->irqn );
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_loopback
(
    mss_uart_instance_t *   this_uart,
    mss_uart_loopback_t     loopback
)
{
    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );

    if( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) )
    {
        if ( loopback == MSS_UART_LOOPBACK_OFF )
        {
            this_uart->hw_reg_bit->MCR_LOOP = 0U;
        }
        else
        {
            this_uart->hw_reg_bit->MCR_LOOP = 1U;
        }
    }
}

/***************************************************************************//**
 * UART0 interrupt service routine.
 * UART0_IRQHandler is included within the Cortex-M3 vector table as part of the
 * Fusion 2 CMSIS.
 */
#if defined(__GNUC__)
__attribute__((__interrupt__)) void UART0_IRQHandler( void )
#else
void UART0_IRQHandler( void )
#endif
{
    MSS_UART_isr( &g_mss_uart0 );
    NVIC_ClearPendingIRQ( UART0_IRQn );
}

/***************************************************************************//**
 * UART1 interrupt service routine.
 * UART2_IRQHandler is included within the Cortex-M3 vector table as part of the
 * Fusion 2 CMSIS.
 */
#if defined(__GNUC__)
__attribute__((__interrupt__)) void UART1_IRQHandler( void )
#else
void UART1_IRQHandler( void )
#endif
{
    MSS_UART_isr( &g_mss_uart1 );
    NVIC_ClearPendingIRQ( UART1_IRQn );
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_rxstatus_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
)
{
    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );
    ASSERT( handler != INVALID_IRQ_HANDLER);

    if( ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) ) &&
        ( handler != INVALID_IRQ_HANDLER) )
    {
        this_uart->linests_handler = handler;

        /* Clear any previously pended interrupts */
        NVIC_ClearPendingIRQ( this_uart->irqn );

        /* Enable receiver line status interrupt. */
        this_uart->hw_reg_bit->IER_ELSI = 1U;

        /* Enable UART instance interrupt in Cortex-M3 NVIC. */
        NVIC_EnableIRQ( this_uart->irqn );
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_tx_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
)
{
    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );
    ASSERT( handler != INVALID_IRQ_HANDLER);

    if( ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) ) &&
        ( handler != INVALID_IRQ_HANDLER) )
    {
        this_uart->tx_handler = handler;

        /* Make TX buffer info invalid */
        this_uart->tx_buffer = (const uint8_t *)0;
        this_uart->tx_buff_size = 0U;

        /* Clear any previously pended interrupts */
        NVIC_ClearPendingIRQ( this_uart->irqn );

        /* Enable transmitter holding register Empty interrupt. */
        this_uart->hw_reg_bit->IER_ETBEI = 1U;

        /* Enable UART instance interrupt in Cortex-M3 NVIC. */
        NVIC_EnableIRQ( this_uart->irqn );
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
void
MSS_UART_set_modemstatus_handler
(
    mss_uart_instance_t * this_uart,
    mss_uart_irq_handler_t handler
)
{
    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );
    ASSERT( handler != INVALID_IRQ_HANDLER );

    if( ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) ) &&
        ( handler != INVALID_IRQ_HANDLER) )
    {
        this_uart->modemsts_handler = handler;

        /* Clear any previously pended interrupts */
        NVIC_ClearPendingIRQ( this_uart->irqn );

        /* Enable modem status interrupt. */
        this_uart->hw_reg_bit->IER_EDSSI = 1U;

        /* Enable UART instance interrupt in Cortex-M3 NVIC. */
        NVIC_EnableIRQ( this_uart->irqn );
    }
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
size_t
MSS_UART_fill_tx_fifo
(
    mss_uart_instance_t * this_uart,
    const uint8_t * tx_buffer,
    size_t tx_size
)
{
    uint8_t status = 0U;
    size_t size_sent = 0U;

    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );
    ASSERT( tx_buffer != ( (uint8_t *)0 ) );
    ASSERT( tx_size > 0 );

    /* Fill the UART's Tx FIFO until the FIFO is full or the complete input
     * buffer has been written. */
    if( ( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1 ) ) &&
        (tx_buffer != ( (uint8_t *)0 ))   &&
        (tx_size > 0u) )
    {
        status = this_uart->hw_reg->LSR;
        this_uart->status |= status;

        if( status & MSS_UART_THRE )
        {
            uint32_t fill_size = TX_FIFO_SIZE;

            if ( tx_size < TX_FIFO_SIZE )
            {
                fill_size = tx_size;
            }
            /* Fill up FIFO */
            for ( size_sent = 0U; size_sent < fill_size; ++size_sent )
            {

                /* Send next character in the buffer. */
                this_uart->hw_reg->THR = tx_buffer[size_sent];
            }
        }
    }
    return size_sent;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
uint8_t
MSS_UART_get_rx_status
(
    mss_uart_instance_t * this_uart
)
{
    uint8_t status = MSS_UART_INVALID_PARAM;

    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );

    if( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) )
    {
        /*
         * Extract UART receive error status.
         * Bit 1 - Overflow error status
         * Bit 2 - Parity error status
         * Bit 3 - Frame error status
         * Bit 4 - Break interrupt indicator
         * Bit 7 - FIFO data error status
         */
        this_uart->status |= (this_uart->hw_reg->LSR);
        status = (this_uart->status & STATUS_ERROR_MASK );
        /* Clear the sticky status after reading */
        this_uart->status = 0U;
    }
    return status;
}

/***************************************************************************//**
 * See mss_uart.h for details of how to use this function.
 */
uint8_t
MSS_UART_get_modem_status
(
    mss_uart_instance_t * this_uart
)
{
    uint8_t status = MSS_UART_INVALID_PARAM;

    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );

    if( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) )
    {
        /*
         * Extract UART modem status and place in lower bits of "status".
         * Bit 0 - Delta Clear to Send Indicator
         * Bit 1 - Delta Clear to Receive Indicator
         * Bit 2 - Trailing edge of Ring Indicator detector
         * Bit 3 - Delta Data Carrier Detect indicator
         * Bit 4 - Clear To Send
         * Bit 5 - Data Set Ready
         * Bit 6 - Ring Indicator
         * Bit 7 - Data Carrier Detect
         */
        status = this_uart->hw_reg->MSR;
    }
    return status;
}

/***************************************************************************//**
 * MSS_UART_get_tx_status.
 * See mss_uart.h for details of how to use this function.
 */
uint8_t
MSS_UART_get_tx_status
(
    mss_uart_instance_t * this_uart
)
{
    uint8_t status = MSS_UART_TX_BUSY;

    ASSERT( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) );

    if( (this_uart == &g_mss_uart0) || (this_uart == &g_mss_uart1) )
    {
        /* Read the Line Status Register and update the sticky record. */
        status = this_uart->hw_reg->LSR;
        this_uart->status |= status;
        /*
         * Extract the transmit status bits from the UART's Line Status Register.
         * Bit 5 - Transmitter Holding Register/FIFO Empty (THRE) status. (If = 1, TX FIFO is empty)
         * Bit 6 - Transmitter Empty (TEMT) status. (If = 1, both TX FIFO and shift register are empty)
         */
        status &= ( MSS_UART_THRE | MSS_UART_TEMT );
    }
    return status;
}

#ifdef __cplusplus
}
#endif
