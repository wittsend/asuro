/*
 * odometer.h
 *
 * Created: 20/10/2017 6:28:34 PM
 *  Author: Fiona
 */ 


#ifndef ODOMETER_H_
#define ODOMETER_H_

///////////[Defines]////////////////////////////////////////////////////////////////////////////////
#define odoLedOn	{(DDRD |= (1<<PD7)); (PORTD |= (1<<PD7));}
#define odoLedOff	{(DDRD |= (1<<PD7)); (PORTD &= ~(1<<PD7));}

#define odoEnable	(DDRC |= (1<<PC0)|(1<<PC1))
#define odoDisable	(DDRC &= ~((1<<PC0)|(1<<PC1)))

///////////[Type Definitions]///////////////////////////////////////////////////////////////////////
typedef enum OdometerState {ODO_LOW, ODO_HIGH} OdometerState;

typedef struct OdometerData
{
	AdcChannels adcChannel;
	uint16_t rawData;
	uint16_t maxValue;
	uint16_t minValue;
	uint16_t midValue;
	uint16_t hysteresis;
	OdometerState curState;
	OdometerState prevState;
	uint16_t time;
} OdometerData;

///////////[Functions]//////////////////////////////////////////////////////////////////////////////
void odoInit(void);
void odoObjectBuild(OdometerData *x, AdcChannels adcChannel);
uint8_t odoUpdateSensors(void);

#endif /* ODOMETER_H_ */