#ifndef __KEY__
#define __KEY__

#include "main.h"

#define KEY0_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY1_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()
#define KEY2_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()

#define KEY0_GPIO  GPIOA
#define KEY1_GPIO  GPIOE
#define KEY2_GPIO  GPIOE


#define KEY0_PIN  GPIO_PIN_0
#define KEY1_PIN  GPIO_PIN_4
#define KEY2_PIN  GPIO_PIN_3

#define KEY0 HAL_GPIO_ReadPin(KEY0_GPIO,KEY0_PIN)
#define KEY1 HAL_GPIO_ReadPin(KEY1_GPIO,KEY1_PIN)
#define KEY2 HAL_GPIO_ReadPin(KEY2_GPIO,KEY2_PIN)

#define KEY_74HC165_CLK_GPIO_RCC_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define KEY_74HC165_QH_GPIO_RCC_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define KEY_74HC165_PL_GPIO_RCC_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define KEY_74HC165_CE_GPIO_RCC_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()

#define K_74HC165_CLK_GPIO    GPIOB
#define K_74HC165_QH_GPIO    GPIOB
#define K_74HC165_PL_GPIO    GPIOB
#define K_74HC165_CE_GPIO    GPIOA

#define K_74HC165_CLK_PIN    GPIO_PIN_3
#define K_74HC165_QH_PIN    GPIO_PIN_4
#define K_74HC165_PL_PIN    GPIO_PIN_5
#define K_74HC165_CE_PIN    GPIO_PIN_15

#define K_PL(x) x?HAL_GPIO_WritePin (K_74HC165_PL_GPIO,K_74HC165_PL_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (K_74HC165_PL_GPIO,K_74HC165_PL_PIN,GPIO_PIN_RESET)
#define K_CE(x) x?HAL_GPIO_WritePin (K_74HC165_CE_GPIO,K_74HC165_CE_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (K_74HC165_CE_GPIO,K_74HC165_CE_PIN,GPIO_PIN_RESET)

void KEY_Init(void);
void KEY_Judgment(void);
void KEY_SCAN(void);
void Keyaction_HIGH(uint8_t Value);
void Keyaction_DOWN(uint8_t Value);
void KEY_Init_74HC165(void);
void KEY_74HC165_READ(uint8_t KEY_num,uint8_t* KEY_VALE_BUF);
void KEY_74HC165_SCAN(void);
void KEY_74HC165_Judgment(void);
void remote_sensing_Init(void);
void r_adjust(uint8_t* k_value,uint16_t a,uint16_t m_m,uint8_t m_min,uint8_t m_max);
#endif
