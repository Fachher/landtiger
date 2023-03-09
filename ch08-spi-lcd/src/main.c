#include "lpc17xx.h"
#include "lpc_types.h"
#include "uart.h"

extern volatile uint32_t UART0Count;
extern volatile uint8_t UART0Buffer[BUFSIZE];

void delay(uint32_t milliseconds) {
	LPC_TIM0->TCR = 0x02; // Reset Timer
	LPC_TIM0->PR  = 0x00; // Set prescaler to zero
	LPC_TIM0->MR0 = SystemCoreClock/4000 * milliseconds;
	LPC_TIM0->IR  = 0xff; // Reset all interrrupts
	LPC_TIM0->MCR = 0x04; // Stop timer on match
	LPC_TIM0->TCR = 0x01; // Start timer

	while (LPC_TIM0->TCR & 0x01); // Wait for timer to stop
}

int main (void)
{
	SystemCoreClockUpdate();

	UARTInit(0, 9600);

	//void UARTSend( uint32_t portNum, uint8_t *BufferPtr, uint32_t Length );
	char message[] = "Hello, World!\r\n";
	int length = sizeof(message) / sizeof(char);

	while (1) 
	{
		UARTSend( 0, (uint8_t *)(message), length);
		delay(1000);
	}
}
