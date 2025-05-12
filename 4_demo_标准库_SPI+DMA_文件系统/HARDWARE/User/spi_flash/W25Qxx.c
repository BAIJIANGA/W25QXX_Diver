//#include "USER_BSP.h"
//#include "USER_Hardware.h"
//#include "main.h"
#include "W25Qxx.h"
#include "stdio.h"
#include "bsp_spi_flash.h"
#include "delay.h"
/* -----------W25Qxx注册 Start ------------- */
void W25Qxx_hw_DMA_SendData(uint8_t* data,uint16_t len)
{
    DMA1_Channel3->CCR &= ~0x01;
    DMA1_Channel3->CNDTR = (uint16_t)len;//传输数量
    DMA1_Channel3->CMAR  = (uint32_t)data;//内存地址
    DMA_Cmd(DMA1_Channel3, ENABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
    delay_ms(2);
    DMA_ClearFlag(DMA1_FLAG_TC3);//清除通道3传输完成标志
    DMA_Cmd(DMA1_Channel3, DISABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);
}

void W25Qxx_hw_DMA_ReadData(uint8_t* data,uint16_t len)
{
    uint8_t a = len*2/256;
    DMA_Cmd(DMA1_Channel3, DISABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);
    DMA_Cmd(DMA1_Channel2, DISABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,DISABLE);
    DMA1_Channel3->CCR &= ~0x01;
    DMA1_Channel2->CNDTR = (uint16_t)len;//传输数量
    DMA1_Channel2->CMAR  = (uint32_t)data;//内存地址
    DMA_Cmd(DMA1_Channel2, ENABLE);
    DMA1_Channel3->CCR |= 0x01;
  	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);

    DMA1_Channel3->CCR &= ~0x01;
    DMA1_Channel3->CNDTR = (uint16_t)len;//传输数量
    DMA1_Channel3->CMAR  = (uint32_t)data;//内存地址
    DMA_Cmd(DMA1_Channel3, ENABLE);
    DMA1_Channel3->CCR |= 0x01;

  	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
        delay_ms(a);
        DMA_ClearFlag(DMA1_FLAG_TC2);//清除通道3传输完成标志
         DMA_ClearFlag(DMA1_FLAG_TC3);//清除通道3传输完成标志
    DMA_Cmd(DMA1_Channel3, DISABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);
    DMA_Cmd(DMA1_Channel2, DISABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,DISABLE);
}

void W25Qxx_hw_DMA_Wait(void)
{
    return;
}
void W25Qxx_hw_CS(uint8_t data)
{
    if(data)
    {
        SPI_FLASH_CS_HIGH();
    }
    else
    {
        SPI_FLASH_CS_LOW();
    }
}

void W25Qxx_hw_DMA_Init()
{
    return;
}

void W25Qxx_hw_SendData(uint8_t* data,uint16_t len)
{
  while (len--)
  {
    /* 发送当前要写入的字节数据 */
    SPI_FLASH_SendByte(*data);
    /* 指向下一字节数据 */
    data++;
  }
}

void W25Qxx_hw_ReadData(uint8_t* data,uint16_t len)
{
  while (len--) /* while there is data to be read */
  {
    /* 读取一个字节*/
    *data = SPI_FLASH_SendByte(Dummy_Byte);
    /* 指向下一个字节缓冲区 */
    data++;
  }
}
  
W25Qxx_InitTypeDef W25Qxx_handler = 
{
    .W25Qxx_HW_Init = W25Qxx_hw_DMA_Init,
    .W25Qxx_HW_DMA_Init = W25Qxx_hw_DMA_Init,
    .W25Qxx_HW_DMA_SendData = W25Qxx_hw_DMA_SendData,
    .W25Qxx_HW_DMA_ReadData = W25Qxx_hw_DMA_ReadData,
    .W25Qxx_HW_SendData = W25Qxx_hw_SendData,
    .W25Qxx_HW_ReadData = W25Qxx_hw_ReadData,
    .W25Qxx_HW_CS = W25Qxx_hw_CS,
    .W25Qxx_HW_DMA_Wait = W25Qxx_hw_DMA_Wait,
//    .W25Qxx_SW_Init = W25Qxx_sw_Init,
//    .W25Qxx_SW_SendData = W25Qxx_sw_SendData,
//    .W25Qxx_SW_ReadData = W25Qxx_sw_ReadData,
    .W25Qxx_SW_CS = W25Qxx_hw_CS,
};

