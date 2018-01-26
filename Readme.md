
# Landtiger development board

![alt text](https://raw.githubusercontent.com/Fachher/landtiger/master/resources/landtiger.png "Landtiger Board")

The board is a clone of the KEIL MCB1700 board. It was cheap and provide many peripherals (i.e. UART, ISP, GPIO) and an ARM Cortex M3 prozesser. 

## Target of this project
* provide IDE independent examples (Keil µVision is not required)
* get a deeper understanding how the linker works

## Requirements
* make
* segger jlink for mac
* gnu arm toolchain

## Examples
* Chapter 1 - blinky written in gnu assembler 
* Chapter 2 - blinky written in C
* Chapter 3 - use the joystick to control leds
* Chapter 4 - connect lcd1602a with landtiger
