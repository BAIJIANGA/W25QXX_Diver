
#include "delay.h"
#include "sys.h"
#include "bsp_spi_flash.h"
#include "bsp_usart.h"
#include "ff.h"
#include "dma.h"
#include "W25Qxx.h"
uint8_t by[500] = {0};
int a = 0;

FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"标准库spi dma读写 + FATFS测试 \r\n"; 
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
   printf("Flash ID号: %x\r\n",w25_id);
   printf("-----------W25QXX驱动库读写测试-----------\r\n");
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
   len = 0;
    SPI_FLASH_SectorErase(0);
    printf("\r\n -----------标准库驱动读写开始 -----------\r\n");
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
    printf("\r\n 扇区0 数据写入正确 %d 个\r\n",len);
    printf("\r\n -----------读写结束----------- \r\n");

printf("****** 这是一个SPI FLASH 文件系统实验 ******\r\n");
  
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	//初始化函数调用流程如下
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"1:",1);

/*----------------------- 格式化测试 -----------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
    /* 格式化 */
		res_flash=f_mkfs("1:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("》FLASH已成功格式化文件系统。\r\n");
      /* 格式化后，先取消挂载 */
			res_flash = f_mount(NULL,"1:",1);			
      /* 重新挂载	*/			
			res_flash = f_mount(&fs,"1:",1);
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
	res_flash = f_open(&fnew, "1:1.txt",FA_CREATE_ALWAYS | FA_WRITE );
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
	res_flash = f_open(&fnew, "1:1.txt",FA_OPEN_EXISTING | FA_READ); 	 
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
	f_mount(NULL,"1:",1);
    while(1);
}

