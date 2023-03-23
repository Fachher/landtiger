#include "lpc17xx.h"
#include "lpc_types.h"


volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
__INLINE static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

/*------------------------------------------------------------------------------
  configer LED pins
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Config(void) {

  LPC_GPIO2->FIODIR = 0x000000ff;               /* LEDs PORT2 are Output */
  LPC_GPIO0->FIODIR = 0x00200000;
  LPC_GPIO0->FIOPIN |=  0x00200000u;
}

/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_On (uint32_t led) {

  LPC_GPIO2->FIOPIN |=  (led);                  /* Turn On  LED */
}

/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
__INLINE static void LED_Off (uint32_t led) {

  LPC_GPIO2->FIOPIN &= ~(led);                  /* Turn Off LED */
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main (void) {
  uint8_t  location;

  if (SysTick_Config(SystemCoreClock / 1000)) { /* Setup SysTick Timer for 1 msec interrupts  */
    while (1);                                  /* Capture error */
  }

  LED_Config();

  while(1)
  {
  	LED_On (0xff);
	Delay (500);
	LED_Off(0xff);
  	for(location=0;location<8;location++)
	{
    	LED_On ((1u<<location));                     /* Turn on the LED. */
    	Delay (100);                                /* delay  100 Msec */
    	LED_Off ((1u<<location));                    /* Turn off the LED. */
    	Delay (100);                                /* delay  100 Msec */
	}
  }

}
#pragma clang diagnostic pop

