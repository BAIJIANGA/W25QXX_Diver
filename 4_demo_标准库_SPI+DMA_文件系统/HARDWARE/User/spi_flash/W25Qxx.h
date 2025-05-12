#ifndef __W25Qxx__
#define __W25Qxx__

//#include "main.h"
#include "sys.h"

//修改此宏可以改变驱动方式 硬件 软件
#define W25Qxx_USE  W25Qxx_USE_HW_SPI_DMA 

#define W25Qxx_USE_SW_SPI   0
#define W25Qxx_USE_HW_SPI   1
#define W25Qxx_USE_HW_SPI_DMA  2

#define W25Qxx_Font_LIB     Font_LIB_ENBALE
#define Font_LIB_DISBALE 0
#define Font_LIB_ENBALE 1

//使用硬件SPI DMA时最小的长度 比此长度小的字节为了加快速度统一使用硬件SPI
#define W25Qxx_DMA_sned_LEN_MIN  10
#define W25Qxx_DMA_read_LEN_MIN  10

//W25QXX 芯片一页的大小
//#define SPI_FLASH_PageSize 256

#define Font_offset 0x400000  //字库地址偏移
// typedef struct W25Qxx_Font_struct
// {
//     unsigned char Font[2];//名字
//     unsigned char Location[2];//位置 多少页 
// }W25Qxx_Font_struct;


//W25Qxx指令表1
#define W25Qxx_Write_Enable						          0x06
#define W25Qxx_Write_Disable                              0x04
#define W25Qxx_Read_Status_register_1				      0x05
#define W25Qxx_Read_Status_register_2				      0x35
#define W25Qxx_Write_Status_register				      0x01
#define W25Qxx_Page_Program							      0x02
#define W25Qxx_Quad_Page_Program				          0x32
#define W25Qxx_Block_Erase_64KB						      0xD8
#define W25Qxx_Block_Erase_32KB						      0x52
#define W25Qxx_Sector_Erase_4KB						      0x20
#define W25Qxx_Chip_Erase							      0xC7
#define W25Qxx_Erase_Suspend					          0x75
#define W25Qxx_Erase_Resume							      0x7A
#define W25Qxx_Power_down							      0xB9
#define W25Qxx_High_Performance_Mode				      0xA3
#define W25Qxx_Continuous_Read_Mode_Reset			      0xFF
#define W25Qxx_Release_Power_Down_HPM_Device_ID		      0xAB
#define W25Qxx_Manufacturer_Device_ID				      0x90
#define W25Qxx_Read_Uuique_ID						      0x4B
#define W25Qxx_JEDEC_ID								      0x9F


//W25Qxx指令集表2(读指令)
#define W25Qxx_Read_Data						          0x03
#define W25Qxx_Fast_Read						          0x0B
#define W25Qxx_Fast_Read_Dual_Output				      0x3B
#define W25Qxx_Fast_Read_Dual_IO					      0xBB
#define W25Qxx_Fast_Read_Quad_Output				      0x6B
#define W25Qxx_Fast_Read_Quad_IO					      0xEB
#define W25Qxx_Octal_Word_Read_Quad_IO				      0xE3

#define W25Qxx_DUMMY_BYTE 1

#define W25Qxx_SCL_GPIO     GPIOB
#define W25Qxx_DO_GPIO      GPIOB
#define W25Qxx_DI_GPIO      GPIOB
#define W25Qxx_CS_GPIO      GPIOB
#define W25Qxx_IO2_GPIO     GPIOA
#define W25Qxx_IO3_GPIO     GPIOA

#define W25Qxx_SCL_PIN      GPIO_PIN_13
#define W25Qxx_DO_PIN       GPIO_PIN_14
#define W25Qxx_DI_PIN       GPIO_PIN_15
#define W25Qxx_CS_PIN       GPIO_PIN_12
#define W25Qxx_IO2_PIN       GPIO_PIN_0
#define W25Qxx_IO3_PIN       GPIO_PIN_0

