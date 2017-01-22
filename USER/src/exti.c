#include "exti.h"
#include "stm32f4xx.h"
#include "delay.h"


void init_exti0()//PA0 <---> EXTI0
{
	RCC->AHB1ENR |= 1<<0;//使能 PA口 时钟
	RCC->APB2ENR |= 1<<14;//使能 SYSCFG 时钟。
	
	SYSCFG->EXTICR[0] &= ~0XF;
	SYSCFG->EXTICR[0] |= 0X00;//0000  配置 PA0 --- EXTI0
	
	EXTI->RTSR |= 1<<0; //EXTI0 允许下降沿触发。
	EXTI->IMR |= 1<<0;  //开发 EXTI0 中断请求
	
	NVIC_EnableIRQ(EXTI0_IRQn);// 使能 NVIC 对应的中断通道。
}


void EXTI0_IRQHandler(void)
{
	EXTI->PR = 1<<0; //清除LINE0上的中断标志位 ,EXTI 中断挂起位，写1，清零。
  delay_ms(10);//去抖动
	if((GPIOA->IDR & 1<<0) && 1)
	{
	  GPIOA->ODR	^= 1 << 7; //每按一次按键，就取反LED灯的状态
	}
	
}
