/*******************************************************************************
 * (c) Copyright 2012 Microsemi SoC Products Group.  All rights reserved.
 *
 * SmartFusion2 system services.
 *
 * SVN $Revision: 4556 $
 * SVN $Date: 2012-08-23 15:22:22 +0100 (Thu, 23 Aug 2012) $
 */
#ifndef __MSS_SYS_SERVICES_H_
#define __MSS_SYS_SERVICES_H_ 1

#include "../../CMSIS/a2fxxxm3.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
 * Status codes:
 */
/*
 * Generic status codes used across all services:
 */
#define MSS_SYS_SUCCESS                 0u
#define MSS_SYS_UNEXPECTED_ERROR        200u

#define MSS_SYS_MEM_ACCESS_ERROR             127u
#define MSS_SYS_SERVICE_DISABLED_BY_FACTORY         254u
#define MSS_SYS_SERVICE_DISABLED_BY_USER            255u

/*
 * Programming services specific status codes:
 */
#define MSS_SYS_CHAINING_MISMATCH                   1u
#define MSS_SYS_UNEXPECTED_DATA_RECEIVED            2u
#define MSS_SYS_INVALID_ENCRYPTION_KEY              3u
#define MSS_SYS_INVALID_COMPONENT_HEADER            4u
#define MSS_SYS_BACK_LEVEL_NOT_SATISFIED            5u
#define MSS_SYS_DSN_BINDING_MISMATCH                7u
#define MSS_SYS_ILLEGAL_COMPONENT_SEQUENCE          8u
#define MSS_SYS_INSUFFICIENT_DEV_CAPABILITIES       9u
#define MSS_SYS_INCORRECT_DEVICE_ID                 10u
#define MSS_SYS_UNSUPPORTED_BITSTREAM_PROT_VER      11u
#define MSS_SYS_VERIFY_NOT_PERMITTED_ON_BITSTR      12u
#define MSS_SYS_ABORT                               127u
#define MSS_SYS_NVM_VERIFY_FAILED                   129u
#define MSS_SYS_DEVICE_SECURITY_PROTECTED           130u
#define MSS_SYS_PROGRAMMING_MODE_NOT_ENABLED        131u
 
/*-------------------------------------------------------------------------*//**
   The MSS_SYS_init function initializes the system services communication with
   the System Controller.
 */
void MSS_SYS_init(void);

/*==============================================================================
 * Device and Design Information Services.
 */
 
/*-------------------------------------------------------------------------*//**
  The MSS_SYS_get_serial_number function fetches the 128-bit Device Serial
  Number (DSN).
  
  @param p_serial_number
    The p_serial_number parameter is a pointer to the 16 bytes buffer where the
    serial number will be written by this system service.
  
  @return
    The MSS_SYS_get_serial_number function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_UNEXPECTED_ERROR
 */
uint8_t MSS_SYS_get_serial_number
(
    uint8_t * p_serial_number
);

/*-------------------------------------------------------------------------*//**
   The MSS_SYS_get_user_code functions fetches the 32-bit USERCODE.
  
  @param p_user_code
    The p_user_code parameter is a pointer to the 4 bytes buffer where the
    USERCODE will be written by this system service.
  
  @return
    The MSS_SYS_get_user_code function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_UNEXPECTED_ERROR
 */
uint8_t MSS_SYS_get_user_code
(
    uint8_t * p_user_code
);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_get_design_version function fetches the design version.
  
  @param p_design_version
    The p_design_version parameter is a pointer to the 2 bytes buffer where the
    design version will be written by this system service.
  
  @return
    The MSS_SYS_get_design_version function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_UNEXPECTED_ERROR
 */
uint8_t MSS_SYS_get_design_version
(
    uint8_t * p_design_version
);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_get_device_certificate function fetches the device certificate.
  
  @param p_device_certificate
    The p_device_certificate parameter is a pointer to the 512 bytes buffer
    where the device certificate will be written by this system service.
  
  @return
    The MSS_SYS_get_device_certificate function returns one of following status
    codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_UNEXPECTED_ERROR
 */
uint8_t MSS_SYS_get_device_certificate
(
    uint8_t * p_device_certificate
);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_flash_freeze function requests the FPGA to enter the Flash Freeze
  mode.
  
  @param options
    For future enhancements. Set to zero.
    
  @return
    The MSS_SYS_flash_freeze function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_UNEXPECTED_ERROR
 */
