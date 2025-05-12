#include "lcd_init.h"
#include "main.h"
extern UART_HandleTypeDef uart_handler;
#if LCD_TFT_14_DRIVER == LCD_TFT_HW_SPI

SPI_HandleTypeDef SPI_handler = {0};

#endif

#if LCD_TFT_14_DRIVER == LCD_TFT_HW_DMA_SPI

SPI_HandleTypeDef SPI_handler = {0};
DMA_HandleTypeDef dma_hander_tx = {0};
DMA_HandleTypeDef dma_hander_rx = {0};

void DMA1_Channel3_IRQHandler()
{
    HAL_DMA_IRQHandler(&dma_hander_tx);
}

void DMA1_Channel2_IRQHandler()
{
    HAL_DMA_IRQHandler(&dma_hander_rx);
}
extern uint8_t finsh;
//SPI DMA�������
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI1)
    {
        LCD_CS_Set();
    }
}

void LCD_SPI_DMA_0()
{
    __HAL_RCC_DMA1_CLK_ENABLE();

//    HAL_DMA_DeInit(&dma_hander_tx);
    dma_hander_tx.Instance = DMA1_Channel3;
    dma_hander_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    dma_hander_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD; //((uint32_t)DMA_CCR_MSIZE_0);//16λ���ݴ���
    dma_hander_tx.Init.MemInc = DMA_MINC_DISABLE; //���ݲ�����
    dma_hander_tx.Init.Mode = DMA_NORMAL;
    dma_hander_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; //((uint32_t)DMA_CCR_MSIZE_0);//16λ���ݴ���
    dma_hander_tx.Init.PeriphInc = DMA_PINC_DISABLE; //�Ĵ�����ַ������
    dma_hander_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

    __HAL_LINKDMA(&SPI_handler,hdmatx,dma_hander_tx);
    HAL_DMA_Init(&dma_hander_tx);
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn,0,0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}

void LCD_SPI_DMA_INIT()
{
    __HAL_RCC_DMA1_CLK_ENABLE();

//    HAL_DMA_DeInit(&dma_hander_tx);
    dma_hander_tx.Instance = DMA1_Channel3;
    dma_hander_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    dma_hander_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;//((uint32_t)DMA_CCR_MSIZE_0); //8λ���ݴ���
    dma_hander_tx.Init.MemInc = DMA_MINC_DISABLE; //���ݲ�����
    dma_hander_tx.Init.Mode = DMA_NORMAL;
    dma_hander_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;//((uint32_t)DMA_CCR_MSIZE_0); //8λ���ݴ���
    dma_hander_tx.Init.PeriphInc = DMA_PINC_DISABLE; //�Ĵ�����ַ������
    dma_hander_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

    __HAL_LINKDMA(&SPI_handler,hdmatx,dma_hander_tx);
    HAL_DMA_Init(&dma_hander_tx);
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn,0,0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}

void LCD_SPI_DMA_1()
{
    __HAL_RCC_DMA1_CLK_ENABLE();

    HAL_DMA_DeInit(&dma_hander_tx);
    dma_hander_tx.Instance = DMA1_Channel3;
    dma_hander_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    dma_hander_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;//0;//((uint32_t)DMA_CCR_MSIZE_0);//8λ���ݴ���
    dma_hander_tx.Init.MemInc = DMA_MINC_ENABLE; //���ݵ���
    dma_hander_tx.Init.Mode = DMA_NORMAL;
    dma_hander_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;//0;//((uint32_t)DMA_CCR_MSIZE_0);//8λ���ݴ���
    dma_hander_tx.Init.PeriphInc = DMA_PINC_DISABLE; //�Ĵ�����ַ������
    dma_hander_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

    __HAL_LINKDMA(&SPI_handler,hdmatx,dma_hander_tx);
    HAL_DMA_Init(&dma_hander_tx);
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn,0,0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}

#endif


