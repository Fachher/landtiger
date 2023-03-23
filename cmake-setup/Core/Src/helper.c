#include "lpc17xx.h"

void delay(uint32_t milliseconds) {
	LPC_TIM0->TCR = 0x02; // Reset Timer
	LPC_TIM0->PR  = 0x00; // Set prescaler to zero
	LPC_TIM0->MR0 = SystemCoreClock/4000 * milliseconds;
	LPC_TIM0->IR  = 0xff; // Reset all interrrupts
	LPC_TIM0->MCR = 0x04; // Stop timer on match
	LPC_TIM0->TCR = 0x01; // Start timer

	while (LPC_TIM0->TCR & 0x01); // Wait for timer to stop
}

void uart_init(int baudrate) {
	uint32_t Fdiv;
	uint32_t pclkdiv, pclk;
	LPC_PINCON->PINSEL0 &= ~0x000000F0;
	LPC_PINCON->PINSEL0 |= 0x00000050;  
	/* RxD0 is P0.3 and TxD0 is P0.2 */
	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	   all the peripherals is 1/4 of the SystemCoreClock. */
	/* Bit 6~7 is for UART0 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
	switch ( pclkdiv )
	{
		case 0x00:
		default:
			pclk = SystemCoreClock/4;
			break;
		case 0x01:
			pclk = SystemCoreClock;
			break;
		case 0x02:
			pclk = SystemCoreClock/2;
			break;
		case 0x03:
			pclk = SystemCoreClock/8;
			break;
	}

	LPC_UART0->LCR = 0x83;    /* 8 bits, no Parity, 1 Stop bit */
	Fdiv = ( pclk / 16 ) / baudrate ;  /*baud rate */
	LPC_UART0->DLM = Fdiv / 256;
	LPC_UART0->DLL = Fdiv % 256;
	LPC_UART0->LCR = 0x03;    /* DLAB = 0 */
	LPC_UART0->FCR = 0x07;    /* Enable and reset TX and RX FIFO. */
}


void send_uart(const char data) {
	// Wait until TX FIFO is not full (in other words transmitter to be ready)
	while (!(LPC_UART0->LSR & (1 << 5)));

	// Send the data
	LPC_UART0->THR = data;
}

void log(const char *str)
{
    while (*str) {
		send_uart(*str++);
    }
}
