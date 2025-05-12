#include "main.h"
//#include "usbd_hid.h"
#include "usbd_customhid.h"
#include "usbd_ctlreq.h"
#include "usb_device.h"
//#include "FreeRTOS.h"
//#include "USER_Hardware.h"
#include "USER_BSP.h"
//#include "cmsis_os.h"
//#include "task.h"
extern USBD_HandleTypeDef hUsbDeviceFS;
uint16_t remote_sensing_value[6] = {0};

uint8_t KEY_Value[6] = {0,1,2,3,4,5};//������ֵ����
uint8_t KEY_STATE[6] = {1,1,1,1,1,1};//��ǰ������״̬
uint8_t OLD_KEY_STATE[6] = {1,1,1,1,1,1};//��һ�ΰ�����״̬

uint8_t MOUSE_BUF[5] = {0,0,0,0,0}; //��걨���ֽ�
uint8_t Keyboard_BUF[15] = {0,0,0,0,0}; //���̱����ֽ�
void KEY_Init()
{
    GPIO_InitTypeDef GPIO_handler = {0};
    KEY0_CLK_ENABLE();
    KEY1_CLK_ENABLE();
    KEY2_CLK_ENABLE();

    GPIO_handler.Mode = GPIO_MODE_INPUT;
    GPIO_handler.Pin = KEY0_PIN;
    GPIO_handler.Pull = GPIO_PULLDOWN;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(KEY0_GPIO,&GPIO_handler);

    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Pin = KEY1_PIN;
    HAL_GPIO_Init(KEY1_GPIO,&GPIO_handler);

    GPIO_handler.Pin = KEY2_PIN;
    HAL_GPIO_Init(KEY2_GPIO,&GPIO_handler);

}

void KEY_Judgment()
{
    static uint8_t OLD_KEY_STATE[6] = {1,1,1};
    for(int a = 0;a < 3;a++)
    {
        if(KEY_STATE[a] != OLD_KEY_STATE[a])
        {
            if(KEY_STATE[a] == 1)
            {
                Keyaction_HIGH(KEY_Value[a]);
            }
            else if(KEY_STATE[a] == 0)
            {
                Keyaction_DOWN(KEY_Value[a]);
            }
            OLD_KEY_STATE[a] = KEY_STATE[a];
        }
    }
}

void KEY_SCAN()
{
    static uint8_t KEY_BUF[6] = {0XFF,0XFF,0XFF,0XFF};

    KEY_BUF[0] = (KEY_BUF[0]<<1)|KEY0;
    KEY_BUF[1] = (KEY_BUF[1]<<1)|KEY1;
    KEY_BUF[2] = (KEY_BUF[2]<<1)|KEY2;
//    KEY_BUF[3] = (KEY_BUF[3]<<1)|KEY3;
//    KEY_BUF[4] = (KEY_BUF[4]<<1)|SW1;
//    KEY_BUF[5] = (KEY_BUF[5]<<1)|SW2;
    
    for(uint8_t a = 0;a < 3;a++)
    {
        if(((KEY_BUF[a])&0x0F) == 0x0F)
        {
            KEY_STATE[a] = 1;
        }
        else if(((KEY_BUF[a])|0xF0) == 0xF0)
        {
            KEY_STATE[a] = 0;
        }
    }

}

//��������
void Keyaction_HIGH(uint8_t Value)
{
//    uint8_t MOUSE_BUF[5] = {0,0,0,0,0};
    switch(Value)
    {
        case 0 : 
            Keyboard_BUF[2] |= 0xFF;
            Keyboard_BUF[3] |= 0xFF;
            Keyboard_BUF[4] |= 0xFF;
            Keyboard_BUF[5] |= 0xFF;
            Keyboard_BUF[6] |= 0x0F;
//            USBD_HID_SendReport(&hUsbDeviceFS, Keyboard_BUF, 15);
//            MOUSE_BUF[0] |= 0x01;
            LED1(1);
//            USBD_HID_SendReport(&hUsbDeviceFS, MOUSE_BUF, 5);
//            HAL_Delay(10);
//            MOUSE_BUF[0] &= 0x00;
//            USBD_HID_SendReport(&hUsbDeviceFS, MOUSE_BUF, 5);
        break;
        case 1 : 
            LED1(0);
        break;
        case 2 : 
            LED1_Toggle();
        break;
    }
}