uint8_t MSS_SYS_flash_freeze(uint8_t options);

/*==============================================================================
 * Cryptographic Services.
 */
/*------------------------------------------------------------------------------
 * AES operation modes:
 */
#define MSS_SYS_ECB_ENCRYPT     0x00u
#define MSS_SYS_ECB_DECRYPT     0x80u
#define MSS_SYS_CBC_ENCRYPT     0x01u
#define MSS_SYS_CBC_DECRYPT     0x81u
#define MSS_SYS_OFB_ENCRYPT     0x02u
#define MSS_SYS_OFB_DECRYPT     0x82u
#define MSS_SYS_CTR_ENCRYPT     0x03u
#define MSS_SYS_CTR_DECRYPT     0x83u

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_128bit_aes function provides access to the SmartFusion2 AES-128
  cryptography service.
  
  @param key
    The key parameter is a pointer to a 16 bytes array containing the key to use
    for the requested encryption/decryption operation.
  
  @param iv
    The iv parameter is a pointer to a 16 bytes array containing the
    intialization vector that will be used as part of the requested 
    encryption/decryption operation. Its use is different depending on the mode.
        -----------------------------------------
        | Mode |             Usage              |
        -----------------------------------------
        | ECB  | Ignored.                       |
        -----------------------------------------
        | CBC  | Randomization.                 |
        -----------------------------------------
        | OFB  | Randomization.                 |
        -----------------------------------------
        | CTR  | Used as initial counter value. |
        -----------------------------------------
  
  @param nb_blocks
    The nb_blocks parameter specifies the number of 128-bit blocks of
    plaintext/cyphertext requested to be processed by the AES-128 system service.
  
  @param mode
    The mode parameter specifies the cypher mode of operation and whether the
    source text must be encrypted or decrypted. The modes of operation are:
        - Electronic Codebook (ECB)
        - Cypher-Block Chaining (CBC)
        - Output Feedback (OFB)
        - Counter (CTR)
    The CTR mode uses the content of the initialization vector as its intial
    counter value. The counter increment is 2^64.
    Allowed values for the mode parameter are:
        - MSS_SYS_ECB_ENCRYPT
        - MSS_SYS_ECB_DECRYPT
        - MSS_SYS_CBC_ENCRYPT
        - MSS_SYS_CBC_DECRYPT
        - MSS_SYS_OFB_ENCRYPT
        - MSS_SYS_OFB_DECRYPT
        - MSS_SYS_CTR_ENCRYPT
        - MSS_SYS_CTR_DECRYPT
    
  @param dest_addr
    The dest_addr parameter is a pointer to the memory buffer where the result
    of the encryption/decryption operation will be stored.
  
  @param src_addr
    The src_addr parameter is a pointer to the memory buffer containg the source
    plaintext/cyphertext to be encrypted/decrypted.
  
  @return
    The MSS_SYS_128bit_aes function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_SERVICE_DISABLED_BY_USER
 */
uint8_t MSS_SYS_128bit_aes
(
    const uint8_t * key,
    const uint8_t * iv,
    uint16_t nb_blocks,
    uint8_t mode,
    uint8_t * dest_addr,
    const uint8_t * src_addr
);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_256bit_aes function provides access to the SmartFusion2 AES-256
  cryptography service.
  
  @param key
    The key parameter is a pointer to a 32 bytes array containing the key to use
    for the requested encryption/decryption operation.
  
  @param iv
    The iv parameter is a pointer to a 16 bytes array containing the
    intialization vector that will be used as part of the requested 
    encryption/decryption operation. Its use is different depending on the mode.
        -----------------------------------------
        | Mode |             Usage              |
        -----------------------------------------
        | ECB  | Ignored.                       |
        -----------------------------------------
        | CBC  | Randomization.                 |
        -----------------------------------------
        | OFB  | Randomization.                 |
        -----------------------------------------
        | CTR  | Used as initial counter value. |
        -----------------------------------------
  
  @param nb_blocks
    The nb_blocks parameter specifies the number of 128-bit blocks of
    plaintext/cyphertext requested to be processed by the AES-128 system service.
  
  @param mode
    The mode parameter specifies the cypher mode of operation and whether the
    source text must be encrypted or decrypted. The modes of operation are:
        - Electronic Codebook (ECB)
        - Cypher-Block Chaining (CBC)
        - Output Feedback (OFB)
        - Counter (CTR)
    The CTR mode uses the content of the initialization vector as its intial
    counter value. The counter increment is 2^64.
    Allowed values for the mode parameter are:
        - MSS_SYS_ECB_ENCRYPT
        - MSS_SYS_ECB_DECRYPT
        - MSS_SYS_CBC_ENCRYPT
        - MSS_SYS_CBC_DECRYPT
        - MSS_SYS_OFB_ENCRYPT
        - MSS_SYS_OFB_DECRYPT
        - MSS_SYS_CTR_ENCRYPT
        - MSS_SYS_CTR_DECRYPT
    
  @param dest_addr
    The dest_addr parameter is a pointer to the memory buffer where the result
    of the encryption/decryption operation will be stored.
  
  @param src_addr
    The src_addr parameter is a pointer to the memory buffer containg the source
    plaintext/cyphertext to be encrypted/decrypted.
  
  @return
    The MSS_SYS_256bit_aes function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_SERVICE_DISABLED_BY_USER
 */
