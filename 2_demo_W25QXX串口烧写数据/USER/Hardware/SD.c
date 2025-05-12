#include "USER_BSP.h"
#include "USER_Hardware.h"

DMA_HandleTypeDef DMA_SDIO_handler = {0};

SD_HandleTypeDef sd_handler = {0};


//SDIO DMA读取完成回调函数
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
    __HAL_DMA_DISABLE(hsd->hdmarx);
}

//SDIO DMA传输完成回调函数
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
    DMA_SDIO_handler.Init.Direction = 0;//SDIO 不需要管方向
    DMA_SDIO_handler.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;//32位数据传输
    DMA_SDIO_handler.Init.MemInc = DMA_MINC_ENABLE; //数据地址递增
    DMA_SDIO_handler.Init.Mode = DMA_NORMAL;
    DMA_SDIO_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;//8位数据传输
    DMA_SDIO_handler.Init.PeriphInc = DMA_PINC_DISABLE; //寄存器地址不递增
    DMA_SDIO_handler.Init.Priority = DMA_PRIORITY_MEDIUM;

    __HAL_LINKDMA(&sd_handler,hdmatx,DMA_SDIO_handler);//发送link
    __HAL_LINKDMA(&sd_handler,hdmarx,DMA_SDIO_handler);//接收link

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
 * @brief       判断SD卡是否可以传输(读写)数据
 * @param       无
 * @retval      返回值:SD_TRANSFER_OK      传输完成，可以继续下一次传输
                       SD_TRANSFER_BUSY SD 卡正忙，不可以进行下一次传输
 */
uint8_t sd_Get_state(void)
{
    return ((HAL_SD_GetCardState(&sd_handler) == HAL_SD_CARD_TRANSFER) ? HAL_OK : HAL_BUSY);
}

/**
 * @brief       写SD卡(fatfs/usb调用)
 * @param       pbuf  : 数据缓存区
 * @param       saddr : 扇区号 第几个扇区
 * @param       cnt   : 扇区个数
 * @retval      0, 正常;  其他, 错误代码(详见SD_Error定义);
 */
uint8_t SD_Write_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
#if SD_USE_DRIVER == SD_SDIO_HW

   uint8_t sta = HAL_OK;
   uint32_t timeout = ((uint32_t)100000000);
   sta = HAL_SD_WriteBlocks(&sd_handler, (uint8_t *)pbuf, saddr, cnt, ((uint32_t)100000000)); //多个sector的写操作
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
    //     printf("写入正常返回hal_ok \r\n");
    // }
//   HAL_Delay(100);
    return sta;

#endif
}

/**
 * @brief       读SD卡(fatfs/usb调用)
 * @param       pbuf  : 数据缓存区
 * @param       saddr : 扇区号 第几个扇区
 * @param       cnt   : 扇区个数
 * @retval      0, 正常;  其他, 错误代码(详见SD_Error定义);
 */
uint8_t sd_read_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
#if SD_USE_DRIVER == SD_SDIO_HW

   uint8_t sta = HAL_OK;
   uint32_t timeout = ((uint32_t)100000000);
   sta = HAL_SD_ReadBlocks(&sd_handler, (uint8_t *)pbuf, saddr, cnt, ((uint32_t)100000000)); /* 多个sector的读操作 */

   /* 等待SD卡读完 */
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
    //    /* 等待SD卡读完 */
    while (sd_Get_state() != HAL_OK)
    {
        if (timeout-- == 0)
        {
            sta = HAL_BUSY;
        }
    }
    // if(sta == HAL_OK)
    // {
    //     printf("读取正常返回hal_ok \r\n");
    // }
    return sta;

#endif
}

HAL_SD_CardCIDTypeDef sd_card_cid;
uint8_t SD_Get_CID() //获取SD卡CID
{
    uint8_t res = 0;
    res = HAL_SD_GetCardCID(&sd_handler, &sd_card_cid);/* 获取CID */
    return res;
}

HAL_SD_CardInfoTypeDef sd_card_info;
uint8_t SD_Get_Info() //获取SD卡Info
{
    uint8_t res = HAL_SD_GetCardInfo(&sd_handler, &sd_card_info);/* 获取CID */
    return res;
}

uint8_t sd_car_info_printf()
{
    uint8_t res = 0;
    res = SD_Get_CID();
    if(res != HAL_OK)
    {
        printf("获取CID失败");
        res = HAL_ERROR;
        return res;
    }
    res = SD_Get_Info();
    if(res != HAL_OK)
    {
        printf("获取SD卡信息失败");
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
   uint64_t card_capacity = (uint64_t)(sd_card_info.LogBlockNbr) * (uint64_t)(sd_card_info.LogBlockSize); /* 计算SD卡容量 */
    printf("CAP %lld 字节\r\n",card_capacity);
    printf("Card Capacity %d MB\r\n",(uint32_t)SD_TOTAL_SIZE_MB(&sd_handler));
    printf("Card BlockSize:%d \r\n", sd_card_info.BlockSize);
    printf("LogBlockNbr:%d \r\n", (uint32_t)(sd_card_info.LogBlockNbr));     /* 显示逻辑块数量 */
    printf("LogBlockSize:%d \r\n", (uint32_t)(sd_card_info.LogBlockSize));   /* 显示逻辑块大小 */
    return res;
}

typedef struct 
{
	unsigned char Index[256];	
	unsigned char Index1[256];
}qwqw; 
qwqw dsds;
/***************************************************************
  *  @brief     测试SD卡基础读写数据(一个扇区)
  *  @param     NULL   
  *  @retval    NULL
  *  @note      此函数需要打开printf 且栈堆需开大点
 **************************************************************/
void SD_test_w_r_peintf()
{
    uint8_t aj[3] = "中";
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
    printf("写入的数据为\r\n");
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