/* -----------W25Qxx注册 End ------------- */

/*-----正式代码----*/

/*******************************************************************************
 * User_W25Qxx_Init
 * 描述  ：W25QXX芯片初始化函数 使用驱动之前需要调用
 * 输入  W25Qxx_handler	 W25Qxx句柄
 * 输出  ：void
 * 调用  ：用户调用
 * 备注  ：
 *******************************************************************************/
void User_W25Qxx_Init(W25Qxx_InitTypeDef* W25Qxx_handler)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)
    
    W25Qxx_handler->W25Qxx_SW_Init();
    W25Qxx_handler->W25Qxx_SW_CS(1);
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    W25Qxx_handler->W25Qxx_HW_Init();
    W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    W25Qxx_handler->W25Qxx_HW_DMA_Init();
    W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif
    
}



/*******************************************************************************
 * 函数名：W25Qxx_WriteEnable
 * 描述  ：W25Qxx发送写使能命令
 * 输入  ：W25Qxx_handler	 W25Qxx句柄
 * 输出  ：void
 * 调用  ：内部调用
 * 备注  ：
 *******************************************************************************/
static void W25Qxx_WriteEnable(W25Qxx_InitTypeDef* W25Qxx_handler)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)
    uint8_t data = W25Qxx_Write_Enable;
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(&data,1);
    W25Qxx_handler->W25Qxx_SW_CS(1);
    #endif
    
    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t data[2] = {W25Qxx_Write_Enable,0};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
//    HAL_SPI_Transmit(&W25Qxx_SPI_handler,,1,1000);
	W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t data[2] = {W25Qxx_Write_Enable,0};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
	W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif
}

/*******************************************************************************
 * 函数名：W25Qxx_WriteDisable
 * 描述  ：W25Qxx发送禁止写使能命令
 * 输入  ：W25Qxx_handler	 W25Qxx句柄
 * 输出  ：void
 * 调用  ：内部调用
 * 备注  ：
 *******************************************************************************/
static void W25Qxx_WriteDisable(W25Qxx_InitTypeDef* W25Qxx_handler)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)
    uint8_t data = W25Qxx_Write_Disable;
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(&data,1);
    W25Qxx_handler->W25Qxx_SW_CS(1);
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t data[1] = {W25Qxx_Write_Disable};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t data[1] = {W25Qxx_Write_Disable};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif
}


/*******************************************************************************
 * 函数名：W25Qxx_WaitForBusyStatus
 * 描述  ：等待 W25Qxx 芯片的忙状态结束
 * 输入  ：W25Qxx_handler	 W25Qxx句柄
 * 输出  ：void
 * 调用  ：内部调用
 * 备注  ：
 *******************************************************************************/
static void W25Qxx_WaitForBusyStatus(W25Qxx_InitTypeDef* W25Qxx_handler)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)
    
    uint32_t Timeout = 0xFFFFFF;
    uint8_t data[2] = {W25Qxx_Read_Status_register_1,0};
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(data,1);
    while (Timeout > 0)
    {
        W25Qxx_handler->W25Qxx_SW_ReadData(data+1,1);
        if ((data[1] & 0x01) == 0)// 检查忙状态是否结束
        {
            break;
        }
        Timeout--;
        if (Timeout == 0)
        {
            printf("W25Qxx ERROR \r\n");
            break;
        }
    }
    W25Qxx_handler->W25Qxx_SW_CS(1);
    #endif
    
    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)

    uint32_t Timeout = 0xFFFFFF;
    uint8_t data[2] = {W25Qxx_Read_Status_register_1,0};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
    while (Timeout > 0)
    {
        W25Qxx_handler->W25Qxx_HW_ReadData(data+1,1);
        if ((data[1] & 0x01) == 0)// 检查忙状态是否结束
        {
            break;
        }    
        Timeout--;
        if (Timeout == 0)
        {
            printf("W25Qxx ERROR \r\n");
            break;
        }
    }
    W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)

    uint32_t Timeout = 0xFFFFFF;
    uint8_t data[2] = {W25Qxx_Read_Status_register_1,0};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
    while (Timeout > 0)
    {
        W25Qxx_handler->W25Qxx_HW_ReadData(data+1,1);
        if ((data[1] & 0x01) == 0)// 检查忙状态是否结束
        {
            break;
        }    
        Timeout--;
        if (Timeout == 0)
        {
            printf("W25Qxx ERROR \r\n");
            break;
        }
    }
    W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif
}


