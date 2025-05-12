#include "USER_BSP.h"
#include "USER_Hardware.h"
#include "main.h"
#include "W25Qxx.h"

/* -----------W25Qxxע�� Start ------------- */
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
 * ��������W25Qxx_DATA_Init
 * ����  ��W25Qxx���ݳ�ʼ��
 * ����  ��void
 * ���  ��void
 * ����  ����ʼ��
 * ��ע  ��
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
            if((*data) & (0x80 >> i))//���½���,�������Ƶ�MOSI������
            {
                W25Qxx_DI(1);
            }
            else
            {
                W25Qxx_DI(0);
            }
            W25Qxx_SCL(1);	// �����ض�ȡ����
            W25Qxx_SCL(0);	// �½���
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
//             if(1 & (0x80 >> i))//���½���,�������Ƶ�MOSI������
//             {
//                 W25Qxx_DI(1);
//             }
//             else
//             {
//                 W25Qxx_DI(0);
//             }
            W25Qxx_SCL(1);	// �����ض�ȡ����
            if (Read_W25Qxx_DO())
            {
                (*data) |= (0x80 >> i);	//������ȡ����
            }
            W25Qxx_SCL(0);	// �½���
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
    
    __HAL_RCC_AFIO_CLK_ENABLE(); //ʹ�ܸ���ʱ��

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

    W25Qxx_SPI_handler.Init.Mode = SPI_MODE_MASTER; //����ģʽ
    W25Qxx_SPI_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; //SPI CRCӲ��У�� У���븽�����ݽ��з���
    W25Qxx_SPI_handler.Init.BaudRatePrescaler  =SPI_BAUDRATEPRESCALER_2; //�����ʷ�Ƶ 4��Ƶ 36/4 = 18MHZ flash��ߵ�18M
    W25Qxx_SPI_handler.Init.CLKPhase  =SPI_PHASE_1EDGE; //�ڼ���ʱ���ؿ�ʼ����
    W25Qxx_SPI_handler.Init.CLKPolarity = SPI_POLARITY_LOW; //SCL���߿���ʱ����Ϊ��
    W25Qxx_SPI_handler.Init.CRCPolynomial = 2; //CRCУ�����Ķ���ʽ û��Ӧ��û��
    W25Qxx_SPI_handler.Init.DataSize = SPI_DATASIZE_8BIT; //���ݷ���֡��ʽ
    W25Qxx_SPI_handler.Init.Direction = SPI_DIRECTION_2LINES; //˫��ȫ˫��
    W25Qxx_SPI_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;//���ݸ�λ��ǰ �����λ��ǰ
    W25Qxx_SPI_handler.Init.NSS = SPI_NSS_SOFT; //���Ƭѡ�� Ӳ��Ƭѡ��ΪPA4��ռ��
    W25Qxx_SPI_handler.Init.TIMode = SPI_TIMODE_DISABLE;//����ʱ�Ӽ��Ժ���λ������ ��ΪĬ������
//    SPI_handler.Init.
    W25Qxx_SPI_handler.Instance = SPI2;
    HAL_SPI_Init(&W25Qxx_SPI_handler);

   __HAL_RCC_DMA1_CLK_ENABLE();

   spi_dma_tx.Instance = DMA1_Channel5;
   spi_dma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;//�����ڴ浽����
   spi_dma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;//8λ���ݴ���
   spi_dma_tx.Init.MemInc = DMA_MINC_ENABLE; //���ݵ�ַ����
   spi_dma_tx.Init.Mode = DMA_NORMAL;
   spi_dma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;//8λ���ݴ���
   spi_dma_tx.Init.PeriphInc = DMA_PINC_DISABLE; //�Ĵ�����ַ������
   spi_dma_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

   __HAL_LINKDMA(&W25Qxx_SPI_handler,hdmatx,spi_dma_tx);

   HAL_DMA_Init(&spi_dma_tx);

    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn,3,3);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    spi_dma_rx.Instance = DMA1_Channel4;
    spi_dma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;//�������赽�ڴ�
    spi_dma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;//8λ���ݴ���
    spi_dma_rx.Init.MemInc = DMA_MINC_ENABLE; //���ݵ�ַ����
    spi_dma_rx.Init.Mode = DMA_NORMAL;
    spi_dma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;//8λ���ݴ���
    spi_dma_rx.Init.PeriphInc = DMA_PINC_DISABLE; //�Ĵ�����ַ������
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

/* -----------W25Qxxע�� End ------------- */

/*-----��ʽ����----*/

