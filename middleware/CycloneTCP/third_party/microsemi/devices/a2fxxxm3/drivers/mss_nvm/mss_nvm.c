/*******************************************************************************
 * (c) Copyright 2009-2011 Actel Corporation.  All rights reserved.
 *
 * SmartFusion Microcontroller Subsystem ENVM bare metal software driver
 * implementation.
 *
 * SVN $Revision: 3764 $
 * SVN $Date: 2011-07-29 11:21:30 +0530 (Fri, 29 Jul 2011) $
 */
#include "mss_nvm.h"
#include "../../CMSIS/mss_assert.h"
#include "../../CMSIS/a2fxxxm3.h"

#ifdef __cplusplus
extern "C" {
#endif 

/***************************************************************************//**
 * There is a maximum of two NVM blocks
 */
#define MAX_NB_NVM_BLOCKS       2

/***************************************************************************//**
 * eNVM registers status bits locations.
 */
#define STATUS_BUSY_0_MASK              0x00000001u
#define STATUS_PROT_ERROR_0_MASK        0x00000002u
#define STATUS_PROG_ERROR_0_MASK        0x00000004u
#define STATUS_OVER_THRESH_0_MASK       0x00000010u
#define STATUS_ECC1_ERROR_0_MASK        0x00000020u
#define STATUS_ECC2_ERROR_0_MASK        0x00000040u
#define STATUS_ILLEGAL_COMMAND_0_MASK   0x00008000u

#define STATUS_BUSY_1_MASK              0x00010000u
#define STATUS_PROT_ERROR_1_MASK        0x00020000u
#define STATUS_PROG_ERROR_1_MASK        0x00040000u
#define STATUS_OVER_THRESH_1_MASK       0x00100000u
#define STATUS_ECC1_ERROR_1_MASK        0x00200000u
#define STATUS_ECC2_ERROR_1_MASK        0x00400000u
#define STATUS_ILLEGAL_COMMAND_1_MASK   0x80000000u

/***************************************************************************//**
 * eNVM memory mapping.
 */
#define ENVM_END_ADDR      0x600881FFu

#define NVM_OFFSET_MASK     0x000FFFFFu
#define PAGE_ALIGN_MASK     0xFFFFFF80u
#define PAGE_AUX_ALIGN_MASK 0xFFFFFFFCu

#define UNPROTECT_CMD       0x02000000u
#define PROGRAM_CMD         0x10000000u
#define DISCARD_PAGE_CMD    0x04000000u
    
#define STICKY_BITS_RESET   0xFFFFFFFFu

#define ENVM0_ARRAY_BASE_ADDR               0x60000000u
#define ENVM1_ARRAY_BASE_ADDR               0x60040000u
#define ENVM0_SPARE_PAGES_BASE_ADDR         0x60080000u
#define ENVM1_SPARE_PAGES_BASE_ADDR         0x60082000u
#define ENVM0_USER_DATA_BASE_ADDR           0x60084000u
#define ENVM1_USER_DATA_BASE_ADDR           0x60086000u
#define ENVM0_USER_SPARE_PAGES_BASE_ADDR    0x60088000u
#define ENVM1_USER_SPARE_PAGES_BASE_ADDR    0x60088100u

#define ENVM_MAIN_ARRAY_SIZE        0x00040000u
#define ENVM_SPARE_PAGE_SIZE        0x00002000u
#define ENVM_USER_DATA_SIZE         0x00002000u
#define ENVM_USER_SPARE_PAGE_SIZE   0x00000100u

/***************************************************************************//**
 * eNVM registers bit masks.
 */
#define ENVM0_PROTECTION_ERROR_MASK     (STATUS_PROT_ERROR_0_MASK)

#define ENVM0_PROG_ERROR_MASK           (STATUS_PROG_ERROR_0_MASK | \
                                         STATUS_OVER_THRESH_0_MASK | \
                                         STATUS_ECC1_ERROR_0_MASK | \
                                         STATUS_ECC2_ERROR_0_MASK | \
                                         STATUS_ILLEGAL_COMMAND_0_MASK)

#define ENVM1_PROTECTION_ERROR_MASK     (STATUS_PROT_ERROR_1_MASK)

#define ENVM1_PROG_ERROR_MASK           (STATUS_PROG_ERROR_1_MASK | \
                                         STATUS_OVER_THRESH_1_MASK | \
                                         STATUS_ECC1_ERROR_1_MASK | \
                                         STATUS_ECC2_ERROR_1_MASK | \
                                         STATUS_ILLEGAL_COMMAND_1_MASK)

