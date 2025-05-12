#include "USER_BSP.h"
#include "USER_Hardware.h"

DMA_HandleTypeDef DMA_SDIO_handler = {0};

SD_HandleTypeDef sd_handler = {0};


//SDIO DMA��ȡ��ɻص�����
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
    __HAL_DMA_DISABLE(hsd->hdmarx);
}

//SDIO DMA������ɻص�����
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
    __HAL_DMA_DISABLE(hsd->hdmatx);
}

uint8_t SD_Init()
{
    uint8_t SD_RES = 0;
    
//    return SD_RES;
#if SD_USE_DRIVER == SD_SDIO_HW

    SD_GPIO_Init();

    __HAL_RCC_SDIO_CLK_ENABLE();
    sd_handler.Instance = SDIO;
    sd_handler.Init.BusWide = SDIO_BUS_WIDE_1B;
    sd_handler.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    sd_handler.Init.ClockDiv = 4;
    sd_handler.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    sd_handler.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    sd_handler.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;

    SD_RES = HAL_SD_Init(&sd_handler);

#endif



#if SD_USE_DRIVER == SD_SDIO_DMA
    SD_GPIO_Init();

    __HAL_RCC_SDIO_CLK_ENABLE();
    sd_handler.Instance = SDIO;
    sd_handler.Init.BusWide = SDIO_BUS_WIDE_1B;
    sd_handler.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    sd_handler.Init.ClockDiv = 4;
    sd_handler.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    sd_handler.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    sd_handler.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;

    SD_RES = HAL_SD_Init(&sd_handler);

        __HAL_RCC_DMA2_CLK_ENABLE();

    DMA_SDIO_handler.Instance = DMA2_Channel4;
    DMA_SDIO_handler.Init.Direction = 0;//SDIO ����Ҫ�ܷ���
    DMA_SDIO_handler.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;//32λ���ݴ���
    DMA_SDIO_handler.Init.MemInc = DMA_MINC_ENABLE; //���ݵ�ַ����
    DMA_SDIO_handler.Init.Mode = DMA_NORMAL;
    DMA_SDIO_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;//8λ���ݴ���
    DMA_SDIO_handler.Init.PeriphInc = DMA_PINC_DISABLE; //�Ĵ�����ַ������
    DMA_SDIO_handler.Init.Priority = DMA_PRIORITY_MEDIUM;

    __HAL_LINKDMA(&sd_handler,hdmatx,DMA_SDIO_handler);//����link
    __HAL_LINKDMA(&sd_handler,hdmarx,DMA_SDIO_handler);//����link

    HAL_DMA_Init(&DMA_SDIO_handler);
    HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn,0,0);
    HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);
    HAL_SD_ConfigWideBusOperation(&sd_handler, SDIO_BUS_WIDE_4B);

#endif
return SD_RES;
}

void SD_GPIO_Init()
{
	GPIO_InitTypeDef gpio_handler = {0};

	__HAL_RCC_AFIO_CLK_ENABLE();
    
    RCC_SDIO_D0_ENABLE();
    RCC_SDIO_D1_ENABLE();
    RCC_SDIO_D2_ENABLE();
    RCC_SDIO_D3_ENABLE();
    RCC_SDIO_CMD_ENABLE();
    RCC_SDIO_SCK_ENABLE();

    gpio_handler.Mode = GPIO_MODE_AF_PP;
    gpio_handler.Pin = SDIO_D0_PIN;
    gpio_handler.Pull = GPIO_NOPULL;
    gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SDIO_D0_GPIO,&gpio_handler);

    gpio_handler.Pin = SDIO_D1_PIN;
    HAL_GPIO_Init(SDIO_D1_GPIO,&gpio_handler);
    
    gpio_handler.Pin = SDIO_D2_PIN;
    HAL_GPIO_Init(SDIO_D2_GPIO,&gpio_handler);
    
    gpio_handler.Pin = SDIO_D3_PIN;
    HAL_GPIO_Init(SDIO_D3_GPIO,&gpio_handler);
    
    gpio_handler.Pin = SDIO_CMD_PIN;
    HAL_GPIO_Init(SDIO_CMD_GPIO,&gpio_handler);
    
    gpio_handler.Pin = SDIO_SCK_PIN;
    HAL_GPIO_Init(SDIO_SCK_GPIO,&gpio_handler);
}

