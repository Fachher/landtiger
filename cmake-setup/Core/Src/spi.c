#include "spi.h"
#include "lpc17xx.h"

void spi_write(unsigned char data) {
    LPC_SPI->SPDR = data;
    while (!(LPC_SPI->SPSR & (1 << 7)));
}

