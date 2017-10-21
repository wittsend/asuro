/*
 * setup.h
 *
 * Created: 20/10/2017 7:34:44 AM
 *  Author: Fiona
 */ 


#ifndef SETUP_H_
#define SETUP_H_

#include <avr/io.h>
#include <stdint.h>

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
void setup(void);
void WDT_off(void);
int32_t capToRangeInt(int32_t valueToCap, int32_t minimumVal, int32_t maximumVal);
uint32_t capToRangeUint(uint32_t valueToCap, uint32_t minimumVal, uint32_t maximumVal);
float capToRangeFlt(float valueToCap, float minimumVal, float maximumVal);

#endif /* SETUP_H_ */