/**
 * @brief       �ж�SD���Ƿ���Դ���(��д)����
 * @param       ��
 * @retval      ����ֵ:SD_TRANSFER_OK      ������ɣ����Լ�����һ�δ���
                       SD_TRANSFER_BUSY SD ����æ�������Խ�����һ�δ���
 */
uint8_t sd_Get_state(void)
{
    return ((HAL_SD_GetCardState(&sd_handler) == HAL_SD_CARD_TRANSFER) ? HAL_OK : HAL_BUSY);
}

/**
 * @brief       дSD��(fatfs/usb����)
 * @param       pbuf  : ���ݻ�����
 * @param       saddr : ������ �ڼ�������
 * @param       cnt   : ��������
 * @retval      0, ����;  ����, �������(���SD_Error����);
 */
uint8_t SD_Write_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
#if SD_USE_DRIVER == SD_SDIO_HW

   uint8_t sta = HAL_OK;
   uint32_t timeout = ((uint32_t)100000000);
   sta = HAL_SD_WriteBlocks(&sd_handler, (uint8_t *)pbuf, saddr, cnt, ((uint32_t)100000000)); //���sector��д����
   while (sd_Get_state() != HAL_OK)
   {
       if (timeout-- == 0)
       {
           sta = HAL_BUSY;
       }
   }
   //printf("sta: %d\r\n",sta);
   return sta;

#endif

#if SD_USE_DRIVER == SD_SDIO_DMA

    uint8_t sta = HAL_OK;
    uint32_t timeout = ((uint32_t)100000000);
    sta = HAL_SD_WriteBlocks_DMA(&sd_handler, (uint8_t *)pbuf, saddr, cnt);
    while (sd_Get_state() != HAL_OK)
    {
        if (timeout-- == 0)
        {
            sta = HAL_BUSY;
        }
    }
    // if(sta == HAL_OK)
    // {
    //     printf("д����������hal_ok \r\n");
    // }
//   HAL_Delay(100);
    return sta;

#endif
}

/**
 * @brief       ��SD��(fatfs/usb����)
 * @param       pbuf  : ���ݻ�����
 * @param       saddr : ������ �ڼ�������
 * @param       cnt   : ��������
 * @retval      0, ����;  ����, �������(���SD_Error����);
 */
uint8_t sd_read_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
#if SD_USE_DRIVER == SD_SDIO_HW

   uint8_t sta = HAL_OK;
   uint32_t timeout = ((uint32_t)100000000);
   sta = HAL_SD_ReadBlocks(&sd_handler, (uint8_t *)pbuf, saddr, cnt, ((uint32_t)100000000)); /* ���sector�Ķ����� */

   /* �ȴ�SD������ */
   while (sd_Get_state() != HAL_OK)
   {
       if (timeout-- == 0)
       {
           sta = HAL_BUSY;
       }
   }
   printf("sta: %d\r\n",sta);
   return sta;

#endif


#if SD_USE_DRIVER == SD_SDIO_DMA

    uint8_t sta = HAL_OK;
    uint32_t timeout = ((uint32_t)100000000);
    sta = HAL_SD_ReadBlocks_DMA(&sd_handler,(uint8_t *)pbuf, saddr, cnt);
    //    /* �ȴ�SD������ */
    while (sd_Get_state() != HAL_OK)
    {
        if (timeout-- == 0)
        {
            sta = HAL_BUSY;
        }
    }
    // if(sta == HAL_OK)
    // {
    //     printf("��ȡ��������hal_ok \r\n");
    // }
    return sta;

