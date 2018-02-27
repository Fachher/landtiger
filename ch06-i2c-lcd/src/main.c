#include "lpc17xx.h"
#include "i2c.h"

#define NUM_NIBBLES 2
#define NUM_DATA_PACKETS 3

#define RS 0x1
#define RW 0x2
#define ENABLE_BIT 0x4

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
void fill_lines(const uint8_t commands[], uint32_t num_of_commands, uint8_t result[], uint8_t reset_flag);
void addPackets(uint8_t nibble, uint8_t reset_flag, uint8_t result[], uint32_t *pos);

int main (void)
{
	SystemInit();

	if (SysTick_Config(SystemCoreClock / 1000)) { /* Setup SysTick Timer for 1 msec interrupts  */
		while (1);                                  /* Capture error */
	}

	i2c0_init(MODE_100kbps, 3);
	uint8_t commands[] = {0x02, 0x28, 0x0e, 0x01, 0x80};
	uint8_t message[] = {'H','e','l','l','o',' ','W','o','r','l','d'};
	uint32_t num_of_commands = sizeof(commands);
	uint32_t num_of_chars = sizeof(message);
	uint32_t num_command_lines = NUM_NIBBLES * NUM_DATA_PACKETS * num_of_commands;
	uint32_t num_message_lines = NUM_NIBBLES * NUM_DATA_PACKETS * num_of_chars;
	uint8_t command_lines[num_command_lines];
	uint8_t message_lines[num_message_lines];


	fill_lines(commands, num_of_commands, command_lines, 0);
	fill_lines(message, num_of_chars, message_lines, 1);

    uint8_t slave = 0x39;

	for(uint32_t i=0;i<num_command_lines;i++){
		i2c0_send(slave, &command_lines[i], 1);
		Delay(1);
	}

	for(uint32_t i=0;i<num_message_lines;i++){
		i2c0_send(slave, &message_lines[i], 1);
		Delay(1);
	}

}

void fill_lines(const uint8_t commands[], uint32_t num_of_commands, uint8_t result[], uint8_t reset_flag){
	uint32_t pos = 0;
	for(uint32_t i=0; i < num_of_commands; i++){
		addPackets((commands[i] & 0xf0), reset_flag, result, &pos);
		addPackets((commands[i] << 4), reset_flag, result, &pos);
	}
}

void addPackets(uint8_t c, uint8_t reset_flag, uint8_t result[], uint32_t *pos){
	for(int i=0;i<3;i++){
		if(i & 0x1){ 
			c |= ENABLE_BIT;
		}
		else{
			c &= ~ENABLE_BIT;
		}
		c |= 0x8;
		c |= reset_flag;
		result[*pos] = c;
		*pos += 1;
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/
