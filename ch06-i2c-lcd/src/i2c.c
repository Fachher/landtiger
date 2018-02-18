/*****************************************************************************
 *   i2c.c:  I2C C file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
 *****************************************************************************/
#include "lpc17xx.h"
#include "i2c.h"
uint8_t buf[32];
extern void Delay(uint32_t delaydata);

/* Global variable                     */
volatile uint8_t     I2C_sla;                                             /* I2C device slave address                */
volatile uint32_t    I2C_suba;                                            /* I2C device internal subaddress            */
volatile uint8_t     I2C_suba_num;                                        /* Number of bytes of I2C subaddress              */
volatile uint8_t     *I2C_buf;                                            /* Data Buffer Pointer               */
volatile uint32_t    I2C_num;                                             /* The number of data to be read / written        */
volatile uint8_t     I2C_end;                                             /* I2C bus end flag: The end of the bus is  */
/* Set 1                          */
volatile uint8_t     I2C_suba_en;         /*  Subaddress control
											  0 - The subaddress has been processed or does not require a subaddress
											  1 - read operation
											  2 - write operation
											  */
/* 
   From device to device, the I2C communication protocol may vary, 
   in the example below, the protocol uses repeated start to read data from or 
   write to the device:
   For master read: the sequence is: STA,Addr(W),offset,RE-STA,Addr(r),data...STO 
   for master write: the sequence is: STA,Addr(W),length,RE-STA,Addr(w),data...STO
   Thus, in state 8, the address is always WRITE. in state 10, the address could 
   be READ or WRITE depending on the I2CCmd.
   */   

/*****************************************************************************
 ** Function name:		I2C0_IRQHandler
 **
 ** Descriptions:		I2C0 interrupt handler, deal with master mode
 **						only.
 **
 ** parameters:			None
 ** Returned value:		None
 ** 
 *****************************************************************************/
