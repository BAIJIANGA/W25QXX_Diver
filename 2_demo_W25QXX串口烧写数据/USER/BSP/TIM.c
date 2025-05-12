#include "main.h"
#include "USER_BSP.h"

TIM_HandleTypeDef base_tim_handler_7 = {0};
TIM_HandleTypeDef base_tim_handler_3 = {0};

void Base_tim_Init()
{
    __HAL_RCC_TIM3_CLK_ENABLE();

    base_tim_handler_3 .Instance = TIM3;
    base_tim_handler_3 .Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    base_tim_handler_3 .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    base_tim_handler_3 .Init.CounterMode = TIM_COUNTERMODE_UP;
    base_tim_handler_3 .Init.Period = 1200; //1.2MS
    base_tim_handler_3 .Init.Prescaler = 36-1;

    HAL_TIM_Base_Init (&base_tim_handler_3 );

    HAL_NVIC_SetPriority (TIM3_IRQn,1,3);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    HAL_TIM_Base_Start_IT (&base_tim_handler_3);
}

TIM_HandleTypeDef cap_tim_handler_2 = {0};
TIM_IC_InitTypeDef cap_CHL_handler_2 = {0};

void Time_Capture_Init()
{

    GPIO_InitTypeDef gpio_handler = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    __HAL_RCC_AFIO_CLK_ENABLE(); //使能复用时钟

    __HAL_AFIO_REMAP_TIM2_DISABLE();
//    gpio_handler.Alternate = 0;
    gpio_handler.Mode = GPIO_MODE_AF_INPUT;
    gpio_handler.Pin = GPIO_PIN_1;
    gpio_handler.Pull = GPIO_NOPULL;
    gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA,&gpio_handler);

    gpio_handler.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOA,&gpio_handler);

    __HAL_RCC_TIM2_CLK_ENABLE();

    cap_tim_handler_2.Instance = TIM2;
    cap_tim_handler_2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    cap_tim_handler_2.Init.Period = 65535-1;
    cap_tim_handler_2.Init.Prescaler = 6400-1;//100us 一个数据
    HAL_TIM_IC_Init(&cap_tim_handler_2);

    cap_CHL_handler_2.ICPolarity = TIM_ICPOLARITY_RISING;
    cap_CHL_handler_2.ICPrescaler = TIM_ICPSC_DIV1;
    cap_CHL_handler_2.ICSelection = TIM_ICSELECTION_DIRECTTI;
    cap_CHL_handler_2.ICFilter = 0x0f;
    HAL_TIM_IC_ConfigChannel(&cap_tim_handler_2,&cap_CHL_handler_2,TIM_CHANNEL_2);

    cap_CHL_handler_2.ICPolarity = TIM_ICPOLARITY_FALLING;
    HAL_TIM_IC_ConfigChannel(&cap_tim_handler_2,&cap_CHL_handler_2,TIM_CHANNEL_3);

    HAL_NVIC_SetPriority (TIM2_IRQn,0,0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    __HAL_TIM_ENABLE_IT(&cap_tim_handler_2,TIM_IT_UPDATE);
    HAL_TIM_IC_Start_IT(&cap_tim_handler_2,TIM_CHANNEL_2);
    HAL_TIM_IC_Start_IT(&cap_tim_handler_2,TIM_CHANNEL_3);

}

void TIM2_IRQHandler()
{
    HAL_TIM_IRQHandler(&cap_tim_handler_2);
}


void TIM4_IRQHandler()
{
    __HAL_RCC_TIM4_CLK_ENABLE();
    
}

void TIM3_IRQHandler()
{
    HAL_TIM_IRQHandler(&base_tim_handler_3);
}

uint32_t cycle[2] = {0};
uint32_t cycle_zq = 0;
uint32_t cycle_zq_max = 0;
uint8_t cap_flag = 0;
uint8_t cap_flag1 = 0;
uint8_t  i = 0;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    
    i = htim->Channel;
    cap_flag1++;
//    __HAL_TIM_CLEAR_FLAG(htim,TIM_FLAG_CC2);
//    __HAL_TIM_CLEAR_FLAG(htim,TIM_FLAG_CC3);
    if(htim->Instance == TIM2)
    {
                //下降沿
        if((htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3))
        {
//            if(cap_flag == 1)
//            {
                cap_flag = 2;
//                __HAL_TIM_DISABLE(htim);
                cycle[1] += cycle[1]*65535+TIM2->CNT; //计数
                cycle_zq = cycle[1] - cycle[0];
                if(cycle_zq > cycle_zq_max)
                {
                    cycle_zq_max = cycle_zq;
                }
                cycle[1] = 0;
//                __HAL_TIM_ENABLE(htim);
//                cycle[1] = 0;
//                __HAL_TIM_DISABLE_IT(htim,TIM_IT_CC2);
//                __HAL_TIM_DISABLE_IT(htim,TIM_IT_CC3);
//            }
        }
        //上升沿
        else if((htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2))
        {
//            if(cap_flag == 0)
//            {
                cap_flag = 1;
                cycle[0] = TIM2->CNT; //计数
//                cycle[1] = 0;
//            }
        }

    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
//        printf("jinshu");
//        cycle_zq++;
//        if(cycle_zq == 65535)
//        {
//            cycle_zq = 0;
//        }
        if(cap_flag == 1)
        {
            cycle[1] ++;
        }

    }
}
