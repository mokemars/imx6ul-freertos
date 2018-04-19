/*******************************************************************************
 * (c) Copyright 2012 Microsemi SoC Products Group.  All rights reserved.
 *
 * SmartFusion2 system services.
 *
 * SVN $Revision: 4399 $
 * SVN $Date: 2012-07-11 16:33:09 +0100 (Wed, 11 Jul 2012) $
 */
#include "mss_sys_services.h"
#include "mss_comblk.h"
#include <string.h>

/*==============================================================================
 *
 */
/*
 * Service request command opcodes:
 */
#define DEVICE_CERTIFICATE_REQUEST_CMD      0
#define SERIAL_NUMBER_REQUEST_CMD           1
#define FLASH_FREEZE_REQUEST_CMD            2
#define AES128_REQUEST_CMD                  3
#define USERCODE_REQUEST_CMD                4
#define DESIGNVER_REQUEST_CMD               5
#define AES256_REQUEST_CMD                  6
#define KEYTREE_REQUEST_CMD                 9
#define SHA256_REQUEST_CMD                  10
#define HMAC_REQUEST_CMD                    12
#define PPUF_CHALLENGE_RESP_REQUEST_CMD     14
#define IAP_PROGRAMMING_REQUEST_CMD         20
#define ISP_PROGRAMMING_REQUEST_CMD         21
#define IAP_RECOVERY_CFG_REQUEST_CMD        22
#define DIGEST_CHECK_REQUEST_CMD            23
#define DRBG_SELF_TEST_REQUEST_CMD          40
#define DRBG_INSTANTIATE_REQUEST_CMD        41
#define DRBG_GENERATE_REQUEST_CMD           42
#define DRBG_RESEED_REQUEST_CMD             43
#define DRBG_UNINSTANTIATE_REQUEST_CMD      44
#define FLASHFREEZE_SHUTDOWN_CMD            224
#define ZEROIZATION_REQUEST_CMD             240

#define POWER_ON_RESET_DIGEST_ERROR_CMD     241

/*
 * Service response lengths:
 */
#define STANDARD_SERV_RESP_LENGTH           6

#define SERIAL_NUMBER_SERV_RESP_LENGTH      6
#define USERCODE_SERV_RESP_LENGTH           6
#define DESIGNVER_SERV_RESP_LENGTH          6
#define DEVICE_CERTIFICATE_SERV_RESP_LENGTH 6
#define ISP_PROGRAMMING_SERV_RESP_LENGTH    2
#define DRBG_SELF_TEST_SERV_RESP_LENGTH     2
#define DRBG_UNINST_SERV_RESP_LENGTH        3

#define DIGEST_CHECK_SERV_RESP_LENGTH       2u

/*
 * Deterministic Random Bit Generator defines:
 */
#define INVALID_DRBG_HANDLE     0xFFu


/*==============================================================================
 * Local functions.
 */
static void request_completion_handler(uint8_t * p_response, uint16_t response_size);
static void signal_request_start(void);
static uint16_t wait_for_current_request_completion(void);
static uint8_t execute_service
(
    uint8_t cmd_opcode,
    uint8_t * cmd_params_ptr,
    uint8_t * response,
    uint16_t response_length
);


/*==============================================================================
 * Global variables
 */
static uint8_t g_request_in_progress = 0;
static uint16_t g_last_response_length = 0;

/*==============================================================================
 *
 */
