#include "lpc17xx.h"
#include "helper.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

// NOTE: each port pin takes two bits to configure
void GPIO_Init(void) {
    // GPIO (P0.0) will be used as CS = chip select
    LPC_PINCON->PINSEL0 |= ~0b11u << (2u * 0);
    LPC_GPIO0->FIODIR |= 0b001u;


    // GPIO (P0.1) will be used as command pin
    LPC_PINCON->PINSEL0 |= ~0b11u << (2u * 1);
    LPC_GPIO0->FIODIR |= 0b010u;

    // GPIO (P0.2) will be used as reset pin
    LPC_PINCON->PINSEL0 |= ~0b11u << (2u * 2);
    LPC_GPIO0->FIODIR |= 0b100u;
}

void SPI_Init(void) {
    // activate SPI (page 55) it is actually not required as the default is ON
    LPC_SC->PCONP |= (1u << 8u);

    // SPI0-Pin-Configuration
    // __ __ __ __ __ __ 10 10 10 __ __ __ __ __ __ __
    // 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00

    // SCK1 (P0.7)
    LPC_PINCON->PINSEL0 |= 0b10u << (2u*7);

    // MISO1 (P0.8)
    LPC_PINCON->PINSEL0 |= 0b10u << (2u*8);

    // MOSI1 (P0.9)
    LPC_PINCON->PINSEL0 |= 0b10u << (2u*9);

    // configure spi a master mode
    LPC_SPI->SPCR = (1u << 5u);

    // spi clock divider
    LPC_SPI->SPCCR = 8;

    // generate hardware interrupt each time SPIF or MODF bits are activated
    LPC_SPI->SPCR |= (1u << 7u);
}

int main(void) {

    LPC_SC->PCONP |= (1u << 15);
    LPC_GPIO2->FIODIR = 0x000000ff;               /* LEDs PORT2 are Output */
    LPC_GPIO0->FIODIR = 0x00200000;
    LPC_GPIO0->FIOPIN |= 0x00200000u;

    while (1) {
        LPC_GPIO2->FIOPIN |= (0x1u);
        delay(500);
        LPC_GPIO2->FIOPIN &= ~(0x1u);
        delay(500);
    }

}

#pragma clang diagnostic pop