/*******************************************************************************
 * User_W25Qxx_Init
 * ����  ��W25QXXоƬ��ʼ������ ʹ������֮ǰ��Ҫ����
 * ����  W25Qxx_handler	 W25Qxx���
 * ���  ��void
 * ����  ���û�����
 * ��ע  ��
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
 * ��������W25Qxx_WriteEnable
 * ����  ��W25Qxx����дʹ������
 * ����  ��W25Qxx_handler	 W25Qxx���
 * ���  ��void
 * ����  ���ڲ�����
 * ��ע  ��
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
 * ��������W25Qxx_WriteDisable
 * ����  ��W25Qxx���ͽ�ֹдʹ������
 * ����  ��W25Qxx_handler	 W25Qxx���
 * ���  ��void
 * ����  ���ڲ�����
 * ��ע  ��
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
 * ��������W25Qxx_WaitForBusyStatus
 * ����  ���ȴ� W25Qxx оƬ��æ״̬����
 * ����  ��W25Qxx_handler	 W25Qxx���
 * ���  ��void
 * ����  ���ڲ�����
 * ��ע  ��
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
        if ((data[1] & 0x01) == 0)// ���æ״̬�Ƿ����
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
        if ((data[1] & 0x01) == 0)// ���æ״̬�Ƿ����
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
        if ((data[1] & 0x01) == 0)// ���æ״̬�Ƿ����
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
 * ��������W25Qxx_SectorErase
 * ����  ��������������
 * ����  ��W25Qxx_handler	 W25Qxx���
           Sector_Address    ������ַ(������*������С)
 * ���  ��void
 * ����  ���ڲ�����
 * ��ע  ����ִ��д�����ǰҪ���в���
 *******************************************************************************/
void W25Qxx_SectorErase(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t Sector_Address)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)

    uint8_t data[4] = {(uint8_t)W25Qxx_Sector_Erase_4KB,(uint8_t)(Sector_Address >> 16),(uint8_t)(Sector_Address >> 8),(uint8_t)Sector_Address};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxxдʹ��
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(data,4);
    W25Qxx_handler->W25Qxx_SW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    #endif
    
    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t data[4] = {(uint8_t)W25Qxx_Sector_Erase_4KB,(uint8_t)(Sector_Address >> 16),(uint8_t)(Sector_Address >> 8),(uint8_t)Sector_Address};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxxдʹ��
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,4);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t data[4] = {(uint8_t)W25Qxx_Sector_Erase_4KB,(uint8_t)(Sector_Address >> 16),(uint8_t)(Sector_Address >> 8),(uint8_t)Sector_Address};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxxдʹ��
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,4);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    #endif
}


/*******************************************************************************
 * ��������W25Qxx_BlockErase
 * ����  �����������
 * ����  ��W25Qxx_handler	 W25Qxx���
           Sector_Address    ���ַ(���*���С)
 * ���  ��void
 * ����  ���ڲ�����
 * ��ע  ����ִ��д�����ǰҪ���в���
 *******************************************************************************/
void W25Qxx_BlockErase(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t Block_Address)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)

    uint8_t data[4] = {(uint8_t)W25Qxx_Block_Erase_64KB,(uint8_t)(Block_Address >> 16),(uint8_t)(Block_Address >> 8),(uint8_t)Block_Address};
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(data,4);
    W25Qxx_handler->W25Qxx_SW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//�ȴ� W25Qxx оƬ��æ״̬����	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    
    #endif
    
    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t data[4] = {(uint8_t)W25Qxx_Block_Erase_64KB,(uint8_t)(Block_Address >> 16),(uint8_t)(Block_Address >> 8),(uint8_t)Block_Address};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,4);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//�ȴ� W25Qxx оƬ��æ״̬����	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t data[4] = {(uint8_t)W25Qxx_Block_Erase_64KB,(uint8_t)(Block_Address >> 16),(uint8_t)(Block_Address >> 8),(uint8_t)Block_Address};
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,4);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//�ȴ� W25Qxx оƬ��æ״̬����	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    #endif
}

/*******************************************************************************
 * ��������W25Qxx_ChipErase
 * ����  ��оƬ��������
 * ����  ��W25Qxx_handler	 W25Qxx���		
 * ���  ��void
 * ����  ���ڲ�����
 * ��ע  ����ִ��д�����ǰҪ���в���
 *******************************************************************************/
void W25Qxx_ChipErase(W25Qxx_InitTypeDef* W25Qxx_handler)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)

    uint8_t data[1] = {W25Qxx_Chip_Erase};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxxдʹ��
    W25Qxx_handler->W25Qxx_SW_CS(0);
    W25Qxx_handler->W25Qxx_SW_SendData(data,1);
    W25Qxx_handler->W25Qxx_SW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//�ȴ� W25Qxx оƬ��æ״̬����	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��

    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t data[1] = {W25Qxx_Chip_Erase};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxxдʹ��
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//�ȴ� W25Qxx оƬ��æ״̬����	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t data[1] = {W25Qxx_Chip_Erase};
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxxдʹ��
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(data,1);
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//�ȴ� W25Qxx оƬ��æ״̬����	
    W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    printf("caowanle\r\n");
    #endif
}