void LCD_GPIO_Init(void)
{
#if LCD_TFT_14_DRIVER == LCD_TFT_HW_DMA_SPI
    GPIO_InitTypeDef GPIO_handler = {0};
    __HAL_RCC_AFIO_CLK_ENABLE(); //ʹ�ܸ���ʱ��

    RCC_LCD_SCK_ENABLE();
    RCC_LCD_SDA_ENABLE();
    RCC_LCD_RES_ENABLE();
    RCC_LCD_DS_ENABLE();
    RCC_LCD_CS_ENABLE();
    RCC_LCD_BLK_ENABLE();

    __HAL_AFIO_REMAP_SPI1_DISABLE();

    GPIO_handler.Mode = GPIO_MODE_AF_PP;
    GPIO_handler.Pin = LCD_SCK_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_SCK_GPIO,&GPIO_handler);

    GPIO_handler.Pin = LCD_SDA_PIN;
    HAL_GPIO_Init(LCD_SDA_GPIO,&GPIO_handler);

    LCD_SPI_DMA_INIT();

    __HAL_RCC_SPI1_CLK_ENABLE();
    SPI_handler.Init.Mode = SPI_MODE_MASTER; //����ģʽ
    SPI_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; //SPI CRCӲ��У�� У���븽�����ݽ��з���
    SPI_handler.Init.BaudRatePrescaler  =SPI_BAUDRATEPRESCALER_2; //�����ʷ�Ƶ 2��Ƶ 64/2 = 32MHZ
    SPI_handler.Init.CLKPhase  =SPI_PHASE_1EDGE; //�ڼ���ʱ���ؿ�ʼ����
    SPI_handler.Init.CLKPolarity = SPI_POLARITY_LOW; //SCL���߿���ʱ����Ϊ��
    SPI_handler.Init.CRCPolynomial = 2; //CRCУ�����Ķ���ʽ û��Ӧ��û��
    SPI_handler.Init.DataSize = SPI_DATASIZE_8BIT; //���ݷ���֡��ʽ
    SPI_handler.Init.Direction = SPI_DIRECTION_2LINES; //˫��ȫ˫��
    SPI_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;//���ݸ�λ��ǰ �����λ��ǰ
    SPI_handler.Init.NSS = SPI_NSS_SOFT; //���Ƭѡ�� Ӳ��Ƭѡ��ΪPA4��ռ��
    SPI_handler.Init.TIMode = SPI_TIMODE_DISABLE;//����ʱ�Ӽ��Ժ���λ������ ��ΪĬ������
//    SPI_handler.Init.
    SPI_handler.Instance = SPI1;
    HAL_SPI_Init(&SPI_handler);


    GPIO_handler.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_handler.Pin = LCD_DC_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_DC_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = LCD_CS_PIN;
    HAL_GPIO_Init(LCD_CS_GPIO,&GPIO_handler);

    GPIO_handler.Pin = LCD_RES_PIN;
    HAL_GPIO_Init(LCD_RES_GPIO,&GPIO_handler);

//    LCD_SPI_DMA_0();
//0x00000000U                  /*!< Memory data alignment: Byte     */
//#define DMA_MDATAALIGN_HALFWORD      ((uint32_t)DMA_CCR_MSIZE_0)  /*!< Memory data alignment: HalfWord */
//#define DMA_MDATAALIGN_WORD          ((uint32_t)DMA_CCR_MSIZE_1)
    


//    dma_hander_rx.Instance = DMA1_Channel2;
//    dma_hander_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    dma_hander_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    dma_hander_rx.Init.MemInc = DMA_MINC_ENABLE; //���ݵ���
//    dma_hander_rx.Init.Mode = DMA_NORMAL;
//    dma_hander_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    dma_hander_rx.Init.PeriphInc = DMA_PINC_DISABLE; //�Ĵ�����ַ������
//    dma_hander_rx.Init.Priority = DMA_PRIORITY_LOW;

//    __HAL_LINKDMA(&SPI_handler,hdmarx,dma_hander_rx);
//    HAL_DMA_Init(&dma_hander_rx);
//    HAL_NVIC_SetPriority(DMA1_Channel2_IRQn,0,0);
//    HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
#endif
    
