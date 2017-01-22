
	
#ifndef _DELAY_H
#define _DELAY_H
#include "stm32f4xx.h"
extern void delay_init(u8 SYSCLK);
extern void delay_us(u32 nus);
extern void delay_xms(u16 nms);
extern void delay_ms(u16 nms);
#endif

