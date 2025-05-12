#include "main.h"
#include "USER_BSP.h"

TIM_HandleTypeDef Motor_pwm_handler = {0};
TIM_OC_InitTypeDef Motor_ch_handler = {0};

void Motor_Init()
{
    GPIO_InitTypeDef gpio_handler = {0};

    M_sleep_RCC_ENABLE();
    M1_IN1_RCC_ENABLE();
    M1_IN2_RCC_ENABLE();
    M2_IN1_RCC_ENABLE();
    M2_IN2_RCC_ENABLE();

    __HAL_RCC_TIM4_CLK_ENABLE();

    Motor_pwm_handler.Instance = TIM4;
    Motor_pwm_handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    Motor_pwm_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    Motor_pwm_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    Motor_pwm_handler.Init.Period = 200-1;
    Motor_pwm_handler.Init.Prescaler = 72-1;
    Motor_pwm_handler.Init.RepetitionCounter = 0;

    HAL_TIM_PWM_Init(&Motor_pwm_handler);
    
    Motor_ch_handler.OCIdleState = TIM_OCIDLESTATE_RESET;
    Motor_ch_handler.OCMode = TIM_OCMODE_PWM1;
    Motor_ch_handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    Motor_ch_handler.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    Motor_ch_handler.OCPolarity = TIM_OCPOLARITY_HIGH;

    Motor_ch_handler.Pulse = 0;

    HAL_TIM_PWM_ConfigChannel(&Motor_pwm_handler,&Motor_ch_handler,TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&Motor_pwm_handler,&Motor_ch_handler,TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&Motor_pwm_handler,&Motor_ch_handler,TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&Motor_pwm_handler,&Motor_ch_handler,TIM_CHANNEL_4);
    
    __HAL_RCC_AFIO_CLK_ENABLE(); //使能复用时钟

    __HAL_AFIO_REMAP_TIM4_DISABLE();
    
    gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_handler.Pin = M_sleep_PIN;
    gpio_handler.Pull = GPIO_NOPULL;
    gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(M_sleep_GPIO,&gpio_handler);

    gpio_handler.Mode = GPIO_MODE_AF_PP;
    gpio_handler.Pin = M1_IN1_PIN;
    HAL_GPIO_Init(M1_IN1_GPIO,&gpio_handler);

    gpio_handler.Pin = M1_IN2_PIN;
    HAL_GPIO_Init(M2_IN2_GPIO,&gpio_handler);
    
    gpio_handler.Pin = M2_IN1_PIN;
    HAL_GPIO_Init(M1_IN1_GPIO,&gpio_handler);

    gpio_handler.Pin = M2_IN2_PIN;
    HAL_GPIO_Init(M2_IN2_GPIO,&gpio_handler);

    HAL_TIM_PWM_Start(&Motor_pwm_handler,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&Motor_pwm_handler,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&Motor_pwm_handler,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&Motor_pwm_handler,TIM_CHANNEL_4);

    M_sleep(1);
    M1_IN1_UPDATA(0);
    M1_IN2_UPDATA(0);
    M2_IN1_UPDATA(0);
    M2_IN2_UPDATA(0);
//    M1_IN2(0);
//    M1_IN1(0);
//    M2_IN2(0);
//    M2_IN1(0);
}

