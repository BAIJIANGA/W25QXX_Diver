#include "USER_BSP.h"
#include "USER_Hardware.h"
#include "main.h"
#include "W25Qxx.h"

/* -----------W25Qxx注册 Start ------------- */
SPI_HandleTypeDef W25Qxx_SPI_handler = {0};
DMA_HandleTypeDef spi_dma_tx = {0};
DMA_HandleTypeDef spi_dma_rx = {0};

void W25Qxx_hw_DMA_Init(void);
void W25Qxx_hw_ReadData(uint8_t* data,uint16_t len);
void W25Qxx_hw_SendData(uint8_t* data,uint16_t len);
void W25Qxx_hw_DMA_Wait(void);
void W25Qxx_hw_CS(uint8_t data);

void W25Qxx_hw_DMA_SendData(uint8_t* data,uint16_t len)
{
    //HAL_SPI_Transmit(&W25Qxx_SPI_handler,data,len,1000);
    HAL_SPI_Transmit_DMA(&W25Qxx_SPI_handler,data,len);
}

void W25Qxx_hw_DMA_ReadData(uint8_t* data,uint16_t len)
{
    //HAL_SPI_Receive(&W25Qxx_SPI_handler,data,len,100000);
    HAL_SPI_Receive_DMA(&W25Qxx_SPI_handler,data,len);
//    HAL_SPI_Transmit(&W25Qxx_SPI_handler,data,len,100);
}

void W25Qxx_GPIO_Init(void)
{
    W25Qxx_SCL_RCC_ENABLE();
    W25Qxx_DO_RCC_ENABLE();
    W25Qxx_DI_RCC_ENABLE();
    W25Qxx_CS_RCC_ENABLE();
    W25Qxx_IO2_RCC_ENABLE();
    W25Qxx_IO3_RCC_ENABLE();

    GPIO_InitTypeDef gpio_handler = {0};
    
    gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_handler.Pin = W25Qxx_SCL_PIN;
    gpio_handler.Pull = GPIO_NOPULL;
    gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(W25Qxx_SCL_GPIO,&gpio_handler);

    gpio_handler.Pin = W25Qxx_DI_PIN;
    HAL_GPIO_Init(W25Qxx_DI_GPIO,&gpio_handler);

    gpio_handler.Pin = W25Qxx_CS_PIN;
    HAL_GPIO_Init(W25Qxx_CS_GPIO,&gpio_handler);

    gpio_handler.Pin = W25Qxx_IO2_PIN;
    HAL_GPIO_Init(W25Qxx_IO2_GPIO,&gpio_handler);

    gpio_handler.Pin = W25Qxx_IO3_PIN;
    HAL_GPIO_Init(W25Qxx_IO3_GPIO,&gpio_handler);

    gpio_handler.Mode = GPIO_MODE_INPUT;
    gpio_handler.Pin = W25Qxx_DO_PIN;
    HAL_GPIO_Init(W25Qxx_DO_GPIO,&gpio_handler);
}

/*******************************************************************************
 * 函数名：W25Qxx_DATA_Init
 * 描述  ：W25Qxx数据初始化
 * 输入  ：void
 * 输出  ：void
 * 调用  ：初始化
 * 备注  ：
*******************************************************************************/
void W25Qxx_DATA_Init(void)
{
    W25Qxx_CS(1);
}

void W25Qxx_sw_Init()
{
    W25Qxx_GPIO_Init();
    W25Qxx_DATA_Init();
}

void W25Qxx_sw_SendData(uint8_t* data1,uint16_t len)
{
    uint16_t j = 0 ,i=0;
    uint8_t* data = data1;
    for(j = 0;j < len;j++)
    {
        //uint8_t i, Outdata = 0x00;	
        for (i = 0; i < 8; i ++)
        {
            if((*data) & (0x80 >> i))//在下降沿,把数据移到MOSI总线上
            {
                W25Qxx_DI(1);
            }
            else
            {
                W25Qxx_DI(0);
            }
            W25Qxx_SCL(1);	// 上升沿读取数据
            W25Qxx_SCL(0);	// 下降沿
        }	
        if(j < len-1)
        {
        data++;
        }
    }
}

void W25Qxx_sw_ReadData(uint8_t* data1,uint16_t len)
{
    uint16_t j = 0,i = 0;
    uint8_t* data = data1;
    * data = 0x00;
    for(j = 0;j < len;j++)
    {
        //uint8_t i, Outdata = 0x00;	
        for (i = 0; i < 8; i ++)
        {
//             if(1 & (0x80 >> i))//在下降沿,把数据移到MOSI总线上
//             {
//                 W25Qxx_DI(1);
//             }
//             else
//             {
//                 W25Qxx_DI(0);
//             }
            W25Qxx_SCL(1);	// 上升沿读取数据
            if (Read_W25Qxx_DO())
            {
                (*data) |= (0x80 >> i);	//掩码提取数据
            }
            W25Qxx_SCL(0);	// 下降沿
        }
//        i = 0;
        if(j < len-1)
        {
        data=data+1;
        (* data) = 0x00;
        }
    }
//    HAL_SPI_Receive(&W25Qxx_SPI_handler,data,len,100000);
//    HAL_SPI_Transmit(&W25Qxx_SPI_handler,data,len,100);
}

