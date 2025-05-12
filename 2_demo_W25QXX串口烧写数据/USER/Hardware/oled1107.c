#include "oled1107.h"
#include "stdlib.h"
#include "oledfont.h"
#include "math.h"
#include "USER_Hardware.h"
#include "USER_BSP.h"
#include "string.h"

#if (USE_OLED_128_128 == 1)

unsigned char OLED_GRAM[16][128];//[16];
uint8_t OLED_GRAM_buf[128*16+5];
uint8_t OLED_SHOW_BUF[25];

#if (OLED_128_128_MOD == OLED_128_128_HW)
SPI_HandleTypeDef OLED_HW_handler = {0};
#endif

#if (OLED_128_128_MOD == OLED_128_128_HW_DMA)

SPI_HandleTypeDef OLED_HW_handler = {0};
DMA_HandleTypeDef OLED_dma_hander_tx = {0};
DMA_HandleTypeDef OLED_dma_hander_rx = {0};
uint32_t OLED_SPI_DMA_OUTTIME = 0;
void DMA1_Channel3_IRQHandler()
{
    HAL_DMA_IRQHandler(&OLED_dma_hander_tx);
}

void DMA1_Channel2_IRQHandler()
{
    HAL_DMA_IRQHandler(&OLED_dma_hander_rx);
}

//SPI DMA传输完成
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI1)
    {
        OLED_CS_Set();
        OLED_DC_Set();
    }
}
#endif
void OLED_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_handler = {0};

    #if (OLED_128_128_MOD == OLED_128_128_HW)
    __HAL_RCC_AFIO_CLK_ENABLE(); //使能复用时钟

    RCC_OLED_SCK_ENABLE();
    RCC_OLED_SDA_ENABLE();
    RCC_OLED_RST_ENABLE();
    RCC_OLED_DC_ENABLE();
    RCC_OLED_CS_ENABLE();
    
    __HAL_AFIO_REMAP_SPI1_DISABLE();
//    GPIO_handler.Alternate = GPIO_AF5_SPI1;
    GPIO_handler.Mode = GPIO_MODE_AF_PP;
    GPIO_handler.Pin = OLED_SCK_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_SCK_GPIO,&GPIO_handler);

    GPIO_handler.Pin = OLED_SDA_PIN;
    HAL_GPIO_Init(OLED_SDA_GPIO,&GPIO_handler);


    __HAL_RCC_SPI1_CLK_ENABLE();
    OLED_HW_handler.Init.Mode = SPI_MODE_MASTER; //主机模式
    OLED_HW_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; //SPI CRC硬件校验 校验码附带数据进行发送
    OLED_HW_handler.Init.BaudRatePrescaler  =SPI_BAUDRATEPRESCALER_2; //波特率分频 2分频 64/2 = 32MHZ
    OLED_HW_handler.Init.CLKPhase  =SPI_PHASE_1EDGE; //第几个时钟沿开始采样
    OLED_HW_handler.Init.CLKPolarity = SPI_POLARITY_LOW; //SCL总线空闲时极性为低
    OLED_HW_handler.Init.CRCPolynomial = 2; //CRC校验计算的多项式 没打开应该没用
    OLED_HW_handler.Init.DataSize = SPI_DATASIZE_8BIT; //数据发送帧格式
    OLED_HW_handler.Init.Direction = SPI_DIRECTION_2LINES; //双线全双工
    OLED_HW_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;//数据高位在前 必须高位在前
    OLED_HW_handler.Init.NSS = SPI_NSS_SOFT; //软件片选脚 硬件片选脚为PA4被占用
    OLED_HW_handler.Init.TIMode = SPI_TIMODE_DISABLE;//禁用时钟极性和相位的设置 即为默认设置

    OLED_HW_handler.Instance = SPI1;
    HAL_SPI_Init(&OLED_HW_handler);

