
# Landtiger development board

![alt text](https://raw.githubusercontent.com/Fachher/landtiger/master/resources/landtiger.png "Landtiger Board")

The board is a clone of the KEIL MCB1700 board. It was cheap and provide many peripherals (i.e. UART, ISP, GPIO) and an ARM Cortex M3 prozesser. 

An ARM Cortex M3 microcontroller is the heart of this board. Cortex M3 only supports the thumb2 instruction set which is 16bit decoded.
The processor of the Cortex M3 is designed in the hardward architecture. This means it can read data and instructions at the same time.
The processor can be programmed in the way you would do it with the von-neumann architecture in a linear way.
ARMv7-M (Cortex-M3)

## Target of this project
* provide IDE independent examples (Keil µVision is not required)
* get a deeper understanding how the linker works

## Requirements
* make
* segger jlink for mac
* gnu arm toolchain

Installing arm toolchain OSX

brew tap PX4/homebrew-px4
brew update
brew install gcc-arm-none-eabi

## Examples
* Chapter 1 - blinky written in gnu assembler 
* Chapter 2 - blinky written in C
* Chapter 3 - use the joystick to control leds
* Chapter 4 - connect lcd1602a with landtiger

## Sources
[Wikipedia Cortex](https://de.wikipedia.org/wiki/ARM_Cortex-M3)   
[Exploreembedded](http://exploreembedded.com/wiki/LPC1768:_Lcd_4bit)
[Ocfreaks](http://www.ocfreaks.com/lpc1768-gpio-programming-tutorial/)

