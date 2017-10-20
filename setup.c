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
	timerInit();
	usartInit();
	odoInit();
	adcInit();
	motorInit();
	sei();
}