/*******************************************************************************
 * 函数名：W25Qxx_SectorErase
 * 描述  ：扇区擦除函数
 * 输入  ：W25Qxx_handler	 W25Qxx句柄
           Sector_Address    扇区地址(扇区号*扇区大小)
 * 输出  ：void
 * 调用  ：内部调用
 * 备注  ：在执行写入操作前要进行擦除
 *******************************************************************************/
void W25Qxx_SectorErase(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t Sector_Address)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)

    uint8_t data[4] = {(uint8_t)W25Qxx_Sector_Erase_4KB,(uint8_t)(Sector_Address >> 16),(uint8_t)(Sector_Address >> 8),(uint8_t)Sector_Address};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxx写使能
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(data,4);
    W25Qxx_handler->W25Qxx_SW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    #endif
    
    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t data[4] = {(uint8_t)W25Qxx_Sector_Erase_4KB,(uint8_t)(Sector_Address >> 16),(uint8_t)(Sector_Address >> 8),(uint8_t)Sector_Address};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxx写使能
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,4);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t data[4] = {(uint8_t)W25Qxx_Sector_Erase_4KB,(uint8_t)(Sector_Address >> 16),(uint8_t)(Sector_Address >> 8),(uint8_t)Sector_Address};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxx写使能
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,4);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    #endif
}


/*******************************************************************************
 * 函数名：W25Qxx_BlockErase
 * 描述  ：块擦除函数
 * 输入  ：W25Qxx_handler	 W25Qxx句柄
           Sector_Address    块地址(块号*块大小)
 * 输出  ：void
 * 调用  ：内部调用
 * 备注  ：在执行写入操作前要进行擦除
 *******************************************************************************/
void W25Qxx_BlockErase(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t Block_Address)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)

    uint8_t data[4] = {(uint8_t)W25Qxx_Block_Erase_64KB,(uint8_t)(Block_Address >> 16),(uint8_t)(Block_Address >> 8),(uint8_t)Block_Address};
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(data,4);
    W25Qxx_handler->W25Qxx_SW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//等待 W25Qxx 芯片的忙状态结束	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    
    #endif
    
    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t data[4] = {(uint8_t)W25Qxx_Block_Erase_64KB,(uint8_t)(Block_Address >> 16),(uint8_t)(Block_Address >> 8),(uint8_t)Block_Address};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,4);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//等待 W25Qxx 芯片的忙状态结束	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t data[4] = {(uint8_t)W25Qxx_Block_Erase_64KB,(uint8_t)(Block_Address >> 16),(uint8_t)(Block_Address >> 8),(uint8_t)Block_Address};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,4);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//等待 W25Qxx 芯片的忙状态结束	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    #endif
}

/*******************************************************************************
 * 函数名：W25Qxx_ChipErase
 * 描述  ：芯片擦除函数
 * 输入  ：W25Qxx_handler	 W25Qxx句柄		
 * 输出  ：void
 * 调用  ：内部调用
 * 备注  ：在执行写入操作前要进行擦除
 *******************************************************************************/
void W25Qxx_ChipErase(W25Qxx_InitTypeDef* W25Qxx_handler)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)

    uint8_t data[1] = {W25Qxx_Chip_Erase};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxx写使能
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(data,1);
    W25Qxx_handler->W25Qxx_SW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//等待 W25Qxx 芯片的忙状态结束	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能

    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t data[1] = {W25Qxx_Chip_Erase};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxx写使能
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//等待 W25Qxx 芯片的忙状态结束	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t data[1] = {W25Qxx_Chip_Erase};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxx写使能
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//等待 W25Qxx 芯片的忙状态结束	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    printf("caowanle\r\n");
    #endif
}

/*******************************************************************************
 * W25Qxx_SectorWrite
 * 描述  ：W25Qxx扇区写操作
 * 输入  W25Qxx_handler	 W25Qxx句柄
         Sector_start	 起始扇区
         *data1	 要写入的数据缓冲区指针。
         Sector_num	扇区数(读出多少给扇区)
 * 输出  ：void
 * 调用  ：内部调用
 * 备注  ：
 *******************************************************************************/
