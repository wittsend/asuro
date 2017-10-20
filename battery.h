/*
 * battery.h
 *
 * Created: 20/10/2017 10:09:12 AM
 *  Author: Fiona
 */ 


#ifndef BATTERY_H_
#define BATTERY_H_

//////////////[Type Definitions]////////////////////////////////////////////////////////////////////
typedef struct BatteryData
{
	AdcChannels adcChannel;
	float conversionFactor;
	uint16_t rawData;
	uint16_t voltage;
} BatteryData;

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
uint8_t batteryUpdate(void);
uint16_t batteryGetVoltage(void);

#endif /* BATTERY_H_ */