#define W25Qxx_SCL_RCC_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define W25Qxx_DO_RCC_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define W25Qxx_DI_RCC_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define W25Qxx_CS_RCC_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define W25Qxx_IO2_RCC_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define W25Qxx_IO3_RCC_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define W25Qxx_SCL(x)    x?HAL_GPIO_WritePin (W25Qxx_SCL_GPIO,W25Qxx_SCL_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (W25Qxx_SCL_GPIO,W25Qxx_SCL_PIN,GPIO_PIN_RESET)
#define W25Qxx_DO(x)    x?HAL_GPIO_WritePin (W25Qxx_DO_GPIO,W25Qxx_DO_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (W25Qxx_DO_GPIO,W25Qxx_DO_PIN,GPIO_PIN_RESET)
#define W25Qxx_DI(x)    x?HAL_GPIO_WritePin (W25Qxx_DI_GPIO,W25Qxx_DI_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (W25Qxx_DI_GPIO,W25Qxx_DI_PIN,GPIO_PIN_RESET)
#define W25Qxx_CS(x)    x?HAL_GPIO_WritePin (W25Qxx_CS_GPIO,W25Qxx_CS_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (W25Qxx_CS_GPIO,W25Qxx_CS_PIN,GPIO_PIN_RESET)
#define W25Qxx_IO2(x)    x?HAL_GPIO_WritePin (W25Qxx_IO2_GPIO,W25Qxx_IO2_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (W25Qxx_IO2_GPIO,W25Qxx_IO2_PIN,GPIO_PIN_RESET)
#define W25Qxx_IO3(x)    x?HAL_GPIO_WritePin (W25Qxx_IO3_GPIO,W25Qxx_IO3_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (W25Qxx_IO3_GPIO,W25Qxx_IO3_PIN,GPIO_PIN_RESET)

#define Read_W25Qxx_DO() HAL_GPIO_ReadPin(W25Qxx_DO_GPIO,W25Qxx_DO_PIN)

#define W25Qxx_CS1(x)    x?HAL_GPIO_WritePin (GPIOD,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin (GPIOD,GPIO_PIN_8,GPIO_PIN_RESET)


typedef struct 
{
    void (*W25Qxx_HW_Init)(void);//硬件SPI初始化
    void (*W25Qxx_HW_SendData)(uint8_t* data,uint16_t len); //硬件SPI发送len个字节数据
    void (*W25Qxx_HW_ReadData)(uint8_t* data,uint16_t len); //硬件SPI读取len个字节数据
    void (*W25Qxx_HW_CS)(uint8_t data); //硬件SPI片选
    void (*W25Qxx_SW_Init)(void);//软件SPI初始化
    void (*W25Qxx_SW_SendData)(uint8_t* data,uint16_t len); //软件SPI发送len个字节数据
    void (*W25Qxx_SW_ReadData)(uint8_t* data,uint16_t len); //硬件SPI读取len个字节数据
    void (*W25Qxx_SW_CS)(uint8_t data); //软件SPI片选
    void (*W25Qxx_HW_DMA_Init)(void);//硬件SPI DMA初始化
    void (*W25Qxx_HW_DMA_SendData)(uint8_t* data,uint16_t len); //硬件SPI DMA发送len个字节数据
    void (*W25Qxx_HW_DMA_ReadData)(uint8_t* data,uint16_t len); //硬件SPI DMA读len个字节数据
    void (*W25Qxx_HW_DMA_Wait)(void);//硬件SPI DMA等待DMA完成
}W25Qxx_InitTypeDef;

void User_W25Qxx_Init(W25Qxx_InitTypeDef* W25Qxx_handler);
void W25Qxx_GPIO_Init(void);
void W25Qxx_DATA_Init(void);
void W25Qxx_ReadID(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t *ID);
void W25Qxx_SectorErase(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t Sector_Address);
void W25Qxx_BlockErase(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t Block_Address);
void W25Qxx_ChipErase(W25Qxx_InitTypeDef* W25Qxx_handler);
void W25Qxx_PageWrite(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t address, uint8_t *data1, uint32_t dataSize);
void W25Qxx_ReadData(W25Qxx_InitTypeDef* W25Qxx_handler,uint32_t address, uint8_t *data1, uint32_t dataSize);
void W25Qxx_SectorWrite(W25Qxx_InitTypeDef* W25Qxx_handler,uint16_t Sector_start, uint8_t *data1, uint16_t Sector_num);
void W25Qxx_Sector_ReadData(W25Qxx_InitTypeDef* W25Qxx_handler,uint16_t Sector_start, uint8_t *data1, uint16_t Sector_num);
void SPI_FLASH_BufferWrite1(W25Qxx_InitTypeDef* W25Qxx_handler,unsigned char* pBuffer, unsigned long WriteAddr, unsigned int NumByteToWrite);

void W25Qx_Read_Font(W25Qxx_InitTypeDef* W25Qxx_handler,uint8_t* Font,uint8_t Font_type,uint8_t* Font_buf);

#endif
