#ifndef __A6_DRIVER_H__
#define __A6_DRIVER_H__
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"


void debug_uart_init(void);
void systick_delay_ms(uint32_t delay);
void a6_uart_write_char(char ch);
void a6_uart_init(void);


#endif
