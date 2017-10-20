/*
 * battery.c
 *
 * Created: 20/10/2017 10:09:21 AM
 *  Author: Fiona
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include "adc.h"
#include "battery.h"

//////////////[Global variables]////////////////////////////////////////////////////////////////////
BatteryData battery = 
{
	.adcChannel			= ADC_BATTERY,
	.conversionFactor	= 11.891503267973
};

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
uint8_t batteryUpdate(void)
{
	if(adcNewData(ADC_BATTERY))
	{
		battery.rawData = adcGetData(battery.adcChannel);
		battery.voltage = battery.rawData*battery.conversionFactor;
		return 0;
	} else 
		return 1;
}

uint16_t batteryGetVoltage(void)
{
	return battery.voltage;
}