#include "stm32f10x.h"
#include "ff.h"
#include "bsp_spi_flash.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "dma.h"

FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��ӭʹ��Ұ��STM32������ �����Ǹ������ӣ�1����\r\n"; 
uint8_t by[256] = {0};
int a;
int main(void)
{
	/* ��ʼ��LED */
	LED_GPIO_Config();	
	LED_BLUE;

        //���ݷ���DMA
    //MYDMA_Config(DMA1_Channel3,(u32)&SPI1->DR,0,0);
//    //���ݽ���DMA
    //MYDMA_Config2(DMA1_Channel2,(u32)&SPI1->DR,0,0);
    
//    DMA1_Channel3->CPAR = 256;//�����ַ
//    DMA1_Channel3->CNDTR = 256;//��������
//    DMA1_Channel3->CMAR  = 256;//�ڴ��ַ
    
	//MYDMA_Config(DMA1_Channel3,(SPI1->DR);
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	USART_Config();	
    SPI_FLASH_Init();
    SPI_Flash_WAKEUP();
    MYDMA_Config(DMA1_Channel3,(u32)&SPI1->DR,(u32)by,256);
    SPI_FLASH_SectorErase(0);
    for(a = 0;a < 256;a++)
    {
        by[a] = 0x0f;
    }
    SPI_FLASH_BufferWrite(by, 0, 256);
    for(a = 0;a < 256;a++)
    {
        by[a] = 0x0;
    }
    SPI_FLASH_BufferRead(by,0,256);
    for(a = 0;a < 256;a++)
    {
        printf("%d  ",by[a]);
        //by[a] = 0x0;
    }
//  printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
//  
//	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
//	//��ʼ������������������
//	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
//	res_flash = f_mount(&fs,"1:",1);
//	
///*----------------------- ��ʽ������ -----------------*/  
//	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
//	if(res_flash == FR_NO_FILESYSTEM)
//	{
//		printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
//    /* ��ʽ�� */
//		res_flash=f_mkfs("1:",0,0);							
//		
//		if(res_flash == FR_OK)
//		{
//			printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
//      /* ��ʽ������ȡ������ */
//			res_flash = f_mount(NULL,"1:",1);			
//      /* ���¹���	*/			
//			res_flash = f_mount(&fs,"1:",1);
//		}
//		else
//		{
//			LED_RED;
//			printf("������ʽ��ʧ�ܡ�����\r\n");
//			while(1);
//		}
//	}
//  else if(res_flash!=FR_OK)
//  {
//    printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
//    printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
//		while(1);
//  }
//  else
//  {
//    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
//  }
//  
///*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/
//	/* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
//	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
//	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
//	if ( res_flash == FR_OK )
//	{
//		printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
//    /* ��ָ���洢������д�뵽�ļ��� */
//		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
//    if(res_flash==FR_OK)
//    {
//      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
//      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
//    }
//    else
//    {
//      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
//    }    
//		/* ���ٶ�д���ر��ļ� */
//    f_close(&fnew);
//	}
//	else
//	{	
//		LED_RED;
//		printf("������/�����ļ�ʧ�ܡ�\r\n");
//	}
//	
///*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
//	printf("****** ���������ļ���ȡ����... ******\r\n");
//	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_OPEN_EXISTING | FA_READ); 	 
//	if(res_flash == FR_OK)
//	{
//		LED_GREEN;
//		printf("�����ļ��ɹ���\r\n");
//		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
//    if(res_flash==FR_OK)
//    {
//      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
//      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
//    }
//    else
//    {
//      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
//    }		
//	}
//	else
//	{
//		LED_RED;
//		printf("�������ļ�ʧ�ܡ�\r\n");
//	}
//	/* ���ٶ�д���ر��ļ� */
//	f_close(&fnew);	
//  
//	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
//	f_mount(NULL,"1:",1);
  
  /* ������ɣ�ͣ�� */
	while(1)
	{
	}
}

/*********************************************END OF FILE**********************/