uint8_t MSS_SYS_256bit_aes
( 
    const uint8_t * key,
    const uint8_t * iv,
    uint16_t nb_blocks,
    uint8_t mode,
    uint8_t * dest_addr,
    const uint8_t * src_addr
);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_sha256 function provides access to the SmartFusion2 SHA-256
  cryptography service.
  
  @param p_data_in
    The p_data_in parameter is a pointer to the memory location containing the
    data that will be hashed using the SHA-256 system service.
  
  @param length
    The length parameter specifies the length in bits of the data to hash.
  
  @param result
    The result parameter is a pointer to a 32 bytes buffer where the hash result
    will be stored.
  
  @return
    The MSS_SYS_sha256 function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_SERVICE_DISABLED_BY_USER
 */
uint8_t MSS_SYS_sha256
(
    const uint8_t * p_data_in,
    uint32_t length,
    uint8_t * result
);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_hmac function provides access to the SmartFusion2 HMAC
  cryptography service. The HMAC system service generates message authentication
  codes using the SHA-256 hash function.
  
  @param key
    The key parameter is a pointer to a 32 bytes array containing the key used
    to generate the message authentication code.
  
  @param p_data_in
    The p_data_in parameter is a pointer to the data to be authenticated.
  
  @param length
    The length parameter specifies the number of data bytes for which to generate
    the authentication code. It is the size of the data pointed to by the
    p_data_in parameter.
  
  @param p_result
    The p_result parameter is a pointer to a 32 bytes buffer where the
    authentication code generated by the HMAC system service will be stored.
  
  @return
    The MSS_SYS_hmac function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_SERVICE_DISABLED_BY_USER
 */
uint8_t MSS_SYS_hmac
(
    const uint8_t * key,
    const uint8_t * p_data_in,
    uint32_t length,
    uint8_t * p_result
);

/*==============================================================================
 * CRI Licensed Services.
 */
#define SYS_SERVICE_NOT_LICENCED        243u

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_CRI_key_tree function...
  
  @param p_key
  
  @param op_type
  
  @param path
  
  @return
    The MSS_SYS_CRI_key_tree function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - SYS_SERVICE_NOT_LICENCED
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_SERVICE_DISABLED_BY_USER
 */
uint8_t MSS_SYS_CRI_key_tree
(
    uint8_t * p_key,
    uint8_t op_type,
    uint8_t * path
);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_CRI_pseudo_puf_challenge function...
  
  @param p_key
  
  @param op_type
  
  @param path
  
  @return
    The MSS_SYS_CRI_pseudo_puf_challenge function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - SYS_SERVICE_NOT_LICENCED
        - MSS_SYS_MEM_ACCESS_ERROR
        - MSS_SYS_SERVICE_DISABLED_BY_USER
 */
uint8_t MSS_SYS_CRI_pseudo_puf_challenge
(
    uint8_t * p_key,
    uint8_t op_type,
    uint8_t * path
);

/*==============================================================================
 * Deterministic Random Bit Generator Services.
 */
/*------------------------------------------------------------------------------
 * Status codes specific to DRBG:
 */
