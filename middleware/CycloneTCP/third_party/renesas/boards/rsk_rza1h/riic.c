/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : riic.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : RIIC driver support for all 4 available i2c channels
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Default  type definition header */
#include "r_typedefs.h"
/* I/O Register root header */
#include "iodefine.h"
/* Device Driver common header */
#include "dev_drv.h"
/* RIIC Driver Header */
#include "riic.h"
/* I2C RSK+RZA1H Eval-board header */
#include "riic_rskrza1h.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum riic_rx_mode
{
    RIIC_RX_MODE_NORMAL,
    RIIC_RX_MODE_ACK,
    RIIC_RX_MODE_LOW_HOLD,
    RIIC_RX_MODE_NACK,
    RIIC_RX_MODE_STOP
} riic_rx_mode_t;


/******************************************************************************
Private global variables and functions
******************************************************************************/
static void    riic_receive (uint32_t channel, riic_rx_mode_t mode,
		                                                     uint8_t * pdata);
static int32_t riic_transmit (uint32_t channel, uint8_t data);
static void    riic_transmit_start (uint32_t channel);
static void    riic_transmit_restart (uint32_t channel);
static void    riic_transmit_stop (uint32_t channel);
static void    riic_detect_stop (uint32_t channel);
static void    riic_clear_nack (volatile struct st_riic * priic);

volatile struct st_riic * priic_get_reg_addr (uint32_t channel);

