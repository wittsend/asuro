/*
 * Asuro.c
 *
 * Created: 19/10/2017 4:31:24 PM
 * Author : Matthew
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>		//sprintf

#include "setup.h"
#include "usart.h"
#include "odometer.h"
#include "timer.h"
#include "battery.h"
#include "motor.h"


int main(void)
{
	char debugString[32];
	uint32_t lastPollTime = 0;
	setup();
	
	motorLeftDrive(-128);
	
    while (1) 
    {
		if(timerGetTimestamp() > lastPollTime + 1000)
		{
			lastPollTime = timerGetTimestamp();
			sprintf(debugString, "%4i\r\n", odoLeft.rps);
			usartWriteString(debugString);
		}
		
		batteryUpdate();
		odoPollAllSensors();
		usartInterpretCommand();
		usartTransmitWriteBuffer();
		
    }
}

