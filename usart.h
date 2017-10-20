/*
 * usart.h
 *
 * Created: 19/10/2017 6:15:04 PM
 *  Author: Matthew
 */ 


#ifndef USART_H_
#define USART_H_

///////////[Defines]////////////////////////////////////////////////////////////////////////////////
#define USART_TX_BUFFER_SZ		33
#define USART_RX_BUFFER_SZ		9

#define usartTxReg				UDR
#define usartRxReg				UDR
#define usartDataRegEmpty		(UCSRA & (1<<UDRE))
#define usartTxComp				(UCSRA & (1<<TXC))
#define usartEnableRx			(UCSRB |= (1<<RXEN))
#define usartDisableRx			(UCSRB &= ~(1<<RXEN))
#define usartEnableTx			(UCSRB |= (1<<TXEN))
#define usartDisableTx			(UCSRB &= ~(1<<TXEN))


///////////[Type Definitions]///////////////////////////////////////////////////////////////////////
////UART Commands
typedef enum UsartCommands
{
	//Debug
	USART_SEND_TXCHECK_CMD	= 0xFF,
	//Fetch ADC Data
	USART_SEND_ADC0_CMD		= 0x10,
	USART_SEND_ADC1_CMD		= 0x11,
	USART_SEND_ADC2_CMD		= 0x12,
	USART_SEND_ADC3_CMD		= 0x13,
	USART_SEND_ADC4_CMD		= 0x14,
	USART_SEND_ADC5_CMD		= 0x15,

	USART_SEND_BATTVOL_CMD	= 0x51	
} UsartCommands;

///////////[Functions]//////////////////////////////////////////////////////////////////////////////
void usartInit(void);
uint8_t usartBufferWrite(uint8_t *bytes, uint8_t numOfBytes);
uint8_t usartBufferRead(uint8_t *byte);
uint8_t usartTransmitWriteBuffer(void);
uint8_t usartWriteString(char *string);
uint8_t usartTransmit(uint8_t byte);
void usartInterpretCommand(void);

#endif /* USART_H_ */