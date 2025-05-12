/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "USER_BSP.h"
#include "USER_Hardware.h"
#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t addr1_num;
uint32_t addr1;
uint32_t addr;
uint8_t flag_tr = 1; 
uint8_t flag_wri ;
uint16_t len_wri;
uint8_t buf1[256];
uint8_t buf2[256];
uint8_t* ptr_buf;
uint8_t bmp_buf[40960];
uint8_t* ptr_bmp = bmp_buf;
extern const unsigned char gImage_q[38080];
extern UART_HandleTypeDef usart_handler;
/* USER CODE END 0 */

FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash = FR_NO_FILESYSTEM;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"----   W25qXX驱动 FATFS文件系统测试Demo   ----\r\n";
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  Debug_Init();
  User_W25Qxx_Init(&W25Qxx_handler);
  /* USER CODE END 2 */
   uint32_t w25_id;
   W25Qxx_ReadID(&W25Qxx_handler,&w25_id);
   printf("Flash ID号: %x\r\n",w25_id);
    printf("-------即将进行扇区写入和读取实验-------\r\n");
    HAL_Delay(2000);
   for(int a = 0;a < 4096;a++)
   {
       bmp_buf[a] = 0x77;
   }
   W25Qxx_SectorErase(&W25Qxx_handler,0);
   W25Qxx_SectorWrite(&W25Qxx_handler,0,bmp_buf,1);
   for(int a = 0;a < 4096;a++)
   {
       bmp_buf[a] = 0;
   }
   W25Qxx_Sector_ReadData(&W25Qxx_handler,0,bmp_buf,1);
   uint16_t len = 0;
   for(int a = 0;a < 4096;a++)
   {
       printf("%x  ",bmp_buf[a]);
       if(bmp_buf[a] == 0x77)
       {
           len++;
       }
   }
   printf("\r\n 扇区0 数据写入正确 %d 个\r\n",len);
HAL_Delay(2000);
   len = 0;
   for(int a = 0;a < 4096;a++)
   {
       bmp_buf[a] = 0x99;
   }
   
   W25Qxx_SectorErase(&W25Qxx_handler,2040<<12);
   W25Qxx_SectorWrite(&W25Qxx_handler,2040,bmp_buf,1);
   for(int a = 0;a < 4096;a++)
   {
       bmp_buf[a] = 0;
   }
   W25Qxx_Sector_ReadData(&W25Qxx_handler,2040,bmp_buf,1);
   for(int a = 0;a < 4096;a++)
   {
       printf("%x  ",bmp_buf[a]);
       if(bmp_buf[a] == 0x99)
       {
           len++;
       }
   }
   printf("\r\n 扇区2040 数据写入正确 %d 个\r\n",len);
HAL_Delay(2000);
 res_flash = f_mount(&fs,"0:",1);
 	if(res_flash == FR_NO_FILESYSTEM)
 	{
 		printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
     /* 格式化 */
 		res_flash=f_mkfs("0:",0,bmp_buf,4096);							
		
 		if(res_flash == FR_OK)
 		{
 			printf("》FLASH已成功格式化文件系统。\r\n");
       /* 格式化后，先取消挂载 */
 			res_flash = f_mount(NULL,"0:",1);			
       /* 重新挂载	*/			
 			res_flash = f_mount(&fs,"0:",1);
 		}
 		else
 		{
 			//LED_RED;
 			printf("《《格式化失败。》》\r\n");
 			while(1);
 		}
 	}
   else if(res_flash!=FR_OK)
   {
     printf("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
     printf("！！可能原因：SPI Flash初始化不成功。\r\n");
 		while(1);
   }
   else
   {
     printf("》文件系统挂载成功，可以进行读写测试\r\n");
   }
  
 /*----------------------- 文件系统测试：写测试 -------------------*/
 	/* 打开文件，每次都以新建的形式打开，属性为可写 */
 	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
 	res_flash = f_open(&fnew, "0:1.txt",FA_CREATE_ALWAYS | FA_WRITE );
 	if ( res_flash == FR_OK )
 	{
 		printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
     /* 将指定存储区内容写入到文件内 */
 		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
     if(res_flash==FR_OK)
     {
       printf("》文件写入成功，写入字节数据：%d\n",fnum);
       printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
     }
     else
     {
       printf("！！文件写入失败：(%d)\n",res_flash);
     }    
 		/* 不再读写，关闭文件 */
     f_close(&fnew);
 	}
 	else
 	{	
 		//LED_RED;
 		printf("！！打开/创建文件失败。\r\n");
 	}
	
 /*------------------- 文件系统测试：读测试 --------------------------*/
 	printf("****** 即将进行文件读取测试... ******\r\n");
 	res_flash = f_open(&fnew, "0:1.txt",FA_OPEN_EXISTING | FA_READ); 	 
 	if(res_flash == FR_OK)
 	{
 		//LED_GREEN;
 		printf("》打开文件成功。\r\n");
 		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
     if(res_flash==FR_OK)
     {
       printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
       printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
     }
     else
     {
       printf("！！文件读取失败：(%d)\n",res_flash);
     }		
 	}
 	else
 	{
 		//LED_RED;
 		printf("！！打开文件失败。\r\n");
 	}
 	/* 不再读写，关闭文件 */
 	f_close(&fnew);	
  
 	/* 不再使用文件系统，取消挂载文件系统 */
 	f_mount(NULL,"0:",1);  
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
      
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
