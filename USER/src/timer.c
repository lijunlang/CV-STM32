#include "stm32f4xx.h"
#include "timer.h"
void TIM3_IRQHandler(void)
{	

	if(TIM3->SR&0X0001)//溢出中断
	{
           GPIOA->ODR	^= 1 << 7; //每按一次按键，就取反LED灯的状态

	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位		
}
//通用定时器3中断初始化,定时器除2/3/4/5定时器为50M外，其余为100M
void TIM3_init(u16 arr,u16 psc)
{
	u32 pro;
  RCC->APB1ENR|=1<<1; //TIM3时钟使能	  
  TIM3->ARR=arr;		//设定计数器自动重装值 
  TIM3->PSC=psc;		//预分频器	 
  TIM3->CNT = 0;        //  重新初始化就会重新计数
  TIM3->DIER|=1<<0;	//允许更新中断	  
  TIM3->CR1|=0x01;	//使能定时器3
	//抢占优先级2bit  子优先级2bit
	NVIC_SetPriorityGrouping(5);//设置中断优先级分组
	pro = NVIC_EncodePriority(5, 2, 0);//抢占优先级的级别：2级 、子优先级级别：0级
	NVIC_SetPriority(TIM3_IRQn, pro);//设置优先级

	
	NVIC_EnableIRQ(TIM3_IRQn);//NVIC中断通道使能。							 
}
void TIM2_initPWM(u32 arr,u32 psc)
{	
  RCC->APB1ENR |= 1<<0;   //TIM2时钟使能  
  RCC->AHB1ENR|=1<<1;   	//使能PORTB时钟	
	
	GPIOB->MODER &= ~(3<<20);
	GPIOB->MODER |= (2<<20);	//PB10  复用功能
	
	
	GPIOB->AFR[1] &= ~(0XF<<8);
	GPIOB->AFR[1] |= (0X1<<8);// AF1 功能
  
  TIM2->ARR=arr;			//设定计数器自动重装值 
  TIM2->PSC=psc;			//预分频器不分频 
  
	TIM2->CCR3  = 250;  //PWM的值电平变化值  
	
	TIM2->CCMR2 &= ~(3<<0);// CH3  输出模式  
	
	TIM2->CCMR2 &= ~(7<<4);  // CH3  PWM1 模式  
	TIM2->CCMR2 |= 6<<4;  // CH3  PWM1 模式     
	TIM2->CCMR2|=1<<3; 	//CH3 预装载使能
	TIM2->CCER  |= 1<<8;      // CH3  通道使能
	TIM2->CCER  &=~(1<<9);   	//CH3  高电平有效
	TIM2->CR1 |= 1;		//计数器使能
  
} 
void TIM2_enablePWM(void)
{
  TIM2->CR1  |=1<<0;    	//使能定时器2	
}
void TIM2_disablePWM(void)
{
  TIM2->CR1  |=0<<0;    	//失能定时器2	
}