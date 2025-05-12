#ifndef __USB__
#define __USB__

//#include "USB_USER_report.h"
#include "main.h"

#define USER_HID_REPORT_DSC  HID_pad_ReportDesc;
#define USER_HID_REPORT_LEN  sizeof(HID_pad_ReportDesc)


#define EN_USB_RCC_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define EN_USB_GPIO  GPIOA

#define EN_USB_PIN   GPIO_PIN_7

#define EN_USB(x)    x?HAL_GPIO_WritePin (EN_USB_GPIO,EN_USB_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin (EN_USB_GPIO,EN_USB_PIN,GPIO_PIN_RESET)

void USB_EN_Init(void);
void USB_ENABLE(void);
void USB_DISENABLE(void);
void USB_OS_RX_decode(void);

#endif
