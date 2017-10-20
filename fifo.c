/*
 * fifo.c
 *
 * Created: 19/10/2017 9:33:20 PM
 *  Author: Fiona
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "fifo.h"

///////////[Functions]//////////////////////////////////////////////////////////////////////////////
void fifoInit(FifoBuffer *x)
{
	x->fifoIn = x->fifoOut = 0;
}

//Place an element in the FIFO buffer
uint8_t fifoPut(FifoBuffer *x, uint8_t new)
{
	if(x->fifoIn == (( x->fifoOut - 1 + x->fifoSize) % x->fifoSize))
		return 1; /* fifo Full*/

	x->buffer[x->fifoIn] = new;
	x->fifoIn = (x->fifoIn + 1) % x->fifoSize;

	return 0; // No errors
}

//Fetch the next element from the FIFO. Returns 0 on success, 1 if FIFO empty
uint8_t fifoGet(FifoBuffer *x, uint8_t *old)
{
	if(x->fifoIn == x->fifoOut)
		return 1; /* fifo Empty - nothing to get*/

	*old = x->buffer[x->fifoOut];
	x->fifoOut = (x->fifoOut + 1) % x->fifoSize;

	return 0; // No errors
}