/*
 * odometer.h
 *
 * Created: 20/10/2017 6:28:34 PM
 *  Author: Fiona
 */ 


#ifndef ODOMETER_H_
#define ODOMETER_H_

#include "adc.h"

///////////[Defines]////////////////////////////////////////////////////////////////////////////////
#define odoLedOn	{(DDRD |= (1<<PD7)); (PORTD |= (1<<PD7));}
#define odoLedOff	{(DDRD |= (1<<PD7)); (PORTD &= ~(1<<PD7));}

#define odoDisable	(DDRC |= (1<<PC0)|(1<<PC1))
#define odoEnable	(DDRC &= ~((1<<PC0)|(1<<PC1)))

///////////[Type Definitions]///////////////////////////////////////////////////////////////////////
typedef enum OdometerState {ODO_LOW, ODO_HIGH} OdometerState;

typedef struct OdometerData
{
	AdcChannels		adcChannel;		//ADC channel of the photo-transistor
	uint16_t		rawData;		//Raw ADC data
	uint16_t		maxValue;		//Max raw ADC data
	uint16_t		minValue;		//Min raw ADC data
	uint16_t		midValue;		//Mid value between max and min
	uint16_t		hysteresis;		//Amount of hysteresis to determine high or low
	OdometerState	curState;		//Current state of sensor (high/low)
	OdometerState	prevState;		//Previous state of sensor (high/low)
	uint32_t		counts;			//Number of rising edges since last poll
	uint16_t		dCounts;		//Counts at last poll (sum to track distance)
	uint16_t		pollInterval;	//Time between rising edge reads
	uint32_t		lastPollTime;	//Time at which sensor was last polled.
	uint16_t		encStepsConv;	//Conv coeff. = (60000ms per min)/(30 enc steps per wheel rev)
	uint16_t		rpm;			//Revolutions per minute
} OdometerData;

///////////[Global Variables]///////////////////////////////////////////////////////////////////////
OdometerData odoLeft;
OdometerData odoRight;

///////////[Functions]//////////////////////////////////////////////////////////////////////////////
void odoInit(void);
void odoObjectBuild(OdometerData *x, AdcChannels adcChannel);
uint8_t odoUpdateSensor(OdometerData *x);
void odoPollAllSensors(void);

#endif /* ODOMETER_H_ */