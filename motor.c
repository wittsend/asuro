/*
 * motor.c
 *
 * Created: 21/10/2017 11:36:30 AM
 *  Author: Fiona
 */ 

///////////[Includes]///////////////////////////////////////////////////////////////////////////////
#include "setup.h"
#include <stdlib.h>		//abs()
#include <avr/io.h>
//#include <stdio.h>		//printf
#include "usart.h"			//debug
#include "timer.h"
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

#define MD_KP			1.5
#define MD_KI			0.2
#define MD_KD			0.1

///////////[Global Variables]///////////////////////////////////////////////////////////////////////
MotorData motorLeft =
{
	.speedRpm	= 0,
	.minSpeed	= 0,
	.speedIn	= &(odoLeft.rpm),
	.kP			= 1
};

MotorData motorRight =
{
	.speedRpm	= 0,
	.minSpeed	= 0,
	.speedIn	= &(odoRight.rpm),
	.kP			= 1
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

//Drive the left motor at a given speed (-255 to 255) negative makes motor go backwards
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

//Drive the Right motor at a given speed (-255 to 255) negative makes motor go backwards
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

//Drive the robot with the given amount of turn ratio. (0 = straight, -255 = rotate anti clockwise
//on the spot and 255 = clockwise on the spot. Anything in between is a mixture)
void steerRobot(int16_t speed, int16_t turnRatio)
{
	int16_t leftMotorSpeed, rightMotorSpeed;
	
	//If speed is set to 0, then save processor cycles
	if(speed == 0.0)
	{
		motorRightStop;
		motorLeftStop;
		return;
	}
		
	//Make sure parameters are in range and correct if necessary
	speed = capToRangeInt(speed, -255, 255);
	turnRatio = capToRangeInt(turnRatio, -255, 255);
		
	//Calculate speed ratios
	float rotationalSpeed = speed*(turnRatio/255.0);
	float straightSpeed = abs(speed)-(abs(rotationalSpeed));
		
	leftMotorSpeed = (int16_t)(straightSpeed + rotationalSpeed);
	rightMotorSpeed = (int16_t)(straightSpeed - rotationalSpeed);
	
	//Apply speeds and directions to motors
	motorLeftDrive(leftMotorSpeed);
	motorRightDrive(rightMotorSpeed);
}

//Drive the motor at the given speed with closed loop control. Speed in RPMs. returns error value
int16_t motorLeftDriveSpeed(int16_t speedRPM)
{
	static uint32_t nextPollTime = 0;
	static int16_t pErr = 0;
	static int16_t iErr = 0;
	static int16_t pErrOld = 0;
	int16_t motorSpeed = 0;
	
	if(timerGetTimestamp() > nextPollTime)
	{
		nextPollTime = timerGetTimestamp() + odoLeft.pollInterval;
		speedRPM = capToRangeInt(speedRPM, -200, 200);
		
		pErrOld = pErr;
		
		//.rpm is always positive, so correct for desired direction
		if(speedRPM > 0)
			pErr = speedRPM - odoLeft.rpm;
		if(speedRPM < 0)
			pErr = speedRPM + odoLeft.rpm;
	
		iErr += pErr;
	
		iErr = capToRangeInt(iErr, -255, 255);
	
		motorSpeed = MD_KP*pErr + MD_KI*iErr + MD_KD*(pErr - pErrOld);
	
		motorSpeed = capToRangeInt(motorSpeed, -255, 255);
	
		motorLeftDrive(motorSpeed);
	
		return pErr;
	}
	return 0;
}

//Drive the motor at the given speed with closed loop control. Speed in RPMs. returns error value
int16_t motorRightDriveSpeed(int16_t speedRPM)
{
	static uint32_t nextPollTime = 0;
	static float pErr = 0;
	static float iErr = 0;
	static float pErrOld = 0;
	int16_t motorSpeed = 0;
	
	if(timerGetTimestamp() > nextPollTime)
	{
		nextPollTime = timerGetTimestamp() + odoRight.pollInterval;
		speedRPM = capToRangeInt(speedRPM, -200, 200);
		
		pErrOld = pErr;
		
		//.rpm is always positive, so correct for desired direction
		if(speedRPM > 0)
		pErr = speedRPM - odoRight.rpm;
		if(speedRPM < 0)
		pErr = speedRPM + odoRight.rpm;
		
		iErr += pErr;
		
		iErr = capToRangeInt(iErr, -255, 255);
		
		motorSpeed = MD_KP*pErr + MD_KI*iErr + MD_KD*(pErr - pErrOld);
		
		motorSpeed = capToRangeInt(motorSpeed, -255, 255);
		
		motorRightDrive(motorSpeed);
		
		return pErr;
	}
	return 0;
}