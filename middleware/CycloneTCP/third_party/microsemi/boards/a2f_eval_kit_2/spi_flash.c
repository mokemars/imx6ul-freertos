/*****************************************************************************
 * (c) Copyright 2009 Actel Corporation.  All rights reserved.
 *
 * Author : Actel Application Team
 * Rev    : 1.0.0.0
 * Description: Device driver for the on-board SPI flash for SmartFusion KITS
 *  Atmel AT25DF641 SPI flash driver implementation.
 *
 */

#include "bsp_config.h"
#include "spi_flash.h"
#include "drivers/mss_spi/mss_spi.h"

#ifdef USE_DMA_FOR_SPI_FLASH
#include "drivers/mss_pdma/mss_pdma.h"
#endif

#define READ_ARRAY_OPCODE         0x1B
#define DEVICE_ID_READ            0x9F


#define WRITE_ENABLE_CMD          0x06
#define WRITE_DISABLE_CMD         0x04
#define PROGRAM_PAGE_CMD          0x02
#define WRITE_STATUS1_OPCODE      0x01
#define CHIP_ERASE_OPCODE         0x60
#define ERASE_4K_BLOCK_OPCODE     0x20
#define ERASE_32K_BLOCK_OPCODE    0x52
#define ERASE_64K_BLOCK_OPCODE    0xD8
#define READ_STATUS               0x05
#define PROGRAM_RESUME_CMD        0xD0


#define READY_BIT_MASK            0x01
#define PROTECT_SECTOR_OPCODE     0x36
#define UNPROTECT_SECTOR_OPCODE   0x39

#define DONT_CARE                    0

#define NB_BYTES_PER_PAGE          256

#define BLOCK_ALIGN_MASK_4K      0xFFFFF000
#define BLOCK_ALIGN_MASK_32K     0xFFFF8000
#define BLOCK_ALIGN_MASK_64K     0xFFFF0000


#if (SPI_FLASH_ON_SF_DEV_KIT == 1)
#define SPI_INSTANCE    &g_mss_spi1
#define SPI_SLAVE       MSS_SPI_SLAVE_0
#define DMA_TO_PERI     PDMA_TO_SPI_1
#define SPI_DEST_TXBUFF 0x40011014
#endif

#if (SPI_FLASH_ON_SF_EVAL_KIT == 1)
#define SPI_INSTANCE    &g_mss_spi0
#define SPI_SLAVE       MSS_SPI_SLAVE_0
#define DMA_TO_PERI     PDMA_TO_SPI_0
#define SPI_DEST_TXBUFF 0x40001014
#endif

#if ((SPI_FLASH_ON_SF_DEV_KIT == 1) && (SPI_FLASH_ON_SF_EVAL_KIT == 1))
#error "Please select either DEV KIT or EVAL KIT \
        based on board usage in bsp_config.h"
#endif

static uint8_t wait_ready( void );

/******************************************************************************
 *For more details please refer the spi_flash.h file
 ******************************************************************************/
spi_flash_status_t spi_flash_init( void )
{
    /*--------------------------------------------------------------------------
     * Configure MSS_SPI.
     */
    MSS_SPI_init( SPI_INSTANCE );
    MSS_SPI_configure_master_mode
    (
        SPI_INSTANCE,
        SPI_SLAVE,
        MSS_SPI_MODE3,
        MSS_SPI_PCLK_DIV_256,
        MSS_SPI_BLOCK_TRANSFER_FRAME_SIZE
    );


#ifdef USE_DMA_FOR_SPI_FLASH
    /*--------------------------------------------------------------------------
     * Configure DMA channel used as part of this MSS_SPI Flash driver.
     */
    PDMA_init();

    PDMA_configure
    (
        SPI_FLASH_DMA_CHANNEL,
        DMA_TO_PERI,
        PDMA_LOW_PRIORITY | PDMA_BYTE_TRANSFER | PDMA_INC_SRC_ONE_BYTE,
        PDMA_DEFAULT_WRITE_ADJ
    );
#endif
    return SPI_FLASH_SUCCESS;
}

/******************************************************************************
 *For more details please refer the spi_flash.h file
 ******************************************************************************/
