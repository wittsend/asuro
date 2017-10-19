/*
 * fifo.h
 *
 * Created: 19/10/2017 9:33:37 PM
 *  Author: Fiona
 */ 


#ifndef FIFO_H_
#define FIFO_H_

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

///////////[Type Definitions]///////////////////////////////////////////////////////////////////////
typedef struct FifoBuffer
{
	uint8_t fifoSize;		//The number of elements the FIFO can hold plus 1
	uint8_t fifoIn;			//Input index
	uint8_t fifoOut;		//Output index
	uint8_t buffer[];		//The buffer (fifoSize long)
} FifoBuffer;

///////////[Functions]//////////////////////////////////////////////////////////////////////////////
void fifoInit(FifoBuffer *x);
uint8_t fifoPut(FifoBuffer *x, uint8_t new);
uint8_t fifoGet(FifoBuffer *x, uint8_t *old);

#endif /* FIFO_H_ */