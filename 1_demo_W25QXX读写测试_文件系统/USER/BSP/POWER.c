#include "USER_BSP.h"
#include "USER_Hardware.h"
#include "main.h"

void POWER_Init()
{
    GPIO_InitTypeDef gpio_handler = {0};

    POWER_RCC_ENABLE();
    POWER_KEY_RCC_ENABLE();

    gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_handler.Pin = POWER_PIN;
    gpio_handler.Pull = GPIO_NOPULL;
    gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(POWER_GPIO,&gpio_handler);

    gpio_handler.Mode = GPIO_MODE_INPUT;
    gpio_handler.Pin = POWER_KEY_PIN;
    HAL_GPIO_Init(POWER_KEY_GPIO,&gpio_handler);
    HAL_Delay(500);
    POWER_OPEN();//打开电源开关
    HAL_Delay(1000);
//    POWER_CLOSE();
}


void POWER_OPEN()
{
    POWER(1);
}

void POWER_CLOSE()
{
    POWER(0);
}

void POWER_Check()
{
    while(1)
    {
        if(POWER_KEY())
        {
            POWER_CLOSE();
        }
    }
}