void W25Qxx_SectorWrite(W25Qxx_InitTypeDef* W25Qxx_handler,uint16_t Sector_start, uint8_t *data1, uint16_t Sector_num)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)
    
    uint8_t* data = data1;
    for(uint8_t a = 0;a< Sector_num;a++)//写一次擦除一遍
    {
        W25Qxx_SectorErase(W25Qxx_handler,(Sector_start+a)*4096);
        SPI_FLASH_BufferWrite(W25Qxx_handler,data,(Sector_start+a)*4096,4096);
        data += 4096;
    }
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t* data = data1;
    for(uint8_t a = 0;a< Sector_num;a++)//写一次擦除一遍
    {
        W25Qxx_SectorErase(W25Qxx_handler,(Sector_start+a)*4096);
        SPI_FLASH_BufferWrite(W25Qxx_handler,data,(Sector_start+a)*4096,4096);
        data += 4096;
    }
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t* data = data1;
    for(uint16_t a = 0;a< Sector_num;a++)//写一次擦除一遍
    {
        W25Qxx_SectorErase(W25Qxx_handler,(Sector_start+a)*4096);
        SPI_FLASH_BufferWrite1(W25Qxx_handler,data,(Sector_start+a)*4096,4096);
        if(a < Sector_num-1)
        {
        data += 4096;
        }
    }
    #endif
}


/*******************************************************************************
 * 函数名：W25Qxx_PageProgram
 * 描述  ：W25Qxx页写操作
 * 输入  ：address	 要写入的起始地址
           *data1	 要写入的数据缓冲区指针。
           dataSize	 要写入的数据大小，单位为字节。最大256否则只传输256
 * 输出  ：void
 * 调用  ：
 * 备注  ：
 *******************************************************************************/
void W25Qxx_PageWrite(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t address, uint8_t *data1, uint32_t dataSize)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)
    
    uint8_t dat[4] = {(uint8_t)W25Qxx_Page_Program,(uint8_t)((address & 0xFF0000) >> 16),(uint8_t)((address & 0xFF00) >> 8),(uint8_t)(address & 0xFF)};
    uint8_t *data = data1;
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxx写使能
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(dat,4);
    if (dataSize <= 256)
    {
        W25Qxx_handler->W25Qxx_SW_SendData(data,dataSize);
    }
    else
    {
        W25Qxx_handler->W25Qxx_SW_SendData(data,256);
    }
    W25Qxx_handler->W25Qxx_SW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//等待 W25Qxx 芯片的忙状态结束	
	W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    #endif
    
    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    
    uint8_t dat[4] = {(uint8_t)W25Qxx_Page_Program,(uint8_t)((address & 0xFF0000) >> 16),(uint8_t)((address & 0xFF00) >> 8),(uint8_t)(address & 0xFF)};
    uint8_t *data = data1;
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxx写使能
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(dat,4);
    if (dataSize <= 256)
    {
        W25Qxx_handler->W25Qxx_HW_SendData(data,dataSize);
    }
    else
    {
        W25Qxx_handler->W25Qxx_HW_SendData(data,256);
    }
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//等待 W25Qxx 芯片的忙状态结束	
	W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    
    uint8_t dat[4] = {(uint8_t)W25Qxx_Page_Program,(uint8_t)((address & 0xFF0000) >> 16),(uint8_t)((address & 0xFF00) >> 8),(uint8_t)(address & 0xFF)};
    uint8_t *data = data1;
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxx写使能
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(dat,4);
    if (dataSize <= 256)
    {

        //大于DMA最短传输长度的字节才能使用DMA传输 提高传输效率
        if(dataSize < W25Qxx_DMA_sned_LEN_MIN)
        {
            W25Qxx_handler->W25Qxx_HW_SendData(data,dataSize);
        }
        else
        {
            W25Qxx_handler->W25Qxx_HW_DMA_SendData(data,dataSize);
            W25Qxx_handler->W25Qxx_HW_DMA_Wait();//等待DMA传输完成 否则数据乱码
        }
    }
    else //大于则只传输256
    {
        W25Qxx_handler->W25Qxx_HW_DMA_SendData(data,256);
        W25Qxx_handler->W25Qxx_HW_DMA_Wait();//等待DMA传输完成 否则数据乱码
    }
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//等待 W25Qxx 芯片的忙状态结束	
	W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx禁止写使能
    #endif
}

