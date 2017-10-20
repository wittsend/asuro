/*
 * timer.c
 *
 * Created: 20/10/2017 10:29:29 AM
 *  Author: Fiona
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

//////////////[Global variables]////////////////////////////////////////////////////////////////////
volatile uint32_t systemTimestamp = 0;			//Number of ms since power up
volatile uint8_t msCounter = TIMER_INTS_PER_MS;	//Number of timer interrupts until next ms

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
//Timer2 initialisation routine
void timerInit(void)
{
	TCCR2 
	=	(1<<WGM21)
	|	(1<<COM20)
	|	(1<<CS20);
	
	OCR2
	=	110;				// 36kHz @8MHz
	
	TIMSK
	|=	(1 << OCIE2);		//Enable interrupt on output compare
}

//Make the program wait the desired number of milliseconds
void timerDelayMs(uint16_t timeMs)
{
	uint32_t startTime = systemTimestamp;
	while(systemTimestamp < (startTime + timeMs));
}

//Return the systemTimestamp
uint32_t timerGetTimestamp(void)
{
	return systemTimestamp;
}

//Timer2 output compare interrupt
ISR(TIMER2_COMP_vect)
{
	msCounter--;
	if(!msCounter)
	{
		msCounter = TIMER_INTS_PER_MS;
		systemTimestamp++;
	}
}