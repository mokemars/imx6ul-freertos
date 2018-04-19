/*******************************************************************************
 * (c) Copyright 2012 Microsemi SoC Products Group.  All rights reserved.
 *
 * SmartFusion2 COMBLK access functions.
 *
 * SVN $Revision: 4556 $
 * SVN $Date: 2012-08-23 15:22:22 +0100 (Thu, 23 Aug 2012) $
 */
 
#include "mss_comblk.h"
#include "../../CMSIS/a2fxxxm3.h"
#include "mss_assert.h"

/*==============================================================================
 *
 */
/*------------------------------------------------------------------------------
 * Control register bit masks.
 */
#define CR_FLUSHOUT_MASK    0x01u
#define CR_FLUSHIN_MASK     0x02u
#define CR_SIZETX_MASK      0x04u
#define CR_SIZERX_MASK      0x08u
#define CR_ENABLE_MASK      0x10u
#define CR_LOOPBACK_MASK    0x20u

/*------------------------------------------------------------------------------
 * Status and interrupt enable registers bit masks.
 */
#define TXTOKAY_MASK    0x01u
#define RCVOKAY_MASK    0x02u
#define OVERFLOW_MASK   0x04u
#define UNDERFLOW_MASK  0x08u
#define FLUSHRCVD       0x20u

/*------------------------------------------------------------------------------
 * DATA8 register bit masks.
 */
#define DATA8_COMMAND_MASK  0x8000u

/*------------------------------------------------------------------------------
 * COMBLK driver states.
 */
#define COMBLK_IDLE             0
#define COMBLK_TX_CMD           1
#define COMBLK_TX_DATA          2
#define COMBLK_WAIT_RESPONSE    3
#define COMBLK_RX_RESPONSE      4

/*==============================================================================
 * Local functions.
 */
static void abort_current_cmd(void);
static void send_cmd_opcode(uint8_t opcode);
static uint32_t fill_tx_fifo(const uint8_t * p_cmd, uint32_t cmd_size);
static void handle_tx_okay_irq(void);
static void handle_rx_okay_irq(void);
static void complete_request(uint16_t response_length);

/*==============================================================================
 * Global variables:
 */
static uint8_t g_comblk_cmd_opcode = 0;
static const uint8_t * g_comblk_p_cmd = 0;
static uint16_t g_comblk_cmd_size = 0;
static const uint8_t * g_comblk_p_data = 0;
static uint32_t g_comblk_data_size = 0;
static uint8_t * g_comblk_p_response = 0;
static uint16_t g_comblk_response_size = 0;
static uint16_t g_comblk_response_idx = 0;
static comblk_completion_handler_t g_comblk_completion_handler = 0;

static uint8_t g_request_in_progress = 0;

static uint8_t g_comblk_state = COMBLK_IDLE;

/*==============================================================================
 *
 */
void MSS_COMBLK_init(void)
{
    uint32_t flush_in_progress;
    
    SYSREG->SOFT_RST_CR |= 0x00008000u;
    /*
     * Disable and clear previous interrupts.
     */
    NVIC_DisableIRQ(ComBlk_IRQn);
    COMBLK->INT_ENABLE = 0;
    NVIC_ClearPendingIRQ(ComBlk_IRQn);
    
    SYSREG->SOFT_RST_CR &= ~0x00008000u;
    
    /*
     * Initialialize COMBLK driver state variables:
     */
    g_request_in_progress = 0;
    g_comblk_cmd_opcode = 0;
    g_comblk_p_cmd = 0;
    g_comblk_cmd_size = 0;
    g_comblk_p_data = 0;
    g_comblk_data_size = 0;
    g_comblk_p_response = 0;
    g_comblk_response_size = 0;
    g_comblk_response_idx = 0;
    g_comblk_completion_handler = 0;
    
    g_comblk_state = COMBLK_IDLE;
    
    COMBLK->CONTROL |= CR_ENABLE_MASK;
    COMBLK->CONTROL &= ~CR_LOOPBACK_MASK;
    
    /*
     * Flush the FIFOs
     */
    COMBLK->CONTROL |= CR_FLUSHOUT_MASK;
    do {
        flush_in_progress = COMBLK->CONTROL & CR_FLUSHOUT_MASK;
    } while(flush_in_progress);
}

