#include "lpc17xx.h"
#include "helper.h"
#include "ST7735.h"
#include "gpio.h"
#include "spi.h"


#define PIN0 0
#define PIN1 1
#define PIN2 2

void GPIO_Init(void) {

    // reset all pins to 0
    LPC_PINCON->PINMODE4 &= ~(0xFFFF);

    // deactivate pullup/pulldown resistors (0b10 deactivates resistor per pin)
    LPC_PINCON->PINMODE4 |= 0xffff;

    // configure P2.0 -> P2.7 as output
    LPC_GPIO2->FIODIR = 0x000000ff;
}

void SPI_Init(void) {
    // set SCK, SSEL and MOSI as output
    LPC_GPIO0->FIODIR = (0x1 << 15 | 0x1 << 16 | 0x1 << 18);

    LPC_PINCON->PINSEL0 |= (0x3 << 30); // SCK: P0.15
    LPC_PINCON->PINSEL1 |= (0x3 << 0);  // SSEL: P0.16
    LPC_PINCON->PINSEL1 |= (0x3 << 2);  // MISO: P0.17
    LPC_PINCON->PINSEL1 |= (0x3 << 4);  // MOSI: P0.18

    // activate SPI
    LPC_SC->PCONP |= (1u << 8u);

    // 1.set clock rate in the clock counter register
    LPC_SPI->SPCCR = 8;

    // 2. set control register
    LPC_SPI->SPCR = 0<<3; // CPHA (Data is sampled on the first clock edge of SCK. A transfer starts and ends with activation and deactivation of the SSEL signal.)
    LPC_SPI->SPCR |= 0<<4; // CPOL (SCK is active low)
    LPC_SPI->SPCR |= 1<<5; // Master mode
}

int main(void) {
    GPIO_Init();
    SPI_Init();
    //lcd_init();

    //draw_pixel(20, 20, 0xCC);
    while (1) {
        digitalWritePort2(PIN0, 1);
        spi_write(0x9a);
        digitalWritePort2(PIN0, 0);
        delay(2000);
    }

}
