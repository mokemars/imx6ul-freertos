#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "SlaveCanOpen.h"

unsigned char digital_input[1] = {0};
unsigned char digital_output[1] = {0};
static QueueHandle_t xSlaveEventQueue;
static TimerHandle_t canIoTimer;

static unsigned char get_inputs(){
	unsigned char value=0;
	
	if(GPIO_ReadPinInput(GPIO4,25)){
		value|=0x1;
	}
	if(GPIO_ReadPinInput(GPIO4,26)){
		value|=0x2;
	}
	if(GPIO_ReadPinInput(GPIO4,27)){
		value|=0x4;
	}
	if(GPIO_ReadPinInput(GPIO4,28)){
		value|=0x8;
	}
	if(GPIO_ReadPinInput(GPIO1,9)){
		value|=0x10;
	}
	if(GPIO_ReadPinInput(GPIO1,5)){
		value|=0x20;
	}
	if(GPIO_ReadPinInput(GPIO5,0)){
		value|=0x40;
	}
	if(GPIO_ReadPinInput(GPIO5,1)){
		value|=0x80;
	}
	return value;
}

static  void set_outputs(unsigned char value){
	if(value&0x1){
		GPIO_WritePinOutput(GPIO3, 21, 1);
	}else{
		GPIO_WritePinOutput(GPIO3, 21, 0);
	}
	if(value&0x2){
		GPIO_WritePinOutput(GPIO3, 22, 1);
	}else{
		GPIO_WritePinOutput(GPIO3, 22, 0);
	}
	if(value&0x4){
		GPIO_WritePinOutput(GPIO3, 23, 1);
	}else{
		GPIO_WritePinOutput(GPIO3, 23, 0);
	}
	if(value&0x8){
		GPIO_WritePinOutput(GPIO3, 24, 1);
	}else{
		GPIO_WritePinOutput(GPIO3, 24, 0);
	}
	if(value&0x10){
		GPIO_WritePinOutput(GPIO3, 25, 1);
	}else{
		GPIO_WritePinOutput(GPIO3, 25, 0);
	}
	if(value&0x20){
		GPIO_WritePinOutput(GPIO3, 26, 1);
	}else{
		GPIO_WritePinOutput(GPIO3, 26, 0);
	}
	if(value&0x40){
		GPIO_WritePinOutput(GPIO3, 27, 1);
	}else{
		GPIO_WritePinOutput(GPIO3, 27, 0);
	}
	if(value&0x80){
		GPIO_WritePinOutput(GPIO3, 28, 1);
	}else{
		GPIO_WritePinOutput(GPIO3, 28, 0);
	}
}

unsigned char DigitalInputHandler(CO_Data* d, unsigned char *newInput, unsigned char size)
{
	unsigned char loops, i, input, transmission = 0;

	loops = (sizeof(Read_Inputs_8_Bit) <= size) ? sizeof(Read_Inputs_8_Bit) : size;

	for (i=0; i < loops; i++)
	{
		input = *newInput ^ Polarity_Input_8_Bit[i];
		if (Read_Inputs_8_Bit[i] != input)
		{
			if (Global_Interrupt_Enable_Digital)
			{
				if ((Interrupt_Mask_Any_Change_8_Bit[i] & (Read_Inputs_8_Bit[i] ^ input)) 
					|| (Interrupt_Mask_Low_to_High_8_Bit[i] & ~Read_Inputs_8_Bit[i] & input)
					|| (Interrupt_Mask_High_to_Low_8_Bit[i] & Read_Inputs_8_Bit[i] & ~input))
					transmission = 1;
			}
			// update object dict
			Read_Inputs_8_Bit[i] = input;
		}
		newInput++;
	}
	if (transmission)
	{
		/* force emission of PDO by artificially changing last emitted*/
		d->PDO_status[0].last_message.cob_id = 0;
		setState(d, Operational);
		sendPDOevent(d);
	}
	return 1;
}

unsigned char DigitalOutputHandler(CO_Data* d, unsigned char *newOutput, unsigned char size)
{
	unsigned char loops, i, error, type;
	unsigned int varsize = 1;

	loops = (sizeof(Write_Outputs_8_Bit) <= size) ? sizeof(Write_Outputs_8_Bit) : size;

	for (i=0; i < loops; i++)
	{
		getODentry(d, 0x1001, 0x0, &error, &varsize, &type, RO);
		if ((getState(d) == Stopped) || (error != 0))	// node stopped or error
		{
			Write_Outputs_8_Bit[i] &= (~Error_Mode_Outputs_8_Bit[i] | Error_Value_Outputs_8_Bit[i]);
			Write_Outputs_8_Bit[i] |= (Error_Mode_Outputs_8_Bit[i] & Error_Value_Outputs_8_Bit[i]);
		}
		*newOutput = Write_Outputs_8_Bit[i] ^ Change_Polarity_Outputs_8_Bit[i];
		newOutput++;
	}
	return 1;
}

