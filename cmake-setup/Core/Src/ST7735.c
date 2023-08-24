//#include <math.h>
#include "ST7735.h"
#include "helper.h"
#include "lpc17xx.h"

#define CS 0b10u

/*
 * Writes a byte to SPI without changing chip select (CSX) state.
 * Called by the write_command() and write_data() functions which
 * control these pins as required.
 *
 * This will use either a software implementation or the hardware
 * implementation depending on if USE_HW_SPI flag is set in the
 * header file. (Software SPI is reeeeeally slow. Measured at
 * 40 kHz clock compared to 2 MHz clock with hardware SPI).
 */

// TFT_CS, HIGH
// void digitalWrite(uint8_t pin, uint8_t val);
void digital_write(){

}

void spi_write(unsigned char data) {
    while (!(LPC_SPI->SPSR & (1 << 7)));
    LPC_SPI->SPDR = data;
    while (!(LPC_SPI->SPSR & (1 << 0)));
}

/*
 * Writes a data byte to the display. Pulls CS low as required.
 */
void lcd_write_data(unsigned char data) {
    //CS LOW
    LPC_GPIO0->FIOCLR = 0b10u;

    CSX = 0;
    //Send data to the SPI register
    spi_write(data);
    //CS HIGH
    CSX = 1;
    LPC_GPIO0->FIOSET = 0b10u; // TODO: Set correct bit
}

/*
 * Writes a command byte to the display
 */
void lcd_write_command(unsigned char data) {
    //Pull the command AND chip select lines LOW
    CMD = 0;
    CSX = 0;
    spi_write(data);
    //Return the control lines to HIGH
    CMD = 1;
    CSX = 1;
}

void lcd_init() {

    //SET control pins for the LCD HIGH (they are active LOW)
    CSX = 1; //CS
    CMD = 1; //Data / command select, the datasheet isn't clear on that.
    RESX = 1; //RESET pin HIGH

    //Cycle reset pin
    RESX = 0;
    delay(500);
    RESX = 1;
    delay(500);

    lcd_init_command_list();
}

/**
 * After a bit of trial and error with the libraries made by Adafruit and others
 * I have settled on this. You may want to add your own settings to the
 * command list here.
 */
void lcd_init_command_list(void)
{

    lcd_write_command(ST7735_SWRESET);
    delay(100);
    lcd_write_command(0x11);//Sleep out
    delay(120);
    //------//

    //Add any custom settings to the command list here

    //------//
    lcd_write_command(0x3A);
    lcd_write_data(0x05);

    lcd_write_command(0x29);//Display on
}

/*
 * Draws a single pixel to the LCD at position X, Y, with
 * Colour.
 */
void draw_pixel(char x, char y, unsigned int colour) {
    //Set the x, y position that we want to write to
    set_draw_window(x, y, x+1, y+1);
    lcd_write_data(colour >> 8);
    lcd_write_data(colour & 0xFF);
}

/*
 * Fills a rectangle with a given colour
 */
void fill_rectangle(char x1, char y1, char x2, char y2, unsigned int colour) {
    //Split the colour int in to two bytes
    unsigned char colour_high = colour >> 8;
    unsigned char colour_low = colour & 0xFF;

    //Set the drawing region
    set_draw_window(x1, y1, x2, y2);

    //We will do the SPI write manually here for speed
    //( the data sheet says it doesn't matter if CSX changes between
    // data sections but I don't trust it.)
    //CSX low to begin data
    CSX = 0;
    //Write colour to each pixel
    for(int y = 0; y < y2-y1+1 ; y++) {
        for(int x = 0; x < x2-x1+1; x++) {
            spi_write(colour_high);
            spi_write(colour_low);
        }
    }
    //Return CSX to high
    CSX = 1;
}

/*
 * Sets the X,Y position for following commands on the display.
 * Should only be called within a function that draws something
 * to the display.
 */