void W25Qxx_hw_DMA_Init()
{
    GPIO_InitTypeDef GPIO_handler = {0};
    
    __HAL_RCC_AFIO_CLK_ENABLE(); //使能复用时钟

    W25Qxx_SCL_RCC_ENABLE();
    W25Qxx_DO_RCC_ENABLE();
    W25Qxx_DI_RCC_ENABLE();
    W25Qxx_CS_RCC_ENABLE();
    W25Qxx_IO2_RCC_ENABLE();
    W25Qxx_IO3_RCC_ENABLE();
    
    GPIO_handler.Mode = GPIO_MODE_AF_PP;
    GPIO_handler.Pin = W25Qxx_SCL_PIN;
    GPIO_handler.Pull = GPIO_NOPULL;//GPIO_PULLDOWN;GPIO_PULLUP
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(W25Qxx_SCL_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = W25Qxx_DI_PIN;
    HAL_GPIO_Init(W25Qxx_DI_GPIO,&GPIO_handler);

    GPIO_handler.Mode = GPIO_MODE_AF_INPUT;
    GPIO_handler.Pin = W25Qxx_DO_PIN;//
    HAL_GPIO_Init(W25Qxx_DO_GPIO,&GPIO_handler);
    
    GPIO_handler.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_handler.Pin = W25Qxx_CS_PIN;//
    HAL_GPIO_Init(W25Qxx_CS_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = W25Qxx_IO2_PIN;//
    HAL_GPIO_Init(W25Qxx_IO2_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = W25Qxx_IO3_PIN;//
    HAL_GPIO_Init(W25Qxx_IO3_GPIO,&GPIO_handler);

    __HAL_RCC_SPI2_CLK_ENABLE();

    W25Qxx_SPI_handler.Init.Mode = SPI_MODE_MASTER; //主机模式
    W25Qxx_SPI_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; //SPI CRC硬件校验 校验码附带数据进行发送
    W25Qxx_SPI_handler.Init.BaudRatePrescaler  =SPI_BAUDRATEPRESCALER_2; //波特率分频 4分频 36/4 = 18MHZ flash最高到18M
    W25Qxx_SPI_handler.Init.CLKPhase  =SPI_PHASE_1EDGE; //第几个时钟沿开始采样
    W25Qxx_SPI_handler.Init.CLKPolarity = SPI_POLARITY_LOW; //SCL总线空闲时极性为低
    W25Qxx_SPI_handler.Init.CRCPolynomial = 2; //CRC校验计算的多项式 没打开应该没用
    W25Qxx_SPI_handler.Init.DataSize = SPI_DATASIZE_8BIT; //数据发送帧格式
    W25Qxx_SPI_handler.Init.Direction = SPI_DIRECTION_2LINES; //双线全双工
    W25Qxx_SPI_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;//数据高位在前 必须高位在前
    W25Qxx_SPI_handler.Init.NSS = SPI_NSS_SOFT; //软件片选脚 硬件片选脚为PA4被占用
    W25Qxx_SPI_handler.Init.TIMode = SPI_TIMODE_DISABLE;//禁用时钟极性和相位的设置 即为默认设置
//    SPI_handler.Init.
    W25Qxx_SPI_handler.Instance = SPI2;
    HAL_SPI_Init(&W25Qxx_SPI_handler);

   __HAL_RCC_DMA1_CLK_ENABLE();

   spi_dma_tx.Instance = DMA1_Channel5;
   spi_dma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;//方向内存到外设
   spi_dma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;//8位数据传输
   spi_dma_tx.Init.MemInc = DMA_MINC_ENABLE; //数据地址递增
   spi_dma_tx.Init.Mode = DMA_NORMAL;
   spi_dma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;//8位数据传输
   spi_dma_tx.Init.PeriphInc = DMA_PINC_DISABLE; //寄存器地址不递增
   spi_dma_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

   __HAL_LINKDMA(&W25Qxx_SPI_handler,hdmatx,spi_dma_tx);

   HAL_DMA_Init(&spi_dma_tx);

    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn,3,3);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    spi_dma_rx.Instance = DMA1_Channel4;
    spi_dma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;//方向外设到内存
    spi_dma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;//8位数据传输
    spi_dma_rx.Init.MemInc = DMA_MINC_ENABLE; //数据地址递增
    spi_dma_rx.Init.Mode = DMA_NORMAL;
    spi_dma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;//8位数据传输
    spi_dma_rx.Init.PeriphInc = DMA_PINC_DISABLE; //寄存器地址不递增
    spi_dma_rx.Init.Priority = DMA_PRIORITY_MEDIUM;

   __HAL_LINKDMA(&W25Qxx_SPI_handler,hdmarx,spi_dma_rx);
   HAL_DMA_Init(&spi_dma_rx);

   HAL_NVIC_SetPriority(DMA1_Channel5_IRQn,1,1);
   HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}


void W25Qxx_hw_DMA_Wait()
{
    HAL_Delay(2);
}

void W25Qxx_hw_SendData(uint8_t* data,uint16_t len)
{
    HAL_SPI_Transmit(&W25Qxx_SPI_handler,data,len,1000);
}

void W25Qxx_hw_ReadData(uint8_t* data,uint16_t len)
{
    HAL_SPI_Receive(&W25Qxx_SPI_handler,data,len,100000);
//    HAL_SPI_Transmit(&W25Qxx_SPI_handler,data,len,100);
}

void W25Qxx_hw_CS(uint8_t data)
{
    W25Qxx_CS(data);
//    HAL_SPI_Transmit(&W25Qxx_SPI_handler,data,len,100);
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
    .W25Qxx_SW_Init = W25Qxx_sw_Init,
    .W25Qxx_SW_SendData = W25Qxx_sw_SendData,
    .W25Qxx_SW_ReadData = W25Qxx_sw_ReadData,
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
        SPI_FLASH_BufferWrite(W25Qxx_handler,data,(Sector_start+a)*4096,4096);
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
void SPI_FLASH_BufferWrite(W25Qxx_InitTypeDef* W25Qxx_handler,unsigned char* pBuffer, unsigned long WriteAddr, unsigned int NumByteToWrite)
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
