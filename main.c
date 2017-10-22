/*
 * Asuro.c
 *
 * Created: 19/10/2017 4:31:24 PM
 * Author : Matthew
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>		//printf

#include "setup.h"
#include "usart.h"
#include "odometer.h"
#include "timer.h"
#include "battery.h"
#include "motor.h"

#if defined DEBUG
//Make printf work
static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
#endif

int main(void)
{
#if defined DEBUG
	stdout = &mystdout; //Required for printf init
	uint32_t lastPollTime = 0;
#endif

	setup();
	
    while (1) 
    {
#if defined DEBUG
		if(timerGetTimestamp() >= lastPollTime + 1000)
		{
			lastPollTime = timerGetTimestamp();
			printf("odo:%4i\n\r", odoLeft.rpm);
			//usartWriteString(debugString);
		}
#endif
		
		motorLeftDriveSpeed(60);
		motorRightDriveSpeed(60);
		
		batteryUpdate();
		odoPollAllSensors();
		usartInterpretCommand();
		usartTransmitWriteBuffer();
		
    }
}

#if defined DEBUG
//Used for printing characters to UART with printf
static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n') uart_putchar('\r', stream);
	
	//usartTransmit(c);
	usartBufferWrite(&c, 1);
	
	return 0;
}
#endif