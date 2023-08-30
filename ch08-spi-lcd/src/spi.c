#include "spi.h"
#include "gpio.h"
#include "lpc17xx.h"

void spi_write(unsigned char data) {
    while (!(LPC_SPI->SPSR & (1 << 7)));
    LPC_SPI->SPDR = data;
    while (!(LPC_SPI->SPSR & (1 << 7)));
}

