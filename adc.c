/*
 * adc.c
 *
 * Created: 20/10/2017 7:26:47 AM
 *  Author: Fiona
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

//////////////[Global variables]////////////////////////////////////////////////////////////////////
volatile uint8_t adcCurrentChannel;
volatile uint8_t adcNewDataFlag = 0x00;		//Each bit represents if there is new data on said
											//adc channel
volatile uint8_t adcEnabledChannels = 0x3F;	//What ADC channels are enabled for sampling
volatile uint16_t adcData[ADC_CHANNELS];	//Stores data from each ADC channel

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
//Sets up the analogue to digital hardware
void adcInit(void)
{
	ADMUX
	=	(0x01<<REFS0);
	//=	0x00;					//Voltage reference selection. (AREF)
	
	adcCurrentChannel = ADC_ODO_R;
	adcSetChannel(adcCurrentChannel);
	
	ADCSRA
	=	(1<<ADEN)				//Enable the ADC
	|	(1<<ADSC)				//Start converting
	|	(1<<ADIE)				//Enable interrupt
	|	ADC_DIV64_CLK;			//1/64th conversion speed
}

//Tells the calling function whether or not new data is available to be read from the ADC driver
uint8_t adcNewData(AdcChannels channel)
{
	if(adcNewDataFlag & (1<<channel))
	{
		adcNewDataFlag &= ~(1<<channel);
		return 1;
	} else
	return 0;
}

//Will return the data last sampled from the given ADC channel
uint16_t adcGetData(AdcChannels channel)
{
	if(channel >= ADC_CHANNELS)
		return 0;
	return adcData[channel];
}

//Enable or disable an ADC channel
void adcEnableChannel(AdcChannels channel, uint8_t action)
{
	if(action == ADC_ENABLE)
		adcEnabledChannels |= (1<<channel);
	else
		adcEnabledChannels &= ~(1<<channel);
}

//ADC conversion complete interrupt
ISR(ADC_vect)
{
	adcData[adcCurrentChannel] = adcLastSample;		//Fetch the last sample and store it in the
													//array
	adcNewDataFlag |= (1<<adcCurrentChannel);		//Update data flag for this channel
	if(adcEnabledChannels)							//If at least one ADC channel is enabled
	{
		do											//Look for next enabled channel
		{
			adcCurrentChannel = (adcCurrentChannel + 1) % ADC_CHANNELS;
		} while (!(adcEnabledChannels & (1<<adcCurrentChannel)));
		
		adcSetChannel(adcCurrentChannel);			//When found, set the ADC mux channel		
	}
	adcStartConv;									//Start the next conversion.
}
