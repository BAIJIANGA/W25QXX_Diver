#ifndef __OLED_H_128_128_
#define __OLED_H_128_128_ 

#include "stdlib.h"
#include "main.h"

//��Ļ�Ŀ���оƬΪSH1107 ֻ֧�� ҳѰַģʽ ���ߴ�ֱ��Ѱַ ��������DMA����ˢ��

//�Ƿ�ʹ�� OLED��Ļ ������ͻ
#define USE_OLED_128_128    1 //0 ʧ��  1 ʹ�� 

#if (USE_OLED_128_128 == 1)



//���ú�������������ʾ 0   ������ʾ
//                     90  ��ת90����ʾ
//                     180 ��ת180����ʾ
//                     270 ��ת270����ʾ
//USE_HORIZONTAL_OLED_128_128_OLED_128_128
#define USE_HORIZONTAL_OLED_128_128 0
//unsigned char char

#define OLED_128_128_MOD    OLED_128_128_HW
#define OLED_128_128_SF  0
#define OLED_128_128_HW  1
#define OLED_128_128_HW_DMA  2
//-----------------����LED�˿ڶ���---------------- 

#define LED_ON GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define LED_OFF GPIO_SetBits(GPIOC,GPIO_Pin_12)

//-----------------OLED�˿ڶ���---------------- 

#define RCC_OLED_SCK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define RCC_OLED_SDA_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define RCC_OLED_RST_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define RCC_OLED_DC_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define RCC_OLED_CS_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()

#define OLED_SCK_GPIO    GPIOA
#define OLED_SDA_GPIO    GPIOA
#define OLED_RST_GPIO    GPIOA
#define OLED_DC_GPIO    GPIOB
#define OLED_CS_GPIO    GPIOB


#define OLED_SCK_PIN     GPIO_PIN_5
#define OLED_SDA_PIN     GPIO_PIN_7
#define OLED_RST_PIN     GPIO_PIN_6
#define OLED_DC_PIN     GPIO_PIN_2
#define OLED_CS_PIN     GPIO_PIN_10

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_SCK_GPIO,OLED_SCK_PIN,GPIO_PIN_RESET)//SCL
#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_SCK_GPIO,OLED_SCK_PIN,GPIO_PIN_SET)//SCL

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_SDA_GPIO,OLED_SDA_PIN,GPIO_PIN_RESET)//SDA
#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_SDA_GPIO,OLED_SDA_PIN,GPIO_PIN_SET)//SDA

#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RST_GPIO,OLED_RST_PIN,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RST_GPIO,OLED_RST_PIN,GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO,OLED_DC_PIN,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO,OLED_DC_PIN,GPIO_PIN_SET)

#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS_GPIO,OLED_CS_PIN,GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS_GPIO,OLED_CS_PIN,GPIO_PIN_SET)

#define OLED_RES_Clr() OLED_RST_Clr()//RES
#define OLED_RES_Set() OLED_RST_Set()

#define OLED_SCL_Clr() OLED_SCLK_Clr()
#define OLED_SCL_Set() OLED_SCLK_Set()

#define OLED_SDA_Clr() OLED_SDIN_Clr()
#define OLED_SDA_Set() OLED_SDIN_Set()

#define OLED_128_128_DELAY(x) HAL_Delay(x)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//#define unsigned char uint8_t
//#define unsigned short uint16_t
//#define unsigned int uint32_t

extern unsigned char OLED_GRAM[16][128];//[16];

void OLED_ClearPoint(unsigned char x,unsigned char y);
void OLED_ColorTurn(unsigned char i);
void OLED_WR_Byte(unsigned char dat,unsigned char mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t);
void OLED_DrawLine(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char mode);
void OLED_DrawCircle(unsigned char x,unsigned char y,unsigned char r);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size1,unsigned char mode);
void OLED_ShowChar6x8(unsigned char x,unsigned char y,unsigned char chr,unsigned char mode);
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char size1,unsigned char mode);
void OLED_ShowNum(unsigned char x,unsigned char y, int num,unsigned char len,unsigned char size1,unsigned char mode);
void OLED_Showfloat(unsigned char x,unsigned char y,double num1,unsigned char dot_before_len,unsigned char dot_after_len,unsigned char size1,unsigned char mode);
void OLED_ShowChinese(unsigned char x,unsigned char y,unsigned char num,unsigned char size1,unsigned char mode);
void OLED_ScrollDisplay(unsigned char num,unsigned char space,unsigned char mode);
void OLED_ShowPicture(unsigned char x,unsigned char y,unsigned char sizex,unsigned char sizey,unsigned char BMP[],unsigned char mode);
void OLED_Init(void);
void OLED_ShowChinese1(unsigned char x,unsigned char y,unsigned char* s,unsigned char size1,unsigned char mode);

#endif
#endif