/*******************************************************************************
 * 函数名：SPI_FLASH_BufferWrite
 * 描述  ：FLASH写操作
 * 输入  W25Qxx_handler	 W25Qxx句柄
         pBuffer	 数据指针
         WriteAddr	 写入的起始地址
         NumByteToWrite	写入的字节数
 * 输出  ：void
 * 调用  ：
 * 备注  ：
 *******************************************************************************/
void SPI_FLASH_BufferWrite1(W25Qxx_InitTypeDef* W25Qxx_handler,unsigned char* pBuffer, unsigned long WriteAddr, unsigned int NumByteToWrite)
{
  unsigned char NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;

	/*差count个数据值，刚好可以对齐到页地址*/
  count = SPI_FLASH_PageSize - Addr;
	/*计算出要写多少整数页*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	/* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if (Addr == 0)
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
        W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*先把整数页都写了*/
      while (NumOfPage--)
      {
        W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,SPI_FLASH_PageSize);
          WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
    W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,NumOfSingle);
    }
  }
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
			/*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				/*先写满当前页*/
          W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,count);
				
        WriteAddr +=  count;
        pBuffer += count;
				/*再写剩余的数据*/
          W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,temp);
      }
      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {
      W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*地址不对齐多出的count分开处理，不加入这个运算*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			/* 先写完count个数据，为的是让下一次要写的地址对齐 */
        W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,count);
      WriteAddr +=  count;
      pBuffer += count;
			/*把整数页都写了*/
      while (NumOfPage--)
      {
        W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,SPI_FLASH_PageSize);
          WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
      if (NumOfSingle != 0)
      {
          W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,NumOfSingle);
      }
    }
  }
}


/*******************************************************************************
 * W25Qxx_Sector_ReadData
 * 描述  ：W25Qxx读扇区数据
 * 输入  W25Qxx_handler	 W25Qxx句柄
         Sector_start	 起始扇区
         *data1	 要写入的数据缓冲区指针。
         Sector_num	扇区数(读出多少给扇区)
 * 输出  ：void
 * 调用  ：
 * 备注  ：
 *******************************************************************************/
void W25Qxx_Sector_ReadData(W25Qxx_InitTypeDef* W25Qxx_handler,uint16_t Sector_start, uint8_t *data1, uint16_t Sector_num)
{
    uint8_t *data = data1;
    if(Sector_num < 16)
    {
        W25Qxx_ReadData(W25Qxx_handler,Sector_start*4096,data,Sector_num*4096);
    }
    else //超出 15个扇区 分次读取
    {
        uint8_t len = Sector_num/15;
        uint8_t len1 = Sector_num%15;
        //uint8_t *data1 = data;
        for(uint8_t a = 0;a < len;a++)
        {
            W25Qxx_ReadData(W25Qxx_handler,(a+Sector_start)*15*4096,data,15*4096);
            data += 15*4096;//地址偏移
        }
        W25Qxx_ReadData(W25Qxx_handler,(Sector_start+len)*15*4096,data,len1*4096);
    }
    //data = data1;
}

/*******************************************************************************
 * 函数名：W25Qxx_ReadData
 * 描述  ：W25Qxx读数据
 * 输入  W25Qxx_handler	 W25Qxx句柄
         address	 起始地址
         *data1	 要写入的数据缓冲区指针。
         dataSize	读出的字节数
 * 输出  ：
 * 调用  ：
 * 备注  ：
 *******************************************************************************/