void I2C0_IRQHandler(void)  
{
	uint8_t StatValue;
	StatValue = LPC_I2C0->I2STAT & 0xf8;

	switch ( StatValue )
	{
		case 0x08:		 										/* Send start condition */
			if (I2C_suba_en == 1)
			{	LPC_I2C0->I2DAT = I2C_sla &0xfe;	
			}
			else
			{	LPC_I2C0->I2DAT = I2C_sla;	
			}	
			LPC_I2C0->I2CONCLR = (1 << 3)|(1 << 5);
			break;

		case 0x10:												/* Repeated start condition has been sent */ 
			LPC_I2C0->I2DAT = I2C_sla;
			LPC_I2C0->I2CONCLR = 0x28;
			break;

		case 0x18:											/* Received ACK send data*/
		case 0x28:											/* Data in I2DAT sent, ACK */
			if (I2C_suba_en == 0)
			{

				if(I2C_num>0)
				{  
					LPC_I2C0->I2DAT = *I2C_buf++; 			
					LPC_I2C0->I2CONCLR = 0x28;
					I2C_num--;
					Delay(1);
				}
				else
				{
					LPC_I2C0->I2CONSET = (1 << 4); 				
					LPC_I2C0->I2CONCLR = 0x28;  				
					I2C_end =1;
				}

			}
			if(I2C_suba_en == 1)
			{
				if(I2C_suba_num == 2)
				{
					LPC_I2C0->I2DAT = ((I2C_suba >> 8) & 0xff);
					LPC_I2C0->I2CONCLR = 0x28;
					I2C_suba_num--; 
					break;
				}
				if(I2C_suba_num == 1)
				{
					LPC_I2C0->I2DAT = (I2C_suba & 0xff);
					LPC_I2C0->I2CONCLR = 0x28;
					I2C_suba_num--; 
					break;
				}
				if(I2C_suba_num == 0)
				{
					LPC_I2C0->I2CONCLR = 0x08;
					LPC_I2C0->I2CONSET = 0x20;
					I2C_suba_en = 0;
					break;		  		 
				}   	
			}

			if ( I2C_suba_en == 2 )
			{
				if(I2C_suba_num > 0)
				{
					if(I2C_suba_num == 2)
					{
						LPC_I2C0->I2DAT = ((I2C_suba >> 8) & 0xff);
						LPC_I2C0->I2CONCLR = 0x28;
						I2C_suba_num--; 
						break;
					}
					if(I2C_suba_num == 1)
					{
						LPC_I2C0->I2DAT = (I2C_suba & 0xff);
						LPC_I2C0->I2CONCLR = 0x28;
						I2C_suba_num--; 
						I2C_suba_en  = 0;
						break;
					}
				}
			}
			break;

		case 0x40:									 /* Sent SLA + R, Received ACK */
			if (I2C_num <= 1)
			{	LPC_I2C0->I2CONCLR = 1 << 2;
			}
			else
			{
				LPC_I2C0->I2CONSET = 1 << 2;			/* assert ACK after data is received */
			}
			LPC_I2C0->I2CONCLR = 0x28;
			break;

		case 0x20:								/* regardless, it's a NACK *//* SLA + W sent */
		case 0x30:	 							/* Data in I2DAT sent, non-acknowledgment received */	
		case 0x38:		
		case 0x48:								/* SLA + R sent, non-acknowledgment received */
			LPC_I2C0->I2CONCLR = 0x28;
			I2C_end = 0xFF;
			break;

		case 0x50:								/* Received data byte, ACK returned */
			*I2C_buf++ = LPC_I2C0->I2DAT;
			I2C_num--;
			if (I2C_num ==1)					    /* Receive last byte             */
			{	LPC_I2C0->I2CONCLR = 0x2c;	   		/* STA,SI,AA = 0                */
			}
			else
			{	LPC_I2C0->I2CONSET = 0x04;			
				LPC_I2C0->I2CONCLR = 0x28;	   
			}
			break;

		case 0x58: 								/* Data bytes received, non-acknowledgment returned */	
			*I2C_buf++ = LPC_I2C0->I2DAT;
			LPC_I2C0->I2CONSET = 0X10;				/* assert ACK after data is received */
			LPC_I2C0->I2CONCLR = 0X28;
			I2C_end = 1;
			break;

		default:
			break;
	}

}
/*****************************************************************************
 ** Function name:		I2CInit
 **
 ** Descriptions:		Initialize I2C controller
 **
 ** parameters:			I2c mode is either MASTER or SLAVE
 ** Returned value:		true or false, return false if the I2C
 **				interrupt handler was not installed correctly
 ** 
 *****************************************************************************/
uint32_t I2CInit( uint32_t I2cMode ) 
{
	LPC_SC->PCONP |= (1 << 19);

	/* set PIO0.27 and PIO0.28 to I2C0 SDA and SCK */
	/* function to 01 on both SDA and SCK. */
	LPC_PINCON->PINSEL1 &= ~0x03C00000;
	LPC_PINCON->PINSEL1 |= 0x01400000;	

	/*--- Reset registers ---*/
	LPC_I2C0->I2SCLL   = I2SCLL_SCLL;
	LPC_I2C0->I2SCLH   = I2SCLH_SCLH;
	if ( I2cMode == I2CSLAVE )
	{
		LPC_I2C0->I2ADR0 = 0xA0;
	}    

	/* Install interrupt handler */
	NVIC_EnableIRQ(I2C0_IRQn);

	LPC_I2C0->I2CONSET = I2CONSET_I2EN;
	return( 1 );
}

/*
 *********************************************************************************************************
 ** Function Name: I2C_WriteNByte ()
 ** Function Function: Write N bytes of data to subaddressed devices
 ** Entrance Parameter: sla Device slave address
 ** suba_type Subaddress structure 1-single-byte address 3-8 + X structure 2-double-byte address
 ** suba device internal physical address
 ** * s Pointer to the data to be written
 ** num The number of data to be written
 ** Exit parameters: 1 The operation was successful
 ** 0 The operation failed
 *********************************************************************************************************
 */