/*==============================================================================
 *
 */
void MSS_COMBLK_send_cmd_with_ptr
(
    uint8_t cmd_opcode,
    uint32_t cmd_params_ptr,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
)
{
    uint32_t tx_okay;
    
    /*--------------------------------------------------------------------------
     * Disable and clear previous interrupts.
     */
    NVIC_DisableIRQ(ComBlk_IRQn);
    COMBLK->INT_ENABLE = 0;
    NVIC_ClearPendingIRQ(ComBlk_IRQn);
    
    /*--------------------------------------------------------------------------
     * Abort current command if any.
     */
    abort_current_cmd();
    
    /*--------------------------------------------------------------------------
     * Initialialize COMBLK driver state variables:
     */
    g_request_in_progress = 1;
    g_comblk_cmd_opcode = cmd_opcode;
    g_comblk_p_cmd = 0;
    g_comblk_cmd_size = 0;
    g_comblk_p_data = 0;
    g_comblk_data_size = 0;
    g_comblk_p_response = p_response;
    g_comblk_response_size = response_size;
    g_comblk_response_idx = 0;
    g_comblk_completion_handler = completion_handler;
    
    /*--------------------------------------------------------------------------
     * Send command opcode as a single byte write to the Tx FIFO.
     */
    send_cmd_opcode(g_comblk_cmd_opcode);
    
    /*--------------------------------------------------------------------------
     * Send the command parameters pointer to the Tx FIFO as a single 4 bytes
     * write to the Tx FIFO.
     */
    COMBLK->CONTROL |= CR_SIZETX_MASK;
    
    /* Wait for space to become available in Tx FIFO. */
    do {
        tx_okay = COMBLK->STATUS & TXTOKAY_MASK;
    } while(!tx_okay);
    
    /* Send command opcode. */
    COMBLK->DATA32 = cmd_params_ptr;
    
    COMBLK->CONTROL &= ~CR_SIZETX_MASK;
    
    g_comblk_state = COMBLK_WAIT_RESPONSE;
    
    /*--------------------------------------------------------------------------
     * Enable interrupt.
     */
    COMBLK->INT_ENABLE |= RCVOKAY_MASK;
    NVIC_EnableIRQ(ComBlk_IRQn);
}

/*==============================================================================
 *
 */
void MSS_COMBLK_send_cmd
(
    const uint8_t * p_cmd,
    uint16_t cmd_size,
    const uint8_t * p_data,
    uint32_t data_size,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
)
{
    uint16_t size_sent;
    
    ASSERT(cmd_size > 0);
    
    /*
     * Disable and clear previous interrupts.
     */
    NVIC_DisableIRQ(ComBlk_IRQn);
    COMBLK->INT_ENABLE = 0;
    NVIC_ClearPendingIRQ(ComBlk_IRQn);
    
    /*
     * Abort current command if any.
     */
    abort_current_cmd();
    
    /*
     * Initialialize COMBLK driver state variables:
     */
    g_request_in_progress = 1;
    g_comblk_cmd_opcode = p_cmd[0];
    g_comblk_p_cmd = p_cmd;
    g_comblk_cmd_size = cmd_size;
    g_comblk_p_data = p_data;
    g_comblk_data_size = data_size;
    g_comblk_p_response = p_response;
    g_comblk_response_size = response_size;
    g_comblk_response_idx = 0;
    g_comblk_completion_handler = completion_handler;
    
    /*
     * Fill FIFO with command.
     */
    send_cmd_opcode(g_comblk_cmd_opcode);
    size_sent = fill_tx_fifo(p_cmd, cmd_size - 1);
    ++size_sent;
    if(size_sent < cmd_size)
    {
        g_comblk_cmd_size = g_comblk_cmd_size - size_sent;
        g_comblk_p_cmd = &g_comblk_p_cmd[size_sent];
        
        g_comblk_state = COMBLK_TX_CMD;
    }
    else
    {
        g_comblk_cmd_size = 0;
        if(g_comblk_data_size > 0)
        {
            g_comblk_state = COMBLK_TX_DATA;
        }
        else
        {
            g_comblk_state = COMBLK_WAIT_RESPONSE;
        }
    }
    
    /*
     * Enable interrupt.
     */
    COMBLK->INT_ENABLE |= (TXTOKAY_MASK | RCVOKAY_MASK);
    NVIC_EnableIRQ(ComBlk_IRQn);
}

