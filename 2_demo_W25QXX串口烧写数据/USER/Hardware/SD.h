#ifndef __SD__
#define __SD__

#include "main.h"



#define SD_USE_DRIVER  SD_SDIO_DMA 

#define SD_SDIO_HW 0
#define SD_SDIO_DMA 1 

#define RCC_SDIO_D0_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define RCC_SDIO_D1_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define RCC_SDIO_D2_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define RCC_SDIO_D3_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define RCC_SDIO_CMD_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()
#define RCC_SDIO_SCK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()

#define SDIO_D0_GPIO    GPIOC
#define SDIO_D1_GPIO    GPIOC
#define SDIO_D2_GPIO    GPIOC
#define SDIO_D3_GPIO    GPIOC
#define SDIO_CMD_GPIO    GPIOD
#define SDIO_SCK_GPIO    GPIOC

#define SDIO_D0_PIN    GPIO_PIN_8
#define SDIO_D1_PIN    GPIO_PIN_9
#define SDIO_D2_PIN    GPIO_PIN_10
#define SDIO_D3_PIN    GPIO_PIN_11
#define SDIO_CMD_PIN    GPIO_PIN_2
#define SDIO_SCK_PIN    GPIO_PIN_12

#define SD_TOTAL_SIZE_MB(__Handle__)    (((uint64_t)((__Handle__)->SdCard.LogBlockNbr)*((__Handle__)->SdCard.LogBlockSize))>>20)

uint8_t SD_Init(void);
void SD_GPIO_Init(void);
uint8_t SD_Get_Info(void);
uint8_t SD_Get_CID(void);
uint8_t sd_car_info_printf(void);
uint8_t SD_Write_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt);
uint8_t sd_read_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt);
void SD_test_w_r_peintf(void);

#endif