//��������
void Keyaction_DOWN(uint8_t Value)
{
    switch(Value)
    {
        case 0 :
            Keyboard_BUF[2] = 0x00;
            Keyboard_BUF[3] = 0x00;
            Keyboard_BUF[4] = 0x00;
            Keyboard_BUF[5] = 0x00;
            Keyboard_BUF[6] = 0x00;
//            USBD_HID_SendReport(&hUsbDeviceFS, Keyboard_BUF, 15);
//            MOUSE_BUF[0] &= 0x00;
//            USBD_HID_SendReport(&hUsbDeviceFS, MOUSE_BUF, 5);
        break;
        case 1 : 
            LED1(0);
        break;
        case 2 :
            LED1_Toggle();
        break;
    }
}

static void KEY_74HC165_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_handler = {0};
    
    KEY_74HC165_CLK_GPIO_RCC_ENABLE();
    KEY_74HC165_QH_GPIO_RCC_ENABLE();
    KEY_74HC165_PL_GPIO_RCC_ENABLE();
    KEY_74HC165_CE_GPIO_RCC_ENABLE();
    
    GPIO_handler.Mode = GPIO_MODE_AF_PP;
    GPIO_handler.Pin = K_74HC165_CLK_PIN;
    GPIO_handler.Pull = GPIO_PULLDOWN;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(K_74HC165_CLK_GPIO,&GPIO_handler);

    GPIO_handler.Mode = GPIO_MODE_AF_INPUT;
    GPIO_handler.Pin = K_74HC165_QH_PIN;//74HC165 ��ȡ���ݽ�
    HAL_GPIO_Init(K_74HC165_QH_GPIO,&GPIO_handler);

    GPIO_handler.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_handler.Pin = K_74HC165_PL_PIN;
    GPIO_handler.Pull = GPIO_NOPULL;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(K_74HC165_PL_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = K_74HC165_CE_PIN;
    HAL_GPIO_Init(K_74HC165_CE_GPIO,&GPIO_handler);
    K_PL(0);//�͵�ƽ��Ч ���ж�ȡ����
    K_CE(1);//�͵�ƽ��Ч
}

SPI_HandleTypeDef KEY_74HC165_SPI_handler = {0};

void KEY_Init_74HC165()
{
    KEY_74HC165_GPIO_Init();//��ʼ��GPIO����
    
    __HAL_RCC_AFIO_CLK_ENABLE(); //ʹ�ܸ���ʱ��

    __HAL_AFIO_REMAP_SPI1_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    KEY_74HC165_SPI_handler.Init.Mode = SPI_MODE_MASTER; //����ģʽ
    KEY_74HC165_SPI_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; //SPI CRCӲ��У�� У���븽�����ݽ��з���
    KEY_74HC165_SPI_handler.Init.BaudRatePrescaler  =SPI_BAUDRATEPRESCALER_2; //�����ʷ�Ƶ 2��Ƶ 64/2 = 32MHZ
    KEY_74HC165_SPI_handler.Init.CLKPhase  =SPI_PHASE_1EDGE; //�ڼ���ʱ���ؿ�ʼ����
    KEY_74HC165_SPI_handler.Init.CLKPolarity = SPI_POLARITY_LOW; //SCL���߿���ʱ����Ϊ��
    KEY_74HC165_SPI_handler.Init.CRCPolynomial = 2; //CRCУ�����Ķ���ʽ û��Ӧ��û��
    KEY_74HC165_SPI_handler.Init.DataSize = SPI_DATASIZE_8BIT; //���ݷ���֡��ʽ
    KEY_74HC165_SPI_handler.Init.Direction = SPI_DIRECTION_2LINES; //˫��ȫ˫��
    KEY_74HC165_SPI_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;//���ݸ�λ��ǰ �����λ��ǰ
    KEY_74HC165_SPI_handler.Init.NSS = SPI_NSS_SOFT; //���Ƭѡ�� Ӳ��Ƭѡ��ΪPA4��ռ��
    KEY_74HC165_SPI_handler.Init.TIMode = SPI_TIMODE_DISABLE;//����ʱ�Ӽ��Ժ���λ������ ��ΪĬ������
//    SPI_handler.Init.
    KEY_74HC165_SPI_handler.Instance = SPI1;
    HAL_SPI_Init(&KEY_74HC165_SPI_handler);
}