//    GPIO_handler.Alternate = 0;
    GPIO_handler.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_handler.Pin = OLED_RST_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_RST_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = OLED_DC_PIN;
    HAL_GPIO_Init(OLED_DC_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = OLED_CS_PIN;
    HAL_GPIO_Init(OLED_CS_GPIO,&GPIO_handler);
    #endif

    #if (OLED_128_128_MOD == OLED_128_128_HW_DMA)
    __HAL_RCC_AFIO_CLK_ENABLE(); //使能复用时钟

    RCC_OLED_SCK_ENABLE();
    RCC_OLED_SDA_ENABLE();
    RCC_OLED_RST_ENABLE();
    RCC_OLED_DC_ENABLE();
    RCC_OLED_CS_ENABLE();
    
    __HAL_AFIO_REMAP_SPI1_DISABLE();

//    GPIO_handler.Alternate = GPIO_AF5_SPI1;
    GPIO_handler.Mode = GPIO_MODE_AF_PP;
    GPIO_handler.Pin = OLED_SCK_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_SCK_GPIO,&GPIO_handler);

    GPIO_handler.Pin = OLED_SDA_PIN;
    HAL_GPIO_Init(OLED_SDA_GPIO,&GPIO_handler);


    __HAL_RCC_SPI1_CLK_ENABLE();
    OLED_HW_handler.Init.Mode = SPI_MODE_MASTER; //主机模式
    OLED_HW_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; //SPI CRC硬件校验 校验码附带数据进行发送
    OLED_HW_handler.Init.BaudRatePrescaler  =SPI_BAUDRATEPRESCALER_8; //波特率分频 2分频 64/2 = 32MHZ
    OLED_HW_handler.Init.CLKPhase  =SPI_PHASE_1EDGE; //第几个时钟沿开始采样
    OLED_HW_handler.Init.CLKPolarity = SPI_POLARITY_LOW; //SCL总线空闲时极性为低
    OLED_HW_handler.Init.CRCPolynomial = 2; //CRC校验计算的多项式 没打开应该没用
    OLED_HW_handler.Init.DataSize = SPI_DATASIZE_8BIT; //数据发送帧格式
    OLED_HW_handler.Init.Direction = SPI_DIRECTION_2LINES; //双线全双工
    OLED_HW_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;//数据高位在前 必须高位在前
    OLED_HW_handler.Init.NSS = SPI_NSS_SOFT; //软件片选脚 硬件片选脚为PA4被占用
    OLED_HW_handler.Init.TIMode = SPI_TIMODE_DISABLE;//禁用时钟极性和相位的设置 即为默认设置

    OLED_HW_handler.Instance = SPI1;
    HAL_SPI_Init(&OLED_HW_handler);

//    GPIO_handler.Alternate = 0;
    GPIO_handler.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_handler.Pin = OLED_RST_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_RST_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = OLED_DC_PIN;
    HAL_GPIO_Init(OLED_DC_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = OLED_CS_PIN;
    HAL_GPIO_Init(OLED_CS_GPIO,&GPIO_handler);

    __HAL_RCC_DMA1_CLK_ENABLE();

//    HAL_DMA_DeInit(&dma_hander_tx);
    OLED_dma_hander_tx.Instance = DMA1_Channel3;
    OLED_dma_hander_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    OLED_dma_hander_tx.Init.MemDataAlignment = 0;//((uint32_t)DMA_CCR_MSIZE_0); //8位数据传输
    OLED_dma_hander_tx.Init.MemInc = DMA_MINC_ENABLE; //数据不递增
    OLED_dma_hander_tx.Init.Mode = DMA_NORMAL;
    OLED_dma_hander_tx.Init.PeriphDataAlignment = 0;//((uint32_t)DMA_CCR_MSIZE_0); //8位数据传输
    OLED_dma_hander_tx.Init.PeriphInc = DMA_PINC_DISABLE; //寄存器地址不递增
    OLED_dma_hander_tx.Init.Priority = DMA_PRIORITY_MEDIUM;

    __HAL_LINKDMA(&OLED_HW_handler,hdmatx,OLED_dma_hander_tx);
    HAL_DMA_Init(&OLED_dma_hander_tx);
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn,0,0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
    #endif
    
    #if (OLED_128_128_MOD == OLED_128_128_SF)
    __HAL_RCC_AFIO_CLK_ENABLE(); //使能复用时钟
    RCC_OLED_SCK_ENABLE();
    RCC_OLED_SDA_ENABLE();
    RCC_OLED_RST_ENABLE();
    RCC_OLED_DC_ENABLE();
    RCC_OLED_CS_ENABLE();
    
//    GPIO_handler.Alternate = 0;
    GPIO_handler.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_handler.Pin = OLED_SCK_PIN;
    GPIO_handler.Pull = GPIO_PULLUP;
    GPIO_handler.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OLED_SCK_GPIO,&GPIO_handler);

    GPIO_handler.Pin = OLED_SDA_PIN;
    HAL_GPIO_Init(OLED_SDA_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = OLED_RST_PIN;
    HAL_GPIO_Init(OLED_RST_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = OLED_DC_PIN;
    HAL_GPIO_Init(OLED_DC_GPIO,&GPIO_handler);
    
    GPIO_handler.Pin = OLED_CS_PIN;
    HAL_GPIO_Init(OLED_CS_GPIO,&GPIO_handler);
    
    #endif


}

//反显函数
void OLED_ColorTurn(unsigned char i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
		}
}