spi_flash_status_t
spi_flash_control_hw
(
    spi_flash_control_hw_t operation,
    uint32_t peram1,
    void *   ptrPeram
)
{
    switch(operation){
        case SPI_FLASH_READ_DEVICE_ID:
        {
            uint8_t read_device_id_cmd = DEVICE_ID_READ;
            uint8_t read_buffer[2];
            struct device_Info *ptrDevInfo = (struct device_Info *)ptrPeram;

            MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );

            MSS_SPI_transfer_block( SPI_INSTANCE,
                                    &read_device_id_cmd,
                                    1,
                                    read_buffer,
                                    sizeof(read_buffer) );
            MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );

            ptrDevInfo->manufacturer_id = read_buffer[0];
            ptrDevInfo->device_id = read_buffer[1];

        }
        break;
        case SPI_FLASH_SECTOR_PROTECT:
        {
            uint8_t cmd_buffer[4];
            uint32_t address = peram1;

            MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );

            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;
            if(wait_ready())
                   return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

            /* protect sector */
            cmd_buffer[0] = PROTECT_SECTOR_OPCODE;
            cmd_buffer[1] = (address >> 16) & 0xFF;
            cmd_buffer[2] = (address >> 8 ) & 0xFF;
            cmd_buffer[3] = address & 0xFF;
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_transfer_block( SPI_INSTANCE,
                                    cmd_buffer,
                                    sizeof(cmd_buffer),
                                    0,
                                    0 );
            MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );

        }
        break;
        case SPI_FLASH_SECTOR_UNPROTECT:
        {
            uint8_t cmd_buffer[4];
            uint32_t address = peram1;

            MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );

            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

            /* Unprotect sector */
            cmd_buffer[0] = UNPROTECT_SECTOR_OPCODE;
            cmd_buffer[1] = (address >> 16) & 0xFF;
            cmd_buffer[2] = (address >> 8 ) & 0xFF;
            cmd_buffer[3] = address & 0xFF;
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_transfer_block( SPI_INSTANCE,
                                    cmd_buffer,
                                    sizeof(cmd_buffer),
                                    0,
                                    0 );
            MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );

        }
        break;

        case SPI_FLASH_GLOBAL_PROTECT:
        case SPI_FLASH_GLOBAL_UNPROTECT:
        {
            uint8_t cmd_buffer[2];
            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;

            if(wait_ready())
               return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );
            MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

            /* Send Chip Erase command */
            cmd_buffer[0] = WRITE_STATUS1_OPCODE;
            cmd_buffer[1] = 0;

            if(wait_ready())
               return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 2, 0, 0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );

        }
        break;
        case SPI_FLASH_CHIP_ERASE:
        {
            uint8_t cmd_buffer;
            /* Send Write Enable command */
            cmd_buffer = WRITE_ENABLE_CMD;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );
            MSS_SPI_transfer_block( SPI_INSTANCE, &cmd_buffer, 1, 0, 0 );

            /* Send Chip Erase command */
            cmd_buffer = CHIP_ERASE_OPCODE;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_transfer_block( SPI_INSTANCE, &cmd_buffer, 1, 0, 0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );
        }
        break;
        case SPI_FLASH_RESET:
        {
            uint8_t cmd_buffer;
            /* Send Write Enable command */
            cmd_buffer = WRITE_ENABLE_CMD;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );
            MSS_SPI_transfer_block( SPI_INSTANCE, &cmd_buffer, 1, 0, 0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );

        }
        break;

        case SPI_FLASH_4KBLOCK_ERASE:
        {
            uint32_t address = peram1 & BLOCK_ALIGN_MASK_4K;
            uint8_t cmd_buffer[4];
            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );
            MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

            /* Send Chip Erase command */
            cmd_buffer[0] = ERASE_4K_BLOCK_OPCODE;
            cmd_buffer[1] = (address >> 16) & 0xFF;
            cmd_buffer[2] = (address >> 8 ) & 0xFF;
            cmd_buffer[3] = address & 0xFF;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_transfer_block( SPI_INSTANCE,
                                    cmd_buffer,
                                    sizeof(cmd_buffer),
                                    0,
                                    0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 
            MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );
        }
        break;
        case SPI_FLASH_32KBLOCK_ERASE:
        {
            uint32_t address = peram1 & BLOCK_ALIGN_MASK_32K;
            uint8_t cmd_buffer[4];
            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;

            wait_ready();
            MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );
            MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

            /* Send Chip Erase command */
            cmd_buffer[0] = ERASE_32K_BLOCK_OPCODE;
            cmd_buffer[1] = (address >> 16) & 0xFF;
            cmd_buffer[2] = (address >> 8 ) & 0xFF;
            cmd_buffer[3] = address & 0xFF;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_transfer_block( SPI_INSTANCE,
                                    cmd_buffer,
                                    sizeof(cmd_buffer),
                                    0,
                                    0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );
        }
        break;
        case SPI_FLASH_64KBLOCK_ERASE:
        {
            uint32_t address = peram1 & BLOCK_ALIGN_MASK_64K;
            uint8_t cmd_buffer[4];
            /* Send Write Enable command */
            cmd_buffer[0] = WRITE_ENABLE_CMD;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );
            MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

             /* Send Chip Erase command */
            cmd_buffer[0] = ERASE_64K_BLOCK_OPCODE;
            cmd_buffer[1] = (address >> 16) & 0xFF;
            cmd_buffer[2] = (address >> 8 ) & 0xFF;
            cmd_buffer[3] = address & 0xFF;

            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 
            MSS_SPI_transfer_block( SPI_INSTANCE,
                                    cmd_buffer,
                                    sizeof(cmd_buffer),
                                    0,
                                    0 );
            if(wait_ready())
                return SPI_FLASH_UNSUCCESS; 

            MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );
        }
        break;
        case SPI_FLASH_GET_STATUS:
        {
            uint8_t status;
            uint8_t command = READ_STATUS;

            MSS_SPI_transfer_block( SPI_INSTANCE,
                                    &command,
                                    sizeof(uint8_t),
                                    &status,
                                    sizeof(status) );
            return status;
        }
        break;

        default:
              return SPI_FLASH_INVALID_ARGUMENTS;
        break;
    }
    return SPI_FLASH_SUCCESS;
}


