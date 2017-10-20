/*
 * odometer.c
 *
 * Created: 20/10/2017 6:28:25 PM
 *  Author: Fiona
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
//#include <stdint.h>
#include "adc.h"
#include "timer.h"
#include "odometer.h"

///////////[Global Variables]///////////////////////////////////////////////////////////////////////
OdometerData odoLeft;
OdometerData odoRight;

///////////[Functions]//////////////////////////////////////////////////////////////////////////////
//Initialise the Pins for the odometer sensors
void odoInit(void)
{
	odoLedOn;
	odoEnable;
	odoObjectBuild(&odoLeft, ADC_ODO_L);
	odoObjectBuild(&odoRight, ADC_ODO_R);
}

//Constructor for new odometer sensor objects
void odoObjectBuild(OdometerData *x, AdcChannels adcChannel)
{
	x->adcChannel	= adcChannel;
	x->maxValue		= 0;
	x->minValue		= 1023;
	x->midValue		= 512;
	x->hysteresis	= 128;
	x->time			= 65535;
}

//See if new data is available and update the states of the sensors
uint8_t odoUpdateSensor(OdometerData *x)
{
	uint8_t rangeShift = 0;
	//Left Sensor
	if(adcNewData(x->adcChannel))
	{
		x->rawData = adcGetData(x->adcChannel);
		
		if(x->rawData > x->maxValue)
		{
			x->maxValue = x->rawData;
			rangeShift = 1;
		}
		
		if(x->rawData < x->minValue)
		{
			x->minValue = x->rawData;
			rangeShift = 1;
		}
		
		if(rangeShift)
		{
			x->midValue = (x->minValue + x->maxValue)/2;
		}
		
		x->prevState = x->curState;
		
		if(x->rawData > (x->midValue + x->hysteresis))
			x->curState = ODO_HIGH;
			
		if(x->rawData < (x->midValue - x->hysteresis))
			x->curState = ODO_LOW;
	} else
		return 1;
	return 0;
}