unsigned char AnalogInputHandler(CO_Data* d, unsigned int *newInput, unsigned char size)
{
	return 0;
}

unsigned char AnalogOutputHandler(CO_Data* d, unsigned int *newOutput, unsigned char size)
{
	return 0;
}


void Can_IO_InitPins(void) {                                /*!< Function assigned for the core: Cortex-A7[ca7] */
	//output io
  	IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_GPIO3_IO21, 0U);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_GPIO3_IO21, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
  	IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_GPIO3_IO22, 0U);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_GPIO3_IO22, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
  	IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_GPIO3_IO23, 0U);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_GPIO3_IO23, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
  	IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_GPIO3_IO24, 0U);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_GPIO3_IO24, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
  	IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_GPIO3_IO25, 0U);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_GPIO3_IO25, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
  	IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_GPIO3_IO26, 0U);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_GPIO3_IO26, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
  	IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_GPIO3_IO27, 0U);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_GPIO3_IO27, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
  	IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0U);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	//INPUT
	IOMUXC_SetPinMux(IOMUXC_CSI_DATA04_GPIO4_IO25, 0U);
	IOMUXC_SetPinConfig(IOMUXC_CSI_DATA04_GPIO4_IO25, 
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_CSI_DATA05_GPIO4_IO26, 0U);
	IOMUXC_SetPinConfig(IOMUXC_CSI_DATA05_GPIO4_IO26, 
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_CSI_DATA06_GPIO4_IO27, 0U);
	IOMUXC_SetPinConfig(IOMUXC_CSI_DATA06_GPIO4_IO27, 
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_CSI_DATA07_GPIO4_IO28, 0U);
	IOMUXC_SetPinConfig(IOMUXC_CSI_DATA07_GPIO4_IO28, 
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09, 
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO05_GPIO1_IO05, 0U);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO05_GPIO1_IO05, 
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_SNVS_TAMPER0_GPIO5_IO00, 0U);
	IOMUXC_SetPinConfig(IOMUXC_SNVS_TAMPER0_GPIO5_IO00, 
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_SNVS_TAMPER1_GPIO5_IO01, 0U);
	IOMUXC_SetPinConfig(IOMUXC_SNVS_TAMPER1_GPIO5_IO01, 
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
}

void canIoTimerHandler(void)
{
	digital_input[0] = get_inputs();
	DigitalInputHandler(&SlaveDict_Data, digital_input, sizeof(digital_input));
	DigitalOutputHandler(&SlaveDict_Data, digital_output, sizeof(digital_output));
	set_outputs(digital_output[0]);
}

void CanOpenSlaveBaseInit(){
	
  	gpio_pin_config_t config = {
        	kGPIO_DigitalOutput, 0,
	};
	Can_IO_InitPins();
	GPIO_PinInit(GPIO3, 21, &config);
	GPIO_WritePinOutput(GPIO3, 21, 0);
	GPIO_PinInit(GPIO3, 22, &config);
	GPIO_WritePinOutput(GPIO3, 22, 0);
	GPIO_PinInit(GPIO3, 23, &config);
	GPIO_WritePinOutput(GPIO3, 23, 0);
	GPIO_PinInit(GPIO3, 24, &config);
	GPIO_WritePinOutput(GPIO3, 24, 0);
	GPIO_PinInit(GPIO3, 25, &config);
	GPIO_WritePinOutput(GPIO3, 25, 0);
	GPIO_PinInit(GPIO3, 26, &config);
	GPIO_WritePinOutput(GPIO3, 26, 0);
	GPIO_PinInit(GPIO3, 27, &config);
	GPIO_WritePinOutput(GPIO3, 27, 0);
	GPIO_PinInit(GPIO3, 28, &config);
	GPIO_WritePinOutput(GPIO3, 28, 0);
	
	config.direction=kGPIO_DigitalInput;
	GPIO_PinInit(GPIO1, 9, &config);
	GPIO_PinInit(GPIO1, 5, &config);
	GPIO_PinInit(GPIO5, 0, &config);
	GPIO_PinInit(GPIO5, 1, &config);
	GPIO_PinInit(GPIO4, 25, &config);
	GPIO_PinInit(GPIO4, 26, &config);
	GPIO_PinInit(GPIO4, 27, &config);
	GPIO_PinInit(GPIO4, 28, &config);

	canIoTimer = xTimerCreate("CAN IO",
                   	100,//1 tick产生一个timerhandler
                   	pdTRUE,/*是否自动重载， 此处设置周期性执行*/
			( void * ) 0,/*记录定时器溢出次数， 初始化零, 用户自己设置*/
			canIoTimerHandler/*回调函数*/
			);
	if(canIoTimer!=NULL){
		xTimerStart(canIoTimer, 0 );
	}
}
