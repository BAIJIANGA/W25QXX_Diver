
#include "delay.h"
#include "sys.h"
#include "bsp_spi_flash.h"
#include "bsp_usart.h"
#include "ff.h"
#include "dma.h"
#include "W25Qxx.h"
uint8_t by[500] = {0};
int a = 0;

FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��׼��spi dma��д + FATFS���� \r\n"; 
uint8_t by1[256] = {0};
uint8_t bmp_buf[5000] = {0};
extern W25Qxx_InitTypeDef W25Qxx_handler;
int main(void)
{
	delay_init();
    USART_Config();	
    SPI_FLASH_Init();
    delay_ms(5);
    SPI_Flash_WAKEUP();
    MYDMA_Config(DMA1_Channel3,(u32)&SPI1->DR,(u32)by,256);
    MYDMA_Config2(DMA1_Channel2,(u32)&SPI1->DR,(u32)by,256);
    uint32_t w25_id;
    W25Qxx_ReadID(&W25Qxx_handler,&w25_id);
   printf("Flash ID��: %x\r\n",w25_id);
   printf("-----------W25QXX�������д����-----------\r\n");
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
   len = 0;
    SPI_FLASH_SectorErase(0);
    printf("\r\n -----------��׼��������д��ʼ -----------\r\n");
    for(a = 0;a < 500;a++)
    {
        by[a] = 0x16;
    }
    SPI_FLASH_BufferWrite(by, 0, 500);
    for(a = 0;a < 500;a++)
    {
        by[a] = 0x0;
    }
    SPI_FLASH_BufferRead(by,0,500);
    for(a = 0;a < 500;a++)
    {
        printf("%X  ",by[a]);
       if(by[a] == 0x16)
       {
           len++;
       }
        //by[a] = 0x0;
    }
    printf("\r\n ����0 ����д����ȷ %d ��\r\n",len);
    printf("\r\n -----------��д����----------- \r\n");

printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
  
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	//��ʼ������������������
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"1:",1);

/*----------------------- ��ʽ������ -----------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
    /* ��ʽ�� */
		res_flash=f_mkfs("1:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
      /* ��ʽ������ȡ������ */
			res_flash = f_mount(NULL,"1:",1);			
      /* ���¹���	*/			
			res_flash = f_mount(&fs,"1:",1);
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
	res_flash = f_open(&fnew, "1:1.txt",FA_CREATE_ALWAYS | FA_WRITE );
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
	res_flash = f_open(&fnew, "1:1.txt",FA_OPEN_EXISTING | FA_READ); 	 
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
	f_mount(NULL,"1:",1);
    while(1);
}

