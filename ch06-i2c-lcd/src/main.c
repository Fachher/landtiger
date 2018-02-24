#include "lpc17xx.h"
#include "i2c.h"

extern  uint8_t buf[32];
volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
/*******************************************************************************
**   Dealy Function  Delay()
*******************************************************************************/
/* local function to generate some delay */
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}
/*******************************************************************************
**   Main Function  main()
*******************************************************************************/
int main (void)
{
	SystemInit();

	i2c0_init(MODE_400kbps, 3);
    char buf[100] = "hello";
    uint8_t slave = 0x39;
    uint32_t res;
    if ((res = i2c0_send(slave, buf, sizeof(buf))) == 0xFFFFFFFF);
        /* slave did not response on bus */
    /*if ((res = i2c0_recv(slave, buf, sizeof(buf))) == 0xFFFFFFFF);*/
        /* slave did not response on bus */
    /*else {*/
        /*buf[res] = '\0';*/
    /*}*/
}

/******************************************************************************
**                            End Of File
******************************************************************************/