uint8_t keyboard_value[25] = {0,0x14,0,0,0,0,0,0,0,0,0,0,0,0};//��е���� ��ֵ����

uint8_t KEY_74HC165_STATE[100] = {1,1,1,1,1,1};//��ǰ������״̬
uint8_t OLD_KEY_74HC165_STATE[100] = {1,1,1,1,1,1};//��һ�ΰ�����״̬
void KEY_74HC165_READ(uint8_t KEY_num,uint8_t* KEY_VALE_BUF)
{
    uint8_t num = KEY_num/8 + 1;
//    K_PL(0);
    K_PL(1);
    K_CE(0);
    HAL_SPI_Receive(&KEY_74HC165_SPI_handler,KEY_VALE_BUF,num,100);
//    HAL_SPI_Receive(&KEY_74HC165_SPI_handler,KEY_VALE_BUF+1,1,100);
    K_PL(0);
    K_CE(1);
}
void r_ad(void);
void Keyaction_74HC165_HIGH(uint8_t a);
void Keyaction_74HC165_DOWN(uint8_t a);
uint8_t pad_vale[6] = {0};
void KEY_74HC165_SCAN()
{
    static uint8_t KEY_BUF[100] = {0};
    static uint8_t keyboard_value_buf[16] = {0};
    KEY_74HC165_READ(100,keyboard_value_buf);
//    KEY_BUF[0] = (KEY_BUF[0]<<1)|KEY0;
//    KEY_BUF[1] = (KEY_BUF[1]<<1)|KEY1;
//    KEY_BUF[2] = (KEY_BUF[2]<<1)|KEY2;
//    for(uint8_t a = 0;a < 100;a++)
//    {
//        uint8_t len = a%8;
//        KEY_BUF[a] = (KEY_BUF[a]<<1)|(keyboard_value[len]&(0x01<<a));
//    }
    for(uint8_t a = 0;a < 100%8+1;a++)
    {
        for(uint8_t b = 0;b < 8;b++)
        {
            KEY_BUF[a*8+b] = (KEY_BUF[a*8+b]<<1)|((keyboard_value_buf[a]&(0x01<<b))>>b);
        }
    }
//    KEY_BUF[3] = (KEY_BUF[3]<<1)|KEY3;
//    KEY_BUF[4] = (KEY_BUF[4]<<1)|SW1;
//    KEY_BUF[5] = (KEY_BUF[5]<<1)|SW2;
    
//    for(uint8_t a = 0;a < 3;a++)
//    {
//        if(((KEY_BUF[a])&0x0F) == 0x0F)
//        {
//            KEY_STATE[a] = 1;
//        }
//        else if(((KEY_BUF[a])|0xF0) == 0xF0)
//        {
//            KEY_STATE[a] = 0;
//        }
//    }
    for(uint8_t a = 0;a < 100;a++)
    {
         if(((KEY_BUF[a])&0x0F) == 0x0F)
        {
            KEY_74HC165_STATE[a] = 1;
            Keyaction_74HC165_HIGH(a);
        }
        else if(((KEY_BUF[a])|0xF0) == 0xF0)
        {
            KEY_74HC165_STATE[a] = 0;
            Keyaction_74HC165_DOWN(a);
        }
    }
//    keyboard_value[4] = 255 - 255*((float)remote_sensing_value[1]/(4040-0));
//    keyboard_value[5] = 255*((float)remote_sensing_value[0]/(4096-0));
//    r_adjust(&keyboard_value[2],remote_sensing_value[1],2223,100,100);
//    r_adjust(&keyboard_value[3],remote_sensing_value[0],2285,100,100);
//    USBD_HID_SendReport(&hUsbDeviceFS, keyboard_value, 6);
    r_ad();
    USBD_CUSTOM_HID_SendReport (&hUsbDeviceFS,keyboard_value,20);
//    USBD_CUSTOM_HID_SendReport (&hUsbDeviceFS,keyboard_value,20);
//    hUsbDeviceFS.ep_out[2].maxpacket
//USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,(u8*)&TXData, 20);
}

