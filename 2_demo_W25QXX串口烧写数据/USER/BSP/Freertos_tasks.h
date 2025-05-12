#ifndef __FREERTOS_TASKS__
#define __FREERTOS_TASKS__

#include "main.h"

struct lll
{
    uint8_t x;
    uint8_t y;
};

void LED_OS_TASK(void *argument);
void OLED_OS_TASK(void *argument);
void KEY_OS_TASK(void *argument);
#endif
