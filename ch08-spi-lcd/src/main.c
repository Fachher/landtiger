#include "lpc17xx.h"
#include "helper.h"


int main (void)
{
	SystemCoreClockUpdate();

	uart_init(9600);

	while (1) {
		log("Hello World\r\n");
		delay(1000);
	}
}