/******************************************************************************
 *For more details please refer the spi_flash.h file
 ******************************************************************************/
spi_flash_status_t
spi_flash_read
(
    uint32_t address,
    uint8_t * rx_buffer,
    size_t size_in_bytes
)
{
    uint8_t cmd_buffer[6];

    cmd_buffer[0] = READ_ARRAY_OPCODE;
    cmd_buffer[1] = (uint8_t)((address >> 16) & 0xFF);
    cmd_buffer[2] = (uint8_t)((address >> 8) & 0xFF);;
    cmd_buffer[3] = (uint8_t)(address & 0xFF);
    cmd_buffer[4] = DONT_CARE;
    cmd_buffer[5] = DONT_CARE;

    MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );
    if(wait_ready())
        return SPI_FLASH_UNSUCCESS; 
    MSS_SPI_transfer_block( SPI_INSTANCE,
                            cmd_buffer,
                            sizeof(cmd_buffer),
                            rx_buffer,
                            size_in_bytes );
    MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );
    return SPI_FLASH_SUCCESS;
}


/*******************************************************************************
 * This function sends the command and data on SPI
 */
static void write_cmd_data
(
    mss_spi_instance_t * this_spi,
    const uint8_t * cmd_buffer,
    uint16_t cmd_byte_size,
    uint8_t * data_buffer,
    uint16_t data_byte_size
)
{
    uint32_t transfer_size;

    transfer_size = cmd_byte_size + data_byte_size;


#ifdef USE_DMA_FOR_SPI_FLASH
    MSS_SPI_disable( this_spi );
    MSS_SPI_set_transfer_byte_count( this_spi, transfer_size );

    PDMA_start
        (
            SPI_FLASH_DMA_CHANNEL,  /* channel_id */
            (uint32_t)cmd_buffer,   /* src_addr */
            SPI_DEST_TXBUFF,             /* dest_addr */
            cmd_byte_size           /* transfer_count */
        );

    PDMA_load_next_buffer
        (
            SPI_FLASH_DMA_CHANNEL,  /* channel_id */
            (uint32_t)data_buffer,  /* src_addr */
            SPI_DEST_TXBUFF,             /* dest_addr */
            data_byte_size          /* transfer_count */
        );
    MSS_SPI_enable( this_spi );
    while ( !MSS_SPI_tx_done(this_spi) )
    {
        ;
    }

#else
    {
#error "Non-DMA Transfers are not implemented. USE only DMA for TX"
    }

#endif

}

