#include "USER_BSP.h"
#include "USB_USER_report.h"
#include "USB.h"
#include "string.h"
void USB_EN_Init()
{
    GPIO_InitTypeDef gpio_handler = {0};

    EN_USB_RCC_ENABLE();

    gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_handler.Pin = EN_USB_PIN;
    gpio_handler.Pull = GPIO_NOPULL;
    gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(EN_USB_GPIO,&gpio_handler);

    USB_DISENABLE();
    HAL_Delay(50);
    USB_ENABLE();
}

void USB_ENABLE()
{
    EN_USB(1);
}

void USB_DISENABLE()
{
    EN_USB(0);
}

extern uint8_t USB_RX_Buffer[9];
extern uint16_t remote_sensing_value[7];
uint8_t USB_RX_Buffer1[8];
//USBΩ” ’Ω‚¬Î
void USB_OS_RX_decode(void)
{
//volatile uint8_t a = ((USB_RX_Buffer[3]/255.0)*80.0+ 119);
//volatile uint8_t c = ((USB_RX_Buffer[4]/255.0)*80.0+ 119);
    while(1)
    {
		if(USB_RX_Buffer[8] == 1 )
	    {
            for(uint8_t a = 0;a < 8;a++)
            {
                USB_RX_Buffer1[a] = USB_RX_Buffer[a];
            }
              USB_RX_Buffer[8] = 0;
//            USB_RX_Buffer[1] = 0x08;
	          if((0x08 == USB_RX_Buffer1[1]))
	          {
//                  int a = 0;
	              if(USB_RX_Buffer1[4] != 0x00)
	              {
	                    M2_IN1_UPDATA(((USB_RX_Buffer1[4]/255.0)*80.0+ 119));
                        M2_IN2_UPDATA(0);
	              }
	              else
	              {
                        M2_IN1_UPDATA(0);
                        M2_IN2_UPDATA(0);
	              }

	              if(USB_RX_Buffer1[3] != 0x00)
	              {
	                    M1_IN1_UPDATA(((USB_RX_Buffer1[3]/255.0)*80.0+ 119));
                        M1_IN2_UPDATA(0);
                  }
	              else
	              {
                        M1_IN1_UPDATA(0);
                        M1_IN2_UPDATA(0);
	              }
	          }
		}
//        printf("%d\r\n",remote_sensing_value[6]);
//		HAL_Delay(1000);
	}
}

