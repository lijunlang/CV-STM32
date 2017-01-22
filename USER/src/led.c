#include "stm32f4xx.h"
#include "led.h"
void led_init(void)
{
	
	RCC->AHB1ENR |= 1<<0;			    //PA端口时钟使能。 第六章
	
	GPIOA->MODER &= ~(3 << 14); 	//32bit,清零
	GPIOA->MODER |= 1 << 14;    	//配置PA7 IO口，为通用输出功能
	
	GPIOA->OTYPER &= ~(1 << 7);	  //配置PA7为推挽输出
	
	GPIOA->OSPEEDR &= ~(3 << 14);
	GPIOA->OSPEEDR |= 2 << 14;  	//配置PA7的输出速度 50MHZ
	
	
	GPIOA->ODR |= 1<<7;			      //PA7输出高电平	
}
