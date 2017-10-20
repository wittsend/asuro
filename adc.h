/*
 * adc.h
 *
 * Created: 20/10/2017 7:26:58 AM
 *  Author: Fiona
 */ 

#ifndef ADC_H_
#define ADC_H_

//////////////[Defines]/////////////////////////////////////////////////////////////////////////////
////Constants////

//Number of ADC channels available
#define ADC_CHANNELS			8

//ADC clock prescaler settings
#define ADC_DIV1_CLK			0x00
#define ADC_DIV2_CLK			0x01
#define ADC_DIV4_CLK			0x02
#define ADC_DIV8_CLK			0x03
#define ADC_DIV16_CLK			0x04
#define ADC_DIV32_CLK			0x05
#define ADC_DIV64_CLK			0x06
#define ADC_DIV128_CLK			0x07

////Macros////
//Changes the mux channel while ensuring previous settings remain
#define adcSetChannel(value)	(ADMUX = (ADMUX&0xE0)|(value&0x1F))
#define adcGetChannel			(ADMUX & 0x1F)

//Retrieve 10bit ADC sample
#define adcLastSample			(ADC)

//ADC conversion status and control
#define adcStartConv			(ADCSRA |= (1<<ADSC))
#define adcConvInProgress		(ADCSRA & (1<<ADSC))

//ADC interrupt control
#define adcDisableInterrupt		(ADCSRA &= ~(1<<ADIE))
#define adcEnableInterrupt		(ADCSRA |= (1<<ADIE))
//#define adcNewDataFlag			(~((ADCSRA & (1<<ADIF))>>ADIF) & 0x01)

#define ADC_ENABLE				1
#define ADC_DISABLE				0

/////////////[Type Definitions]/////////////////////////////////////////////////////////////////////
//ADC channel defines
typedef enum AdcChannels
{
	ADC_ODO_R			= 0x00,
	ADC_ODO_L			= 0x01,
	ADC_LINE_R			= 0x02,
	ADC_LINE_L			= 0x03,
	ADC_COL_SW			= 0x04,
	ADC_BATTERY			= 0x05
} AdcChannels;

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
//Sets up the analogue to digital hardware
void adcInit(void);

//Tells the calling function whether or not new data is available to be read from the ADC driver
uint8_t adcNewData(AdcChannels channel);

//Will return the data last sampled from the given ADC channel
uint16_t adcGetData(AdcChannels channel);

//Enable or disable an ADC channel
void adcEnableChannel(AdcChannels channel, uint8_t action);

#endif /* ADC_H_ */