/*==============================================================================
 * COMBLK interrupt handler.
 */
void ComBlk_IRQHandler(void)
{
    uint8_t status;
    uint8_t tx_okay;
    
    status = COMBLK->STATUS;
    
    /* Mask off interrupt that are not enabled.*/
    status &= COMBLK->INT_ENABLE;
    
    tx_okay = status & TXTOKAY_MASK;
    if(tx_okay)
    {
        handle_tx_okay_irq();
    }
    else
    {
        uint8_t rcv_okay;
        rcv_okay = status & RCVOKAY_MASK;
        if(rcv_okay)
        {
            handle_rx_okay_irq();
        }
    }
}

/*==============================================================================
 *
 */
void handle_tx_okay_irq(void)
{
    switch(g_comblk_state)
    {
        /*----------------------------------------------------------------------
         * The TX_OKAY interrupt should only be enabled for states COMBLK_TX_CMD
         * and COMBLK_TX_DATA. 
         */
        case COMBLK_TX_CMD:
            if(g_comblk_cmd_size > 0)
            {
                uint32_t size_sent;
                size_sent = fill_tx_fifo(g_comblk_p_cmd, g_comblk_cmd_size);
                if(size_sent < g_comblk_cmd_size)
                {
                    g_comblk_cmd_size = g_comblk_cmd_size - size_sent;
                    g_comblk_p_cmd = &g_comblk_p_cmd[size_sent];
                }
                else
                {
                    g_comblk_cmd_size = 0;
                    if(g_comblk_data_size > 0)
                    {
                        g_comblk_state = COMBLK_TX_DATA;
                    }
                    else
                    {
                        g_comblk_state = COMBLK_WAIT_RESPONSE;
                    }
                }
            }
            else
            {
                /*
                 * This is an invalid situation indicating a bug in the driver
                 * or corrupted memory.
                 */
                ASSERT(0);
                abort_current_cmd();
            }
            break;
            
        case COMBLK_TX_DATA:
            if(g_comblk_data_size > 0)
            {
                uint32_t size_sent;
                size_sent = fill_tx_fifo(g_comblk_p_data, g_comblk_data_size);
                if(size_sent < g_comblk_data_size)
                {
                    g_comblk_data_size = g_comblk_data_size - size_sent;
                    g_comblk_p_data = &g_comblk_p_data[size_sent];
                }
                else
                {
                    COMBLK->INT_ENABLE &= ~TXTOKAY_MASK;
                    g_comblk_state = COMBLK_WAIT_RESPONSE;
                }
            }
            else
            {
                /*
                 * This is an invalid situation indicating a bug in the driver
                 * or corrupted memory.
                 */
                ASSERT(0);
                abort_current_cmd();
            }
            break;
            
        /*----------------------------------------------------------------------
         * The TX_OKAY interrupt should NOT be enabled for states COMBLK_IDLE,
         * COMBLK_WAIT_RESPONSE and COMBLK_RX_RESPONSE.
         */
        case COMBLK_IDLE:
            /* Fall through */
        case COMBLK_WAIT_RESPONSE:
            /* Fall through */
        case COMBLK_RX_RESPONSE:
            /* Fall through */
        default:
            COMBLK->INT_ENABLE &= ~TXTOKAY_MASK;
            g_comblk_state = COMBLK_IDLE;
            break;
    }
}

/*==============================================================================
 *
 */