//void KEY_74HC165_Judgment()
//{
////    static uint8_t OLD_KEY_STATE[6] = {1,1,1};
//    uint8_t flag = 0;
//    for(int a = 0;a < 100;a++)
//    {
//        if(KEY_74HC165_STATE[a] != OLD_KEY_74HC165_STATE[a])
//        {
//            flag  = 1;
//            if(KEY_74HC165_STATE[a] == 1)
//            {
////                Keyaction_HIGH(KEY_Value[a]);
//                Keyaction_74HC165_HIGH(a);
//            }
//            else if(KEY_74HC165_STATE[a] == 0)
//            {
////                Keyaction_DOWN(KEY_Value[a]);
//                Keyaction_74HC165_DOWN(a);
//            }
//            OLD_KEY_74HC165_STATE[a] = KEY_74HC165_STATE[a];
//        }
//    }
//    if(flag)
//    {
//        flag = 0;
//        USBD_HID_SendReport(&hUsbDeviceFS, keyboard_value, 15);
//    }
//}

void Keyaction_74HC165_HIGH(uint8_t a)
{   
    switch(a)
    {
        case 0 :  keyboard_value[2] &= (~(0x01<<0));break;//ʮ�ּ���
        case 1 :  keyboard_value[2] &= (~(0x01 << 2));break;//ʮ�ּ���
        case 2 :  keyboard_value[2] &= (~(0x01 << 1));break;//ʮ�ּ���
        case 3 :  keyboard_value[2] &= (~(0x01 << 5));break;//�м����
        case 4 :  keyboard_value[2] &= (~(0x01 << 7));break;//��ң�а���
//        case 5 :  keyboard_value[2] &= (~(0x01 << 5));break;//�м����
        case 6 :  keyboard_value[2] &= (~(0x01 << 6));break;//��ң�а���
        case 7 :  keyboard_value[2] &= (~(0x01 << 3));break;//ʮ�ּ���
        case 8 :  keyboard_value[3] &= (~(0x01 << 6));break;//��X
        case 9 :  keyboard_value[3] &= (~(0x01 << 4));break;//��A
        case 10 :  keyboard_value[2] &= (~(0x01 << 4));break;//�м����
//        case 11 :  keyboard_value[2] &= (~(0x01 << 3));break;//�м����
        case 12 :  keyboard_value[3] &= (~(0x01 << 2));break;//�м����
        case 13 :  keyboard_value[3] &= (~(0x01 << 1));break;//��RB
        case 14 :  keyboard_value[3] &= (~(0x01 << 7));break;//��Y
        case 15 :  keyboard_value[3] &= (~(0x01 << 5));break;//��B
        case 16 :  keyboard_value[3] &= (~(0x01 << 0));break;//��LB
        default : break;
    }
}

void Keyaction_74HC165_DOWN(uint8_t a)
{
    switch(a)
    {
        case 0 :  keyboard_value[2] |= (0x01 << 0);break;//ʮ�ּ���
        case 1 :  keyboard_value[2] |= (0x01 << 2);break;//ʮ�ּ���
        case 2 :  keyboard_value[2] |= (0x01 << 1);break;
        case 3 :  keyboard_value[2] |= (0x01 << 5);break;
        case 4 :  keyboard_value[2] |= (0x01 << 7);break;
//        case 5 :  keyboard_value[2] |= (0x01 << 5);break;
        case 6 :  keyboard_value[2] |= (0x01 << 6);break;
        case 7 :  keyboard_value[2] |= (0x01 << 3);break;
        case 8 :  keyboard_value[3] |= (0x01 << 6);break;
        case 9 :  keyboard_value[3] |= (0x01 << 4);break;
        case 10 :  keyboard_value[2] |= (0x01 << 4);break;
//        case 11 :  keyboard_value[2] |= (0x01 << 4);break;
        case 12 :  keyboard_value[3] |= (0x01 << 2);break;
        case 13 :  keyboard_value[3] |= (0x01 << 1);break;
        case 14 :  keyboard_value[3] |= (0x01 << 7);break;
        case 15 :  keyboard_value[3] |= (0x01 << 5);break;
        case 16 :  keyboard_value[3] |= (0x01 << 0);break;
        default : break;
    }
}