void MSS_SYS_init(void)
{
    MSS_COMBLK_init();
    
    g_request_in_progress = 0;
    g_last_response_length = 0;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_get_serial_number
(
    uint8_t * p_serial_number
)
{
    uint8_t response[SERIAL_NUMBER_SERV_RESP_LENGTH];
    uint8_t status;
    
    status = execute_service(SERIAL_NUMBER_REQUEST_CMD,
                             p_serial_number,
                             response,
                             SERIAL_NUMBER_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_get_user_code
(
    uint8_t * p_user_code
)
{
    uint8_t response[USERCODE_SERV_RESP_LENGTH];
    uint8_t status;
    
    status = execute_service(USERCODE_REQUEST_CMD,
                             p_user_code,
                             response,
                             USERCODE_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_get_design_version
(
    uint8_t * p_design_version
)
{
    uint8_t response[DESIGNVER_SERV_RESP_LENGTH];
    uint8_t status;
    
    status = execute_service(DESIGNVER_REQUEST_CMD,
                             p_design_version,
                             response,
                             DESIGNVER_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_get_device_certificate
(
    uint8_t * p_device_certificate
)
{
    uint8_t response[DEVICE_CERTIFICATE_SERV_RESP_LENGTH];
    uint8_t status;
    
    status = execute_service(DEVICE_CERTIFICATE_REQUEST_CMD,
                             p_device_certificate,
                             response,
                             DEVICE_CERTIFICATE_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_flash_freeze(uint8_t options)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    
    signal_request_start();
    
    MSS_COMBLK_send_cmd_with_ptr
        (
            FLASH_FREEZE_REQUEST_CMD,       /* cmd_opcode */
            (uint32_t)&options,             /* cmd_params_ptr */
            response,                       /* p_response */
            STANDARD_SERV_RESP_LENGTH,      /* response_size */
            request_completion_handler      /* completion_handler */
        );
    
    actual_response_length = wait_for_current_request_completion();
    
    if((FLASH_FREEZE_REQUEST_CMD == response[0]) && (actual_response_length > 1))
    {
        status = response[1];
    }
    else
    {
        if(FLASHFREEZE_SHUTDOWN_CMD == response[0])
        {
            status = MSS_SYS_SUCCESS;
        }
        else
        {
            status = MSS_SYS_UNEXPECTED_ERROR;
        }
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
#define AES128_KEY_LENGTH      16
#define IV_LENGTH       32

#define AES256_KEY_LENGTH      32

#define HMAC_KEY_LENGTH         32

uint8_t MSS_SYS_128bit_aes
(
    const uint8_t * key,
    const uint8_t * iv,
    uint16_t nb_blocks,
    uint8_t mode,
    uint8_t * dest_addr,
    const uint8_t * src_addr)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[44];
    uint8_t status;
    
    memcpy(&params[0], key, AES128_KEY_LENGTH);
    memcpy(&params[16], iv, IV_LENGTH);
    
    params[32] = (uint8_t)nb_blocks;
    params[33] = (uint8_t)(nb_blocks >> 8u);
    params[34] = mode;
    params[35] = 0u;
    
    params[36] = (uint8_t)((uint32_t)dest_addr);
    params[37] = (uint8_t)((uint32_t)dest_addr >> 8u);
    params[38] = (uint8_t)((uint32_t)dest_addr >> 16u);
    params[39] = (uint8_t)((uint32_t)dest_addr >> 24u);

    params[40] = (uint8_t)((uint32_t)src_addr);
    params[41] = (uint8_t)((uint32_t)src_addr >> 8u);
    params[42] = (uint8_t)((uint32_t)src_addr >> 16u);
    params[43] = (uint8_t)((uint32_t)src_addr >> 24u);
    
    status = execute_service(AES128_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_256bit_aes
( 
    const uint8_t * key,
    const uint8_t * iv,
    uint16_t nb_blocks,
    uint8_t mode,
    uint8_t * dest_addr,
    const uint8_t * src_addr
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[60];
    uint8_t status;
    
    memcpy(&params[0], key, AES256_KEY_LENGTH);
    memcpy(&params[32], iv, IV_LENGTH);
    
    params[48] = (uint8_t)nb_blocks;
    params[49] = (uint8_t)(nb_blocks >> 8u);
    params[50] = mode;
    params[51] = 0u;
    
    params[52] = (uint8_t)((uint32_t)dest_addr);
    params[53] = (uint8_t)((uint32_t)dest_addr >> 8u);
    params[54] = (uint8_t)((uint32_t)dest_addr >> 16u);
    params[55] = (uint8_t)((uint32_t)dest_addr >> 24u);

    params[56] = (uint8_t)((uint32_t)src_addr);
    params[57] = (uint8_t)((uint32_t)src_addr >> 8u);
    params[58] = (uint8_t)((uint32_t)src_addr >> 16u);
    params[59] = (uint8_t)((uint32_t)src_addr >> 24u);
    
    status = execute_service(AES256_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_sha256
(
    const uint8_t * p_data_in,
    uint32_t length,
    uint8_t * result
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[12];
    uint8_t status;
    
    params[0] = (uint8_t)((uint32_t)length);
    params[1] = (uint8_t)((uint32_t)length >> 8u);
    params[2] = (uint8_t)((uint32_t)length >> 16u);
    params[3] = (uint8_t)((uint32_t)length >> 24u);
    
    params[4] = (uint8_t)((uint32_t)result);
    params[5] = (uint8_t)((uint32_t)result >> 8u);
    params[6] = (uint8_t)((uint32_t)result >> 16u);
    params[7] = (uint8_t)((uint32_t)result >> 24u);

    params[8] = (uint8_t)((uint32_t)p_data_in);
    params[9] = (uint8_t)((uint32_t)p_data_in >> 8u);
    params[10] = (uint8_t)((uint32_t)p_data_in >> 16u);
    params[11] = (uint8_t)((uint32_t)p_data_in >> 24u);
    
    status = execute_service(SHA256_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_hmac
(
    const uint8_t * key,
    const uint8_t * p_data_in,
    uint32_t length,
    uint8_t * p_result
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[58];
    uint8_t status;
    
    memcpy(&params[0], key, HMAC_KEY_LENGTH);
    
    params[32] = (uint8_t)((uint32_t)length);
    params[33] = (uint8_t)((uint32_t)length >> 8u);
    params[34] = (uint8_t)((uint32_t)length >> 16u);
    params[35] = (uint8_t)((uint32_t)length >> 24u);
    
    params[36] = (uint8_t)((uint32_t)p_data_in);
    params[37] = (uint8_t)((uint32_t)p_data_in >> 8u);
    params[38] = (uint8_t)((uint32_t)p_data_in >> 16u);
    params[39] = (uint8_t)((uint32_t)p_data_in >> 24u);

    params[40] = (uint8_t)((uint32_t)p_result);
    params[41] = (uint8_t)((uint32_t)p_result >> 8u);
    params[42] = (uint8_t)((uint32_t)p_result >> 16u);
    params[43] = (uint8_t)((uint32_t)p_result >> 24u);
    
    status = execute_service(HMAC_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
#define KEYTREE_KEY_LENGTH      32
#define KEYTREE_PATH_LENGTH     16

uint8_t MSS_SYS_CRI_key_tree
(
    uint8_t * p_key,
    uint8_t op_type,
    uint8_t * path
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[49];
    uint8_t status;
    
    memcpy(&params[0], p_key, KEYTREE_KEY_LENGTH);
    
    params[32] = op_type;
    
    memcpy(&params[33], path, KEYTREE_PATH_LENGTH);
    
    status = execute_service(KEYTREE_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_CRI_pseudo_puf_challenge
(
    uint8_t * p_key,
    uint8_t op_type,
    uint8_t * path
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[21];
    uint8_t status;
    
    params[0] = (uint8_t)((uint32_t)p_key);
    params[1] = (uint8_t)((uint32_t)p_key >> 8u);
    params[2] = (uint8_t)((uint32_t)p_key >> 16u);
    params[3] = (uint8_t)((uint32_t)p_key >> 24u);
    
    params[3] = op_type;
    
    memcpy(&params[5], path, KEYTREE_PATH_LENGTH);
    
    status = execute_service(PPUF_CHALLENGE_RESP_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_drbg_self_test(void)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t self_test;
    uint8_t response[DRBG_SELF_TEST_SERV_RESP_LENGTH];
    
    signal_request_start();
    
    self_test = DRBG_SELF_TEST_REQUEST_CMD;

    MSS_COMBLK_send_cmd
        (
            &self_test,                         /* p_cmd */
            sizeof(self_test),                  /* cmd_size */
            0,                                  /* p_data */
            0,                                  /* data_size */
            response,                           /* p_response */
            DRBG_SELF_TEST_SERV_RESP_LENGTH,    /* response_size */
            request_completion_handler          /* completion_handler */
        );
        
    
    actual_response_length = wait_for_current_request_completion();
    
    if((DRBG_SELF_TEST_SERV_RESP_LENGTH == actual_response_length) &&
       (DRBG_SELF_TEST_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR; // TODO figure out more generic error code
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_drbg_reserve
(
    const uint8_t * personalization_str,
    uint16_t personalization_str_length,
    uint8_t * p_drbg_handle
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t intantiate_params[7];
    uint8_t status;
    
    intantiate_params[0] = (uint8_t)((uint32_t)personalization_str);
    intantiate_params[1] = (uint8_t)((uint32_t)personalization_str >> 8u);
    intantiate_params[2] = (uint8_t)((uint32_t)personalization_str >> 16u);
    intantiate_params[3] = (uint8_t)((uint32_t)personalization_str >> 24u);
    intantiate_params[4] = (uint8_t)personalization_str_length;
    intantiate_params[5] = (uint8_t)(personalization_str_length >> 8u);
    intantiate_params[6] = INVALID_DRBG_HANDLE;
    
    status = execute_service(DRBG_INSTANTIATE_REQUEST_CMD,
                             intantiate_params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
                             
    if(MSS_SYS_SUCCESS == status)
    {
        *p_drbg_handle = intantiate_params[6];
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_drbg_generate
(
    const uint8_t * p_requested_data,
    const uint8_t * p_additional_input,
    uint8_t requested_length,
    uint8_t additional_input_length,
    uint8_t pr_req,
    uint8_t drbg_handle
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t generate_params[12];
    uint8_t status;
    
    generate_params[0] = (uint8_t)((uint32_t)p_requested_data);
    generate_params[1] = (uint8_t)((uint32_t)p_requested_data >> 8u);
    generate_params[2] = (uint8_t)((uint32_t)p_requested_data >> 16u);
    generate_params[3] = (uint8_t)((uint32_t)p_requested_data >> 24u);
    generate_params[4] = (uint8_t)((uint32_t)p_additional_input);
    generate_params[5] = (uint8_t)((uint32_t)p_additional_input >> 8u);
    generate_params[6] = (uint8_t)((uint32_t)p_additional_input >> 16u);
    generate_params[7] = (uint8_t)((uint32_t)p_additional_input >> 24u);
    generate_params[8] = requested_length;
    generate_params[9] = additional_input_length;
    generate_params[10] = pr_req;
    generate_params[11] = drbg_handle;
    
    status = execute_service(DRBG_GENERATE_REQUEST_CMD,
                             generate_params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_drbg_reseed
(
    const uint8_t * p_additional_input,
    uint8_t additional_input_length,
    uint8_t drbg_handle
)
{
    uint8_t response[STANDARD_SERV_RESP_LENGTH];
    uint8_t params[6];
    uint8_t status;
    
    params[0] = (uint8_t)((uint32_t)p_additional_input);
    params[1] = (uint8_t)((uint32_t)p_additional_input >> 8u);
    params[2] = (uint8_t)((uint32_t)p_additional_input >> 16u);
    params[3] = (uint8_t)((uint32_t)p_additional_input >> 24u);
    params[4] = (uint8_t)additional_input_length;
    params[5] = drbg_handle;
    
    status = execute_service(DRBG_RESEED_REQUEST_CMD,
                             params,
                             response,
                             STANDARD_SERV_RESP_LENGTH);
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_drbg_release
(
    uint8_t drbg_handle
)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t uninstantiate_req[2];
    uint8_t response[DRBG_UNINST_SERV_RESP_LENGTH];
    
    signal_request_start();
    
    uninstantiate_req[0] = DRBG_UNINSTANTIATE_REQUEST_CMD;
    uninstantiate_req[1] = drbg_handle;

    MSS_COMBLK_send_cmd
        (
            uninstantiate_req,                  /* p_cmd */
            sizeof(uninstantiate_req),          /* cmd_size */
            0,                                  /* p_data */
            0,                                  /* data_size */
            response,                           /* p_response */
            DRBG_UNINST_SERV_RESP_LENGTH,       /* response_size */
            request_completion_handler          /* completion_handler */
        );
        
    
    actual_response_length = wait_for_current_request_completion();
    
    if((DRBG_UNINST_SERV_RESP_LENGTH == actual_response_length) &&
       (DRBG_UNINSTANTIATE_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR; // TODO figure out more generic error code
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
void MSS_SYS_zeroize_device(void)
{
    /*
     * We only need to send one command byte to initiate zeroization.
     * The zeroization process should result in the Cortex-M3 being reset. This
     * means we should never reach the end of this function if zeroization is
     * actually performed.
     */
    uint8_t zeroization_req = ZEROIZATION_REQUEST_CMD;
    
    signal_request_start();
    
    MSS_COMBLK_send_cmd
        (
            &zeroization_req,                   /* p_cmd */
            sizeof(zeroization_req),            /* cmd_size */
            0,                                  /* p_data */
            0,                                  /* data_size */
            0,                                  /* p_response */
            0,                                  /* response_size */
            request_completion_handler          /* completion_handler */
        );
        
    /*
     * Handle case where zeroization is not enabled in the device.
     */
    wait_for_current_request_completion();
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
void MSS_SYS_initiate_IAP
(
    uint8_t mode,
    uint32_t bitstream_spi_addr
)
{
    uint8_t iap_prog_req[6];

    /*
     * There is no response from this system service because the Cortex-M3 will
     * get reset on completion of the system service.
     */
    iap_prog_req[0] = IAP_PROGRAMMING_REQUEST_CMD;
    iap_prog_req[1] = mode;

    iap_prog_req[2] = (uint8_t)(bitstream_spi_addr);
    iap_prog_req[3] = (uint8_t)(bitstream_spi_addr >> 8u);
    iap_prog_req[4] = (uint8_t)(bitstream_spi_addr >> 16u);
    iap_prog_req[5] = (uint8_t)(bitstream_spi_addr >> 24u);
    
    signal_request_start();
    
    MSS_COMBLK_send_cmd
        (
            iap_prog_req,                       /* p_cmd */
            sizeof(iap_prog_req),               /* cmd_size */
            0,                                  /* p_data */
            0,                                  /* data_size */
            0,                                  /* p_response */
            0,                                  /* response_size */
            request_completion_handler          /* completion_handler */
        );
        
    /*
     * Handle case where service is not implemented/enabled in the device.
     */
    wait_for_current_request_completion();
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_send_isp_bitstream
(
    uint8_t mode,
    const uint8_t * p_bitstream,
    uint32_t bitstream_length 
)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t isp_prog_request[2];
    uint8_t response[ISP_PROGRAMMING_SERV_RESP_LENGTH];
    
    signal_request_start();
    
    isp_prog_request[0] = ISP_PROGRAMMING_REQUEST_CMD;
    isp_prog_request[1] = mode;

    MSS_COMBLK_send_cmd
        (
            isp_prog_request,                   /* p_cmd */
            sizeof(isp_prog_request),           /* cmd_size */
            p_bitstream,                        /* p_data */
            bitstream_length,                   /* data_size */
            response,                           /* p_response */
            ISP_PROGRAMMING_SERV_RESP_LENGTH,   /* response_size */
            request_completion_handler          /* completion_handler */
        );
        
    
    actual_response_length = wait_for_current_request_completion();
    
    if((ISP_PROGRAMMING_SERV_RESP_LENGTH == actual_response_length) &&
       (ISP_PROGRAMMING_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR;
    }
    
    return status;
}

/*==============================================================================
 * See mss_sys_services.h for details.
 */
uint8_t MSS_SYS_check_digest
(
    uint8_t options
)
{
    uint8_t status;
    uint16_t actual_response_length;
    uint8_t digest_check_req[2];
    uint8_t response[DIGEST_CHECK_SERV_RESP_LENGTH];
    
    signal_request_start();
    
    digest_check_req[0] = DIGEST_CHECK_REQUEST_CMD;
    digest_check_req[1] = options;

    MSS_COMBLK_send_cmd
        (
            digest_check_req,                   /* p_cmd */
            sizeof(digest_check_req),           /* cmd_size */
            0,                                  /* p_data */
            0,                                  /* data_size */
            response,                           /* p_response */
            DIGEST_CHECK_SERV_RESP_LENGTH,      /* response_size */
            request_completion_handler          /* completion_handler */
        );
        
    
    actual_response_length = wait_for_current_request_completion();
    
    if((DIGEST_CHECK_SERV_RESP_LENGTH == actual_response_length) &&
       (DIGEST_CHECK_REQUEST_CMD == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR; // TODO figure out more generic error code
    }
    
    return status;
}


/*==============================================================================
 *
 */
uint8_t execute_service
(
    uint8_t cmd_opcode,
    uint8_t * cmd_params_ptr,
    uint8_t * response,
    uint16_t response_length
)
{
    uint8_t status;
    uint16_t actual_response_length;
    
    signal_request_start();
    
    MSS_COMBLK_send_cmd_with_ptr
        (
            cmd_opcode,                     /* cmd_opcode */
            (uint32_t)cmd_params_ptr,       /* cmd_params_ptr */
            response,                       /* p_response */
            response_length,                /* response_size */
            request_completion_handler      /* completion_handler */
        );
    
    actual_response_length = wait_for_current_request_completion();
    
    if((response_length == actual_response_length) && (cmd_opcode == response[0]))
    {
        status = response[1];
    }
    else
    {
        status = MSS_SYS_UNEXPECTED_ERROR; // TODO figure out more generic error code
    }
    
    return status;
}

/*==============================================================================
 *
 */
void request_completion_handler
(
    uint8_t * p_response,
    uint16_t response_size
)
{
    g_request_in_progress = 0;
    g_last_response_length = response_size;
}

/*==============================================================================
 *
 */
void signal_request_start(void)
{
    /* Wait for current request to complete. */
    while(g_request_in_progress)
    {
        ;
    }
    
    g_request_in_progress = 1;
    g_last_response_length = 0;
}

/*==============================================================================
 *
 */
uint16_t wait_for_current_request_completion(void)
{
    while(g_request_in_progress)
    {
        ;
    }
    
    return g_last_response_length;
}