void W25Qxx_ReadData(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t address, uint8_t *data1, uint32_t dataSize)
{
    //uint8_t* data = data1
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)
    uint8_t* data = data1;
    uint8_t dat[4] = {(uint8_t)W25Qxx_Read_Data,(uint8_t)((address & 0xFF0000) >> 16),(uint8_t)((address & 0xFF00) >> 8),(uint8_t)(address & 0xFF)};
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(dat,4);
    W25Qxx_handler->W25Qxx_SW_ReadData(data,dataSize);
    W25Qxx_handler->W25Qxx_SW_CS(1);

    #endif
    
    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t* data = data1;
    uint8_t dat[4] = {(uint8_t)W25Qxx_Read_Data,(uint8_t)((address & 0xFF0000) >> 16),(uint8_t)((address & 0xFF00) >> 8),(uint8_t)(address & 0xFF)};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(dat,4);
    W25Qxx_handler->W25Qxx_HW_ReadData(data,dataSize);
    HAL_Delay(1);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t* data = data1;
    uint8_t dat[4] = {(uint8_t)W25Qxx_Read_Data,(uint8_t)((address & 0xFF0000) >> 16),(uint8_t)((address & 0xFF00) >> 8),(uint8_t)(address & 0xFF)};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(dat,4);
    
    if(dataSize < W25Qxx_DMA_read_LEN_MIN)
    {
        W25Qxx_handler->W25Qxx_HW_ReadData(data,dataSize);
    }
    else
    {
        W25Qxx_handler->W25Qxx_HW_DMA_ReadData(data,dataSize);
        W25Qxx_handler->W25Qxx_HW_DMA_Wait();
    }
    W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif
}



/*******************************************************************************
 * 函数名：W25Qxx_ReadID
 * 描述  ：W25Qxx读取设备ID
 * 输入  ：*ID	存储ID的变量	
 * 输出  ：void
 * 调用  ：
 * 备注  ：
 *******************************************************************************/
void W25Qxx_ReadID(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t *ID)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)
    
    W25Qxx_handler->W25Qxx_SW_CS(0);
    uint8_t data[4] = {W25Qxx_JEDEC_ID,0,0,0};
    W25Qxx_handler->W25Qxx_SW_SendData(data,1);
    W25Qxx_handler->W25Qxx_SW_ReadData(data+1,3);
    *ID = data[1];
    *ID <<= 8;
    *ID |= data[2];
    *ID <<= 8;
    *ID |= data[3];
    W25Qxx_handler->W25Qxx_SW_CS(1);

    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    W25Qxx_handler->W25Qxx_HW_CS(0);
    uint8_t data[4] = {W25Qxx_JEDEC_ID,0,0,0};
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
    W25Qxx_handler->W25Qxx_HW_ReadData(data+1,3);
    *ID = data[1];
    *ID <<= 8;
    *ID |= data[2];
    *ID <<= 8;
    *ID |= data[3];
    W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    W25Qxx_handler->W25Qxx_HW_CS(0);
    uint8_t data[4] = {W25Qxx_JEDEC_ID,0,0,0};
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
    W25Qxx_handler->W25Qxx_HW_ReadData(data+1,3);
    *ID = data[1];
    *ID <<= 8;
    *ID |= data[2];
    *ID <<= 8;
    *ID |= data[3];
    W25Qxx_handler->W25Qxx_HW_CS(1);
    #endif
}

/*******************************************************************************
 * 函数名：W25Qx_Read_Font
 * 描述  ：W25Qxx读取字库
 * 输入  W25Qxx_handler	 W25Qxx句柄
         Font	 字符指针
         Font_type	 字库尺寸
         Font_buf	字库缓冲指针
 * 输出  ：void
 * 调用  ：
 * 备注  ：
 *******************************************************************************/
void W25Qx_Read_Font(W25Qxx_InitTypeDef* W25Qxx_handler,uint8_t* Font,uint8_t Font_type,uint8_t* Font_buf)
{
    //块 扇区 页 索引字符位置
    uint8_t Po_Index = 0;
    uint16_t Po_bsp = 0;
    uint32_t Po_Font = 0x000000;

    switch(Font_type)
    {
        case 16 : //每行2个字节共16行
        {
            while(Po_Font<0x400000)
            {
                //读字节索引
                W25Qxx_ReadData(W25Qxx_handler,Font_offset+Po_Font,Font_buf,2);
                if((*Font == Font_buf[0])&&(*(Font+1) == Font_buf[1]))
                {
                    W25Qxx_ReadData(W25Qxx_handler,Font_offset+Po_Font+2,Font_buf,32);
                    return ;
                }
                Po_Index += 34;
                if(Po_Index >= 238)
                {
                    Po_Index = 0;
                    if(++Po_bsp >= 0x4000)
                    {
                        Po_bsp = 0;
                        printf("\r\n执行错误\r\n");
                    }
                }
                
                Po_Font = ((uint32_t)(Po_bsp << 8)|(Po_Index));
            }
            break;
        }
    
    }
}