void set_draw_window(char x1, char y1, char x2, char y2) {
    //SEt the column to write to
    lcd_write_command(ST7735_CASET);
    lcd_write_data(0x00);
    lcd_write_data(x1);
    lcd_write_data(0x00);
    lcd_write_data(x2);

    //Set the row range to write to
    lcd_write_command(ST7735_RASET);
    lcd_write_data(0x00);
    lcd_write_data(y1);
    lcd_write_data(0x00);
    lcd_write_data(y2);

    //Write to RAM
    lcd_write_command(ST7735_RAMWR);
}

/*
 * Draws a single char to the screen.
 * Called by the various string writing functions like print().
 */
void draw_char(char x, char y, char c, unsigned int colour, char size){
    int i, j;
    char line;
    unsigned int font_index = (c - 32) * 5;

    //Get the line of pixels from the font file
    for(i=0; i<5; i++ ) {
        //We have to pick from a different font file depending on the character.
        //See note in Font[] definition.
        if(c < 'T')
            line = Font1[font_index + i];
        else
            line = Font2[((c - 'S') * 5) + i];

        //Draw the pixels to screen
        for(j=0; j<7; j++) {
            if(line & 0x01) {
                if(size == 1)
                    //If we are just doing the smallest size font then do a single pixel each
                    draw_pixel(x+i, y+j, colour);
                else
                    //Otherwise do a small box to represent each pixel
                    fill_rectangle(x+(i*size), y+(j*size), x+(i*size)+size, y+(j*size)+size, colour);
            }

            line >>= 1; //Next row of pixels in the font
        }
    }
}

/*
 * Writes a string to the display as an array of chars at position x, y with
 * a given colour and size.
 */
void draw_string(char x, char y, unsigned int colour, char size, char *str) {
    //Work out the size of each character
    int char_width = size * 6;
    //Iterate through each character in the string
    int counter = 0;
    while(str[counter] != '\0') {
        //Calculate character position
        int char_pos = x + (counter * char_width);
        //Write char to the display
        draw_char(char_pos, y, str[counter], colour, size);
        //Next character
        counter++;
    }
}

/* Draws a bitmap array of colours to the display.
 * First two bytes should be width and height respectively.
 * Subsequent bits are uint16 representations of the pixel colours.
 *
 * NOTE: This could be made more efficient by not using the fill_rectangle
 * method. But I didn't need the speed, and it simplified the code a lot.
 */
void draw_bitmap(int x, int y, int scale, unsigned int *bmp) {
    int width = bmp[0];
    int height = bmp[1];
    unsigned int this_byte;
    int this_x;
    int this_y;
    //Set the drawing region
    //set_draw_window(x, y, x+width, y+height);

    //We will do the SPI write manually here for speed
    //CSX low to begin data
    //CSX = 0;
    //Write colour to each pixel
    for(int i = 0; i < height ; i++) {
        for(int j = 0; j < width; j++) {
            this_byte = bmp[(width * i) + j + 2];
            this_x = x + (j * scale);
            this_y = y + (i * scale);
            //Draw the pixel with appropriate scaling
            fill_rectangle(this_x, this_y, this_x + scale, this_y + scale, this_byte);

        }
    }
    //Return CSX to high
    //CSX = 1;
}

/*
 * Draw a line between two points, using the desired colour. Doesn't do any
 * fancy aliasing or anything.
 *
 * NOTE: Using floats takes up a huge amount of program memory. I would
 * recommend commenting out this function unless absolutely necessary.
 * Use fill_rectangle() to draw horizontal and vertical lines instead.
 *
 * TODO: Also this function doesn't actually work properly.
 */
/*
void draw_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned int colour) {
    //Calculate the horizontal length of the line
    int length = x2 - x1;
    //And calculate the rate of change of the graph
    float gradient = (y2 - y1) / length;

    //For each horizontal pixel, move down (or up) by the gradient value
    for(int i = 0; i <= length; i++) {
        draw_pixel(x1+i, y1 + (gradient * i), colour);
    }
}
*/