#define MSS_SYS_DRBG_CATASTROPHIC_ERROR     1u
#define MSS_SYS_DRBG_MAX_INST_EXCEEDED      2u
#define MSS_SYS_DRBG_INVALID_HANDLE         3u
#define MSS_SYS_DRBG_GEN_REQ_TOO_BIG        4u
#define MSS_SYS_DRBG_MAX_LENGTH_EXCEEDED    5u

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_drbg_self_test function performs a self test of the deterministic
  random bit generator (DRBG).
  
  @return
    The MSS_SYS_drbg_self_test function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_DRBG_CATASTROPHIC_ERROR
        - MSS_SYS_DRBG_MAX_INST_EXCEEDED
        - MSS_SYS_DRBG_INVALID_HANDLE
        - MSS_SYS_DRBG_GEN_REQ_TOO_BIG
        - MSS_SYS_DRBG_MAX_LENGTH_EXCEEDED
        - MSS_SYS_SERVICE_DISABLED_BY_FACTORY
        - MSS_SYS_SERVICE_DISABLED_BY_USER
 */
uint8_t MSS_SYS_drbg_self_test(void);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_drbg_reserve function instantiates a DRBG instance.
  A maximum of two concurrent instances are available.
  
  @param personalization_str
    The personalization_str parameter is a pointer to a buffer containing a
    random bit generator personalization string. The personalization string
    can be up to 128 bytes long.
  
  @param personalization_str_length
    The personalization_str_length parameter specifies the byte length of the
    personalization string pointed to by personalization_str.
  
  @param p_drbg_handle
    The p_drbg_handle parameter is a pointer to a byte that will contain the
    handle of the instantiated DRBG if this function call suceeds.
  
  @return
    The MSS_SYS_drbg_reserve function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_DRBG_CATASTROPHIC_ERROR
        - MSS_SYS_DRBG_MAX_INST_EXCEEDED
        - MSS_SYS_DRBG_INVALID_HANDLE
        - MSS_SYS_DRBG_GEN_REQ_TOO_BIG
        - MSS_SYS_DRBG_MAX_LENGTH_EXCEEDED
        - MSS_SYS_SERVICE_DISABLED_BY_FACTORY
        - MSS_SYS_SERVICE_DISABLED_BY_USER
 */
uint8_t MSS_SYS_drbg_reserve
(
    const uint8_t * personalization_str,
    uint16_t personalization_str_length,
    uint8_t * p_drbg_handle
);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_drbg_generate function generates a random bit sequence up to
  128 bytes long.
  
  @param p_requested_data
    The p_requested_data parameter is a pointer to the buffer where the requested
    random data will be stored on completion of this system servide.
  
  @param p_additional_input
    The p_additional_input parameter is a pointer to the buffer containing
    additional input data for the random bit generation.
  
  @param requested_length
    The requested_length parameter specifies the number of random data bytes
    requested to be generated. The maximum generated data length is 128 bytes.
  
  @param additional_input_length
    The additional_input_length parameter specifies the number of addditonal
    input bytes to use in the random data generation.
  
  @param pr_req
    The pr_req parameter specifies if prediction resistance is requested.
  
  @param drbg_handle
    The drbg_handle parameter specifies which random bit generator instance
    will be used to generate the random data. The value of drbg_handle will have
    been obtained as a result of a call to the MSS_SYS_drbg_reserve() function.
  
  @return
    The MSS_SYS_drbg_generate function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_DRBG_CATASTROPHIC_ERROR
        - MSS_SYS_DRBG_MAX_INST_EXCEEDED
        - MSS_SYS_DRBG_INVALID_HANDLE
        - MSS_SYS_DRBG_GEN_REQ_TOO_BIG
        - MSS_SYS_DRBG_MAX_LENGTH_EXCEEDED
        - MSS_SYS_SERVICE_DISABLED_BY_FACTORY
        - MSS_SYS_SERVICE_DISABLED_BY_USER
   
 */