#if LCD_TFT_14_DRIVER == LCD_TFT_HW_SPI
        GPIO_InitTypeDef GPIO_handler = {0};
    __HAL_RCC_AFIO_CLK_ENABLE(); //ʹ�ܸ���ʱ��

    RCC_LCD_SCK_ENABLE();
    RCC_LCD_SDA_ENABLE();
    RCC_LCD_RES_ENABLE();
    RCC_LCD_DS_ENABLE();
    RCC_LCD_CS_ENABLE();
    RCC_LCD_BLK_ENABLE();

    __HAL_AFIO_REMAP_SPI1_DISABLE();
    
    GPIO_handler.Mode = GPIO_MODE_AF_PP;
    GPIO_handler.Pin = LCD_SCK_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_SCK_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = LCD_SDA_PIN;
    HAL_GPIO_Init(LCD_SDA_GPIO,&GPIO_handler);


    __HAL_RCC_SPI1_CLK_ENABLE();
    SPI_handler.Init.Mode = SPI_MODE_MASTER; //����ģʽ
    SPI_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; //SPI CRCӲ��У�� У���븽�����ݽ��з���
    SPI_handler.Init.BaudRatePrescaler  =SPI_BAUDRATEPRESCALER_2; //�����ʷ�Ƶ 2��Ƶ 64/2 = 32MHZ
    SPI_handler.Init.CLKPhase  =SPI_PHASE_1EDGE; //�ڼ���ʱ���ؿ�ʼ����
    SPI_handler.Init.CLKPolarity = SPI_POLARITY_LOW; //SCL���߿���ʱ����Ϊ��
    SPI_handler.Init.CRCPolynomial = 2; //CRCУ�����Ķ���ʽ û��Ӧ��û��
    SPI_handler.Init.DataSize = SPI_DATASIZE_8BIT; //���ݷ���֡��ʽ
    SPI_handler.Init.Direction = SPI_DIRECTION_2LINES; //˫��ȫ˫��
    SPI_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;//���ݸ�λ��ǰ �����λ��ǰ
    SPI_handler.Init.NSS = SPI_NSS_SOFT; //���Ƭѡ�� Ӳ��Ƭѡ��ΪPA4��ռ��
    SPI_handler.Init.TIMode = SPI_TIMODE_DISABLE;//����ʱ�Ӽ��Ժ���λ������ ��ΪĬ������
//    SPI_handler.Init.
    SPI_handler.Instance = SPI1;
    HAL_SPI_Init(&SPI_handler);


    GPIO_handler.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_handler.Pin = LCD_DC_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_DC_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = LCD_CS_PIN;
    HAL_GPIO_Init(LCD_CS_GPIO,&GPIO_handler);

    GPIO_handler.Pin = LCD_RES_PIN;
    HAL_GPIO_Init(LCD_RES_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = LCD_BLK_PIN;
    HAL_GPIO_Init(LCD_BLK_GPIO,&GPIO_handler);
    
#endif
    
    #if LCD_TFT_14_DRIVER == LCD_TFT_SF_SPI
    GPIO_InitTypeDef GPIO_handler = {0};
    
    RCC_LCD_SCK_ENABLE();
    RCC_LCD_SDA_ENABLE();
    RCC_LCD_RES_ENABLE();
    RCC_LCD_DS_ENABLE();
    RCC_LCD_CS_ENABLE();
    RCC_LCD_BLK_ENABLE();
    
    GPIO_handler.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_handler.Pin = LCD_SCK_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_SCK_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = LCD_SDA_PIN;
    HAL_GPIO_Init(LCD_SDA_GPIO,&GPIO_handler);

    GPIO_handler.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_handler.Pin = LCD_DC_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_DC_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = LCD_CS_PIN;
    HAL_GPIO_Init(LCD_CS_GPIO,&GPIO_handler);

    GPIO_handler.Pin = LCD_RES_PIN;
    HAL_GPIO_Init(LCD_RES_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = LCD_BLK_PIN;
    HAL_GPIO_Init(LCD_BLK_GPIO,&GPIO_handler);
    #endif
    
//HAL_DMA_Start(&dma_hander,)
}


/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
//	u8 i;
//	LCD_CS_Clr();

#if LCD_TFT_14_DRIVER == LCD_TFT_SF_SPI
    uint8_t i;
    LCD_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
    LCD_CS_Set();
#endif

#if LCD_TFT_14_DRIVER == LCD_TFT_HW_DMA_SPI

    LCD_CS_Clr();
    HAL_SPI_Transmit(&SPI_handler,&dat,1,100);
    LCD_CS_Set();

#endif

#if LCD_TFT_14_DRIVER == LCD_TFT_HW_SPI
    LCD_CS_Clr();
    HAL_SPI_Transmit(&SPI_handler,&dat,1,100);
    LCD_CS_Set();
#endif
//    buf = dat;
//    HAL_SPI_Transmit(&SPI_handler,&dat,1,100);
//  LCD_CS_Set();	
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+2);
		LCD_WR_DATA(x2+2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+1);
		LCD_WR_DATA(y2+1);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+2);
		LCD_WR_DATA(x2+2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+1);
		LCD_WR_DATA(y2+1);
		LCD_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+1);
		LCD_WR_DATA(x2+1);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+2);
		LCD_WR_DATA(y2+2);
		LCD_WR_REG(0x2c);//������д
	}
	else
	{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+1);
		LCD_WR_DATA(x2+1);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+2);
		LCD_WR_DATA(y2+2);
		LCD_WR_REG(0x2c);//������д
	}
}