DMA_HandleTypeDef dma_hander_adc = {0};
ADC_HandleTypeDef adc_handler = {0};

void remote_sensing_DMA_INIT()
{
    __HAL_RCC_DMA1_CLK_ENABLE();

//    HAL_DMA_DeInit(&dma_hander_tx);
    dma_hander_adc.Instance = DMA1_Channel1;
    dma_hander_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_hander_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD; //16λ���ݴ���
    dma_hander_adc.Init.MemInc = DMA_MINC_ENABLE; //���ݵ���
    dma_hander_adc.Init.Mode = DMA_CIRCULAR;
    dma_hander_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; //16λ���ݴ���
    dma_hander_adc.Init.PeriphInc = DMA_PINC_DISABLE; //�Ĵ�����ַ������
    dma_hander_adc.Init.Priority = DMA_PRIORITY_HIGH;

    __HAL_LINKDMA(&adc_handler,DMA_Handle,dma_hander_adc);
    HAL_DMA_Init(&dma_hander_adc);
    HAL_ADC_Start_DMA(&adc_handler,(uint32_t*)remote_sensing_value,7);
//    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn,0,0);
//    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}

void remote_sensing_Init() //ң�г�ʼ��
{

    GPIO_InitTypeDef GPIO_handler = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    GPIO_handler.Mode = GPIO_MODE_AF_INPUT;
    GPIO_handler.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA,&GPIO_handler);

    GPIO_handler.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB,&GPIO_handler);
    
// ADC ����ת��ģʽ��һֱִ�У������ʱ����ADC��δʹ��DMA��CPU��Ƶ���Ľ����жϣ�����ռ�ô���CPU��Դ
//ADC ɨ��ģʽ�¿����ж� ��������ͨ��ת�����֮�󷢳��ж����� ����ֻ�ܵõ����һ��ͨ��������
    __HAL_RCC_ADC1_CLK_ENABLE();

    adc_handler.Init.ContinuousConvMode = ENABLE;//�ŵ�ת��ģʽ ����ת��
    adc_handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;//�����Ҷ���
    adc_handler.Init.DiscontinuousConvMode = DISABLE;//�����ģʽ
    adc_handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;//����Դѡ��
    adc_handler.Init.NbrOfConversion = 7;//��Ҫת����ͨ����
    adc_handler.Init.NbrOfDiscConversion  =  1; //ת��ͨ����
    adc_handler.Init.ScanConvMode = ADC_SCAN_ENABLE;//ɨ��ģʽʹ��
    adc_handler.Instance = ADC1;

    HAL_ADC_Init(&adc_handler); //��ʼ��ADC

    ADC_ChannelConfTypeDef ch_handler = {0};

    ch_handler.Channel = ADC_CHANNEL_1;
    ch_handler.Rank = 1;
    ch_handler.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;//ADC_SAMPLETIME_1CYCLE_5;//ADC_SAMPLETIME_13CYCLES_5;
    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);

    ch_handler.Channel = ADC_CHANNEL_2;
    ch_handler.Rank = 2;
    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);

    ch_handler.Channel = ADC_CHANNEL_3;
    ch_handler.Rank = 3;
    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);
    
    ch_handler.Channel = ADC_CHANNEL_4;
    ch_handler.Rank = 4;
    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);
    
    ch_handler.Channel = ADC_CHANNEL_5;
    ch_handler.Rank = 5;
    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);
    
    ch_handler.Channel = ADC_CHANNEL_6;
    ch_handler.Rank = 6;
    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);
    
    ch_handler.Channel = ADC_CHANNEL_8;
    ch_handler.Rank = 7;
    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);
