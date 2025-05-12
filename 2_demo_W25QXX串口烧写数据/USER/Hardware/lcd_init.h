#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#define LCD_TFT_14_DRIVER   LCD_TFT_HW_DMA_SPI
#define LCD_TFT_SF_SPI   0
#define LCD_TFT_HW_SPI   1
#define LCD_TFT_HW_DMA_SPI   2


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif

#define RCC_LCD_SCK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define RCC_LCD_SDA_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define RCC_LCD_RES_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define RCC_LCD_DS_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define RCC_LCD_CS_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()
#define RCC_LCD_BLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()


#define LCD_SCK_GPIO    GPIOA
#define LCD_SDA_GPIO    GPIOA
#define LCD_RES_GPIO    GPIOC
#define LCD_DC_GPIO     GPIOB
#define LCD_CS_GPIO     GPIOF
#define LCD_BLK_GPIO    GPIOF

#define LCD_SCK_PIN     GPIO_PIN_5
#define LCD_SDA_PIN     GPIO_PIN_7
#define LCD_RES_PIN     GPIO_PIN_5
#define LCD_DC_PIN     GPIO_PIN_1
#define LCD_CS_PIN     GPIO_PIN_11
#define LCD_BLK_PIN     GPIO_PIN_13


//-----------------LCD端口定义---------------- 

//#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SCL=SCLK
//#define LCD_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

//#define LCD_MOSI_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SDA=MOSI
//#define LCD_MOSI_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

//#define LCD_RES_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_6)//RES
//#define LCD_RES_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_6)

//#define LCD_DC_Clr()   GPIO_ResetBits(GPIOB,GPIO_Pin_0)//DC
//#define LCD_DC_Set()   GPIO_SetBits(GPIOB,GPIO_Pin_0)
// 		     
//#define LCD_CS_Clr()   GPIO_ResetBits(GPIOB,GPIO_Pin_1)//CS
//#define LCD_CS_Set()   GPIO_SetBits(GPIOB,GPIO_Pin_1)

//#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_5)//BLK
//#define LCD_BLK_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(LCD_SCK_GPIO,LCD_SCK_PIN,GPIO_PIN_RESET)//SCL=SCLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(LCD_SCK_GPIO,LCD_SCK_PIN,GPIO_PIN_SET)

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(LCD_SDA_GPIO,LCD_SDA_PIN,GPIO_PIN_RESET)//SDA=MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(LCD_SDA_GPIO,LCD_SDA_PIN,GPIO_PIN_SET)

#define LCD_RES_Clr()  HAL_GPIO_WritePin(LCD_RES_GPIO,LCD_RES_PIN,GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(LCD_RES_GPIO,LCD_RES_PIN,GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(LCD_DC_GPIO,LCD_DC_PIN,GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(LCD_DC_GPIO,LCD_DC_PIN,GPIO_PIN_SET)
 		     
#define LCD_CS_Clr()   HAL_GPIO_WritePin(LCD_CS_GPIO,LCD_CS_PIN,GPIO_PIN_RESET)//CS
#define LCD_CS_Set()   HAL_GPIO_WritePin(LCD_CS_GPIO,LCD_CS_PIN,GPIO_PIN_SET)

#define LCD_BLK_Clr()  HAL_GPIO_WritePin(LCD_BLK_GPIO,LCD_BLK_PIN,GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()  HAL_GPIO_WritePin(LCD_BLK_GPIO,LCD_BLK_PIN,GPIO_PIN_SET)

#define delay_ms(x) HAL_Delay(x)

//void SPI_SSS();
void LCD_SPI_DMA_0(void);
void LCD_SPI_DMA_1(void);
void LCD_SPI_DMA_INIT(void);
void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




