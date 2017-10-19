/*
 * Asuro.c
 *
 * Created: 19/10/2017 4:31:24 PM
 * Author : Matthew
 */ 

#include <stdint.h>
#include <avr/io.h>
#include "setup.h"

int main(void)
{
	setup();
    while (1) 
    {

		usartTransmitWriteBuffer();
    }
}