/******************************************************************************
 *For more details please refer the spi_flash.h file
 ******************************************************************************/
spi_flash_status_t
spi_flash_write
(
    uint32_t address,
    uint8_t * write_buffer,
    size_t size_in_bytes
)
{
    uint8_t cmd_buffer[4];

    uint32_t in_buffer_idx;
    uint32_t nb_bytes_to_write;
    uint32_t target_addr;

    MSS_SPI_set_slave_select( SPI_INSTANCE, SPI_SLAVE );

    /* Send Write Enable command */
    cmd_buffer[0] = WRITE_ENABLE_CMD;
    wait_ready();
    MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

    /* Unprotect sector */
    cmd_buffer[0] = UNPROTECT_SECTOR_OPCODE;
    cmd_buffer[1] = (address >> 16) & 0xFF;
    cmd_buffer[2] = (address >> 8 ) & 0xFF;
    cmd_buffer[3] = address & 0xFF;
    wait_ready();
    MSS_SPI_transfer_block( SPI_INSTANCE,
                            cmd_buffer,
                            sizeof(cmd_buffer),
                            0,
                            0 );

    /* Send Write Enable command */
    cmd_buffer[0] = WRITE_ENABLE_CMD;
    if(wait_ready())
        return SPI_FLASH_UNSUCCESS; 

    MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

    /**/
    in_buffer_idx = 0;
    nb_bytes_to_write = size_in_bytes;
    target_addr = address;

    while ( in_buffer_idx < size_in_bytes )
    {
        uint32_t size_left;
        nb_bytes_to_write = 0x100 - (target_addr & 0xFF);
        /* adjust max possible size to page boundary. */
        size_left = size_in_bytes - in_buffer_idx;
        if ( size_left < nb_bytes_to_write )
        {
            nb_bytes_to_write = size_left;
        }

        if(wait_ready())
            return SPI_FLASH_UNSUCCESS; 

        /* Send Write Enable command */
        cmd_buffer[0] = WRITE_ENABLE_CMD;
        MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );

        /* Program page */
        if(wait_ready())
            return SPI_FLASH_UNSUCCESS; 


        cmd_buffer[0] = PROGRAM_PAGE_CMD;
        cmd_buffer[1] = (target_addr >> 16) & 0xFF;
        cmd_buffer[2] = (target_addr >> 8 ) & 0xFF;
        cmd_buffer[3] = target_addr & 0xFF;

        write_cmd_data
          (
            SPI_INSTANCE,
            cmd_buffer,
            sizeof(cmd_buffer),
            &write_buffer[in_buffer_idx],
            nb_bytes_to_write
          );

        target_addr += nb_bytes_to_write;
        in_buffer_idx += nb_bytes_to_write;
    }

    /* Send Write Disable command. */
    cmd_buffer[0] = WRITE_DISABLE_CMD;

    if(wait_ready())
        return SPI_FLASH_UNSUCCESS; 

    MSS_SPI_transfer_block( SPI_INSTANCE, cmd_buffer, 1, 0, 0 );
    MSS_SPI_clear_slave_select( SPI_INSTANCE, SPI_SLAVE );
    return SPI_FLASH_SUCCESS;
}


/******************************************************************************
 * This function waits for the SPI operation to complete
 ******************************************************************************/
static uint8_t wait_ready( void )
{
    uint32_t count = 0;
    uint8_t ready_bit = 1;
    uint8_t command = READ_STATUS;

    do {
        MSS_SPI_transfer_block( SPI_INSTANCE,
                                &command, 
                                sizeof(command),
                                &ready_bit,
                                sizeof(ready_bit) );
        ready_bit = ready_bit & READY_BIT_MASK;
        count++;
    } while(( ready_bit == 1 ) /*&& (count <= 0x7FFFFFFF)*/);

    return (ready_bit);
}