void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{

    if(cmd)
      OLED_DC_Set();
    else
      OLED_DC_Clr();
    
    
    #if (OLED_128_128_MOD == OLED_128_128_HW)
    OLED_CS_Clr();
    HAL_SPI_Transmit(&OLED_HW_handler,&dat,1,1000);
    OLED_CS_Set();
    OLED_DC_Set();
    #endif
    
    #if (OLED_128_128_MOD == OLED_128_128_HW_DMA)
    OLED_CS_Clr();
    HAL_SPI_Transmit(&OLED_HW_handler,&dat,1,1000);
    OLED_CS_Set();
    OLED_DC_Set();
    #endif
    
    #if (OLED_128_128_MOD == OLED_128_128_SF)
    OLED_CS_Clr();
    for(int i=0;i<8;i++)
    {
        OLED_SCLK_Clr();
        if(dat&0x80)
           OLED_SDIN_Set();
        else 
           OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat<<=1;   
    }

    OLED_CS_Set();
    OLED_DC_Set();
    
    #endif

}

//开启OLED显示
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭屏幕
}

//更新显存到OLED
void OLED_Refresh(void)
{
    #if (OLED_128_128_MOD == OLED_128_128_HW)
    unsigned char i,n;
    for(i=0;i<16;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
        OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
        for(n=0;n<128;n++)
        {
        OLED_WR_Byte(OLED_GRAM[i][n],OLED_DATA);
        }
    }
    #endif
    
    #if (OLED_128_128_MOD == OLED_128_128_HW_DMA)
    unsigned char i;
//    for(int a = 0;a < 16;a++)
//    {
//        memcpy(OLED_GRAM_buf+128*a,OLED_GRAM[a],128);
//    }
//       OLED_DC_Set();
//       OLED_CS_Clr();
//       HAL_SPI_Transmit_DMA(&OLED_HW_handler,OLED_GRAM_buf,128*8);

//        while(1)
//        {
//            if(__HAL_DMA_GET_FLAG(&OLED_dma_hander_tx,DMA_FLAG_TCIF3_7) != 0)
//            {
//                OLED_SPI_DMA_OUTTIME = 0;
//                break;
//            }
//            if(++OLED_SPI_DMA_OUTTIME >= 65535*2)
//            {
//                OLED_SPI_DMA_OUTTIME = 0;
//                break;
//            }
//        }

    for(i=0;i<16;i++)
    {
       OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
       OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
       OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
       OLED_DC_Set();
       OLED_CS_Clr();
       HAL_SPI_Transmit_DMA(&OLED_HW_handler,OLED_GRAM[i],128);
        while(1)
        {
            if(__HAL_DMA_GET_FLAG(&OLED_dma_hander_tx,DMA_FLAG_TC3) != 0)
            {
                OLED_SPI_DMA_OUTTIME = 0;
                break;
            }
            if(++OLED_SPI_DMA_OUTTIME >= 65535/12)
            {
                OLED_SPI_DMA_OUTTIME = 0;
                break;
            }
        }
    }
    #endif
    
    #if (OLED_128_128_MOD == OLED_128_128_SF)
    unsigned char i,n;
    for(i=0;i<16;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
        OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
        for(n=0;n<128;n++)
        {
        OLED_WR_Byte(OLED_GRAM[i][n],OLED_DATA);
        }
    }
    #endif
    
    
//////        HAL_Delay(100);
//////	   for(n=0;n<128;n++)
//////		 OLED_WR_Byte(OLED_GRAM[i][n],OLED_DATA);
//    }
}

