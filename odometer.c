/*
 * odometer.c
 *
 * Created: 20/10/2017 6:28:25 PM
 *  Author: Fiona
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <avr/io.h>
#include "timer.h"
#include "odometer.h"

///////////[Functions]//////////////////////////////////////////////////////////////////////////////
//Initialise the Pins for the odometer sensors
void odoInit(void)
{
	odoLedOn;
	odoEnable;
	odoObjectBuild(&odoLeft, ADC_ODO_L);
	odoObjectBuild(&odoRight, ADC_ODO_R);
}

//Constructor for initialising new odometer sensor objects
void odoObjectBuild(OdometerData *x, AdcChannels adcChannel)
{
	x->adcChannel	= adcChannel;
	x->maxValue		= 0;
	x->minValue		= 1023;
	x->midValue		= 512;
	x->hysteresis	= 128;
	x->pollInterval	= 250;
	x->lastPollTime	= 0;
	x->encStepsConv = 33;
	x->rps			= 0;
	x->counts		= 0;
	x->curState		= ODO_LOW;
	x->prevState	= ODO_LOW;
}

//See if new data is available and update the states of the sensors
uint8_t odoUpdateSensor(OdometerData *x)
{
	uint8_t rangeShift = 0;	//Indicates if calibration data has been updated
	
	//If new ADC data available for the sensor
	if(adcNewData(x->adcChannel))
	{
		//Retrieve the data from the ADC driver
		x->rawData = adcGetData(x->adcChannel);
		
		//Perform self calibration if necessary
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
			x->hysteresis = (x->maxValue - x->midValue)/2;
			//[Perhaps a routine to store the updated data in EEPROM should go here]
		}
		//Update the previous state
		x->prevState = x->curState;
		
		//See if there has been a state change, and if so update current state
		if(x->rawData > (x->midValue + x->hysteresis))
			x->curState = ODO_HIGH;
		if(x->rawData < (x->midValue - x->hysteresis))
			x->curState = ODO_LOW;
		
		//Increment the counts if rising edge detected
		if(x->curState == ODO_HIGH && x->prevState == ODO_LOW)
			x->counts++;
	}
	
	//If the poll interval has elapsed, then calculate a new revs per second value
	uint32_t timestamp = timerGetTimestamp();
	if(timestamp > (x->lastPollTime + x->pollInterval))
	{
		x->rps = (x->counts*x->encStepsConv)/(timestamp - x->lastPollTime);
		x->counts = 0;
	}
	return 0;
}

//Will poll all odometer sensors for new data
void odoPollAllSensors(void)
{
	odoUpdateSensor(&odoLeft);
	odoUpdateSensor(&odoRight);
}