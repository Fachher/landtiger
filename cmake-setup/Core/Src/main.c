#include "lpc17xx.h"
#include "lpc_types.h"
#include "helper.h"

__INLINE static void LED_Config(void) {
  LPC_GPIO2->FIODIR = 0x000000ff;               /* LEDs PORT2 are Output */
  LPC_GPIO0->FIODIR = 0x00200000;
  LPC_GPIO0->FIOPIN |=  0x00200000u;
}

__INLINE static void LED_On (uint32_t led) {
  LPC_GPIO2->FIOPIN |=  (led);                  /* Turn On  LED */
}

__INLINE static void LED_Off (uint32_t led) {
  LPC_GPIO2->FIOPIN &= ~(led);                  /* Turn Off LED */
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main (void) {
  uint8_t  location;

  LED_Config();

  while(1)
  {
  	LED_On (0xff);
  	delay(500);
	LED_Off(0xff);
  	for(location=0;location<8;location++)
	{
    	LED_On ((1u<<location));                     /* Turn on the LED. */
        delay(100);
    	LED_Off ((1u<<location));                    /* Turn off the LED. */
        delay(100);
	}
  }

}
#pragma clang diagnostic pop

