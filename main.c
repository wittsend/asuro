/*
 * Asuro.c
 *
 * Created: 19/10/2017 4:31:24 PM
 * Author : Matthew
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

#include "setup.h"
#include "usart.h"
#include "battery.h"

int main(void)
{
	char debugString[32];
	setup();
	
    while (1) 
    {
		if(0)
		{
			sprintf(debugString, " Raw Battery: %4i \r\n", batteryGetVoltage());
			usartWriteString(debugString);
		}
		
		batteryUpdate();
		usartInterpretCommand();
		usartTransmitWriteBuffer();
		
    }
}