/***************************************************************************//**
 * eNVM mode system register bits.
 */
#define SYSREG_ENVM_MODE_MASK       0x000000C0u
#define ENVM_6_CYCLES_MODE          0x000000C0u

#define NVM_AUX_PAGE_SIZE      4u
#define NVM_PAGE_SIZE        128u

/* Global definitions related to page size and page mask */
nvm_dev_type_t smartfusion_dev_type = NVM_A2F200_DEVICE;
uint32_t nvm_boundary_addr = 0u;

/* Global definitions related to page size and page mask */
uint8_t nvm_page_size = NVM_PAGE_SIZE ;
uint32_t nvm_page_align_mask = PAGE_ALIGN_MASK ;

/***************************************************************************//**
  The get_nvm_block_idx() function returns the index of the eNVM block in which
  the start address passed as parameter is located. It returns 0 to identify
  eNVM0, 1 to identify eNVM1 and -1 to indicate that the address parameter is
  not within a valid eNVM address range.
  The end address for the eNVM section accessed is returned through the
  p_end_addr pointer.
 */
static int32_t get_nvm_block_idx
(
    uint32_t start_addr,
    uint32_t * p_end_addr
)
{
    int32_t nvm_block_idx = -1;

    if ( ( start_addr <= ENVM_END_ADDR ) &&( start_addr >= ENVM0_ARRAY_BASE_ADDR) )
    {
        if ( start_addr >= ENVM0_SPARE_PAGES_BASE_ADDR )
        {
            if ( start_addr < ENVM0_USER_DATA_BASE_ADDR )
            {
                /* Spare pages. */
                /* Which spare page? */
                if ( start_addr < ENVM1_SPARE_PAGES_BASE_ADDR )
                {
                    /* We are accessing eNVM0 spare pages. */
                    nvm_block_idx = 0;

                    if ( smartfusion_dev_type == NVM_A2F060_DEVICE )
                    {
                        *p_end_addr = ENVM0_SPARE_PAGES_BASE_ADDR + ((ENVM_SPARE_PAGE_SIZE/2u) - 1u);
                    }
                    else if (smartfusion_dev_type == NVM_A2F500_DEVICE)
                    {
                        nvm_boundary_addr = ENVM0_SPARE_PAGES_BASE_ADDR + ENVM_SPARE_PAGE_SIZE ;
                        *p_end_addr = ENVM1_SPARE_PAGES_BASE_ADDR + (ENVM_SPARE_PAGE_SIZE - 1u);
                    }
                    else /* By Default assume smartfusion_dev_type == A2F200_DEVICE*/
                    {
                        *p_end_addr = ENVM0_SPARE_PAGES_BASE_ADDR + (ENVM_SPARE_PAGE_SIZE - 1u);
                    }

                }
                else
                {
                    /* We are accessing eNVM1 spare pages. */
                    if (smartfusion_dev_type == NVM_A2F500_DEVICE )
                    {
                        nvm_block_idx = 1;
                    }
                    *p_end_addr = ENVM1_SPARE_PAGES_BASE_ADDR + (ENVM_SPARE_PAGE_SIZE - 1u);
                }

                nvm_page_size = NVM_PAGE_SIZE ;
                nvm_page_align_mask = PAGE_ALIGN_MASK ;
            }
            else if ( start_addr < ENVM0_USER_SPARE_PAGES_BASE_ADDR )
            {
                /* User data. */
                if ( start_addr < ENVM1_USER_DATA_BASE_ADDR )
                {
                    /* We are accessing eNVM0 user data array. */
                    nvm_block_idx = 0;

                    if ( smartfusion_dev_type == NVM_A2F060_DEVICE )
                    {
                        *p_end_addr = ENVM0_USER_DATA_BASE_ADDR + ((ENVM_USER_DATA_SIZE/2u) - 1u);
                    }
                    else if (smartfusion_dev_type == NVM_A2F500_DEVICE)
                    {
                        nvm_boundary_addr = ENVM0_USER_DATA_BASE_ADDR + ENVM_USER_DATA_SIZE ;
                        *p_end_addr = ENVM1_USER_DATA_BASE_ADDR + (ENVM_USER_DATA_SIZE - 1u);
                    }
                    else /* By Default assume smartfusion_dev_type == A2F200_DEVICE*/
                    {
                        *p_end_addr = ENVM0_USER_DATA_BASE_ADDR + (ENVM_USER_DATA_SIZE - 1u);
                    }
                }
                else
                {
                    /* We are accessing eNVM1 user data array. */
                    if (smartfusion_dev_type == NVM_A2F500_DEVICE )
                    {
                        nvm_block_idx = 1;
                    }
                    *p_end_addr = ENVM1_USER_DATA_BASE_ADDR + (ENVM_USER_DATA_SIZE - 1u);
                }
                nvm_page_size = NVM_AUX_PAGE_SIZE ;
                nvm_page_align_mask = PAGE_AUX_ALIGN_MASK ;
            }
            else
            {
                /* User data spare pages. */
                if ( start_addr < ENVM1_USER_SPARE_PAGES_BASE_ADDR )
                {
                    /* We are accessing eNVM0 user data spare pages. */
                    nvm_block_idx = 0;

                    if ( smartfusion_dev_type == NVM_A2F060_DEVICE )
                    {
                        *p_end_addr = ENVM0_USER_SPARE_PAGES_BASE_ADDR + ((ENVM_USER_SPARE_PAGE_SIZE/2u) - 1u) ;
                    }
                    else if (smartfusion_dev_type == NVM_A2F500_DEVICE)
                    {
                        nvm_boundary_addr = ENVM0_USER_SPARE_PAGES_BASE_ADDR + ENVM_USER_SPARE_PAGE_SIZE;
                        *p_end_addr = ENVM1_USER_SPARE_PAGES_BASE_ADDR + (ENVM_USER_SPARE_PAGE_SIZE - 1u);
                    }
                    else /* By Default assume smartfusion_dev_type == A2F200_DEVICE*/
                    {
                        *p_end_addr = ENVM0_USER_SPARE_PAGES_BASE_ADDR + (ENVM_USER_SPARE_PAGE_SIZE - 1u);
                    }
                }
                else
                {
                    /* We are accessing eNVM1 user data spare pages. */
                    if (smartfusion_dev_type == NVM_A2F500_DEVICE )
                    {
                        nvm_block_idx = 1;
                    }
                    *p_end_addr = ENVM1_USER_SPARE_PAGES_BASE_ADDR + (ENVM_USER_SPARE_PAGE_SIZE - 1u);
                }
                nvm_page_size = NVM_AUX_PAGE_SIZE ;
                nvm_page_align_mask = PAGE_AUX_ALIGN_MASK ;
            }
        }
        else
        {  
            /* Which user array? */
            if ( start_addr < ENVM1_ARRAY_BASE_ADDR )
            {
                /* We are accessing the main eNVM0 array. */
                nvm_block_idx = 0;

                if ( smartfusion_dev_type == NVM_A2F060_DEVICE )
                {
                    *p_end_addr = ENVM0_ARRAY_BASE_ADDR + ((ENVM_MAIN_ARRAY_SIZE/2u) - 1u) ;
                }
                else if (smartfusion_dev_type == NVM_A2F500_DEVICE)
                {
                    nvm_boundary_addr = ENVM0_ARRAY_BASE_ADDR + ENVM_MAIN_ARRAY_SIZE ;
                    *p_end_addr = ENVM1_ARRAY_BASE_ADDR + (ENVM_MAIN_ARRAY_SIZE - 1u);
                }
                else /* By Default assume smartfusion_dev_type == A2F200_DEVICE*/
                {
                    *p_end_addr = ENVM0_ARRAY_BASE_ADDR + (ENVM_MAIN_ARRAY_SIZE - 1u);
                }
            }
            else
            {
                /* We are accessing the main eNVM1 array. */
                if (smartfusion_dev_type == NVM_A2F500_DEVICE )
                {
                    nvm_block_idx = 1;
                }
                *p_end_addr = ENVM1_ARRAY_BASE_ADDR + (ENVM_MAIN_ARRAY_SIZE - 1u);
            }

            nvm_page_size = NVM_PAGE_SIZE ;
            nvm_page_align_mask = PAGE_ALIGN_MASK ;
        }
    }
    
    return nvm_block_idx;
}

