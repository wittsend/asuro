/*
 * setup.c
 *
 * Created: 20/10/2017 7:34:14 AM
 *  Author: Fiona
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "adc.h"
#include "usart.h"
#include "odometer.h"
#include "motor.h"
#include "setup.h"

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
void setup(void)
{
	WDT_off();
	timerInit();
	usartInit();
	odoInit();
	adcInit();
	motorInit();
	sei();
}

void WDT_off(void)
{
	/* Write logical one to WDCE and WDE */
	WDTCR |= (1<<WDCE) | (1<<WDE);
	/* Turn off WDT */
	WDTCR = 0x00;
}

int32_t capToRangeInt(int32_t valueToCap, int32_t minimumVal, int32_t maximumVal)
{
	if(valueToCap > maximumVal)
		valueToCap = maximumVal;
	if(valueToCap < minimumVal)
		valueToCap = minimumVal;
	return valueToCap;
}

uint32_t capToRangeUint(uint32_t valueToCap, uint32_t minimumVal, uint32_t maximumVal)
{
	if(valueToCap > maximumVal)
		valueToCap = maximumVal;
	if(valueToCap < minimumVal)
		valueToCap = minimumVal;
	return valueToCap;
}

float capToRangeFlt(float valueToCap, float minimumVal, float maximumVal)
{
	if(valueToCap > maximumVal)
		valueToCap = maximumVal;
	if(valueToCap < minimumVal)
		valueToCap = minimumVal;
	return valueToCap;
}