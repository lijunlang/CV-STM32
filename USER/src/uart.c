
#include "UART.h"

u8 uart_rx_buf[20];//串口接收缓存区。
u8 uart_rx_len = 0; //指示，当前接收缓存区，收到了几个数据。   

void init_uart1()
{
	float DIV_Fraction;
	u16 DIV_Mantissa;
	u32 pro;

	RCC->AHB1ENR |= 1; //PA时钟使能
	
	//PA9   PA10
	GPIOA->MODER &= ~(3<<18);
	GPIOA->MODER |= (2<<18); //PA9 复用功能  TX
	
	GPIOA->AFR[1] &= ~(0XF << 4);   	//参见 图18  寄存器描述
	GPIOA->AFR[1] |= 7 << 4;  		//PA9  uart1功能    AF7
	
	GPIOA->MODER &= ~(3<<20);
	GPIOA->MODER |= (2<<20); //PA10 复用功能 RX 
	
	GPIOA->AFR[1] &= ~(0XF << 8);
	GPIOA->AFR[1] |= 7 << 8;  //PA10  uart1功能   AF7
	
	
	RCC->APB2ENR |= 1<<4; //使能usart1 时钟
	
	DIV_Fraction = (float)100000000/9600/16; //计算波特率  
	DIV_Mantissa = (u16)DIV_Fraction;
	DIV_Fraction = DIV_Fraction - DIV_Mantissa;
	DIV_Mantissa = (DIV_Mantissa << 4) + DIV_Fraction * 16;
	
	USART1->BRR = DIV_Mantissa;//配置波特率寄存器
	
	USART1->CR1 |= 1<<2;//使能接收
	USART1->CR1 |= 1<<3;//使能发送
	USART1->CR1 |= 1<<13;//使能串口
	
	USART1->CR1 |= 1<<5;//串口1 接收中断使能。  外设
	
	//抢占优先级2bit  子优先级2bit
	NVIC_SetPriorityGrouping(5);//设置中断优先级分组
	pro = NVIC_EncodePriority(5, 3, 0);//抢占优先级的级别：3级 、子优先级级别：0级
	NVIC_SetPriority(USART1_IRQn, pro);//设置优先级
//	NVIC->IP[37] = 0xc0;
	
	NVIC_EnableIRQ(USART1_IRQn);//NVIC中断通道使能。
}


//查接收一个字节函数
u8 rev_byte()
{
	while ( (USART1->SR & 1<<5) == 0 )
	{
		;//等待 接收缓冲区非空。
	}
	
	return USART1->DR; //接收数据。
}


//发送一个字节函数
void send_byte(u8 val)
{
	while ( (USART1->SR & 1<<6) == 0 )
	{
		;//等待发送完成
	}
	
	USART1->DR = val; //发送数据
}


void Uart1_sendData(u8 *dataBuf,u8 lenth)
{
  while (lenth--)
  {
    send_byte(*dataBuf);
    dataBuf++;
  }
}
void USART1_IRQHandler(void)
{
  static u8 cnt = 0;

  //有多种情况会进中断函数，所以要根据状态寄存器来判读
  if (USART1->SR & 1<<5)//接收缓冲区非空
  {
    uart_rx_buf[cnt] = USART1->DR;//读取串口收到的数据
    cnt++;
    if(5 == cnt)  //接收到5个字节的数据，就进行数据协议解释
    {
      uart_rx_len = cnt;
      Uart1_sendData(uart_rx_buf,5);  //接收到5个字节就回发
      cnt = 0;
    }
  }

}


