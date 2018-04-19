/**
 * @file demo.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"
#include "lv_theme.h"
#include "imx6x_adc_touch.h"

#define CALIBRATION_POLL_PERIOD			20				// milliseconds
#define CALIBRATION_MINPRESS_PERIOD		300				// milliseconds
#define CALIBRATION_MAXPRESS_PERIOD		5000			// milliseconds

#define CALIBRATION_BACKGROUND			LV_COLOR_BLACK
#define CALIBRATION_CROSS_COLOR1		LV_COLOR_GREEN
#define CALIBRATION_CROSS_COLOR2		LV_COLOR_RED
#define CALIBRATION_CROSS_INNERGAP		2
#define CALIBRATION_CROSS_RADIUS		15

#define CALIBRATION_TITLE			"Calibration"
#define CALIBRATION_TITLE_Y			5
#define CALIBRATION_TITLE_HEIGHT		30
#define CALIBRATION_TITLE_COLOR			LV_COLOR_BLUE
static lv_point_t p[12][2];
static lv_style_t style_cross;
static lv_style_t style_rect;
static void lv_calibration_cross_draw(const lv_point_t *pp) {
	
	lv_style_copy(&style_cross, &lv_style_plain);
	lv_style_copy(&style_rect, &lv_style_plain);

	style_cross.line.color = LV_COLOR_GREEN;
	style_cross.line.width = 2;
	
	p[0][0].x=pp->x-CALIBRATION_CROSS_RADIUS;
	p[0][0].y=pp->y;
	p[0][1].x=pp->x-CALIBRATION_CROSS_INNERGAP;
	p[0][1].y=pp->y;
	lv_obj_t *line0 = lv_line_create(lv_scr_act(), NULL);
	lv_obj_set_pos(line0, 0, 0);
	lv_line_set_points(line0, p[0], 2);
	lv_line_set_style(line0, &style_cross);
		
	p[1][0].x=pp->x+CALIBRATION_CROSS_INNERGAP;
	p[1][0].y=pp->y;
	p[1][1].x=pp->x+CALIBRATION_CROSS_RADIUS;
	p[1][1].y=pp->y;
	lv_obj_t *line1 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line1, p[1], 2);
	lv_line_set_style(line1, &style_cross);
	
	p[2][0].x=pp->x;
	p[2][0].y=pp->y-CALIBRATION_CROSS_RADIUS;
	p[2][1].x=pp->x;
	p[2][1].y=pp->y-CALIBRATION_CROSS_INNERGAP;
	lv_obj_t *line2 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line2, p[2], 2);
	lv_line_set_style(line2, &style_cross);
		
	p[3][0].x=pp->x;
	p[3][0].y=pp->y+CALIBRATION_CROSS_INNERGAP;
	p[3][1].x=pp->x;
	p[3][1].y=pp->y+CALIBRATION_CROSS_RADIUS;
	lv_obj_t *line3 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line3, p[3], 2);
	lv_line_set_style(line3, &style_cross);
			
	style_rect.line.color = LV_COLOR_RED;
	style_rect.line.width = 2;
	p[4][0].x=pp->x-CALIBRATION_CROSS_RADIUS;
	p[4][0].y=pp->y+CALIBRATION_CROSS_RADIUS;
	p[4][1].x=pp->x-CALIBRATION_CROSS_RADIUS/2;
	p[4][1].y=pp->y+CALIBRATION_CROSS_RADIUS;
	lv_obj_t *line4 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line4, p[4], 2);
	lv_line_set_style(line4, &style_rect);
	
	p[5][0].x=pp->x-CALIBRATION_CROSS_RADIUS;
	p[5][0].y=pp->y+CALIBRATION_CROSS_RADIUS/2;
	p[5][1].x=pp->x-CALIBRATION_CROSS_RADIUS;
	p[5][1].y=pp->y+CALIBRATION_CROSS_RADIUS;	
	lv_obj_t *line5 = lv_line_create(lv_scr_act(), line4);
	lv_line_set_points(line5, p[5], 2);
	lv_line_set_style(line5, &style_rect);
	
	p[6][0].x=pp->x-CALIBRATION_CROSS_RADIUS;
	p[6][0].y=pp->y-CALIBRATION_CROSS_RADIUS;
	p[6][1].x=pp->x-CALIBRATION_CROSS_RADIUS/2;
	p[6][1].y=pp->y-CALIBRATION_CROSS_RADIUS;
	lv_obj_t *line6 = lv_line_create(lv_scr_act(), line5);
	lv_line_set_points(line6, p[6], 2);
	lv_line_set_style(line6, &style_rect);
	
	p[7][0].x=pp->x-CALIBRATION_CROSS_RADIUS;
	p[7][0].y=pp->y-CALIBRATION_CROSS_RADIUS/2;
	p[7][1].x=pp->x-CALIBRATION_CROSS_RADIUS;
	p[7][1].y=pp->y-CALIBRATION_CROSS_RADIUS;
	lv_obj_t *line7 = lv_line_create(lv_scr_act(), line6);
	lv_line_set_points(line7, p[7], 2);
	lv_line_set_style(line7, &style_rect);	
	
	p[8][0].x=pp->x+CALIBRATION_CROSS_RADIUS/2;
	p[8][0].y=pp->y+CALIBRATION_CROSS_RADIUS;
	p[8][1].x=pp->x+CALIBRATION_CROSS_RADIUS;
	p[8][1].y=pp->y+CALIBRATION_CROSS_RADIUS;
	lv_obj_t *line8 = lv_line_create(lv_scr_act(), line7);
	lv_line_set_points(line8, p[8], 2);
	lv_line_set_style(line8, &style_rect);
	
	p[9][0].x=pp->x+CALIBRATION_CROSS_RADIUS;
	p[9][0].y=pp->y+CALIBRATION_CROSS_RADIUS/2;
	p[9][1].x=pp->x+CALIBRATION_CROSS_RADIUS;
	p[9][1].y=pp->y+CALIBRATION_CROSS_RADIUS;
	lv_obj_t *line9 = lv_line_create(lv_scr_act(), line8);
	lv_line_set_points(line9, p[9], 2);
	lv_line_set_style(line9, &style_rect);
	
	p[10][0].x=pp->x+CALIBRATION_CROSS_RADIUS/2;
	p[10][0].y=pp->y-CALIBRATION_CROSS_RADIUS;
	p[10][1].x=pp->x+CALIBRATION_CROSS_RADIUS;
	p[10][1].y=pp->y-CALIBRATION_CROSS_RADIUS;
	lv_obj_t *line10 = lv_line_create(lv_scr_act(), line9);
	lv_line_set_points(line10, p[10], 2);
	lv_line_set_style(line10, &style_rect);
	
	p[11][0].x=pp->x+CALIBRATION_CROSS_RADIUS;
	p[11][0].y=pp->y-CALIBRATION_CROSS_RADIUS;
	p[11][1].x=pp->x+CALIBRATION_CROSS_RADIUS;
	p[11][1].y=pp->y-CALIBRATION_CROSS_RADIUS/2;
	lv_obj_t *line11 = lv_line_create(lv_scr_act(), line10);
	lv_line_set_points(line11, p[11], 2);
	lv_line_set_style(line11, &style_rect);
}

static void lv_calibration_cross_clear(const lv_point_t *pp) {
	lv_disp_fill(pp->x - CALIBRATION_CROSS_RADIUS-1, pp->y - CALIBRATION_CROSS_RADIUS-1, pp->x+CALIBRATION_CROSS_RADIUS+1, pp->y+CALIBRATION_CROSS_RADIUS+1, CALIBRATION_BACKGROUND);
}

static void lv_task_delay(uint32_t delay)
{
	vTaskDelay(delay);
}

bool lv_save_touch_calibration(lv_indev_t * indev,imx6x_touch_calibration_t *cal){
	imx6x_touch_private_t *private=NULL;
	
	if((indev==NULL)||(cal==NULL))
		return false;
	private=(imx6x_touch_private_t *)indev->driver.user_data;
	memcpy(&private->cal,cal,sizeof(imx6x_touch_calibration_t));
	return true;
}

static uint32_t lv_calibrate_touch() {
	int16_t	w, h;
	lv_point_t		cross[4];		// The locations of the test points on the display
	lv_point_t		points[4];		// The x, y readings obtained from the mouse for each test point
	uint32_t	err=0;
	unsigned	i, maxpoints;
	imx6x_touch_private_t *private=NULL;
	imx6x_touch_calibration_t caldata;
	lv_indev_t * indev=NULL;
	imx6x_touch_jitter_t *pj;
	
	err = 0;
	w  =  LV_HOR_RES;
	h  =  LV_VER_RES;
	
	cross[0].x = w/4;
	cross[0].y = h/4;
	cross[1].x = w-w/4;
	cross[1].y = h/4;
	cross[2].x = w-w/4;
	cross[2].y = h-h/4;
	cross[3].x = w/2;
	cross[3].y = h/2;
	
	while(indev==NULL){
		lv_task_delay(10);
		indev=lv_indev_next(NULL);
	}
	// Set up the calibration display
	lv_disp_fill(0, 0,w-1,h-1,CALIBRATION_BACKGROUND);
	private=(imx6x_touch_private_t *)indev->driver.user_data;
	private->touch_use_cal=0;
	maxpoints =  4 ;

	// Loop through the calibration points
	for(i = 0; i < maxpoints; i++) {
		int32_t		px, py;
		unsigned	j;

		// Draw the current calibration point
		lv_calibration_cross_draw(&cross[i]);

		// Get a valid "point pressed" average reading
		do {
			// Wait for the mouse to be pressed
			while(indev->proc.state==LV_INDEV_STATE_REL)
				lv_task_delay(CALIBRATION_POLL_PERIOD);

			// Sum samples taken every CALIBRATION_POLL_PERIOD milliseconds while the mouse is down
			px = py = j = 0;
			while(indev->proc.state & LV_INDEV_STATE_PR) {
				// Limit sampling period to prevent overflow
				if (j < CALIBRATION_MAXPRESS_PERIOD/CALIBRATION_POLL_PERIOD) {
					px += indev->proc.act_point.x;
					py += indev->proc.act_point.y;
					j++;
				}
				lv_task_delay(CALIBRATION_POLL_PERIOD);
			}
			// Ignore presses less than CALIBRATION_MINPRESS_PERIOD milliseconds
		} while(j < CALIBRATION_MINPRESS_PERIOD/CALIBRATION_POLL_PERIOD);
		points[i].x = px / j;
		points[i].y = py / j;
		// Clear the current calibration point
		lv_calibration_cross_clear(&cross[i]);
	}

	// Apply 3 point calibration algorithm
	imx6x_calibration_calculate(&caldata,cross, points);

	/* Verification of correctness of calibration (optional) :
	*  See if the 4th point (Middle of the screen) coincides with the calibrated
	*  result. If point is within +/- Squareroot(ERROR) pixel margin, then successful calibration
	*  Else return the error.
	*/

	// Are we in pen or finger mode
	pj = &private->jitter;

	// Transform the co-ordinates
	imx6x_calibration_transform(&points[3], &caldata);


	// Is this accurate enough?
	err = (points[3].x - cross[3].x) * (points[3].x - cross[3].x) + (points[3].y - cross[3].y) * (points[3].y - cross[3].y);
	if (err > (uint32_t)pj->calibrate * (uint32_t)pj->calibrate) {
#if GDISP_NEED_TEXT
		// No - Display error and return
		gdispGFillStringBox(m->display,0, CALIBRATION_ERROR_Y, w, CALIBRATION_ERROR_HEIGHT,CALIBRATION_ERROR_TEXT, font2,  CALIBRATION_ERROR_COLOR, CALIBRATION_ERROR_BACKGROUND,justifyCenter);
		gfxSleepMilliseconds(CALIBRATION_ERROR_DELAY);
#endif
	} else
		err = 0;
		
	// Save the calibration data (if possible)
	if (!err) {
		lv_save_touch_calibration(indev,&caldata);
	}
	private->touch_use_cal=1;
	return err;
}

void calibrate_touch_create(void)
{
	
	lv_calibrate_touch();
	
	return;
}