/***************************************************************************//**
 * See mss_nvm.h for details.
 */
void NVM_init(nvm_dev_type_t device_type)
{
    smartfusion_dev_type = device_type ;
}
/***************************************************************************//**
 * See mss_nvm.h for details.
 */
nvm_status_t
NVM_write
(
     uint32_t start_addr,
     const uint8_t * p_data,
     size_t nb_bytes
)
{
    uint32_t inc;
    const uint32_t status_busy_mask[MAX_NB_NVM_BLOCKS] = { STATUS_BUSY_0_MASK, STATUS_BUSY_1_MASK };
    const uint32_t protection_error_mask[MAX_NB_NVM_BLOCKS] = { ENVM0_PROTECTION_ERROR_MASK, ENVM1_PROTECTION_ERROR_MASK };
    const uint32_t prog_error_mask[MAX_NB_NVM_BLOCKS] = { ENVM0_PROG_ERROR_MASK, ENVM1_PROG_ERROR_MASK };
    int32_t nvm_block_idx;
    nvm_status_t outcome = NVM_SUCCESS;
    volatile uint8_t * p_byte_nvm = (uint8_t *)start_addr;
    uint32_t busy;
    uint32_t nb_bytes_left;
    uint32_t nb_bytes_in_page;
    uint32_t end_addr;
    uint32_t initial_envm_cr;

    /*--------------------------------------------------------------------------
     * Figure out which eNVM block is accessed.
     */
    nvm_block_idx = get_nvm_block_idx( start_addr, &end_addr );
    ASSERT( nvm_block_idx >= 0 );
    ASSERT( nb_bytes <= (ENVM_END_ADDR - ENVM0_ARRAY_BASE_ADDR) );
    ASSERT( nb_bytes <= ((end_addr - start_addr) + 1u) );

    if ( (nvm_block_idx < 0) ||
         (nb_bytes > (ENVM_END_ADDR - ENVM0_ARRAY_BASE_ADDR)) ||
         (nb_bytes > ((end_addr - start_addr) + 1u)) )
    {
        /* Return immediately if address out of range. */
        outcome = NVM_INVALID_ADDRESS;
    }
    else
    {
        /*--------------------------------------------------------------------------
         * Set eNVM mode to 6 cycles (6:1:1:1).
         */
        initial_envm_cr = SYSREG->ENVM_CR;
        SYSREG->ENVM_CR = (initial_envm_cr & ~((uint32_t)SYSREG_ENVM_MODE_MASK)) | ENVM_6_CYCLES_MODE;
        
        /*--------------------------------------------------------------------------
         * Perform programming.
         */
        nb_bytes_left = nb_bytes;
        while ((nb_bytes_left > 0u) && (NVM_SUCCESS == outcome))
        {
           uint32_t command;
           uint32_t error;
           uint32_t page_start_addr;
            
            page_start_addr = ((uint32_t)p_byte_nvm) & nvm_page_align_mask;
            
            if ( ( page_start_addr == nvm_boundary_addr ) && (0 == nvm_block_idx))
            {
                nvm_block_idx = 1;
            }
            /*----------------------------------------------------------------------
             * Unprotect page.
             */
            /* Reset status register sticky bits. */
            ENVM_REGS->STATUS = STICKY_BITS_RESET;
            
            /* Unprotect page. */
            command = UNPROTECT_CMD | (page_start_addr & NVM_OFFSET_MASK);
            ENVM_REGS->CONTROL = command;
            
            /* Wait for unprotect complete. */
            do
            {
                busy = ENVM_REGS->STATUS & status_busy_mask[nvm_block_idx];
            } while ( busy != 0u );
            
            error = ENVM_REGS->STATUS & protection_error_mask[nvm_block_idx];
            
            if ( error )
            {
                outcome = NVM_PROTECTION_ERROR;
            }
            else
            {
                /*------------------------------------------------------------------
                 * Copy programming data to page buffer
                 */
                nb_bytes_in_page = (page_start_addr + nvm_page_size) - ((uint32_t)p_byte_nvm);
                
                if ( nb_bytes_left < nb_bytes_in_page )
                {
                    nb_bytes_in_page = nb_bytes_left;
                }
                
                for( inc = 0u; inc < nb_bytes_in_page; ++inc )
                {        
                    *p_byte_nvm = *p_data;
                    ++p_byte_nvm;
                    ++p_data;
                }
                nb_bytes_left = nb_bytes_left - nb_bytes_in_page;
                
                /*------------------------------------------------------------------
                 * Program page.
                 */
                command = PROGRAM_CMD | (page_start_addr & NVM_OFFSET_MASK);
                ENVM_REGS->CONTROL = command;
                /* Wait for program to complete. */
                do
                {
                    busy = ENVM_REGS->STATUS & status_busy_mask[nvm_block_idx];
                } while ( busy != 0u );
                
                error = ENVM_REGS->STATUS & prog_error_mask[nvm_block_idx];
                if ( error )
                {
                    outcome = NVM_WRITE_ERROR;
                }
            }
        }
        
        /*--------------------------------------------------------------------------
         * Issue DISCARD_PAGE command and restore eNVM mode.
         */
        ENVM_REGS->CONTROL = DISCARD_PAGE_CMD;
        SYSREG->ENVM_CR = initial_envm_cr;
    }
    
    return outcome;
}

#ifdef __cplusplus
}
#endif


