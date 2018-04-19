/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_uart_freertos.h"
#include "fsl_uart.h"
#include "fsl_epit.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "imx6x_elcdif.h"
#include "imx6x_adc_touch.h"
#include "lvgl/lvgl.h"
#include "demo.h"
#include "ts_calibrate.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_UART UART1
#define DEMO_UART_CLK_FREQ BOARD_DebugConsoleSrcFreq()
#define DEMO_IRQn UART1_IRQn
/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)
#define gui_task_PRIORITY (configMAX_PRIORITIES - 2)

/*******************************************************************************
 * Code
 ******************************************************************************/

/*Provides own implementation of vApplicationIRQHandler() to call SystemIrqHandler()
 *directly without saving the FPU registers on interrupt entry.
 */
void vApplicationIRQHandler(uint32_t ulICCIAR)
{
	SystemIrqHandler(ulICCIAR);
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )

{
	PRINTF("Task:%s Stack Overflow\r\n", pcTaskName);
	while(1);
}

void vApplicationMallocFailedHook(){
	PRINTF("Malloc Failed\r\n");
	while(1);
}

void vApplicationTickHook( void ){
	lv_tick_inc(1);
}

unsigned char gui_init_sucess=0;
void guiTask(void *param)
{
	lv_theme_t * th = lv_theme_night_init(210, &lv_font_dejavu_20);     //Set a HUE value and a Font for the Night Theme
	lv_theme_set_current(th);     
	lv_init();
	imx6x_elcdif_init();
	imx6x_adc_touch_init();
	demo_create();
	gui_init_sucess=1;
	while(1)
	{
		vTaskDelay(5);
		lv_task_handler(); 
	}
}

void calibrateTask(void *param){
	while(gui_init_sucess==0)
	{
		vTaskDelay(1);
	}
	calibrate_touch_create();
	while(1)
	{
		vTaskDelay(1);
	}
}

/*!
 * @brief Application entry point.
 */
int main(void)
{
	
	/* Init board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitMemory();
	debugInit(115200);
	/* Install IRQ Handler */
	SystemInitIrqTable();
	//Create a task to gui task
	if (xTaskCreate(guiTask,"GUI", 4096, NULL, gui_task_PRIORITY, NULL) != pdPASS)
	{
		PRINTF("Task gui creation failed!.\r\n");
	}
	
	/*if (xTaskCreate(calibrateTask,"calibrate", 512, NULL, gui_task_PRIORITY, NULL) != pdPASS)
	{
		PRINTF("Task calibrate creation failed!.\r\n");
	}*/
	vTaskStartScheduler();
	for (;;);
}