//    ch_handler.Channel = ADC_CHANNEL_3;
//    ch_handler.Rank = 3;
//    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);

//    ch_handler.Channel = ADC_CHANNEL_4;
//    ch_handler.Rank = 4;
//    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);

//    ch_handler.Channel = ADC_CHANNEL_9;
//    ch_handler.Rank = 5;
//    ch_handler.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
//    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);
    
//    ch_handler.Channel = ADC_CHANNEL_0;
//    ch_handler.Rank = 6;
//    ch_handler.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
////    ch_handler.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//    HAL_ADC_ConfigChannel(&adc_handler,&ch_handler);

    HAL_ADCEx_Calibration_Start(&adc_handler);//У׼ADC

    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};//����ADCʱ��

    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;    /* ADC����ʱ�� */
    adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV8;       /* ��Ƶ����6ʱ��Ϊ64M/8=8MHz */
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);                 /* ����ADCʱ�� */

    remote_sensing_DMA_INIT();

}

void r_adjust(uint8_t* k_value,uint16_t a,uint16_t m_m,uint8_t m_min,uint8_t m_max)
{
    if(a < m_m && (m_m-a) > 100)
    {
        * k_value = 256*((float)(a-(m_m-m_min))/4095);
    }
    else if(a > m_m && (a - m_m) > 100)
    {
        * k_value = 256*((float)(a-(m_m-m_max))/4095);
    }
    else
    {
        * k_value = 256/2;
    }
}

