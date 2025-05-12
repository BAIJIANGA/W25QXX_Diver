#ifndef __Motor__
#define __Motor__

#define M_sleep_RCC_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define M1_IN1_RCC_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define M1_IN2_RCC_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define M2_IN1_RCC_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define M2_IN2_RCC_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()

#define M_sleep_GPIO  GPIOA
#define M1_IN1_GPIO  GPIOB
#define M1_IN2_GPIO  GPIOB
#define M2_IN1_GPIO  GPIOB
#define M2_IN2_GPIO  GPIOB

#define M_sleep_PIN   GPIO_PIN_8
#define M1_IN1_PIN   GPIO_PIN_6
#define M1_IN2_PIN   GPIO_PIN_7
#define M2_IN1_PIN   GPIO_PIN_8
#define M2_IN2_PIN   GPIO_PIN_9


#define M_sleep(x)    x?HAL_GPIO_WritePin (M_sleep_GPIO,M_sleep_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (M_sleep_GPIO,M_sleep_PIN,GPIO_PIN_RESET)
#define M1_IN1(x)      x?HAL_GPIO_WritePin (M1_IN1_GPIO,M1_IN1_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (M1_IN1_GPIO,M1_IN1_PIN,GPIO_PIN_RESET)
#define M1_IN2(x)      x?HAL_GPIO_WritePin (M1_IN2_GPIO,M1_IN2_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (M1_IN2_GPIO,M1_IN2_PIN,GPIO_PIN_RESET)
#define M2_IN1(x)      x?HAL_GPIO_WritePin (M2_IN1_GPIO,M2_IN1_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (M2_IN1_GPIO,M2_IN1_PIN,GPIO_PIN_RESET)
#define M2_IN2(x)      x?HAL_GPIO_WritePin (M2_IN2_GPIO,M2_IN2_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (M2_IN2_GPIO,M2_IN2_PIN,GPIO_PIN_RESET)


#define M1_IN1_UPDATA(x) (TIM4->CCR1 = x)
#define M1_IN2_UPDATA(x) (TIM4->CCR2 = x)
#define M2_IN1_UPDATA(x) (TIM4->CCR3 = x)
#define M2_IN2_UPDATA(x) (TIM4->CCR4 = x)


void Motor_Init(void);

#endif

