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

FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash = FR_NO_FILESYSTEM;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"----   W25qXX���� FATFS�ļ�ϵͳ����Demo   ----\r\n";
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
   printf("Flash ID��: %x\r\n",w25_id);
    printf("-------������������д��Ͷ�ȡʵ��-------\r\n");
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
   printf("\r\n ����0 ����д����ȷ %d ��\r\n",len);
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
   printf("\r\n ����2040 ����д����ȷ %d ��\r\n",len);
HAL_Delay(2000);
 res_flash = f_mount(&fs,"0:",1);
 	if(res_flash == FR_NO_FILESYSTEM)
 	{
 		printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
     /* ��ʽ�� */
 		res_flash=f_mkfs("0:",0,bmp_buf,4096);							
		
 		if(res_flash == FR_OK)
 		{
 			printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
       /* ��ʽ������ȡ������ */
 			res_flash = f_mount(NULL,"0:",1);			
       /* ���¹���	*/			
 			res_flash = f_mount(&fs,"0:",1);
 		}
 		else
 		{
 			//LED_RED;
 			printf("������ʽ��ʧ�ܡ�����\r\n");
 			while(1);
 		}
 	}
   else if(res_flash!=FR_OK)
   {
     printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
     printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
 		while(1);
   }
   else
   {
     printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
   }
  
 /*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/
 	/* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
 	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
 	res_flash = f_open(&fnew, "0:1.txt",FA_CREATE_ALWAYS | FA_WRITE );
 	if ( res_flash == FR_OK )
 	{
 		printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
     /* ��ָ���洢������д�뵽�ļ��� */
 		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
     if(res_flash==FR_OK)
     {
       printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
       printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
     }
     else
     {
       printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
     }    
 		/* ���ٶ�д���ر��ļ� */
     f_close(&fnew);
 	}
 	else
 	{	
 		//LED_RED;
 		printf("������/�����ļ�ʧ�ܡ�\r\n");
 	}
	
 /*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
 	printf("****** ���������ļ���ȡ����... ******\r\n");
 	res_flash = f_open(&fnew, "0:1.txt",FA_OPEN_EXISTING | FA_READ); 	 
 	if(res_flash == FR_OK)
 	{
 		//LED_GREEN;
 		printf("�����ļ��ɹ���\r\n");
 		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
     if(res_flash==FR_OK)
     {
       printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
       printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
     }
     else
     {
       printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
     }		
 	}
 	else
 	{
 		//LED_RED;
 		printf("�������ļ�ʧ�ܡ�\r\n");
 	}
 	/* ���ٶ�д���ر��ļ� */
 	f_close(&fnew);	
  
 	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
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