void handle_rx_okay_irq(void)
{
    uint16_t data16;
    uint16_t is_command;
    
    data16 = COMBLK->DATA8;
    is_command = data16 & DATA8_COMMAND_MASK;
            
    switch(g_comblk_state)
    {
        /*----------------------------------------------------------------------
         * The RCV_OKAY interrupt should only be enabled for states
         * COMBLK_WAIT_RESPONSE and COMBLK_RX_RESPONSE. 
         */
        case COMBLK_WAIT_RESPONSE:
            if(is_command)
            {
                uint8_t rxed_opcode;
                rxed_opcode = (uint8_t)data16;
                if(rxed_opcode == g_comblk_cmd_opcode)
                {
                    g_comblk_response_idx = 0;
                    g_comblk_p_response[g_comblk_response_idx] = rxed_opcode;
                    ++g_comblk_response_idx;
                    g_comblk_state = COMBLK_RX_RESPONSE;
                }
                else
                {
                    /*
                     * Unexpected command response.
                     * Call the command completion handler with the response
                     * size set to zero to indicate a problem.
                     */
                    complete_request(0u);
                }
            }
            break;
            
        case COMBLK_RX_RESPONSE:
            if(is_command)
            {
                /*
                 * Unexpected command response.
                 * Call the command completion handler with the response size
                 * set to the number of response bytes received so far.
                 * The caller may simply have set a response buffer which is
                 * not large enough for the full response.
                 */
                complete_request(0u);
                g_comblk_state = COMBLK_WAIT_RESPONSE;
            }
            else
            {
                if(g_comblk_response_idx < g_comblk_response_size)
                {
                    uint8_t rxed_data;
                    
                    rxed_data = (uint8_t)data16;
                    g_comblk_p_response[g_comblk_response_idx] = rxed_data;
                    ++g_comblk_response_idx;
                }
                
                if(g_comblk_response_idx == g_comblk_response_size)
                {
                    complete_request(g_comblk_response_idx);
                    g_comblk_state = COMBLK_IDLE;
                }
            }
            break;
            
        /*----------------------------------------------------------------------
         * The RCV_OKAY interrupt should NOT be enabled for states
         * COMBLK_IDLE, COMBLK_TX_CMD and COMBLK_TX_DATA. 
         */
        case COMBLK_IDLE:
            /* Fall through */
        case COMBLK_TX_CMD:
            /* Fall through */
        case COMBLK_TX_DATA:
            /* Fall through */
        default:
            COMBLK->INT_ENABLE &= ~RCVOKAY_MASK;
            g_comblk_state = COMBLK_IDLE;
            break;
    }
}

/*==============================================================================
 *
 */
void complete_request
(
    uint16_t response_length
)
{
    if(g_comblk_completion_handler != 0)
    {
        g_comblk_completion_handler(g_comblk_p_response, response_length);
        g_comblk_completion_handler = 0;
        g_request_in_progress = 0;
    }
}

/*==============================================================================
 *
 */
void abort_current_cmd(void)
{
    if(g_request_in_progress)
    {
        uint32_t flush_in_progress;
        
        /*
         * Call completion handler just in case we are in a multi threaded system
         * to avoid a task lockup.
         */
        complete_request(g_comblk_response_idx);
        
        /*
         * Flush the FIFOs
         */
        COMBLK->CONTROL |= CR_FLUSHOUT_MASK;
        do {
            flush_in_progress = COMBLK->CONTROL & CR_FLUSHOUT_MASK;
        } while(flush_in_progress);
    }
}

/*==============================================================================
 *
 */
void send_cmd_opcode
(
    uint8_t opcode
)
{
    uint32_t tx_okay;
    
    /* Set transmit FIFO to transfer bytes. */
    COMBLK->CONTROL &= ~CR_SIZETX_MASK;
    
    /* Wait for space to become available in Tx FIFO. */
    do {
        tx_okay = COMBLK->STATUS & TXTOKAY_MASK;
    } while(!tx_okay);
    
    /* Send command opcode. */
    COMBLK->FRAME_START8 = opcode;
}

/*==============================================================================
 *
 */
uint32_t fill_tx_fifo
(
    const uint8_t * p_cmd,
    uint32_t cmd_size
)
{
    uint32_t tx_okay;
    uint32_t size_sent;

    size_sent = 0;
    tx_okay = COMBLK->STATUS & TXTOKAY_MASK;
    while(tx_okay && (size_sent < cmd_size))
    {
        COMBLK->DATA8 = p_cmd[size_sent];
        ++size_sent;
        tx_okay = COMBLK->STATUS & TXTOKAY_MASK;
    }
    
    return size_sent;
}
 