uint8_t MSS_SYS_DRBG_generate
(
    const uint8_t * p_requested_data,
    const uint8_t * p_additional_input,
    uint8_t requested_length,
    uint8_t additional_input_length,
    uint8_t pr_req,
    uint8_t drbg_handle
);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_drbg_reseed function is used to reseed the random bit generator
  identified by the drbg_handle parameter.
  
  @param p_additional_input
    The additional_input_length parameter specifies the number of addditonal
    input bytes used to reseed the random bit generator identified by the 
    drbg_handle parameter.
  
  @param additional_input_length
    The additional_input_length parameter specifies the number of addditonal
    input bytes used to reseed the random bit generator.
  
  @param drbg_handle
    The drbg_handle parameter specifies which random bit generator instance
    to reseed. The value of drbg_handle will have been obtained as a result of
    a call to the MSS_SYS_drbg_reserve() function.
  
  @return
    The MSS_SYS_drbg_reseed function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_DRBG_CATASTROPHIC_ERROR
        - MSS_SYS_DRBG_MAX_INST_EXCEEDED
        - MSS_SYS_DRBG_INVALID_HANDLE
        - MSS_SYS_DRBG_GEN_REQ_TOO_BIG
        - MSS_SYS_DRBG_MAX_LENGTH_EXCEEDED
        - MSS_SYS_SERVICE_DISABLED_BY_FACTORY
        - MSS_SYS_SERVICE_DISABLED_BY_USER
   
 */
uint8_t MSS_SYS_drbg_reseed
(
    const uint8_t * p_additional_input,
    uint8_t additional_input_length,
    uint8_t drbg_handle
);

/*-------------------------------------------------------------------------*//**
  The MSS_SYS_drbg_release function releases the random bit generator
  identified by the drbg_handle parameter.
  
  @param drbg_handle
    The drbg_handle parameter specifies which random bit generator instance
    will be uninstantiated. The value of drbg_handle will have been obtained as
    a result of a call to the MSS_SYS_drbg_reserve() function.
  
  @return
    The MSS_SYS_drbg_release function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_DRBG_CATASTROPHIC_ERROR
        - MSS_SYS_DRBG_MAX_INST_EXCEEDED
        - MSS_SYS_DRBG_INVALID_HANDLE
        - MSS_SYS_DRBG_GEN_REQ_TOO_BIG
        - MSS_SYS_DRBG_MAX_LENGTH_EXCEEDED
        - MSS_SYS_SERVICE_DISABLED_BY_FACTORY
        - MSS_SYS_SERVICE_DISABLED_BY_USER
   
 */
uint8_t MSS_SYS_drbg_release
(
    uint8_t drbg_handle
);

/*==============================================================================
 * Zeroization Services.
 */
/*-------------------------------------------------------------------------*//**
  The MSS_SYS_zeroize_device() function destroys sensitive information stored
  on the device. The level of information destroyed by this function is
  configured as part of the hardware flow of the design programmed into the
  device.
 */
void MSS_SYS_zeroize_device(void);

/*==============================================================================
 * Programming Services.
 */

#define MSS_SYS_PROG_AUTHENTICATE    0u
#define MSS_SYS_PROG_PROGRAM         1u
#define MSS_SYS_PROG_VERIFY          2u
   
/*-------------------------------------------------------------------------*//**
  The IAP Service requests the System Controller to reprogram the device using
  a bitstream already programmed into MSS SPI 0.
  
  @param mode
    The mode parameter specifies IAP service to perform. It can be one of:
        - MSS_SYS_PROG_AUTHENTICATE
        - MSS_SYS_PROG_PROGRAM
        - MSS_SYS_PROG_VERIFY
  
  @param bitstream_spi_addr
    The bitstream_spi_addr parameter specifies the address of the programming
    bitstream within the SPI flash connected to MSS SPI 0.
  
  @return
    none.
 */
void MSS_SYS_initiate_IAP
(
    uint8_t mode,
    uint32_t bitstream_spi_addr
);

