#include "lpc_types.h"

#ifndef LPC_HELPER_H
#define LPC_HELPER_H


void delay(uint32_t milliseconds);

void uart_init(int baudrate);

void log(const char *str);

#endif  // __LPC_HELPER_H__
