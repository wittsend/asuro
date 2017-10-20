/*
 * motor.c
 *
 * Created: 21/10/2017 11:36:30 AM
 *  Author: Fiona
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>		//abs()
#include <avr/io.h>
#include "odometer.h"
#include "motor.h"

///////////[Defines]////////////////////////////////////////////////////////////////////////////////
#define motorLeftFwd	PORTD |= (0x02<<PD4)
#define motorLeftRev	PORTD |= (0x01<<PD4)
#define motorLeftStop	PORTD |= (0x00<<PD4)

#define motorRightFwd	PORTB |= (0x02<<PB4)
#define motorRightRev	PORTB |= (0x01<<PB4)
#define motorRightStop	PORTB |= (0x00<<PB4)

#define motorLeftPWM	OCR1A
#define motorRightPWM	OCR1B

///////////[Global Variables]///////////////////////////////////////////////////////////////////////
MotorData motorLeft =
{
	.speedOut	= 0,
	.minSpeed	= 0,
	.speedIn	= &(odoLeft.rps)
};

MotorData motorRight =
{
	.speedOut	= 0,
	.minSpeed	= 0,
	.speedIn	= &(odoRight.rps)
};

///////////[Functions]//////////////////////////////////////////////////////////////////////////////
//Set up PIO for motor drive
void motorInit(void)
{
	//Left motor direction output
	DDRD
	|=	(1<<PD5)
	|	(1<<PD4);
	
	//Right motor direction output
	DDRB
	|=	(1<<PB5)
	|	(1<<PB4)
	|	(1<<PB2)	//PWM output for right motor
	|	(1<<PB1);	//PWM output for left motor

	//Configure Timer1 for PWM output
	// for PWM (8-Bit PWM) on OC1A & OC1B
	TCCR1A 
	=	(1 << WGM10)
	|	(1 << COM1A1)
	|	(1 << COM1B1);
	
	// tmr1 running on MCU clock/8
	TCCR1B	
	=	(1 << CS11);
}

void motorLeftDrive(int16_t speed)
{
	if(speed < 0)
		motorLeftRev;
	else if(speed > 0)
		motorLeftFwd;
	else
		motorLeftStop;
		
	motorLeftPWM = abs(speed);
}

void motorRightDrive(int16_t speed)
{
	if(speed < 0)
		motorRightRev;
	else if(speed > 0)
		motorRightFwd;
	else
		motorRightStop;
	
	motorRightPWM = abs(speed);
}