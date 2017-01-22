#ifndef __KEY_H__
#define __KEY_H__

//头文件： 函数声明、变量声明、宏定义、数据类型定义。

#include "stm32f4xx.h"

#define KEY ((GPIOA->IDR & 1<<0) && 1)  //PA0  高有效


void delay(u32 t);
extern void key_init(void);
u8 key_scan(void);


#endif