/******************************************************************************
* Function Name: R_RIIC_Init
*              : Calls the relevant I2C channel initialisation function
*              : Uses the following interrupts:
*              : Receive data full interrupt
*              : Transmit data empty interrupt
*              : Transmission complete interrupt
* Arguments    : uint32_t channel: I2c channel selection
*              : uint32_t cks    : I2c Clock Selection
*              :                 : (CKS bit setting in RIICnMR1 register)
*              :                 : RIIC_CKS_DIVISION_1   : P0/1 clock
*              :                 : RIIC_CKS_DIVISION_2   : P0/2 clock
*              :                 : RIIC_CKS_DIVISION_4   : P0/4 clock
*              :                 : RIIC_CKS_DIVISION_8   : P0/8 clock
*              :                 : RIIC_CKS_DIVISION_16  : P0/16 clock
*              :                 : RIIC_CKS_DIVISION_32  : P0/32 clock
*              :                 : RIIC_CKS_DIVISION_64  : P0/64 clock
*              :                 : RIIC_CKS_DIVISION_128 : P0/128 clock
*              : uint32_t brl    : Lower byte value of the width of the SCL clock
*              : uint32_t brh    : High byte value of the width of the SCL clock
* Return Value : DEVDRV_SUCCESS  : Success of RIIC initialisation
*              : DEVDRV_ERROR    : Failure of RIIC initialisation
******************************************************************************/
int32_t R_RIIC_Init (uint32_t channel, uint32_t cks, uint32_t brl, uint32_t brh)
{
    /*  Argument check  */
    if ((((channel >= RIIC_CH_TOTAL) || (cks > 7)) || (brl > 31)) || (brh > 31))
    {
        return DEVDRV_ERROR;        /* Argument error */
    }

    /*  RIIC initialisation  */
    switch (channel)
    {
        case DEVDRV_CH_0:
            userdef_riic0_init(cks, brl, brh);
        break;
        case DEVDRV_CH_1:
            userdef_riic1_init(cks, brl, brh);
        break;
        case DEVDRV_CH_2:
            userdef_riic2_init(cks, brl, brh);
        break;
        case DEVDRV_CH_3:
            userdef_riic3_init(cks, brl, brh);
        break;
        default:

            /* Do not reach here based on the assumption */
            __asm__("nop");
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* Function Name: R_RIIC_SingleRead
* Description  : Reads 1 byte only of data from specified I2c channel
* Arguments    : uint32_t channel : I2c channel selection
*              : uint8_t * data   : Pointer to allocated storage (1 byte)
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_SingleRead (uint32_t channel, uint8_t * pdata)
{
    /* Ensure channel is valid */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    /* Read single byte */
    riic_receive(channel, RIIC_RX_MODE_NORMAL, pdata);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* End of Function R_RIIC_SingleRead
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_SingleWrite
* Description  : 
* Description  : Writes 1 byte only of data to specified I2c channel
* Arguments    : uint32_t channel : I2c channel selection
*              : uint8_t * data   : Pointer to allocated storage (1 byte)
*              : mode             : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
*              :                  : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_SingleWrite (uint32_t channel, uint8_t data, uint32_t mode)
{
    volatile struct st_riic * priic;
    int32_t ret;

    /* Ensure channel is valid */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    /* Write single byte */
    ret = riic_transmit(channel, data);

    if (DEVDRV_SUCCESS == ret)
    {
        switch (channel)
        {
            case DEVDRV_CH_0:
                userdef_riic0_wait_tx_end(mode);
            break;
            case DEVDRV_CH_1:
                userdef_riic1_wait_tx_end(mode);
            break;
            case DEVDRV_CH_2:
                userdef_riic2_wait_tx_end(mode);
            break;
            case DEVDRV_CH_3:
                userdef_riic3_wait_tx_end(mode);
            break;
            default:

                /* Can not be reached due to parameter checking above */
                __asm__("nop");
            break;
        }

        /* Check for transmission status */
        priic = priic_get_reg_addr(channel);
        if (0 == rza_io_reg_read_8(&(priic->RIICnSR2.UINT8[0]),
        		                    RIICn_RIICnSR2_NACKF_SHIFT,
        		                    RIICn_RIICnSR2_NACKF))
        {
        	/* ACK */
            ret = DEVDRV_SUCCESS;
        }
        else
        {
        	/* NACK */
            ret = DEVDRV_ERROR;
        }
    }

    return (ret);
}

/******************************************************************************
* End of Function R_RIIC_SingleWrite
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_WriteCond
* Description  : Send specified condition on selected channel
* Arguments    : uint32_t channel : I2c channel selection
*              : uint32_t mode    : Three condition types are available
*              :                  : RIIC_TX_MODE_START   : Start Condition
*              :                  : RIIC_TX_MODE_RESTART : Restart Condition
*              :                  : RIIC_TX_MODE_STOP    : Stop Condition
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_WriteCond (uint32_t channel, uint32_t mode)
{
    /* Argument check */
    if ((channel >= RIIC_CH_TOTAL) || (mode > RIIC_TX_MODE_STOP))
    {
    	/* Argument error */
        return DEVDRV_ERROR;
    }

    /* Mode check */
    switch (mode)
    {
        case RIIC_TX_MODE_START:
            riic_transmit_start(channel);
        break;
        case RIIC_TX_MODE_RESTART:
            riic_transmit_restart(channel);
        break;
        case RIIC_TX_MODE_STOP:
            riic_transmit_stop(channel);
        break;
        default:

            /* Do not reach here based on the assumption */
            __asm__("nop");
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* End of Function R_RIIC_WriteCond
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_Write
* Description  : Send specified condition on selected channel
* Arguments    : uint32_t channel : I2c channel selection
*              : uint8_t *buffer  : pointer to data for writing
*              : uint32_t byte    : number of bytes to transmit
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_Write (uint32_t channel, const uint8_t * pbuffer, uint32_t byte)
{
    volatile struct st_riic * priic;
    uint32_t offset;
    int32_t  ret;

    /* Ensure channel is valid (parameter checking) */
    if (channel >= RIIC_CH_TOTAL)
    {
    	/* Argument error */
        return DEVDRV_ERROR;
    }

    if (0 == byte)
    {
        return DEVDRV_SUCCESS;
    }

    ret = DEVDRV_SUCCESS;

    for (offset = 0; ((offset < byte) && (ret == DEVDRV_SUCCESS)); offset++)
    {
        /* Single byte transmission */
        ret = riic_transmit(channel, *(pbuffer + offset));

        if (DEVDRV_SUCCESS != ret)
        {
            /* Exit the loop */
            offset = byte;
        }
    }

    if (DEVDRV_SUCCESS == ret)
    {
        switch (channel)
        {
            case DEVDRV_CH_0:
                userdef_riic0_wait_tx_end(RIIC_TEND_WAIT_TRANSMIT);
                userdef_riic0_init_tx_end();
            break;
            case DEVDRV_CH_1:
                userdef_riic1_wait_tx_end(RIIC_TEND_WAIT_TRANSMIT);
                userdef_riic1_init_tx_end();
            break;
            case DEVDRV_CH_2:
                userdef_riic2_wait_tx_end(RIIC_TEND_WAIT_TRANSMIT);
                userdef_riic2_init_tx_end();
            break;
            case DEVDRV_CH_3:
                userdef_riic3_wait_tx_end(RIIC_TEND_WAIT_TRANSMIT);
                userdef_riic3_init_tx_end();
            break;
            default:

                /* Do not reach here based on the assumption */
                __asm__("nop");
            break;
        }

        /* Check for transmission status */
        priic = priic_get_reg_addr(channel);

        /* ACK */
        if (0 == rza_io_reg_read_8(&(priic->RIICnSR2.UINT8[0]),
        		                    RIICn_RIICnSR2_NACKF_SHIFT,
        		                    RIICn_RIICnSR2_NACKF))
        {
            ret = DEVDRV_SUCCESS;
        }

        /* NACK */
        else
        {
            ret = DEVDRV_ERROR;
        }
    }

    return (ret);
}

/******************************************************************************
* End of Function R_RIIC_Write
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_Read
* Description  : Send specified condition on selected channel
* Arguments    : uint32_t channel : I2c channel selection
*              : uint8_t *buffer  : pointer to allocated space for data
*              : uint32_t byte    : number of bytes to receive
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_Read (uint32_t channel, uint8_t * pbuffer, uint32_t byte)
{
    volatile struct st_riic * priic;
    uint32_t  cnt;
    uint8_t * pbuffer2;
    uint8_t dummy_buf;

    /* Ensure channel is valid (parameter checking) */
    if (channel >= RIIC_CH_TOTAL)
    {
    	/* Argument error */
        return DEVDRV_ERROR;
    }

    /* Ensure at least 1 byte is specified to receive */
    if (0 == byte)
    {
        return DEVDRV_SUCCESS;
    }

    pbuffer2 = pbuffer;

    /* 1 byte random read does not need a dummy read */
    if (1 == byte)
    {
        /* (In the case of only 1 byte) reception start */
    	riic_receive(channel, RIIC_RX_MODE_NACK, pbuffer2);

    	/* Last byte read */
    	riic_receive(channel, RIIC_RX_MODE_STOP, pbuffer2);
    }
    else
    {
    	/* Start reading data */
    	riic_receive(channel, RIIC_RX_MODE_ACK, pbuffer2);

		/* Reads data bytes */
		while (1 != byte)
		{
			riic_receive(channel, RIIC_RX_MODE_ACK, pbuffer2);
			pbuffer2++;
			byte--;
		}

		/* Data read of the last byte (-1) */
		riic_receive(channel, RIIC_RX_MODE_NACK, pbuffer2);

		/* Last byte read */
		riic_receive(channel, RIIC_RX_MODE_STOP, &dummy_buf);
    }

    priic = priic_get_reg_addr(channel);

    if((struct st_riic *) DEVDRV_ERROR != priic)
    {
        /* Process next Read */
        rza_io_reg_write_8(&(priic->RIICnMR3.UINT8[0]),
        		            0,
        		            RIICn_RIICnMR3_WAIT_SHIFT,
        		            RIICn_RIICnMR3_WAIT);

        /* Stop condition detection confirmation */
        riic_detect_stop(channel);
    }
    return DEVDRV_SUCCESS;
}

/******************************************************************************
* End of Function R_RIIC_Read
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_ReadDummy
* Description  : Perform dummy read on selected channel
* Arguments    : uint32_t channel : I2c channel selection
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_ReadDummy (uint32_t channel)
{
    uint8_t dummy_buf_8b;

    /* Argument check */
    if (channel >= RIIC_CH_TOTAL)
    {
    	/* Argument error */
        return DEVDRV_ERROR;
    }

    /* Perform Dummy byte read */
    riic_receive(channel, RIIC_RX_MODE_NORMAL, &dummy_buf_8b);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* End of Function R_RIIC_ReadDummy
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_DetectStop
* Description  : Detect Stop Condition on selected channel
* Arguments    : uint32_t channel : I2c channel selection
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_DetectStop (uint32_t channel)
{
    /* Argument check */
    if (channel >= RIIC_CH_TOTAL)
    {
    	/* Argument error */
        return DEVDRV_ERROR;
    }

    /* Perform detection */
    riic_detect_stop(channel);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* End of Function R_RIIC_DetectStop
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_ClearNack
* Description  : Clear NACK bit on selected channel
* Arguments    : uint32_t channel : I2C channel selection
* Return Value : DEVDRV_SUCCESS   : Success of RIIC operation
*              : DEVDRV_ERROR     : Failure of RIIC operation
******************************************************************************/
int32_t R_RIIC_ClearNack (uint32_t channel)
{
    volatile struct st_riic * priic;

    /*  Argument check  */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    priic = priic_get_reg_addr(channel);

    /* NACK clear flag */
    riic_clear_nack(priic);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* End of Function R_RIIC_ClearNack
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_RiInterrupt
* Description  : Receive data full interrupt notification for RIIC of channel
*              : specified in the channel argument.
*              : Receive data full by a user-defined function
*              : Userdef_RIICn_SetReceiveFull.
* Arguments    : uint32_t channel: RIIC channel (0 to 3)
* Return Value : DEVDRV_SUCCESS   : Function completes with no errors
*              : DEVDRV_ERROR     : Function fails to complete with errors
******************************************************************************/
int32_t R_RIIC_RiInterrupt (uint32_t channel)
{
    /*  Argument check  */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    /* Receive data full set of conditions */
    switch (channel)
    {
        case DEVDRV_CH_0:
            userdef_riic0_set_rx_full();
        break;
        case DEVDRV_CH_1:
            userdef_riic1_set_rx_full();
        break;
        case DEVDRV_CH_2:
            userdef_riic2_set_rx_full();
        break;
        case DEVDRV_CH_3:
            userdef_riic3_set_rx_full();
        break;
        default:

            /* Do not reach here based on the assumption */
            __asm__("nop");
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* End of Function R_RIIC_RiInterrupt
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_TiInterrupt
* Description  : Transmit data empty interrupt notification for RIIC of channel
*              : specified in the channel argument.
*              : Send data by user-defined function
*              : Userdef_RIICn_SetTransmitEmpty
* Arguments    : uint32_t channel: RIIC channel (0 to 3)
* Return Value : DEVDRV_SUCCESS   : Function completes with no errors
*              : DEVDRV_ERROR     : Function fails to complete with errors
******************************************************************************/
int32_t R_RIIC_TiInterrupt (uint32_t channel)
{
    /*  Argument check  */
    if (channel >= RIIC_CH_TOTAL)
    {
    	/* Argument error */
        return DEVDRV_ERROR;
    }

    /* Transmit-data-empty set of conditions */
    switch (channel)
    {
        case DEVDRV_CH_0:
            userdef_riic0_set_tx_empty();
        break;
        case DEVDRV_CH_1:
            userdef_riic1_set_tx_empty();
        break;
        case DEVDRV_CH_2:
            userdef_riic2_set_tx_empty();
        break;
        case DEVDRV_CH_3:
            userdef_riic3_set_tx_empty();
        break;
        default:

            /* Do not reach here based on the assumption */
        break;
    }

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* End of Function R_RIIC_TiInterrupt
******************************************************************************/

/******************************************************************************
* Function Name: R_RIIC_TeiInterrupt
* Description  : Transmission end interrupt notification for RIIC of channel
*              : specified in the channel argument.
*              : Send notice of termination by user-defined function
*              : Userdef_RIICn_SetTransmitEnd
* Arguments    : uint32_t channel: RIIC channel (0 to 3)
* Return Value : DEVDRV_SUCCESS   : Function completes with no errors
*              : DEVDRV_ERROR     : Function fails to complete with errors
******************************************************************************/
int32_t R_RIIC_TeiInterrupt (uint32_t channel)
{
    volatile struct  st_riic * priic;
    volatile uint8_t dummy_buf_8b = 0u;

    /* Suppresses the 'variable set but not used' warning */
    UNUSED_VARIABLE(dummy_buf_8b);

    /*  Argument check  */
    if (channel >= RIIC_CH_TOTAL)
    {
        return DEVDRV_ERROR;
    }

    switch (channel)
    {
        case DEVDRV_CH_0:
            userdef_riic0_set_tx_end();
        break;
        case DEVDRV_CH_1:
            userdef_riic1_set_tx_end();
        break;
        case DEVDRV_CH_2:
            userdef_riic2_set_tx_end();
        break;
        case DEVDRV_CH_3:
            userdef_riic3_set_tx_end();
        break;
        default:

            /* Do not reach here based on the assumption */
            __asm__("nop");
        break;
    }

    /* Transmit end flag clear */
    priic = priic_get_reg_addr(channel);

    rza_io_reg_write_8(&(priic->RIICnSR2.UINT8[0]),
    		            0,
    		            RIICn_RIICnSR2_TEND_SHIFT,
    		            RIICn_RIICnSR2_TEND);

    dummy_buf_8b = rza_io_reg_read_8(&(priic->RIICnSR2.UINT8[0]),
    		                          RIICn_RIICnSR2_TEND_SHIFT,
    		                          RIICn_RIICnSR2_TEND);

    return DEVDRV_SUCCESS;
}

/******************************************************************************
* End of Function R_RIIC_TeiInterrupt
******************************************************************************/

/******************************************************************************
* Function Name: riic_receive
* Description  : Receive data (single byte)
* Arguments    : uint32_t channel    : RIIC channel (0 to 3)
*              : riic_rx_mode_t mode : receive mode
*              :                     : RIIC_RX_MODE_NORMAL: Normal mode
*              :                     : RIIC_RX_MODE_ACK: ACK mode
*              :                     : RIIC_RX_MODE_LOW_HOLD: Low hold mode
*              :                     : RIIC_RX_MODE_NACK: NACK mode
*              :                     : RIIC_RX_MODE_STOP: Stop condition mode
*              : uint8_t * data      : received data
* Return Value : none
******************************************************************************/
static void riic_receive (uint32_t channel, riic_rx_mode_t mode, uint8_t * pdata)
{
    volatile struct st_riic * priic;

    /* Wait until Receive data full conditions and clear receive-data-full
       condition have been satisfied */
    switch (channel)
    {
        case DEVDRV_CH_0:
            userdef_riic0_wait_rx_full();
            userdef_riic0_init_rx_full();
        break;
        case DEVDRV_CH_1:
            userdef_riic1_wait_rx_full();
            userdef_riic1_init_rx_full();
        break;
        case DEVDRV_CH_2:
            userdef_riic2_wait_rx_full();
            userdef_riic2_init_rx_full();
        break;
        case DEVDRV_CH_3:
            userdef_riic3_wait_rx_full();
            userdef_riic3_init_rx_full();
        break;
        default:

            /* Do not reach here based on the assumption */
            __asm__("nop");
        break;
    }

    priic = priic_get_reg_addr(channel);

    switch (mode)
    {
        case RIIC_RX_MODE_ACK:

        	/* Enable Modification of the ACKBT bit */
            rza_io_reg_write_8(&(priic->RIICnMR3.UINT8[0]),
            		            1,
            		            RIICn_RIICnMR3_ACKWP_SHIFT,
            		            RIICn_RIICnMR3_ACKWP);

            /* Send '0' acknowledge bit NACK Transmission */
            rza_io_reg_write_8(&(priic->RIICnMR3.UINT8[0]),
                                0,
                                RIICn_RIICnMR3_ACKBT_SHIFT,
                                RIICn_RIICnMR3_ACKBT);
        break;

        case RIIC_RX_MODE_LOW_HOLD:

        	/* Period between ninth clock cycle and first
        	 * clock cycle is held low */
            rza_io_reg_write_8(&(priic->RIICnMR3.UINT8[0]),
            		            1,
            		            RIICn_RIICnMR3_WAIT_SHIFT,
            		            RIICn_RIICnMR3_WAIT);
        break;

        case RIIC_RX_MODE_NACK:
            rza_io_reg_write_8(&(priic->RIICnMR3.UINT8[0]),
            		            1,
            		            RIICn_RIICnMR3_ACKWP_SHIFT,
            		            RIICn_RIICnMR3_ACKWP);

            /* Send '1' acknowledge bit NACK Transmission */
            rza_io_reg_write_8(&(priic->RIICnMR3.UINT8[0]),
            		            1,
            		            RIICn_RIICnMR3_ACKBT_SHIFT,
            		            RIICn_RIICnMR3_ACKBT);
        break;

        case RIIC_RX_MODE_STOP:

        	/* Stop condition request */
            riic_transmit_stop(channel);
        break;

        case RIIC_RX_MODE_NORMAL:

            /* Do Nothing */
        break;

        default:

            /* Do not reach here based on the assumption */
        break;
    }

    /* Read data from wire */
    (*pdata) = priic->RIICnDRR.UINT8[0];
}

/******************************************************************************
* Function Name: riic_transmit
* Description  : Transmit data (single byte)
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
*              : uint8_t * data   : received data
* Return Value : DEVDRV_SUCCESS   : Function completes with no errors
*              : DEVDRV_ERROR     : Function fails to complete with errors
******************************************************************************/
static int32_t riic_transmit (uint32_t channel, uint8_t data)
{
    volatile struct st_riic * priic;
    int32_t ret;

    priic = priic_get_reg_addr(channel);

    /* ACK */
    if (0 == rza_io_reg_read_8(&(priic->RIICnSR2.UINT8[0]),
    		                    RIICn_RIICnSR2_NACKF_SHIFT,
    		                    RIICn_RIICnSR2_NACKF))
    {
        ret = DEVDRV_SUCCESS;
    }

    /* NACK */
    else
    {
        ret = DEVDRV_ERROR;
    }

    if (DEVDRV_SUCCESS == ret)
    {
        switch (channel)
        {
            case DEVDRV_CH_0:
                userdef_riic0_wait_tx_empty();
                userdef_riic0_init_tx_empty();
            break;
            case DEVDRV_CH_1:
                userdef_riic1_wait_tx_empty();
                userdef_riic1_init_tx_empty();
            break;
            case DEVDRV_CH_2:
                userdef_riic2_wait_tx_empty();
                userdef_riic2_init_tx_empty();
            break;
            case DEVDRV_CH_3:
                userdef_riic3_wait_tx_empty();
                userdef_riic3_init_tx_empty();
            break;
            default:

                /* Do not reach here based on the assumption */
                __asm__("nop");
            break;
        }

        /* Write data to wire */
        priic->RIICnDRT.UINT8[0] = data;
    }

    return (ret);
}

/******************************************************************************
* Function Name: riic_transmit_start
* Description  : Make request to issue start condition fore selected channel
*              : Perform the following sequence of actions
*              : Wait till bus is free
*              : Clear Receive data full flag
*              : Clear transmit end flag
*              : Clear transmit-data-empty flag
*              : Requests to issue a start condition
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : none
******************************************************************************/
static void riic_transmit_start (uint32_t channel)
{
    volatile struct st_riic * priic;

    switch (channel)
    {
        case DEVDRV_CH_0:
            userdef_riic0_wait_bus_master(RIIC_BUS_MASTERSHIP_WAIT_FREE);

            userdef_riic0_init_rx_full();
            userdef_riic0_init_tx_empty();
            userdef_riic0_init_tx_end();
        break;
        case DEVDRV_CH_1:
            userdef_riic1_wait_bus_master(RIIC_BUS_MASTERSHIP_WAIT_FREE);

            userdef_riic1_init_rx_full();
            userdef_riic1_init_tx_empty();
            userdef_riic1_init_tx_end();
        break;
        case DEVDRV_CH_2:
            userdef_riic2_wait_bus_master(RIIC_BUS_MASTERSHIP_WAIT_FREE);
            userdef_riic2_init_rx_full();
            userdef_riic2_init_tx_empty();
            userdef_riic2_init_tx_end();
        break;
        case DEVDRV_CH_3:
            userdef_riic3_wait_bus_master(RIIC_BUS_MASTERSHIP_WAIT_FREE);
            userdef_riic3_init_rx_full();
            userdef_riic3_init_tx_empty();
            userdef_riic3_init_tx_end();
        break;
        default:

            /* Do not reach here based on the assumption */
            __asm__("nop");
        break;
    }

    priic = priic_get_reg_addr(channel);
    rza_io_reg_write_8(&(priic->RIICnCR2.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR2_ST_SHIFT,
    		            RIICn_RIICnCR2_ST);
}

/******************************************************************************
* Function Name: riic_transmit_restart
* Description  : Make the request to issue a restart condition.
*              : Perform the following sequence of actions
*              : Wait till bus is busy
*              : Clear Receive data full flag
*              : Clear transmit end flag
*              : Clear transmit-data-empty flag
*              : Requests to issue a restart condition
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : none
******************************************************************************/
static void riic_transmit_restart (uint32_t channel)
{
    volatile struct st_riic * priic;

    switch (channel)
    {
        case DEVDRV_CH_0:
            userdef_riic0_wait_bus_master(RIIC_BUS_MASTERSHIP_WAIT_BUSY);

            userdef_riic0_init_rx_full();
            userdef_riic0_init_tx_empty();
            userdef_riic0_init_tx_end();
        break;
        case DEVDRV_CH_1:
            userdef_riic1_wait_bus_master(RIIC_BUS_MASTERSHIP_WAIT_BUSY);

            userdef_riic1_init_rx_full();
            userdef_riic1_init_tx_empty();
            userdef_riic1_init_tx_end();
        break;
        case DEVDRV_CH_2:
            userdef_riic2_wait_bus_master(RIIC_BUS_MASTERSHIP_WAIT_BUSY);

            userdef_riic2_init_rx_full();
            userdef_riic2_init_tx_empty();
            userdef_riic2_init_tx_end();
        break;
        case DEVDRV_CH_3:
            userdef_riic3_wait_bus_master(RIIC_BUS_MASTERSHIP_WAIT_BUSY);

            userdef_riic3_init_rx_full();
            userdef_riic3_init_tx_empty();
            userdef_riic3_init_tx_end();
        break;
        default:

            /* Do not reach here based on the assumption */
            __asm__("nop");
        break;
    }

    priic = priic_get_reg_addr(channel);
    rza_io_reg_write_8(&(priic->RIICnCR2.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR2_RS_SHIFT,
    		            RIICn_RIICnCR2_RS);
}

/******************************************************************************
* Function Name: riic_transmit_stop
* Description  : Make the request to issue a restart condition.
*              : Perform the following sequence of actions
*              : Clear Receive data full flag
*              : Requests to issue a stop condition
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : none
******************************************************************************/
static void riic_transmit_stop (uint32_t channel)
{
    volatile struct st_riic * priic;

    priic = priic_get_reg_addr(channel);

    /* Stop condition detection flag initialisation */
    rza_io_reg_write_8(&(priic->RIICnSR2.UINT8[0]),
    		            0,
    		            RIICn_RIICnSR2_STOP_SHIFT,
    		            RIICn_RIICnSR2_STOP);

    /* Stop condition issuance request */
    rza_io_reg_write_8(&(priic->RIICnCR2.UINT8[0]),
    		            1,
    		            RIICn_RIICnCR2_SP_SHIFT,
    		            RIICn_RIICnCR2_SP);
}

/******************************************************************************
* Function Name: riic_detect_stop
* Description  : Make the request to detect stop.
*              : Perform the following sequence of actions
*              : Detect stop condition
*              : Clear NACK
*              : Clear stop condition
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : none
******************************************************************************/
static void riic_detect_stop (uint32_t channel)
{
    volatile struct st_riic * priic;

    switch (channel)
    {
        case DEVDRV_CH_0:
            userdef_riic0_wait_stop();
        break;
        case DEVDRV_CH_1:
            userdef_riic1_wait_stop();
        break;
        case DEVDRV_CH_2:
            userdef_riic2_wait_stop();
        break;
        case DEVDRV_CH_3:
            userdef_riic3_wait_stop();
        break;
        default:

            /* Do not reach here based on the assumption */
        break;
    }

    priic = priic_get_reg_addr(channel);

    riic_clear_nack(priic);
    rza_io_reg_write_8(&(priic->RIICnSR2.UINT8[0]),
    		           0,
    		           RIICn_RIICnSR2_STOP_SHIFT,
    		           RIICn_RIICnSR2_STOP);
}

/******************************************************************************
* Function Name: riic_clear_nack
* Description  : Clear NACK detection flag of RIIC of channel that are
*              : specified in the argument channel (NACKF)
* Arguments    : struct st_riic * riic : specified RIIC channel register
* Return Value : none
******************************************************************************/
static void riic_clear_nack (volatile struct st_riic * priic)
{
    rza_io_reg_write_8(&(priic->RIICnSR2.UINT8[0]),
    		            0,
    		            RIICn_RIICnSR2_NACKF_SHIFT,
    		            RIICn_RIICnSR2_NACKF);
}


/******************************************************************************
* Function Name: priic_get_reg_addr
* Description  : Convert channel ID into pointer to RIIC channel register
* Arguments    : uint32_t channel : RIIC channel (0 to 3)
* Return Value : struct st_riic * : Start address of RIIC register by channel
******************************************************************************/
volatile struct st_riic * priic_get_reg_addr (uint32_t channel)
{
    volatile struct st_riic * priic;

    switch (channel)
    {
        case DEVDRV_CH_0:
            priic = (&RIIC0);
        break;
        case DEVDRV_CH_1:
            priic = (&RIIC1);
        break;
        case DEVDRV_CH_2:
            priic = (&RIIC2);
        break;
        case DEVDRV_CH_3:
            priic = (&RIIC3);
        break;
        default:

            /* Do not reach here based on the assumption */
            priic = (&RIIC0);
        break;
    }

    return (priic);
}


/* End of File */
