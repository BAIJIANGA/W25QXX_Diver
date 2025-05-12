#include "LED.h"
#include "main.h"


void LED_Init()
{
    GPIO_InitTypeDef gpio_handler = {0};
    
    LED0_LED_RCC_ENABLE();
    LED1_LED_RCC_ENABLE();

//    gpio_handler.Alternate = 0;
    gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_handler.Pin = LED0_LED_PIN;
    gpio_handler.Pull = GPIO_NOPULL;
    gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(LED0_LED_GPIO,&gpio_handler);

    gpio_handler.Pin = LED1_LED_PIN;
    HAL_GPIO_Init(LED1_LED_GPIO,&gpio_handler);

//    LED0(0);
//    HAL_Delay(50);
    LED0(0);
    LED1(1);
    HAL_Delay(1000);
    

}


void LED_OS_TEST()
{
    while(1)
    {
      LED0_Toggle();
      LED1_Toggle();
      HAL_Delay(500);
    }
}
