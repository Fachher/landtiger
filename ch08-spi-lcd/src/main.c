#include "lpc17xx.h"
#include "helper.h"
#include "ST7735.h"

// Definitionen für SPI-Pins
#define SCK_PIN     15 // P0.15
#define MISO_PIN    17 // P0.17
#define MOSI_PIN    18 // P0.18

// SPI-Geschwindigkeitseinstellungen
#define SPI_CLOCK_DIVIDER 8 // PCLK / SPI_CLOCK_DIVIDER

void pinMode(){
    df
}

void SPI_Init(void) {
    // Einschalten des SPI-Moduls
    LPC_SC->PCONP |= (1 << 8); // PCONP Bit 8 aktiviert das SPI0-Modul

    // SPI0-Pin-Konfiguration

    LPC_PINCON->PINSEL0 |= (3 << (2 * SCK_PIN));

    | (3 << (2 * MISO_PIN)) | (3 << (2 * MOSI_PIN)));

    LPC_PINCON->PINSEL0 &= ~((3 << (2 * SCK_PIN)) | (3 << (2 * MISO_PIN)) | (3 << (2 * MOSI_PIN)));
    LPC_PINCON->PINSEL0 |= ((3 << (2 * SCK_PIN)) | (3 << (2 * MISO_PIN)) | (3 << (2 * MOSI_PIN)));

    // SPI-Konfiguration
    LPC_SPI->SPCR = (1 << 2) | (1 << 5); // Master-Modus, MSB-first

    // Einstellung der Übertragungsgeschwindigkeit
    uint32_t spiClockDivider = SPI_CLOCK_DIVIDER;
    if (spiClockDivider < 8) {
        spiClockDivider = 8; // Minimum Wert
    } else if (spiClockDivider > 255) {
        spiClockDivider = 255; // Maximum Wert
    }
    LPC_SPI->SPCCR = spiClockDivider;

    // Aktivierung des SPI-Modus
    LPC_SPI->SPCR |= (1 << 7); // SPI-Modul aktivieren
}

#define PIN_NUMBER 23 // P0.23

int main (void)
{
	SystemCoreClockUpdate();

	// configure GPIOs
    LPC_SC->PCONP |= (1 << 15); // PCONP Bit 15 aktiviert GPIO

    // Pin als Ausgang konfigurieren
    LPC_GPIO0->FIODIR |= (1 << PIN_NUMBER); // Setze PIN_NUMBER als Ausgang

    // Pin auf HIGH setzen
    LPC_GPIO0->FIOSET |= (1 << PIN_NUMBER); // Setze PIN_NUMBER auf HIGH

    // Pin auf LOW setzen
    LPC_GPIO0->FIOCLR |= (1 << PIN_NUMBER); // Setze PIN_NUMBER auf LOW



    SPI_Init();

    lcd_init();

    fill_rectangle((int)0, (int)0, (int)128, (int)128, BG_COLOUR);

	uart_init(9600);

	delay(2000);

    while(1);

    return;

}
