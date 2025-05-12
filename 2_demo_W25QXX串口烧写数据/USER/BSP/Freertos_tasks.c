#include "main.h"
#include "FreeRTOS.h"
#include "USER_Hardware.h"
#include "USER_BSP.h"
#include "cmsis_os.h"
#include "task.h"
#include "queue.h"

TickType_t KEY_TIME_PRE;
void KEY_OS_TASK(void *argument)
{
    KEY_TIME_PRE = xTaskGetTickCount();
    while(1)
    {

        KEY_Judgment();
        KEY_SCAN();
        vTaskDelay(1);//挂起1ms主动放弃CPU
        vTaskDelayUntil(&KEY_TIME_PRE,5);
    }
}

void LED_OS_TASK(void *argument)
{
    while(1)
    {
        LED_OS_TEST();
    }
}
uint8_t use_spi = 1;
extern QueueHandle_t OLED_SPI_Que_handler;
void OLED_OS_TASK(void *argument)
{
    struct lll* buf_s = argument;
//    static uint8_t buf = 0;
    uint32_t cnt = 0; //任务是一直在跑动的所有的东西都保存在栈里面
    uint8_t oled_free = 1;
    while(1)
    {
//        if(use_spi)
//        {
        if(xQueueReceive(OLED_SPI_Que_handler,&oled_free,portMAX_DELAY) == pdPASS)
        {
//            use_spi = 0;
            OLED_ShowNum(buf_s->x,buf_s->y,cnt++,7,16,1);
            OLED_Refresh();//不加延时最可能这里被切换
//            use_spi = 1;
            xQueueSend(OLED_SPI_Que_handler,&oled_free,NULL);
        }
//        }
//        OLED_Refresh();
        HAL_Delay(500);
//        buf++;
//        if(buf >= 200)
//        {
//            buf = 0;
//        }
    }
}
