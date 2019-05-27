#ifndef __CTB_DRIVER_UART_H__
#define __CTB_DRIVER_UART_H__

#include "stm32f7xx_hal.h"

void MX_USART1_UART_Init(void);
void uart_print(const char* msg, unsigned size);

#endif
