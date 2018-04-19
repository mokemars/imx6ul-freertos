#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "event.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_debug_console.h"

#include "fsl_usdhc.h"
#include "fsl_card.h"
#define SD_DISK_TEST_ENABLE 0
extern void GPIO_IRQ_Handler();
extern void USDHC1_DriverIRQHandler(void);
#if SD_DISK_TEST_ENABLE
#define SD_TEST_BUFFER_SIZE 128
uint8_t g_SdBufferWrite[SD_TEST_BUFFER_SIZE];;
uint8_t g_SdBufferRead[SD_TEST_BUFFER_SIZE];
static FATFS g_SdFileSystem;
#endif

void SdInitPins(void) {                                /*!< Function assigned for the core: Cortex-A7[ca7] */
	IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0U);
	IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_SD1_CLK_USDHC1_CLK, 0U);
	IOMUXC_SetPinConfig(IOMUXC_SD1_CLK_USDHC1_CLK, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(1U) |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_SD1_CMD_USDHC1_CMD, 0U);
	IOMUXC_SetPinConfig(IOMUXC_SD1_CMD_USDHC1_CMD, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_SD1_DATA0_USDHC1_DATA0, 0U);
	IOMUXC_SetPinConfig(IOMUXC_SD1_DATA0_USDHC1_DATA0, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_SD1_DATA1_USDHC1_DATA1, 0U);
	IOMUXC_SetPinConfig(IOMUXC_SD1_DATA1_USDHC1_DATA1, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_SD1_DATA2_USDHC1_DATA2, 0U);
	IOMUXC_SetPinConfig(IOMUXC_SD1_DATA2_USDHC1_DATA2, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_SD1_DATA3_USDHC1_DATA3, 0U);
	IOMUXC_SetPinConfig(IOMUXC_SD1_DATA3_USDHC1_DATA3, 
		IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_DSE(1U) |
		IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PUS(1U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
	IOMUXC_SetPinMux(IOMUXC_UART1_RTS_B_GPIO1_IO19, 0U);
	IOMUXC_SetPinConfig(IOMUXC_UART1_RTS_B_GPIO1_IO19, 
		IOMUXC_SW_PAD_CTL_PAD_SPEED(2U) |
		IOMUXC_SW_PAD_CTL_PAD_PUS(2U) |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK);
}

#if SD_DISK_TEST_ENABLE
static int SdTest(){
	FIL g_fileObject;   /* File object */
	FRESULT error;
	DIR directory; /* Directory object */
	FILINFO fileInformation;
	UINT bytesWritten;
	UINT bytesRead;
	const TCHAR sdDriverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
	volatile bool failedFlag = false;
	BYTE work[_MAX_SS];
	
	if (f_mount(&g_SdFileSystem, sdDriverNumberBuffer, 0U))
	{
		PRINTF("Mount volume failed.\r\n");
		return -1;
	}
#if (_FS_RPATH >= 2U)
	error = f_chdrive((char const *)&sdDriverNumberBuffer[0U]);
	if (error)
	{
		PRINTF("Change drive failed.\r\n");
		return -2;
	}
#endif
#if _USE_MKFS
	/*TRACE_INFO("\r\nMake file system......The time may be long if the card capacity is big.\r\n");
	if (f_mkfs(sdDriverNumberBuffer, FM_ANY, 0U, work, sizeof work))
	{
		TRACE_ERROR("Make file system failed.\r\n");
		vTaskSuspend(NULL);
	}*/
#endif /* _USE_MKFS */
	PRINTF("\r\nCreate directory......\r\n");
	error = f_mkdir(_T("/dir_1"));
	if (error)
	{
		if (error == FR_EXIST)
		{
			PRINTF("Directory exists.\r\n");
		}
		else
		{
			PRINTF("Make directory failed.\r\n");
			return -1;
		}
	}

	PRINTF("\r\nCreate a file in that directory......\r\n");
	error = f_open(&g_fileObject, _T("/dir_1/f_1.dat"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
	if (error)
	{
		if (error == FR_EXIST)
		{
			PRINTF("File exists.\r\n");
		}
		else
		{
			PRINTF("Open file failed.\r\n");
			return -1;
		}
	}

	PRINTF("\r\nCreate a directory in that directory......\r\n");
	error = f_mkdir(_T("/dir_1/dir_2"));
	if (error)
	{
		if (error == FR_EXIST)
		{
			PRINTF("Directory exists.\r\n");
		}
		else
		{
			PRINTF("Directory creation failed.\r\n");
			return -1;
		}
	}

	PRINTF("\r\nList the file in that directory......\r\n");
	if (f_opendir(&directory, "/dir_1"))
	{
		PRINTF("Open directory failed.\r\n");
		return -1;
	}

	for (;;)
	{
		error = f_readdir(&directory, &fileInformation);

		/* To the end. */
		if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
		{
			break;
		}
		if (fileInformation.fname[0] == '.')
		{
			continue;
		}
		if (fileInformation.fattrib & AM_DIR)
		{
			PRINTF("Directory file : %s.\r\n", fileInformation.fname);
		}
		else
		{
			PRINTF("General file : %s.\r\n", fileInformation.fname);
		}
	}

	memset(g_SdBufferWrite, 'a', sizeof(g_SdBufferWrite));
	g_SdBufferWrite[SD_TEST_BUFFER_SIZE - 2U] = '\r';
	g_SdBufferWrite[SD_TEST_BUFFER_SIZE - 1U] = '\n';

	PRINTF("\r\nWrite/read file until encounters error......\r\n");
        PRINTF("\r\nWrite to above created file.\r\n");
        error = f_write(&g_fileObject, g_SdBufferWrite, sizeof(g_SdBufferWrite), &bytesWritten);
        if ((error) || (bytesWritten != sizeof(g_SdBufferWrite)))
        {
		PRINTF("Write file failed. \r\n");
		failedFlag = true;
        }

	/* Move the file pointer */
	if (f_lseek(&g_fileObject, 0U))
	{
		PRINTF("Set file pointer position failed. \r\n");
		failedFlag = true;
	}

	PRINTF("Read from above created file.\r\n");
	memset(g_SdBufferRead, 0U, sizeof(g_SdBufferRead));
	error = f_read(&g_fileObject, g_SdBufferRead, sizeof(g_SdBufferRead), &bytesRead);
	if ((error) || (bytesRead != sizeof(g_SdBufferRead)))
	{
		PRINTF("Read file failed. \r\n");
		failedFlag = true;
	}

        PRINTF("Compare the read/write content......\r\n");
	if (memcmp(g_SdBufferWrite, g_SdBufferRead, sizeof(g_SdBufferWrite)))
	{
		PRINTF("Compare read/write content isn't consistent.\r\n");
		failedFlag = true;
        }
        PRINTF("The read/write content is consistent.\r\n");
    
	if (f_close(&g_fileObject))
	{
		PRINTF("\r\nClose file failed.\r\n");
		return -1;
	}
	return 0;
}
#endif

int InitSD(){
	/* Tricky here: As IRQ handler in example doesn't use parameters, just ignore them */
	SdInitPins();
	CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    	SystemInstallIrqHandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)GPIO_IRQ_Handler, NULL);
	GIC_SetPriority(USDHC1_IRQn, 21); 
        GIC_EnableIRQ(USDHC1_IRQn); 
#if SD_DISK_TEST_ENABLE
	SdTest();
#endif
	return 0;
}