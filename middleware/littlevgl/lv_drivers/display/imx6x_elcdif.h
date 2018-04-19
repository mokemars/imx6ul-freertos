/**
 * @file ILI9320.h
 * 
 */

#ifndef IMX6X_ELCDIF_H
#define IMX6X_ELCDIF_H

/*********************
 *      INCLUDES
 *********************/
#include "lv_drv_conf.h"
#if USE_IMX6X_ELCDIF != 0

#include <stdint.h>
#include "lvgl/lv_misc/lv_color.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void imx6x_elcdif_init(void);
void imx6x_elcdif_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
void imx6x_elcdif_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t  color);
void imx6x_elcdif_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);

/**********************
 *      MACROS
 **********************/

#endif

#endif
