/*
 * usart.h
 *
 * Created: 19/10/2017 6:15:04 PM
 *  Author: Matthew
 */ 


#ifndef USART_H_
#define USART_H_

///////////[Defines]////////////////////////////////////////////////////////////////////////////////
#define USART_TX_BUFFER_SZ	33
#define USART_RX_BUFFER_SZ	33

#define usartTxReg				UDR
#define usartRxReg				UDR
#define usartDataRegEmpty		(UCSRA & (1<<UDRE))


///////////[Functions]//////////////////////////////////////////////////////////////////////////////
void initUSART(void);
uint8_t usartBufferWrite(uint8_t byte);
uint8_t usartBufferRead(uint8_t *byte);
uint8_t usartTransmitWriteBuffer(void);
uint8_t usartWriteString(char *string);
uint8_t usartTransmit(uint8_t byte);


#endif /* USART_H_ */