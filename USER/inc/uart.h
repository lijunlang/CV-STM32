
#ifndef __UART_H__
#define __UART_H__

#include "stm32f4xx.h"


void init_uart1(void);
u8 rev_byte(void);
void send_byte(u8 val);



#endif 
