#include "mbed.h"

// Pin definitions
DigitalOut cs(p8);
DigitalOut reset(p9);
DigitalOut dc(p10);
DigitalOut backlight(p11);
SPI spi(p5, NC, p7); // MOSI, MISO, SCLK

// ST7735 commands
#define ST7735_SWRESET 0x01
#define ST7735_SLPOUT 0x11
#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C

// Initialize the ST7735 display
void initST7735() {
    cs = 1;
    reset = 0;
    wait_us(500);
    reset = 1;
    wait_ms(5);
    
    cs = 0;
    dc = 0;
    spi.write(ST7735_SWRESET);
    cs = 1;
    wait_ms(150);
    
    cs = 0;
    dc = 0;
    spi.write(ST7735_SLPOUT);
    cs = 1;
    wait_ms(500);
    
    cs = 0;
    dc = 0;
    spi.write(ST7735_COLMOD);
    dc = 1;
    spi.write(0x05);
    cs = 1;
    
    cs = 0;
    dc = 0;
    spi.write(ST7735_MADCTL);
    dc = 1;
    spi.write(0xC8);
    cs = 1;
}

// Draw a rectangle on the ST7735 display
void drawRect(int x, int y, int w, int h, uint16_t color) {
    cs = 0;
    dc = 0;
    spi.write(ST7735_CASET);
    dc = 1;
    spi.write(x >> 8);
    spi.write(x & 0xFF);
    spi.write((x + w - 1) >> 8);
    spi.write((x + w - 1) & 0xFF);
    cs = 1;

    cs = 0;
    dc = 0;
    spi.write(ST7735_RASET);
    dc = 1;
    spi.write(y >> 8);
    spi.write(y & 0xFF);
    spi.write((y + h - 1) >> 8);
    spi.write((y + h - 1) & 0xFF);
    cs = 1;

    cs = 0;
    dc = 0;
    spi.write(ST7735_RAMWR);
    dc = 1;
    for (int i = 0; i < w * h; i++) {
        spi.write(color >> 8);
        spi.write(color & 0xFF);
    }
    cs = 1;
}

int main() {
    // Initialize the SPI and pins
    spi.format(8, 3); // 8-bit data, mode 3
    spi.frequency(20000000); // 20 MHz SPI clock
    cs = 1;
    reset = 1;
    dc = 1;
    backlight = 1;
    
    // Initialize the ST7735 display
    initST7735();
    
    // Draw a red rectangle at (10, 10) with size 50x30
    drawRect(
