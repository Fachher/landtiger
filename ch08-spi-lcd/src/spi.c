#include "spi.h"
#include "gpio.h"
#include "lpc17xx.h"

uint8_t dummy_u8;


void spi_write(unsigned char data) {
    LPC_SPI->SPDR = data;
    //while (!(LPC_SPI->SPSR & (1 << 7)));
	//dummy_u8 = LPC_SPI->SPSR;
    //dummy_u8 = LPC_SPI->SPDR;

    //LPC_SPI->SPDR = data;
    //while (!(LPC_SPI->SPSR & (1 << 7)));
	//
	    // Warte auf Leerlauf (TXRDY-Flag)
    //while (!(LPC_SPI->SPSR & (1 << 7)));

    // Schreibe das Byte in das SPDR-Register

    //while (!(LPC_SPI->SPSR & (1 << 7)));
    //LPC_SPI->SPDR = data;

    // Warte auf das Ende der Übertragung (SPIF-Flag)
    //while (!(LPC_SPI->SPSR & (1 << 0)));
}

