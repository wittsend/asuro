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
#include "adc.h"		//Comms
#include "battery.h"	//Comms

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
void usartInit(void)
{
	UBRRL
	//=	1;				//250kbps @ 8.00MHz
	//=	12;				//38.4kbps @ 8.00MHz
	//=	51;				//9600bps @ 8.00MHz
	//=	103;			//4800bps @ 8.00MHz
	=	207;			//2400bps @ 8.00MHz
	
	UCSRB
	=	(1<<RXCIE);		//Enable receive complete interrupt
	
	UCSRC
	=	(1<<URSEL)		//Allow writing to UCSRC instead of UBRRH
	|	(3<<UCSZ0);		//8-bit, 1 stop bit
	


	usartEnableRx;
	usartEnableTx;
	usartTransmit(0xFF);//Send something to reset TX complete
}

//Write multiple bytes to the transmit buffer
//Return 0 on success, or 1 if buffer full.
uint8_t usartBufferWrite(uint8_t *bytes, uint8_t numOfBytes)
{
	for(uint8_t i = 0; i < numOfBytes; i++)
	{
		if(fifoPut(&usartTxFifo, bytes[i]))
			return 1;
	}
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
	
	do	//If theres more data to send
	{
		fifoEmpty = fifoGet(&usartTxFifo, &fetchedByte);
		if(!fifoEmpty) 
		{
			usartTransmit(fetchedByte);
			bytesSent++;
		}
	} while(!fifoEmpty);
	
	return bytesSent;
}

//Write a string to the TX FIFO buffer
uint8_t usartWriteString(char *string)
{
	for(uint8_t i = 0; string[i]; i++)			//Keep looping until a NULL is seen in the string
	{
		if(fifoPut(&usartTxFifo, string[i]))	//Place next char in the TX FIFO
		{
			return 1;							//If FIFO is full return 1
		}
	}
	return 0;									//If successful, then return 0
}

//Transmit a byte on the UART
uint8_t usartTransmit(uint8_t byte)
{
	while(!usartDataRegEmpty);				//Wait for UDR register to empty
	usartClearTxComp;
	usartTxReg = byte;						//Load byte into UDR
	return 0;
}

//Will read next command in the FIFO and act upon it. Will be moved to a comms module eventually
void usartInterpretCommand(void)
{
	const uint8_t dataBufferSz = 4;
	UsartCommands command;
	uint8_t dataBuffer[dataBufferSz];
	uint8_t numOfBytes = 0;
	
	if(!usartBufferRead(&command))	//If command found
	{
		switch(command)
		{
			case USART_SEND_TXCHECK_CMD:
				dataBuffer[0] = USART_SEND_TXCHECK_CMD;
				numOfBytes = 1;
				break;
				
			case USART_SEND_ADC0_CMD:
				dataBuffer[0] = USART_SEND_ADC0_CMD;
				dataBuffer[1] = ((adcGetData(0x00)>>8) & 0xFF);
				dataBuffer[2] = ((adcGetData(0x00)>>0) & 0xFF);
				numOfBytes = 3;
				break;
				
			case USART_SEND_ADC1_CMD:
				dataBuffer[0] = USART_SEND_ADC1_CMD;
				dataBuffer[1] = ((adcGetData(0x01)>>8) & 0xFF);
				dataBuffer[2] = ((adcGetData(0x01)>>0) & 0xFF);
				numOfBytes = 3;
				break;
				
			case USART_SEND_ADC2_CMD:
				dataBuffer[0] = USART_SEND_ADC2_CMD;
				dataBuffer[1] = ((adcGetData(0x02)>>8) & 0xFF);
				dataBuffer[2] = ((adcGetData(0x02)>>0) & 0xFF);
				numOfBytes = 3;
				break;
				
			case USART_SEND_ADC3_CMD:
				dataBuffer[0] = USART_SEND_ADC3_CMD;
				dataBuffer[1] = ((adcGetData(0x03)>>8) & 0xFF);
				dataBuffer[2] = ((adcGetData(0x03)>>0) & 0xFF);
				numOfBytes = 3;
				break;
				
			case USART_SEND_ADC4_CMD:
				dataBuffer[0] = USART_SEND_ADC4_CMD;
				dataBuffer[1] = ((adcGetData(0x04)>>8) & 0xFF);
				dataBuffer[2] = ((adcGetData(0x04)>>0) & 0xFF);
				numOfBytes = 3;
				break;
				
			case USART_SEND_ADC5_CMD:
				dataBuffer[0] = USART_SEND_ADC5_CMD;
				dataBuffer[1] = ((adcGetData(0x05)>>8) & 0xFF);
				dataBuffer[2] = ((adcGetData(0x05)>>0) & 0xFF);
				numOfBytes = 3;
				break;
				
			case USART_SEND_BATTVOL_CMD:
				dataBuffer[0] = USART_SEND_BATTVOL_CMD;
				dataBuffer[1] = ((batteryGetVoltage()>>8) & 0xFF);
				dataBuffer[2] = ((batteryGetVoltage()>>0) & 0xFF);
				numOfBytes = 3;
				break;
			
		}
		usartBufferWrite(dataBuffer, numOfBytes);
	}
}

//UART Receive complete interrupt
ISR(USART_RXC_vect)
{
	//Load into the FIFO
	if(usartTxComp)
		fifoPut(&usartRxFifo, usartRxReg);
	else
		usartRxReg;
}

