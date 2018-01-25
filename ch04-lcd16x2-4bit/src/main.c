#include"lpc17xx.h"

#define LcdDataBusPort      LPC_GPIO0->FIOPIN
#define LcdControlBusPort   LPC_GPIO2->FIOPIN

#define LcdDataBusDirnReg   LPC_GPIO0->FIODIR
#define LcdCtrlBusDirnReg   LPC_GPIO2->FIODIR

#define LCD_D4     15
#define LCD_D5     16
#define LCD_D6     17
#define LCD_D7     18

#define LCD_RS     4
#define LCD_RW     1
#define LCD_EN     3


volatile uint32_t msTicks;                            /* counts 1ms timeTicks */

void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/* Masks for configuring the DataBus and Control Bus direction */
#define LCD_ctrlBusMask   ((1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN))
#define LCD_dataBusMask   ((1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7))

/* local function to generate some delay */
__INLINE static void delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

void sendNibble(char nibble)
{
    LcdDataBusPort&=~(LCD_dataBusMask);                   // Clear previous data
    LcdDataBusPort|= (((nibble >>0x00) & 0x01) << LCD_D4);
    LcdDataBusPort|= (((nibble >>0x01) & 0x01) << LCD_D5);
    LcdDataBusPort|= (((nibble >>0x02) & 0x01) << LCD_D6);
    LcdDataBusPort|= (((nibble >>0x03) & 0x01) << LCD_D7);
}

void Lcd_CmdWrite(char cmd)
{
    sendNibble((cmd >> 0x04) & 0x0F);  //Send higher nibble
    LcdControlBusPort &= ~(1<<LCD_RS); // Send LOW pulse on RS pin for selecting Command register
    LcdControlBusPort &= ~(1<<LCD_RW); // Send LOW pulse on RW pin for Write operation
    LcdControlBusPort |= (1<<LCD_EN);  // Generate a High-to-low pulse on EN pin
    delay(1);
    LcdControlBusPort &= ~(1<<LCD_EN);

    delay(10);

    sendNibble(cmd & 0x0F);            //Send Lower nibble
    LcdControlBusPort &= ~(1<<LCD_RS); // Send LOW pulse on RS pin for selecting Command register
    LcdControlBusPort &= ~(1<<LCD_RW); // Send LOW pulse on RW pin for Write operation
    LcdControlBusPort |= (1<<LCD_EN);  // Generate a High-to-low pulse on EN pin
    delay(1);
    LcdControlBusPort &= ~(1<<LCD_EN); 

    delay(10);
}

void Lcd_DataWrite(char dat)
{
    sendNibble((dat >> 0x04) & 0x0F);  //Send higher nibble
    LcdControlBusPort |= (1<<LCD_RS);  // Send HIGH pulse on RS pin for selecting data register
    LcdControlBusPort &= ~(1<<LCD_RW); // Send LOW pulse on RW pin for Write operation
    LcdControlBusPort |= (1<<LCD_EN);  // Generate a High-to-low pulse on EN pin
    delay(1);
    LcdControlBusPort &= ~(1<<LCD_EN);

    delay(10);

    sendNibble(dat & 0x0F);            //Send Lower nibble
    LcdControlBusPort |= (1<<LCD_RS);  // Send HIGH pulse on RS pin for selecting data register
    LcdControlBusPort &= ~(1<<LCD_RW); // Send LOW pulse on RW pin for Write operation
    LcdControlBusPort |= (1<<LCD_EN);  // Generate a High-to-low pulse on EN pin
    delay(1);
    LcdControlBusPort &= ~(1<<LCD_EN); 

    delay(10);
}



int main()
{
    char a[]={"Good morning!"};
    SystemInit();                         //Clock and PLL configuration

	if (SysTick_Config(SystemCoreClock / 1000)) { /* Setup SysTick Timer for 1 msec interrupts  */
		while (1);                                  /* Capture error */
	}


    LcdDataBusDirnReg |= LCD_dataBusMask;  // Configure all the LCD pins as output
    LcdCtrlBusDirnReg |= LCD_ctrlBusMask;

	Lcd_CmdWrite(0x02);                   // Initialize Lcd in 4-bit mode
    Lcd_CmdWrite(0x28);                   // enable 5x7 mode for chars 
    Lcd_CmdWrite(0x0E);                   // Display OFF, Cursor ON
    Lcd_CmdWrite(0x01);                   // Clear Display
    Lcd_CmdWrite(0x80);                   // Move the cursor to beginning of first line


    Lcd_DataWrite('H');
	Lcd_DataWrite('e');
	Lcd_DataWrite('l');
	Lcd_DataWrite('l');
	Lcd_DataWrite('o');
	Lcd_DataWrite(' ');
	Lcd_DataWrite('w');
	Lcd_DataWrite('o');
	Lcd_DataWrite('r');
	Lcd_DataWrite('l');
	Lcd_DataWrite('d');

    Lcd_CmdWrite(0xc0);
    for(int i=0;a[i]!=0;i++)
    {
        Lcd_DataWrite(a[i]);
    }

    while(1);
}
