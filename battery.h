/*
 * battery.h
 *
 * Created: 20/10/2017 10:09:12 AM
 *  Author: Fiona
 */ 


#ifndef BATTERY_H_
#define BATTERY_H_

#include "adc.h"
//////////////[Type Definitions]////////////////////////////////////////////////////////////////////
typedef struct BatteryData
{
	AdcChannels adcChannel;
	float conversionFactor;
	uint16_t rawData;
	uint16_t voltage;
	uint32_t lastPollTime;
	uint16_t pollInterval;
} BatteryData;

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
uint8_t batteryUpdate(void);
uint16_t batteryGetVoltage(void);

#endif /* BATTERY_H_ */