//清屏函数
void OLED_Clear(void)
{
	unsigned char i,n;
	for(i=0;i<16;i++)
	{
	   for(n=0;n<128;n++)
        {
         OLED_GRAM[i][n]=0;//清除所有数据
        }
  }
//OLED_Refresh();//更新显示
}

//画点 
//x:横坐标
//y:纵坐标
//t:1 填充 0,清空
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t)
{
	unsigned char i,m,n;
	unsigned char x0=x,y0=y;
	if(USE_HORIZONTAL_OLED_128_128==90)
	{
		x=127-y0;
		y=x0;
	}
	else if(USE_HORIZONTAL_OLED_128_128==270)
	{
		x=y0;
		y=127-x0;
	}
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[i][x]|=n;}
	else
	{
		OLED_GRAM[i][x]=~OLED_GRAM[i][x];
		OLED_GRAM[i][x]|=n;
		OLED_GRAM[i][x]=~OLED_GRAM[i][x];
	}
}

//画线
//x1,y1:起点坐标
//x2,y2:结束坐标
void OLED_DrawLine(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char mode)
{
	unsigned short t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(unsigned char x,unsigned char y,unsigned char r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size1,unsigned char mode)
{
	unsigned char i,m,temp,size2,chr1;
	unsigned char x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //调用0806字体
		else if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
//mode:0,反色显示;1,正常显示
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char size1,unsigned char mode)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
  }
}

