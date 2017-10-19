/*
 * usart.c
 *
 * Created: 19/10/2017 4:31:49 PM
 *  Author: Matthew
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
#include <avr/interrupt.h>
#include "fifo.h"
#include "usart.h"

///////////[Global variables]///////////////////////////////////////////////////////////////////////
FifoBuffer usartRxFifo =
{
	.fifoSize = USART_RX_BUFFER_SZ,
	.fifoIn = 0,
	.fifoOut = 0,
	.buffer[USART_RX_BUFFER_SZ] = 0
};

FifoBuffer usartTxFifo =
{
	.fifoSize = USART_TX_BUFFER_SZ,
	.fifoIn = 0,
	.fifoOut = 0,
	.buffer[USART_TX_BUFFER_SZ] = 0
};

///////////[Functions]//////////////////////////////////////////////////////////////////////////////
void initUSART(void)
{
	UCSRA
	=	0x00;
	
	UCSRB
	=	0x00;
	
	UCSRC
	=	(1<<URSEL)		//Allow writing to UCSRC instead of UBRRH
	|	(3<<UCSZ0);		//8-bit, 1 stop bit
	
	UBRRL
	=	1;				//250kbps @ 8.00MHz
	//= 207;			//2400bps @ 8.00MHz

}

//Write a byte to the transmit buffer
//Return 0 on success, or 1 if buffer full.
uint8_t usartBufferWrite(uint8_t byte)
{
	if(fifoPut(&usartTxFifo, byte))
		return 1;
	else
		return 0;
}

//Read a byte from the receive buffer
//Return 0 on success, or 1 if nothing to read
uint8_t usartBufferRead(uint8_t *byte)
{
	if(fifoGet(&usartRxFifo, byte))
		return 1;
	else
		return 0;
}

//Transmit the contents of the transmit buffer from the USART
uint8_t usartTransmitWriteBuffer(void)
{
	uint8_t bytesSent = 0;
	uint8_t fifoEmpty = 0;
	uint8_t fetchedByte = 0;
	
	while(!fifoEmpty)	//If theres more data to send
	{
		fifoEmpty = fifoGet(&usartTxFifo, &fetchedByte);
		if(!fifoEmpty) 
		{
			usartTransmit(fetchedByte);
			bytesSent++;
		}
	}
	return bytesSent;
}

//Write a string to the TX FIFO buffer
uint8_t usartWriteString(char *string)
{
	for(uint8_t i = 0; string[i]; i++)
	{
		if(fifoPut(&usartTxFifo, string[i]))
		{
			return 1;
		}
	}
	return 0;
}

//Transmit a byte on the UART
uint8_t usartTransmit(uint8_t byte)
{
	while(!usartDataRegEmpty);				//Wait for UDR register to empty
	usartTxReg = byte;						//Load byte into UDR
	return 0;
}

//UART Receive complete interrupt
SIGNAL(USART_RXC_vect)
{
	//Load into the FIFO
	fifoPut(&usartRxFifo, usartRxReg);
}