/*******************************************************************************
 * W25Qxx_SectorWrite
 * ����  ��W25Qxx����д����
 * ����  W25Qxx_handler	 W25Qxx���
         Sector_start	 ��ʼ����
         *data1	 Ҫд������ݻ�����ָ�롣
         Sector_num	������(�������ٸ�����)
 * ���  ��void
 * ����  ���ڲ�����
 * ��ע  ��
 *******************************************************************************/
void W25Qxx_SectorWrite(W25Qxx_InitTypeDef* W25Qxx_handler,uint16_t Sector_start, uint8_t *data1, uint16_t Sector_num)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)
    
    uint8_t* data = data1;
    for(uint8_t a = 0;a< Sector_num;a++)//дһ�β���һ��
    {
        W25Qxx_SectorErase(W25Qxx_handler,(Sector_start+a)*4096);
        SPI_FLASH_BufferWrite(W25Qxx_handler,data,(Sector_start+a)*4096,4096);
        data += 4096;
    }
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    uint8_t* data = data1;
    for(uint8_t a = 0;a< Sector_num;a++)//дһ�β���һ��
    {
        W25Qxx_SectorErase(W25Qxx_handler,(Sector_start+a)*4096);
        SPI_FLASH_BufferWrite(W25Qxx_handler,data,(Sector_start+a)*4096,4096);
        data += 4096;
    }
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    uint8_t* data = data1;
    for(uint16_t a = 0;a< Sector_num;a++)//дһ�β���һ��
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
 * ��������W25Qxx_PageProgram
 * ����  ��W25Qxxҳд����
 * ����  ��address	 Ҫд�����ʼ��ַ
           *data1	 Ҫд������ݻ�����ָ�롣
           dataSize	 Ҫд������ݴ�С����λΪ�ֽڡ����256����ֻ����256
 * ���  ��void
 * ����  ��
 * ��ע  ��
 *******************************************************************************/
void W25Qxx_PageWrite(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t address, uint8_t *data1, uint32_t dataSize)
{
    #if (W25Qxx_USE == W25Qxx_USE_SW_SPI)
    
    uint8_t dat[4] = {(uint8_t)W25Qxx_Page_Program,(uint8_t)((address & 0xFF0000) >> 16),(uint8_t)((address & 0xFF00) >> 8),(uint8_t)(address & 0xFF)};
    uint8_t *data = data1;
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxxдʹ��
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
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//�ȴ� W25Qxx оƬ��æ״̬����	
	W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    #endif
    
    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI)
    
    uint8_t dat[4] = {(uint8_t)W25Qxx_Page_Program,(uint8_t)((address & 0xFF0000) >> 16),(uint8_t)((address & 0xFF00) >> 8),(uint8_t)(address & 0xFF)};
    uint8_t *data = data1;
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxxдʹ��
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
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//�ȴ� W25Qxx оƬ��æ״̬����	
	W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    #endif

    #if (W25Qxx_USE == W25Qxx_USE_HW_SPI_DMA)
    
    uint8_t dat[4] = {(uint8_t)W25Qxx_Page_Program,(uint8_t)((address & 0xFF0000) >> 16),(uint8_t)((address & 0xFF00) >> 8),(uint8_t)(address & 0xFF)};
    uint8_t *data = data1;
    W25Qxx_WriteEnable(W25Qxx_handler);//W25Qxxдʹ��
    W25Qxx_handler->W25Qxx_HW_CS(0);
    W25Qxx_handler->W25Qxx_HW_SendData(dat,4);
    if (dataSize <= 256)
    {

        //����DMA��̴��䳤�ȵ��ֽڲ���ʹ��DMA���� ��ߴ���Ч��
        if(dataSize < W25Qxx_DMA_sned_LEN_MIN)
        {
            W25Qxx_handler->W25Qxx_HW_SendData(data,dataSize);
        }
        else
        {
            W25Qxx_handler->W25Qxx_HW_DMA_SendData(data,dataSize);
            W25Qxx_handler->W25Qxx_HW_DMA_Wait();//�ȴ�DMA������� ������������
        }
    }
    else //������ֻ����256
    {
        W25Qxx_handler->W25Qxx_HW_DMA_SendData(data,256);
        W25Qxx_handler->W25Qxx_HW_DMA_Wait();//�ȴ�DMA������� ������������
    }
    W25Qxx_handler->W25Qxx_HW_CS(1);
    W25Qxx_WaitForBusyStatus(W25Qxx_handler);//�ȴ� W25Qxx оƬ��æ״̬����	
	W25Qxx_WriteDisable(W25Qxx_handler);//W25Qxx��ֹдʹ��
    #endif
}

