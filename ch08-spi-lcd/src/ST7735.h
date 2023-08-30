/*
 * File:   ST7735.h
 * Author: tommy
 *
 * Created on 17 October 2018, 7:23 PM
 */

#ifndef ST7735_H
#define	ST7735_H

#ifdef	__cplusplus
extern "C" {
#endif

//Command definitions
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

//SPI Bus status register and transmission buffer
//Set these to suit your particular microcontroller
#define SPIBUF  SPI1TXB
#define SPIIDLE SPI1STATUS & 0x20

void lcd_write_command(unsigned char data);
void lcd_write_data(unsigned char data);
void lcd_init(void);
void lcd_init_command_list(void);
void draw_pixel(char x, char y, unsigned int colour);
void set_draw_window(char row_start, char row_end, char col_start, char col_end);
void fill_rectangle(char x1, char y1, char x2, char y2, unsigned int colour);
void draw_char(char x, char y, char c, unsigned int colour, char size);
void draw_string(char x, char y, unsigned int colour, char size, char *str);
void draw_line(char x1, char y1, char x2, char y2, unsigned int colour);
void draw_bitmap(int x, int y, int scale, unsigned int *bmp);

#ifdef	__cplusplus
}
#endif

#endif	/* ST7735_H */