void LCD_Init(void)
{
	LCD_GPIO_Init();//��ʼ��GPIO

	LCD_RES_Clr();//��λ
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);

	LCD_BLK_Set();//�򿪱���
    delay_ms(100);
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out 
	delay_ms(120);              //Delay 120ms 
	//------------------------------------ST7735S Frame Rate-----------------------------------------// 
	LCD_WR_REG(0xB1); 
	LCD_WR_DATA8(0x05); 
	LCD_WR_DATA8(0x3C); 
	LCD_WR_DATA8(0x3C); 
	LCD_WR_REG(0xB2); 
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C); 
	LCD_WR_DATA8(0x3C); 
	LCD_WR_REG(0xB3); 
	LCD_WR_DATA8(0x05); 
	LCD_WR_DATA8(0x3C); 
	LCD_WR_DATA8(0x3C); 
	LCD_WR_DATA8(0x05); 
	LCD_WR_DATA8(0x3C); 
	LCD_WR_DATA8(0x3C); 
	//------------------------------------End ST7735S Frame Rate---------------------------------// 
	LCD_WR_REG(0xB4); //Dot inversion 
	LCD_WR_DATA8(0x03); 
	//------------------------------------ST7735S Power Sequence---------------------------------// 
	LCD_WR_REG(0xC0); 
	LCD_WR_DATA8(0x28); 
	LCD_WR_DATA8(0x08); 
	LCD_WR_DATA8(0x04); 
	LCD_WR_REG(0xC1); 
	LCD_WR_DATA8(0XC0); 
	LCD_WR_REG(0xC2); 
	LCD_WR_DATA8(0x0D); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_REG(0xC3); 
	LCD_WR_DATA8(0x8D); 
	LCD_WR_DATA8(0x2A); 
	LCD_WR_REG(0xC4); 
	LCD_WR_DATA8(0x8D); 
	LCD_WR_DATA8(0xEE); 
	//---------------------------------End ST7735S Power Sequence-------------------------------------// 
	LCD_WR_REG(0xC5); //VCOM 
	LCD_WR_DATA8(0x1A); 
	LCD_WR_REG(0x36); //MX, MY, RGB mode 
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0); 
	//------------------------------------ST7735S Gamma Sequence---------------------------------// 
	LCD_WR_REG(0xE0); 
	LCD_WR_DATA8(0x04); 
	LCD_WR_DATA8(0x22); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0x2E); 
	LCD_WR_DATA8(0x30); 
	LCD_WR_DATA8(0x25); 
	LCD_WR_DATA8(0x2A); 
	LCD_WR_DATA8(0x28); 
	LCD_WR_DATA8(0x26); 
	LCD_WR_DATA8(0x2E); 
	LCD_WR_DATA8(0x3A); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0x13); 
	LCD_WR_REG(0xE1); 
	LCD_WR_DATA8(0x04); 
	LCD_WR_DATA8(0x16); 
	LCD_WR_DATA8(0x06); 
	LCD_WR_DATA8(0x0D); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x26); 
	LCD_WR_DATA8(0x23); 
	LCD_WR_DATA8(0x27); 
	LCD_WR_DATA8(0x27); 
	LCD_WR_DATA8(0x25); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x3B); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x04); 
	LCD_WR_DATA8(0x13); 
	//------------------------------------End ST7735S Gamma Sequence-----------------------------// 
	LCD_WR_REG(0x3A); //65k mode 
	LCD_WR_DATA8(0x05); 
	LCD_WR_REG(0x29); //Display on 
} 







