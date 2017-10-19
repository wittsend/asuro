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
	.adcChannel			= ADC_BATTERY_CH,
	.conversionFactor	= 1
};

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
uint8_t updateBattery(void)
{
	if(adcNewData(ADC_BATTERY_CH))
	{
		battery.rawData = adcGetData(battery.adcChannel);
		battery.voltage = battery.rawData*battery.conversionFactor;
		return 0;
	} else 
		return 1;
}

uint16_t getBatteryVoltage(void)
{
	return battery.voltage;
}