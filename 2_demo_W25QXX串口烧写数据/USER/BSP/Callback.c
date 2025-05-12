#include "Callback.h"
#include "main.h"
#include "USER_Hardware.h"
#include "USER_BSP.h"

////定时器更新回调函数
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
////    ERROR_LED_Toggle();
////    if()
//}


//IO口中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case GPIO_PIN_3 : ERROR_LED_Toggle(); break;
        case GPIO_PIN_4 : ERROR_LED_Toggle(); break;
        case GPIO_PIN_5 : ERROR_LED_Toggle(); break;
        case GPIO_PIN_11 : KEY_Value = KEY0;ERROR_LED_Toggle(); break;
        case GPIO_PIN_10 : KEY_Value = KEY1;ERROR_LED_Toggle(); break;
        case GPIO_PIN_15 : KEY_Value = KEY2;ERROR_LED_Toggle(); break;
        default : break;
    }
}
