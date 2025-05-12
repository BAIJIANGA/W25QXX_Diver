#ifndef __DEBUG__
#define __DEBUG__

#include "main.h"

#define RCC_Debug_usart_TX_GPIO_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define RCC_Debug_usart_RX_GPIO_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define RCC_Debug_usart_ENABLE()        __HAL_RCC_USART2_CLK_ENABLE()

#define Debug_usart_Instance  USART2
#define Debug_usart_BaudRate  57600

#define Alternate_Debug     GPIO_AF7_USART1

#define Debug_usart_TX_GPIO    GPIOA
#define Debug_usart_RX_GPIO    GPIOA

#define Debug_usart_TX_PIN     GPIO_PIN_2
#define Debug_usart_RX_PIN     GPIO_PIN_3



void Debug_Init(void);
void USART_DMA_Init(void);
void us_os_test(void);

//extern UART_HandleTypeDef usart_handler;
#endif
