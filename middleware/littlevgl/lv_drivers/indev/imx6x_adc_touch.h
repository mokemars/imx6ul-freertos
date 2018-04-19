/**
 * @file th2046.h
 * 
 */

#ifndef IMX6X_ADC_TOUCH_H
#define IMX6X_ADC_TOUCH_H

/*********************
 *      INCLUDES
 *********************/
#include "lv_drv_conf.h"
#if IMX6X_ADC_TOUCH != 0

#include <stdint.h>
#include <stdbool.h>
#include "lvgl/lv_hal/lv_hal_indev.h"

//add by mokemars 2018.3.22
typedef struct imx6x_touch_calibration {
	float	ax;
	float	bx;
	float	cx;
	float	ay;
	float	by;
	float	cy;
} imx6x_touch_calibration_t;

typedef struct imx6x_touch_jitter {
	int16_t		calibrate;// Maximum error for a calibration to succeed
	int16_t		click;// Movement allowed without discarding the CLICK or CLICKCXT event
	int16_t		move;// Movement allowed without discarding the MOVE event
}imx6x_touch_jitter_t;

typedef struct imx6x_touch_private{
	imx6x_touch_calibration_t cal;
	imx6x_touch_jitter_t jitter;
	int16_t touch_use_cal;
}imx6x_touch_private_t;
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void imx6x_adc_touch_init(void);
bool imx6x_adc_touch_read(lv_indev_data_t * data);
void imx6x_calibration_calculate(imx6x_touch_calibration_t *cal,const lv_point_t *cross, const lv_point_t *points);
void imx6x_calibration_transform(lv_point_t *pt, const imx6x_touch_calibration_t *c);

/**********************
 *      MACROS
 **********************/

#endif

#endif