void r_ad()
{
    uint16_t a = 0;
    //RX
    if(remote_sensing_value[0] < 2400 && remote_sensing_value[0] > 2160)
    {//���ڹ���
        keyboard_value[10] = (uint8_t)65535; //��λֵ
        keyboard_value[11] = (uint8_t)(65535>>8);
    }
    else if(remote_sensing_value[0] > 4070) //��Ϊ���
    {
        keyboard_value[10] = (uint8_t)(32768+10); //���ֵ
        keyboard_value[11] = (uint8_t)((32768+10)>>8);
    }
    else if(remote_sensing_value[0] < 70) //��Ϊ��С
    {
        keyboard_value[10] = (uint8_t)(32768-10); //��Сֵ
        keyboard_value[11] = (uint8_t)((32768-10)>>8);
    }
    else if(remote_sensing_value[0] < 2160)
    {
        a = 32768.0 - 100 -((32768.0 - 100)*((float)remote_sensing_value[0]/(float)(2160)));
        keyboard_value[10] = (uint8_t)(a);
        keyboard_value[11] = (uint8_t)(a>>8);
    }
    else if(remote_sensing_value[0] > 2400)
    {
        a = 65535 - (32768.0+100)*((float)(remote_sensing_value[0]-2400)/(float)(4090-2400));
        keyboard_value[10] = (uint8_t)(a);
        keyboard_value[11] = (uint8_t)(a>>8);
    }


    //RY
    if(remote_sensing_value[1] < 2400 && remote_sensing_value[1] > 2160)
    {//���ڹ���
        keyboard_value[12] = (uint8_t)65535; //��λֵ
        keyboard_value[13] = (uint8_t)(65535>>8);
    }
    else if(remote_sensing_value[1] > 4050) //��Ϊ���
    {
        keyboard_value[12] = (uint8_t)(32768-10); //���ֵ
        keyboard_value[13] = (uint8_t)((32768-10)>>8);
    }
    else if(remote_sensing_value[1] < 390) //��Ϊ��С
    {
        keyboard_value[12] = (uint8_t)(32768+10); //��Сֵ
        keyboard_value[13] = (uint8_t)((32768+10)>>8);
    }
    else if(remote_sensing_value[1] < 2160)
    {
        a = 65535 - ((32768.0 + 5000)*((float)(2160 - remote_sensing_value[1] )/(float)(2160)));
        keyboard_value[12] = (uint8_t)(a);
        keyboard_value[13] = (uint8_t)(a>>8);
    }
    else if(remote_sensing_value[1] > 2400)
    {
        a = (32768.0-100)*((float)(remote_sensing_value[1] -  2400)/(float)(4090-2400));
        keyboard_value[12] = (uint8_t)(a);
        keyboard_value[13] = (uint8_t)(a>>8);
    }

    //LX
    if(remote_sensing_value[2] < 2300 && remote_sensing_value[2] > 2160)
    {//���ڹ���
        keyboard_value[6] = (uint8_t)65535; //��λֵ
        keyboard_value[7] = (uint8_t)(65535>>8);
    }
    else if(remote_sensing_value[2] > 4070) //��Ϊ���
    {
        keyboard_value[6] = (uint8_t)(32768+10); //���ֵ
        keyboard_value[7] = (uint8_t)((32768+10)>>8);
    }
    else if(remote_sensing_value[2] < 70) //��Ϊ��С
    {
        keyboard_value[6] = (uint8_t)(32768-10); //��Сֵ
        keyboard_value[7] = (uint8_t)((32768-10)>>8);
    }
    else if(remote_sensing_value[2] < 2160)
    {
        a = 32768.0 - 100 -((32768.0 - 100)*((float)remote_sensing_value[2]/(float)(2160)));
        keyboard_value[6] = (uint8_t)(a);
        keyboard_value[7] = (uint8_t)(a>>8);
    }
    else if(remote_sensing_value[2] > 2400)
    {
        a = 65535 - (32768.0+100)*((float)(remote_sensing_value[2]-2400)/(float)(4090-2400));
        keyboard_value[6] = (uint8_t)(a);
        keyboard_value[7] = (uint8_t)(a>>8);
    }

    //LY
    if(remote_sensing_value[3] < 2300 && remote_sensing_value[3] > 2160)
    {//���ڹ���
        keyboard_value[8] = (uint8_t)65535; //��λֵ
        keyboard_value[9] = (uint8_t)(65535>>8);
    }
    else if(remote_sensing_value[3] > 4070) //��Ϊ���
    {
        keyboard_value[8] = (uint8_t)(32768-10); //���ֵ
        keyboard_value[9] = (uint8_t)((32768-10)>>8);
    }
    else if(remote_sensing_value[3] < 70) //��Ϊ��С
    {
        keyboard_value[8] = (uint8_t)(32768+10); //��Сֵ
        keyboard_value[9] = (uint8_t)((32768+10)>>8);
    }
    else if(remote_sensing_value[3] < 2160)
    {
        a = 65535 -((32768.0 - 100)*((float)(2160-remote_sensing_value[3])/(float)(2160)));
        keyboard_value[8] = (uint8_t)(a);
        keyboard_value[9] = (uint8_t)(a>>8);
    }
    else if(remote_sensing_value[3] > 2400)
    {
        a =  (32768.0-100)*((float)(remote_sensing_value[3]-2400)/(float)(4090-2400));
        keyboard_value[8] = (uint8_t)(a);
        keyboard_value[9] = (uint8_t)(a>>8);
    }

    if(remote_sensing_value[4] > 2250)
    {
        keyboard_value[4] = 0;
    }
    else if (remote_sensing_value[4] < 80)
    {
        keyboard_value[4] = 255;
    }
    else 
    {
        keyboard_value[4] = 255*((2250 - remote_sensing_value[4])/2250.0);
    }
    
    if(remote_sensing_value[5] > 2250)
    {
        keyboard_value[5] = 0;
    }
    else if (remote_sensing_value[5] < 80)
    {
        keyboard_value[5] = 255;
    }
    else 
    {
        keyboard_value[5] = (255*((float)(2250 - remote_sensing_value[5])/2250.0));
    }
}

//ADC�жϺ���
void ADC1_2_IRQHandler()
{
    HAL_ADC_IRQHandler(&adc_handler);
}

//ADC�ж���ɻص�����
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

}
