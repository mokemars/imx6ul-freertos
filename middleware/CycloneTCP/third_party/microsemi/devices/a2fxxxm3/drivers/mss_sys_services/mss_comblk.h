/*******************************************************************************
 * (c) Copyright 2012 Microsemi SoC Products Group.  All rights reserved.
 *
 * SmartFusion2 COMBLK access functions.
 *
 * SVN $Revision: 4556 $
 * SVN $Date: 2012-08-23 15:22:22 +0100 (Thu, 23 Aug 2012) $
 */
#ifndef __MSS_COMBLK_H_
#define __MSS_COMBLK_H_ 1

#include "../../CMSIS/a2fxxxm3.h"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 *
 */
typedef void(*comblk_completion_handler_t)(uint8_t * p_response, uint16_t response_size);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_init(void);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_send_cmd_with_ptr
(
    uint8_t cmd_opcode,
    uint32_t cmd_params_ptr,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
);

/*------------------------------------------------------------------------------
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
);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_read
(
    const uint8_t * p_data,
    uint16_t cmd_size,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
);

#ifdef __cplusplus
}
#endif

#endif /* __MSS_COMBLK_H_ */