/*-------------------------------------------------------------------------*//**
  The ISP Service allows the MSS M3 processor to directly provide a bitstream
  for programming. 
  
  @param mode
    The mode parameter specifies ISP service to perform. It can be one of:
        - MSS_SYS_PROG_AUTHENTICATE
        - MSS_SYS_PROG_PROGRAM
        - MSS_SYS_PROG_VERIFY
 
  @param p_bitstream
    The p_bitstream parameter is a pointer to the buffer containing the bitstream
    that will be used by the ISP service to authenticate, program or verify the
    programming of the SmartFusion 2 device.
 
  @param bitstream_length
    The bitstream_length parameter is the length in bytes of the programming
    bitstream found at the address pointed to by p_bitstream.
 
  @return
    The MSS_SYS_send_isp_bitstream function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_CHAINING_MISMATCH
        - MSS_SYS_UNEXPECTED_DATA_RECEIVED
        - MSS_SYS_INVALID_ENCRYPTION_KEY
        - MSS_SYS_INVALID_COMPONENT_HEADER
        - MSS_SYS_BACK_LEVEL_NOT_SATISFIED
        - MSS_SYS_DSN_BINDING_MISMATCH
        - MSS_SYS_ILLEGAL_COMPONENT_SEQUENCE
        - MSS_SYS_INSUFFICIENT_DEV_CAPABILITIES
        - MSS_SYS_INCORRECT_DEVICE_ID
        - MSS_SYS_UNSUPPORTED_BITSTREAM_PROT_VER
        - MSS_SYS_VERIFY_NOT_PERMITTED_ON_BITSTR
        - MSS_SYS_ABORT
        - MSS_SYS_NVM_VERIFY_FAILED
        - MSS_SYS_DEVICE_SECURITY_PROTECTED
        - MSS_SYS_PROGRAMMING_MODE_NOT_ENABLED
        - MSS_SYS_SERVICE_DISABLED_BY_USER
 */
uint8_t MSS_SYS_send_isp_bitstream
(
    uint8_t mode,
    const uint8_t * p_bitstream,
    uint32_t bitstream_length 
);

/*-------------------------------------------------------------------------*//**
  This service defines the required configuration necessary for recovering from
  a failed IAP/ISP.  This data is stored in a reserved area of eNVM and is
  erased upon successful completion of a programming operation.
  
  @param config
    <???>
 
  @param dpc
    <???>
 
  @param bitstream_spi_addr
    The bitstream_spi_addr parameter specifies the address of the programming
    bitstream within the SPI flash connected to MSS SPI 0.
 
  @return
    The MSS_SYS_set_IAP_recovery_cfg function returns one of following status codes:
        - MSS_SYS_SUCCESS
        - MSS_SYS_CHAINING_MISMATCH
        - MSS_SYS_UNEXPECTED_DATA_RECEIVED
        - MSS_SYS_INVALID_ENCRYPTION_KEY
        - MSS_SYS_INVALID_COMPONENT_HEADER
        - MSS_SYS_BACK_LEVEL_NOT_SATISFIED
        - MSS_SYS_DSN_BINDING_MISMATCH
        - MSS_SYS_ILLEGAL_COMPONENT_SEQUENCE
        - MSS_SYS_INSUFFICIENT_DEV_CAPABILITIES
        - MSS_SYS_INCORRECT_DEVICE_ID
        - MSS_SYS_UNSUPPORTED_BITSTREAM_PROT_VER
        - MSS_SYS_VERIFY_NOT_PERMITTED_ON_BITSTR
        - MSS_SYS_ABORT
        - MSS_SYS_NVM_VERIFY_FAILED
        - MSS_SYS_DEVICE_SECURITY_PROTECTED
        - MSS_SYS_PROGRAMMING_MODE_NOT_ENABLED
        - MSS_SYS_SERVICE_DISABLED_BY_USER
 */
/*
TBD. Not implemented yet in system controller.
uint8_t MSS_SYS_set_IAP_recovery_cfg
(
    uint8_t config,
    uint8_t dpc,
    uint32_t bitstream_spi_addr
);
*/
/*-------------------------------------------------------------------------*//**
  Recalculates and compares digests of selected components.
  
  @param options
    The options parameter specifies which components' digest will be recalculated
    and checked. Each bit is used to identify a componetn as follows:
        - bit 0: FPGA fabric
        - bit 1: eNVM0
        - bit 2: eNVM1
    Note: The FPGA fabric will enter the FlashFreeze state if powered up when
          its digest is checked.
 
  @return
    The MSS_SYS_check_digest function returns the result of the digest check. The
    meaning of the digest check return value is as follows:
        bit 0: Fabric digest error
        bit 1: ENVM0 digest error
        bit 2: ENVM1 digest error
    A '1' in one of the above bits indicates a digest mismatch.
 */
uint8_t MSS_SYS_check_digest
(
    uint8_t options
);

#ifdef __cplusplus
}
#endif

#endif /* __MSS_SYS_SERVICES_H_ */