uint8_t I2C_WriteNByte(uint8_t sla, uint8_t suba_type, uint32_t suba, uint8_t *s, uint32_t num)
{
	if (num > 0)                                                /* If the number of reads is 0, it returns an error */
	{
		if (suba_type == 1)
		{
			I2C_sla         = sla;                                      /* Read device slave address */
			I2C_suba        = suba;                                     /* Device Sub Address */
			I2C_suba_num    = 1;                                        /* Device subaddress is 1 byte */
		}
		if (suba_type == 2)
		{
			I2C_sla         = sla;                                      /* Read device slave address */
			I2C_suba        = suba;                                     /* Device Sub Address */
			I2C_suba_num    = 2;                                        /* Device subaddress is 1 byte */
		}
		if (suba_type == 3)
		{
			I2C_sla         = sla + ((suba >> 7 )& 0x0e);               /* Read device slave address */
			I2C_suba        = suba;                                     /* Device Sub Address */
			I2C_suba_num    = 1;                                        /* Device subaddress is 1 byte */
		}

		I2C_buf     	= s;                                                /* Data */
		I2C_num     	= num;                                              /* The number of data */
		I2C_suba_en 	= 2;                                                /* Have sub address, write operation */
		I2C_end     	= 0;
		LPC_I2C0->I2CONCLR = (1 << 2)|
			(1 << 3)|
			(1 << 5);  /* Clear SI flag */

		LPC_I2C0->I2CONSET = (1 << 5)|
			(1 << 6);	/* Set Start flag */

		while(I2C_end == 0);
		if(I2C_end)
		{
			Delay(20);
			return 1;
		}
		else
		{
			Delay(20);
			return 0;
		}
	}
	Delay(20);
	return 0;
}
/*
 ************************************************** ************************************************** *****
 ** Function Name: I2C_ReadNByte ()
 ** Function Function: Read N bytes of data from any address that has subaddress
 ** Entrance Parameter: sla Device slave address
 ** suba_type Subaddress structure 1 - single byte address 2-8 + X structure 2 - double byte address
 ** suba Device sub address
 ** s data receive buffer pointer
 ** num read the number
 ** Exit parameters: 1 The operation was successful
 ** 0 The operation failed
 ************************************************** ************************************************** *****
 */
uint8_t I2C_ReadNByte (uint8_t sla, uint8_t suba_type, uint32_t suba, uint8_t *s, uint32_t num)
{
	if (num > 0)
	{
		if (suba_type == 1)
		{
			I2C_sla         = sla + 1;                                  /* Read device slave address, R=1          */
			I2C_suba        = suba;                                     /* Device sub address                   */
			I2C_suba_num    = 1;                                        /* Device subaddress is 1 byte            */
		}
		if (suba_type == 2)
		{
			I2C_sla         = sla + 1;                                  /* Read device slave address, R=1          */
			I2C_suba        = suba;                                     /* Device sub address                   */
			I2C_suba_num    = 2;                                        /* Device subaddress is 1 byte            */
		}
		if (suba_type == 3)
		{
			I2C_sla         = sla + ((suba >> 7 )& 0x0e) + 1;            /* Read device slave address, R=1          */
			I2C_suba        = suba & 0x0ff;                              /* Device sub address                   */
			I2C_suba_num    = 1;                                        /* Device subaddress is 1 byte            */
		}



		I2C_buf     	= s;                                                /* Data receive buffer pointer           */
		I2C_num     	= num;                                              /* The number of reads                 */
		I2C_suba_en	 	= 1;                                                /* Have subaddress read                   */
		I2C_end     	= 0;

		LPC_I2C0->I2CONCLR = (1 << 2)|
			(1 << 3)|
			(1 << 5);  /* Clear SI flag */

		LPC_I2C0->I2CONSET = (1 << 5)|
			(1 << 6);	/* Set Start flag */

		while(I2C_end == 0);
		if(I2C_end)
		{
			Delay(20);
			return 1;
		}
		else
		{
			Delay(20);
			return 0;
		}

	}
	Delay(20);
	return 0;
}
/******************************************************************************
 **                            End Of File
 ******************************************************************************/

