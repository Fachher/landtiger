#include "lpc17xx.h"
#include "helper.h"
#include "ST7735.h"
#include "gpio.h"
#include "spi.h"

#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7

#define SCK 15
#define MISO 17
#define MOSI 18

// P2.0-P2.7 are directly connected to a 74LV244 driver (U11) followed by 8 red LEDs
void GPIO_Init(void) {

	// reset all pins to 0
    LPC_PINCON->PINMODE4 &= ~(0xFFFF); 

	// deactivate pullup/pulldown resistors (0b10 deactivates resistor per pin)
	LPC_PINCON->PINMODE4 |= 0xAAAA;

	// configure P2.0 -> P2.7 as output
	LPC_GPIO2->FIODIR = 0x000000ff;
}

void SPI_Init(void) {

    LPC_SC->PCONP |= (1u << 8u);

  	LPC_PINCON->PINSEL0 |= (0b10 << 30); // SCK: P0.15
    LPC_PINCON->PINSEL1 |= (0b10 << 2);  // MISO: P0.17
    LPC_PINCON->PINSEL1 |= (0b10 << 4);  // MOSI: P0.18

	// 1.set clock counter register to the desired clock rate.
    LPC_SPI->SPCCR = 8;

	// 2. set control register to the desired settings.
    LPC_SPI->SPCR = (1u << 5u);
    LPC_SPI->SPCR |= (1u << 7u);

}

int main(void) {

    GPIO_Init();
    //SPI_Init();
    //lcd_init();

	//delay(1000);
    //draw_pixel(20, 20, 0xCC);
	
	unsigned char x = 0;

    while (1) {
		//spi_write(0x9a);
		//digitalWritePort0(1, x);
		//digitalWritePort0(10, x);
		//
		delay(500);
		//x = (x == 0 ? 1 : 0);
		//digitalWritePort2(0, 0);
		//digitalWritePort2(1, 0);
		//digitalWritePort2(2, 0);
		//digitalWritePort2(3, 0);
		//digitalWritePort2(4, 0);
		//digitalWritePort2(5, 0);
		//digitalWritePort2(6, 0);
		//digitalWritePort2(7, 0);
		//delay(500);
		//digitalWritePort2(1, x);
		//delay(500);
		//digitalWritePort2(2, x);
		//delay(500);
	//	digitalWritePort0(10, x);
	//	x = (x == 0 ? 1 : 0);
    //    delay(1000);
    }

}