//m^n
unsigned int OLED_Pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(unsigned char x,unsigned char y,int num,unsigned char len,unsigned char size1,unsigned char mode)
{
	unsigned char t,temp,m=0;
	if(size1==8)m=2;
    if(num < 0)
    {
        num = -num;
        OLED_ShowChar(x,y,'-',size1,mode);
        x += (size1/2+m);
    }
	for(t=0;t<len;t++)//数据长度
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;//数据求余 OLED_Pow(10,len-t-1)) 10的次方
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
  }
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//dot_before_len :整数长度
//dot_after_len :小数长度
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_Showfloat(unsigned char x,unsigned char y,double num1,unsigned char dot_before_len,unsigned char dot_after_len,unsigned char size1,unsigned char mode)
{
	unsigned char t,temp,len=0;
    if(num1 < 0)
    {
        num1 = -num1;
        OLED_SHOW_BUF[len++] = '-';
    }
    unsigned int integer = num1;
    unsigned int decimal_fraction = (num1-integer)*OLED_Pow(10,dot_after_len);

    for(t=0;t<dot_before_len;t++)//数据长度
	{
		temp=(integer/OLED_Pow(10,dot_before_len-t-1))%10;//数据求余 OLED_Pow(10,len-t-1)) 10的次方
        OLED_SHOW_BUF[len++] = '0'+temp;
    }
    OLED_SHOW_BUF[len++] = '.';
    for(t=0;t<dot_after_len;t++)//数据长度
	{
		temp=(decimal_fraction/OLED_Pow(10,dot_after_len-t-1))%10;//数据求余 OLED_Pow(10,len-t-1)) 10的次方
        OLED_SHOW_BUF[len++] = '0'+temp;
    }
    OLED_SHOW_BUF[len] = '\0';
    OLED_ShowString(x,y,OLED_SHOW_BUF,size1,1);
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//mode:0,反色显示;1,正常显示
void OLED_ShowChinese(unsigned char x,unsigned char y,unsigned char num,unsigned char size1,unsigned char mode)
{
	unsigned char m,temp;
	unsigned char x0=x,y0=y;
	unsigned short i,size3=(size1/8+((size1%8)?1:0))*size1;  //得到字体一个字符对应点阵集所占的字节数
	//size3 数组大小
    for(i=0;i<size3;i++)
	{
        if(size1==8)
				{temp=Font_GB8[num][i];}//调用16*16字体
		else if(size1==16)
				{temp=Font_GB16[num][i];}//调用16*16字体
		else if(size1==24)
				{temp=Font_GB24[num][i];}//调用24*24字体
		else if(size1==32)
				{temp=Font_GB32[num][i];}//调用32*32字体
		else if(size1==64)
				{temp=Font_GB64[num][i];}//调用64*64字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size1)
		{x=x0;y0=y0+8;}
		y=y0;
	}
}

void OLED_ShowChinese8x8(unsigned char x,unsigned char y,unsigned char* s,unsigned char size1,unsigned char mode)
{
    unsigned short  HZnum;//汉字数目
    HZnum=sizeof(Chinese_GB8)/sizeof(typFNT_GB_oled_128_128);	//统计汉字数目
    for(unsigned short a = 0;a < HZnum;a++)
    {
        if((Chinese_GB8[a].Index[0] == *s)&&(Chinese_GB8[a].Index[1] == *(s+1)))
        {
            OLED_ShowChinese(x,y,a,size1,mode);
            break;
        }
        continue;
    }
}

void OLED_ShowChinese16x16(unsigned char x,unsigned char y,unsigned char* s,unsigned char size1,unsigned char mode)
{
    unsigned short  HZnum;//汉字数目
    HZnum=sizeof(Chinese_GB16)/sizeof(typFNT_GB_oled_128_128);	//统计汉字数目
    for(unsigned short a = 0;a < HZnum;a++)
    {
        if((Chinese_GB16[a].Index[0] == *s)&&(Chinese_GB16[a].Index[1] == *(s+1)))
        {
            OLED_ShowChinese(x,y,a,size1,mode);
            break;
        }
        continue;
    }
}

void OLED_ShowChinese24x24(unsigned char x,unsigned char y,unsigned char* s,unsigned char size1,unsigned char mode)
{
    unsigned short  HZnum;//汉字数目
    HZnum=sizeof(Chinese_GB24)/sizeof(typFNT_GB_oled_128_128);	//统计汉字数目
    for(unsigned short a = 0;a < HZnum;a++)
    {
        if((Chinese_GB24[a].Index[0] == *s)&&(Chinese_GB24[a].Index[1] == *(s+1)))
        {
            OLED_ShowChinese(x,y,a,size1,mode);
            break;
        }
        continue;
    }
}

void OLED_ShowChinese32x32(unsigned char x,unsigned char y,unsigned char* s,unsigned char size1,unsigned char mode)
{
    unsigned short  HZnum;//汉字数目
    HZnum=sizeof(Chinese_GB32)/sizeof(typFNT_GB_oled_128_128);	//统计汉字数目
    for(unsigned short a = 0;a < HZnum;a++)
    {
        if((Chinese_GB32[a].Index[0] == *s)&&(Chinese_GB32[a].Index[1] == *(s+1)))
        {
            OLED_ShowChinese(x,y,a,size1,mode);
            break;
        }
        continue;
    }
}

void OLED_ShowChinese1(unsigned char x,unsigned char y,unsigned char* s,unsigned char size1,unsigned char mode)
{
    while(*s != 0)
    {
        switch(size1)
        {
            case 8 : OLED_ShowChinese8x8(x,y,s,size1,mode);break;
            
            case 16 : OLED_ShowChinese16x16(x,y,s,size1,mode);break;
                    
            case 24 : OLED_ShowChinese24x24(x,y,s,size1,mode);break;
            
            case 32 : OLED_ShowChinese32x32(x,y,s,size1,mode);break;

        }
        s+=2; x+=size1;
    }
}

//num 显示汉字的个数
//space 每一遍显示的间隔
//mode:0,反色显示;1,正常显示
void OLED_ScrollDisplay(unsigned char num,unsigned char space,unsigned char mode)
{
	unsigned char i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    //OLED_ShowChinese(128,56,t,16,mode); //写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //显示间隔
				 {
					for(i=1;i<144;i++)
						{
							for(n=0;n<16;n++)
							{
								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
							}
						}
           OLED_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=1;i<144;i++)   //实现左移
		{
			for(n=0;n<16;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//x,y：起点坐标
//sizex,sizey,图片长宽
//BMP[]：要写入的图片数组
//mode:0,反色显示;1,正常显示
void OLED_ShowPicture(unsigned char x,unsigned char y,unsigned char sizex,unsigned char sizey,unsigned char BMP[],unsigned char mode)
{
	unsigned short j=0;
	unsigned char i,n,temp,m;
	unsigned char x0=x,y0=y;
	sizey=sizey/8+((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
		 for(i=0;i<sizex;i++)
		 {
				temp=BMP[j];
				j++;
				for(m=0;m<8;m++)
				{
					if(temp&0x01)OLED_DrawPoint(x,y,mode);
					else OLED_DrawPoint(x,y,!mode);
					temp>>=1;
					y++;
				}
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
     }
	 }
}



//OLED的初始化
void OLED_Init(void)
{
    OLED_GPIO_Init();

	OLED_RES_Clr();
	OLED_128_128_DELAY(20);
	OLED_RES_Set();
	OLED_128_128_DELAY(20);//RES加延时 否则部分高速单片机可能反转失效

//    OLED_WR_Byte(0x20,OLED_CMD); // Set Page Addressing Mode
//    OLED_WR_Byte(0x02,OLED_CMD); // Set Page Addressing Mode
    
	OLED_WR_Byte(0xAE,OLED_CMD);// 关闭显示屏
	OLED_WR_Byte(0xa8,OLED_CMD); // 设置驱动扫描的行数
	OLED_WR_Byte(0x7f,OLED_CMD);//扫描的行数为128行
	OLED_WR_Byte(0xd5,OLED_CMD); // Set Frame Frequency
	OLED_WR_Byte(0x50,OLED_CMD); // 104Hz
    if(USE_HORIZONTAL_OLED_128_128==180)
	{
		OLED_WR_Byte(0xC8,OLED_CMD);//逆向上下扫描
//        OLED_128_128_DELAY(20);
		OLED_WR_Byte(0xA1,OLED_CMD);//逆向左右扫描
//        OLED_128_128_DELAY(20);
	}
	else
	{
		OLED_WR_Byte(0xC0,OLED_CMD);
		OLED_WR_Byte(0xA0,OLED_CMD);
	}
	OLED_WR_Byte(0xd3,OLED_CMD); // Set Display Offset
	OLED_WR_Byte(0x00,OLED_CMD);//偏移量为0
	OLED_WR_Byte(0xdc,OLED_CMD); // Set Display Start Line
	OLED_WR_Byte(0x00,OLED_CMD);//设置起始行为第0行
    
    
//   OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0x20,OLED_CMD); // Set Page Addressing Mode

	OLED_WR_Byte(0x81,OLED_CMD); // 设置对比度 0x00-0xff 暗-亮
	OLED_WR_Byte(0xdf,OLED_CMD);
	OLED_WR_Byte(0xa4,OLED_CMD); // Set Entire Display OFF/ON
	OLED_WR_Byte(0xa6,OLED_CMD); // 设置为正向显示模式 A7反向
	OLED_WR_Byte(0xad,OLED_CMD); // 设置电荷泵
	OLED_WR_Byte(0x8a,OLED_CMD);
	OLED_WR_Byte(0xd9,OLED_CMD); // Set Phase Leghth
	OLED_WR_Byte(0x22,OLED_CMD);
	OLED_WR_Byte(0xdb,OLED_CMD); // Set Vcomh voltage
	OLED_WR_Byte(0x35,OLED_CMD);
	OLED_Clear();
	OLED_WR_Byte(0xAF,OLED_CMD);//Display ON
	OLED_Refresh();
}
#endif
