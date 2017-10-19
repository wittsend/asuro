/*
 * adc.c
 *
 * Created: 20/10/2017 7:26:47 AM
 *  Author: Fiona
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
//#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

//////////////[Global variables]////////////////////////////////////////////////////////////////////
volatile uint8_t adcCurrentChannel;
volatile uint8_t adcNewDataFlag = 0;		//1 When new data, otherwise 0
volatile uint16_t adcData[ADC_CHANNELS];	//Stores data from each ADC channel

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
//Sets up the analogue to digital hardware
void adcInit(void)
{
	ADMUX
	=	(0x01<<REFS0);
	//=	0x00;					//Voltage reference selection. (AREF)
	
	adcCurrentChannel = ADC_ODO_R_CH;
	adcSetChannel(adcCurrentChannel);
	
	ADCSRA
	=	(1<<ADEN)				//Enable the ADC
	|	(1<<ADSC)				//Start converting
	|	(1<<ADIE)				//Enable interrupt
	|	ADC_DIV64_CLK;			//1/64th conversion speed
}

//Tells the calling function whether or not new data is available to be read from the ADC driver
uint8_t adcNewData(void)
{
	if(adcNewDataFlag)
	{
		adcNewDataFlag = 0;
		return 1;
	} else
	return 0;
}

//Will return the data last sampled from the given ADC channel
uint16_t adcGetData(uint8_t channel)
{
	if(channel >= ADC_CHANNELS)
		return 0;
	return adcData[channel];
}

//ADC conversion complete interrupt
ISR(ADC_vect)
{
	//Don't constantly interrupt the code. Only perform a read when the last data has been read.
	if(!adcNewDataFlag)
	{
		adcData[adcCurrentChannel] = adcLastSample;	//Fetch the last sample and store it in the
													//array
		
		switch(adcCurrentChannel)					//Switch to the next ADC mux channel to read
		{
			case ADC_ODO_R_CH:
				adcCurrentChannel = ADC_ODO_L_CH;
				break;
			
			case ADC_ODO_L_CH:
				adcCurrentChannel = ADC_LINE_R_CH;
				break;

			case ADC_LINE_R_CH:
				adcCurrentChannel = ADC_LINE_L_CH;
				break;

			case ADC_LINE_L_CH:
				adcCurrentChannel = ADC_COL_SW_CH;
				break;

			case ADC_COL_SW_CH:
				adcCurrentChannel = ADC_BATTERY_CH;
				break;
			
			case ADC_BATTERY_CH:
				adcCurrentChannel = ADC_ODO_R_CH;
				adcNewDataFlag = 1;					//If all ADC channels have been sampled, then
				break;								//indicate that new data is available.
		}
		adcSetChannel(adcCurrentChannel);			//Set the ADC mux channel
	}
	adcStartConv;									//Start the next conversion.
}