#endif
}

HAL_SD_CardCIDTypeDef sd_card_cid;
uint8_t SD_Get_CID() //��ȡSD��CID
{
    uint8_t res = 0;
    res = HAL_SD_GetCardCID(&sd_handler, &sd_card_cid);/* ��ȡCID */
    return res;
}

HAL_SD_CardInfoTypeDef sd_card_info;
uint8_t SD_Get_Info() //��ȡSD��Info
{
    uint8_t res = HAL_SD_GetCardInfo(&sd_handler, &sd_card_info);/* ��ȡCID */
    return res;
}

uint8_t sd_car_info_printf()
{
    uint8_t res = 0;
    res = SD_Get_CID();
    if(res != HAL_OK)
    {
        printf("��ȡCIDʧ��");
        res = HAL_ERROR;
        return res;
    }
    res = SD_Get_Info();
    if(res != HAL_OK)
    {
        printf("��ȡSD����Ϣʧ��");
        res = HAL_ERROR;
        return res;
    }
    
    switch (sd_card_info.CardType)
    {
        case CARD_SDSC:
        {
            if (sd_card_info.CardVersion == CARD_V1_X)
            {
                printf("Card Type:SDSC V1\r\n");
            }
            else if (sd_card_info.CardVersion == CARD_V2_X)
            {
                printf("Card Type:SDSC V2\r\n");
            }
        }
        break;

        case CARD_SDHC_SDXC:
            printf("Card Type:SDHC\r\n");
            break;

        default: break;
    }
   uint64_t card_capacity = (uint64_t)(sd_card_info.LogBlockNbr) * (uint64_t)(sd_card_info.LogBlockSize); /* ����SD������ */
    printf("CAP %lld �ֽ�\r\n",card_capacity);
    printf("Card Capacity %d MB\r\n",(uint32_t)SD_TOTAL_SIZE_MB(&sd_handler));
    printf("Card BlockSize:%d \r\n", sd_card_info.BlockSize);
    printf("LogBlockNbr:%d \r\n", (uint32_t)(sd_card_info.LogBlockNbr));     /* ��ʾ�߼������� */
    printf("LogBlockSize:%d \r\n", (uint32_t)(sd_card_info.LogBlockSize));   /* ��ʾ�߼����С */
    return res;
}

typedef struct 
{
	unsigned char Index[256];	
	unsigned char Index1[256];
}qwqw; 
qwqw dsds;
/***************************************************************
  *  @brief     ����SD��������д����(һ������)
  *  @param     NULL   
  *  @retval    NULL
  *  @note      �˺�����Ҫ��printf ��ջ���迪���
 **************************************************************/
void SD_test_w_r_peintf()
{
    uint8_t aj[3] = "��";
    uint8_t sd_buf[512] = {0,0,0x20,0x00,0x20,0x00,0x20,0x00,0xFE,0x03,0x22,0x02,0x22,0x02,0x22,0x02,0xFE,0x03,
0x22,0x02,0x20,0x00,0x20,0x00,0x20,0x00};
//    uint8_t sd_buf1[512];
    uint16_t a = 0;
    for(a = 0;a < 256;a++)
    {
        //sd_buf[a] = a;
        sd_buf[a+256] = a;
    }
    sd_buf[0] = aj[0];
    sd_buf[0] = aj[1];
    printf("д�������Ϊ\r\n");
   for(a = 0;a<512;a++)
   {
       printf("%d  ",sd_buf[a]);
   }

   printf("\r\n");
   SD_Write_disk((uint8_t*)sd_buf,1000,1);
   sd_read_disk((uint8_t*)&dsds,1000,1);

   for(a = 0;a<256;a++)
   {
       //printf("%d  ",sd_buf1[a]);
       printf("%d  ",dsds.Index[a]);
   }
   for(a = 0;a<256;a++)
   {
       //printf("%d  ",sd_buf1[a]);
       printf("%d  ",dsds.Index1[a]);
   }
}