/*******************************************************************************
 * ��������SPI_FLASH_BufferWrite
 * ����  ��FLASHд����
 * ����  W25Qxx_handler	 W25Qxx���
         pBuffer	 ����ָ��
         WriteAddr	 д�����ʼ��ַ
         NumByteToWrite	д����ֽ���
 * ���  ��void
 * ����  ��
 * ��ע  ��
 *******************************************************************************/
void SPI_FLASH_BufferWrite(W25Qxx_InitTypeDef* W25Qxx_handler,unsigned char* pBuffer, unsigned long WriteAddr, unsigned int NumByteToWrite)
{
  unsigned char NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;

	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = SPI_FLASH_PageSize - Addr;
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	/* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0)
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
        W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*�Ȱ�����ҳ��д��*/
      while (NumOfPage--)
      {
        W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,SPI_FLASH_PageSize);
          WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
    W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,NumOfSingle);
    }
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				/*��д����ǰҳ*/
          W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,count);
				
        WriteAddr +=  count;
        pBuffer += count;
				/*��дʣ�������*/
          W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,temp);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {
      W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			/* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
        W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,count);
      WriteAddr +=  count;
      pBuffer += count;
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,SPI_FLASH_PageSize);
          WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
          W25Qxx_PageWrite(W25Qxx_handler,WriteAddr,pBuffer,NumOfSingle);
      }
    }
  }
}


/*******************************************************************************
 * W25Qxx_Sector_ReadData
 * ����  ��W25Qxx����������
 * ����  W25Qxx_handler	 W25Qxx���
         Sector_start	 ��ʼ����
         *data1	 Ҫд������ݻ�����ָ�롣
         Sector_num	������(�������ٸ�����)
 * ���  ��void
 * ����  ��
 * ��ע  ��
 *******************************************************************************/
void W25Qxx_Sector_ReadData(W25Qxx_InitTypeDef* W25Qxx_handler,uint16_t Sector_start, uint8_t *data1, uint16_t Sector_num)
{
    uint8_t *data = data1;
    if(Sector_num < 16)
    {
        W25Qxx_ReadData(W25Qxx_handler,Sector_start*4096,data,Sector_num*4096);
    }
    else //���� 15������ �ִζ�ȡ
    {
        uint8_t len = Sector_num/15;
        uint8_t len1 = Sector_num%15;
        //uint8_t *data1 = data;
        for(uint8_t a = 0;a < len;a++)
        {
            W25Qxx_ReadData(W25Qxx_handler,(a+Sector_start)*15*4096,data,15*4096);
            data += 15*4096;//��ַƫ��
        }
        W25Qxx_ReadData(W25Qxx_handler,(Sector_start+len)*15*4096,data,len1*4096);
    }
    //data = data1;
}

/*******************************************************************************
 * ��������W25Qxx_ReadData
 * ����  ��W25Qxx������
 * ����  W25Qxx_handler	 W25Qxx���
         address	 ��ʼ��ַ
         *data1	 Ҫд������ݻ�����ָ�롣
         dataSize	�������ֽ���
 * ���  ��
 * ����  ��
 * ��ע  ��
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
 * ��������W25Qxx_ReadID
 * ����  ��W25Qxx��ȡ�豸ID
 * ����  ��*ID	�洢ID�ı���	
 * ���  ��void
 * ����  ��
 * ��ע  ��
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
 * ��������W25Qx_Read_Font
 * ����  ��W25Qxx��ȡ�ֿ�
 * ����  W25Qxx_handler	 W25Qxx���
         Font	 �ַ�ָ��
         Font_type	 �ֿ�ߴ�
         Font_buf	�ֿ⻺��ָ��
 * ���  ��void
 * ����  ��
 * ��ע  ��
 *******************************************************************************/
void W25Qx_Read_Font(W25Qxx_InitTypeDef* W25Qxx_handler,uint8_t* Font,uint8_t Font_type,uint8_t* Font_buf)
{
    //�� ���� ҳ �����ַ�λ��
    uint8_t Po_Index = 0;
    uint16_t Po_bsp = 0;
    uint32_t Po_Font = 0x000000;

    switch(Font_type)
    {
        case 16 : //ÿ��2���ֽڹ�16��
        {
            while(Po_Font<0x400000)
            {
                //���ֽ�����
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
                        printf("\r\nִ�д���\r\n");
                    }
                }
                
                Po_Font = ((uint32_t)(Po_bsp << 8)|(Po_Index));
            }
            break;
        }
    
    }
}
