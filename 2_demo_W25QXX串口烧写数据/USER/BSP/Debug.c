
#include "main.h"
#include "USER_Hardware.h"
#include "USER_BSP.h"

DMA_HandleTypeDef usart_dma_tx = {0};
DMA_HandleTypeDef usart_dma_rx = {0};
uint8_t us_buf[300];
uint8_t us_buf1[300];
uint8_t* ut_buf_ptr = us_buf;

UART_HandleTypeDef usart_handler = {0};

int fputc(int ch, FILE *f)
{
uint8_t temp[1] = {ch};
HAL_UART_Transmit(&usart_handler, temp, 1, 200);
return ch;
}

void Debug_Init()
{
//    UART_HandleTypeDef usart_handler = {0};

   GPIO_InitTypeDef gpio_handler = {0};
   
   RCC_Debug_usart_TX_GPIO_ENABLE();
   
   __HAL_RCC_AFIO_CLK_ENABLE(); //使能复用时钟
   //__HAL_AFIO_REMAP_USART1_DISABLE();//PA9 PA10
   __HAL_AFIO_REMAP_USART2_DISABLE();//PA9 PA10
//    gpio_handler.Alternate = Alternate_Debug;
   gpio_handler.Mode = GPIO_MODE_AF_PP;
   gpio_handler.Pin = Debug_usart_TX_PIN;
   gpio_handler.Pull = GPIO_NOPULL;
   gpio_handler.Speed = GPIO_SPEED_FREQ_HIGH;
   
   HAL_GPIO_Init(Debug_usart_TX_GPIO,&gpio_handler);
   
   gpio_handler.Mode = GPIO_MODE_AF_INPUT;
   gpio_handler.Pin = Debug_usart_RX_PIN;
   HAL_GPIO_Init(Debug_usart_RX_GPIO,&gpio_handler);

   RCC_Debug_usart_ENABLE();

   usart_handler.Instance = Debug_usart_Instance;

   usart_handler.Init.BaudRate = Debug_usart_BaudRate;
   usart_handler.Init.HwFlowCtl =  UART_HWCONTROL_NONE;//不启用硬件流控
   usart_handler.Init.Mode = UART_MODE_TX_RX;//收发模式
   usart_handler.Init.OverSampling = UART_OVERSAMPLING_16;
   usart_handler.Init.Parity = UART_PARITY_NONE;
   usart_handler.Init.StopBits = UART_STOPBITS_1;
   usart_handler.Init.WordLength = UART_WORDLENGTH_8B;

   HAL_UART_Init(&usart_handler);
   
   USART_DMA_Init();

   HAL_NVIC_SetPriority(USART2_IRQn,0,0);
   HAL_NVIC_EnableIRQ(USART2_IRQn);
   //使能总线空闲中断
   //__HAL_UART_ENABLE_IT(&usart_handler, UART_IT_IDLE);
   __HAL_UART_CLEAR_FLAG(&usart_handler,UART_FLAG_IDLE);//清除空闲中断标志位
   HAL_UARTEx_ReceiveToIdle_DMA(&usart_handler,us_buf,256);
   __HAL_DMA_DISABLE_IT(&usart_dma_rx, DMA_IT_HT);//失能半传输中断

   //HAL_UART_Receive_DMA(&usart_handler,us_buf,256);
//    MOD_LED_RCC_ENABLE();
   
}

extern uint8_t buf[256];
void USART_DMA_Init()
{
   __HAL_RCC_DMA1_CLK_ENABLE();

   
   usart_dma_tx.Instance = DMA1_Channel7;
   usart_dma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;//方向内存到外设
   usart_dma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;//8位数据传输
   usart_dma_tx.Init.MemInc = DMA_MINC_ENABLE; //数据地址递增
   usart_dma_tx.Init.Mode = DMA_NORMAL;
   usart_dma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;//8位数据传输
   usart_dma_tx.Init.PeriphInc = DMA_PINC_DISABLE; //寄存器地址不递增
   usart_dma_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

   __HAL_LINKDMA(&usart_handler,hdmatx,usart_dma_tx);
   HAL_DMA_Init(&usart_dma_tx);

    HAL_NVIC_SetPriority(DMA1_Channel7_IRQn,1,1);
    HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

   usart_dma_rx.Instance = DMA1_Channel6;
   usart_dma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;//方向外设到内存
   usart_dma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;//8位数据传输
   usart_dma_rx.Init.MemInc = DMA_MINC_ENABLE; //数据地址递增
   usart_dma_rx.Init.Mode = DMA_NORMAL;
   usart_dma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;//8位数据传输
   usart_dma_rx.Init.PeriphInc = DMA_PINC_DISABLE; //寄存器地址不递增
   usart_dma_rx.Init.Priority = DMA_PRIORITY_MEDIUM;

   __HAL_LINKDMA(&usart_handler,hdmarx,usart_dma_rx);
   HAL_DMA_Init(&usart_dma_rx);

   
   HAL_NVIC_SetPriority(DMA1_Channel6_IRQn,1,1);
   HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
}

extern DMA_HandleTypeDef spi_dma_tx;
extern DMA_HandleTypeDef spi_dma_rx;

void DMA1_Channel4_IRQHandler()
{
   HAL_DMA_IRQHandler(&spi_dma_rx);
}

void DMA1_Channel5_IRQHandler()
{
   HAL_DMA_IRQHandler(&spi_dma_tx);
}

void DMA1_Channel6_IRQHandler()
{
   HAL_DMA_IRQHandler(&usart_dma_rx);
}

void DMA1_Channel7_IRQHandler()
{
   HAL_DMA_IRQHandler(&usart_dma_tx);
}

void USART2_IRQHandler()
{

   HAL_UART_IRQHandler(&usart_handler);
   //触发串口空闲中断
}
uint16_t a = 0;
uint8_t b = 1;
extern uint8_t flag_wri ;
extern uint16_t len_wri;
extern uint8_t buf1[256];
extern uint8_t buf2[256];
extern uint8_t* ptr_buf;
extern  uint8_t addr1_num;
extern  uint32_t addr1;
extern uint8_t flag_tr; 
uint8_t aqq = 0;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
   if(huart->Instance == Debug_usart_Instance)
   {
       //HAL_UART_DMAStop(&usart_handler);//停止DMA传输
       a = 256 - __HAL_DMA_GET_COUNTER(&usart_dma_rx);
       
       // memcpy(buf,ut_buf_ptr,a);
       aqq++;
       flag_wri = 1;
       addr1_num++;
       if(ut_buf_ptr == us_buf)
       {
           ut_buf_ptr = us_buf1;
           ptr_buf = us_buf;
       }
       else
       {
           ut_buf_ptr = us_buf;
           ptr_buf = us_buf1;

       }
       
       HAL_UARTEx_ReceiveToIdle_DMA(&usart_handler,ut_buf_ptr,256);
       __HAL_DMA_DISABLE_IT(&usart_dma_rx, DMA_IT_HT);//失能半传输中断

   }
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
   flag_tr = 1;
   addr1_num--;
   addr1 = addr1+256;
}


