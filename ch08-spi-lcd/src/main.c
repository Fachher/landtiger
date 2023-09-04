#include "lpc17xx.h"
#include "helper.h"
#include "ST7735.h"
#include "gpio.h"
#include "spi.h"


void GPIO_Init(void) {

	// reset all pins to 0
    LPC_PINCON->PINMODE4 &= ~(0xFFFF); 

	// deactivate pullup/pulldown resistors (0b10 deactivates resistor per pin)
	LPC_PINCON->PINMODE4 |= 0xAAAA;

	// configure P2.0 -> P2.7 as output
	LPC_GPIO2->FIODIR = 0x000000ff;
}

void SPI_Init(void) {

	// set SCK, SSEL and MOSI as output
	LPC_GPIO0->FIODIR = (0x1 << 15 | 0x1 << 16 | 0x1 << 18);

	// Pull-up-Widerstand für SCK aktivieren (falls erforderlich)
	//LPC_PINCON->PINMODE0 &= ~(0x3 << 30); // Clear bits 30 and 31
	//LPC_PINCON->PINMODE0 |= (0x2 << 30);  // Enable pull-up (10)

	// Pull-up-Widerstand für MOSI aktivieren
	//LPC_PINCON->PINMODE1 &= ~(0x3 << 2); // Clear bits 2 and 3
	//LPC_PINCON->PINMODE1 |= (0x2 << 2);  // Enable pull-up (10)

	// Pull-up-Widerstand für MISO aktivieren
	//LPC_PINCON->PINMODE1 &= ~(0x3 << 4); // Clear bits 4 and 5
	//LPC_PINCON->PINMODE1 |= (0x2 << 4);  // Enable pull-up (10)


  	LPC_PINCON->PINSEL0 |= (0x3 << 30); // SCK: P0.15
    LPC_PINCON->PINSEL1 |= (0x3 << 0);  // SSEL: P0.16
    LPC_PINCON->PINSEL1 |= (0x3 << 2);  // MISO: P0.17
    LPC_PINCON->PINSEL1 |= (0x3 << 4);  // MOSI: P0.18

	digitalWritePort0(16, 1);
    
	LPC_SC->PCONP |= (1u << 8u);

	// 1.set clock counter register to the desired clock rate.
    LPC_SPI->SPCCR = 8;

	// 2. set control register to the desired settings.
    //LPC_SPI->SPCR = (1u << 5u);
    //LPC_SPI->SPCR |= (1u << 4u);
	LPC_SPI->SPCR = ((0<<3) | (1<<4) | (1<<5));

}

int main(void) {

    GPIO_Init();
    SPI_Init();
    //lcd_init();

	//delay(1000);
    //draw_pixel(20, 20, 0xCC);
	
	unsigned char x = 0;


    while (1) {
		delay(2000);
		x = (x == 0 ? 1 : 0);
		digitalWritePort2(0, x);
		delay(2000);
		digitalWritePort2(1, x);
		delay(2000);
		spi_write(0x9a);
		delay(2000);
    }

}
