/*
 * motor.h
 *
 * Created: 21/10/2017 11:36:22 AM
 *  Author: Fiona
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

///////////[Type Definitions]///////////////////////////////////////////////////////////////////////
typedef struct MotorData
{
	int16_t		speedOut;		//PWM speed output (-255 to 255) sign represents direction
	uint8_t		minSpeed;		//Minimum PWM speed to get robot moving
	uint16_t	*speedIn;		//Pointer to odometer speed data
} MotorData;

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
void motorInit(void);
void motorLeftDrive(int16_t speed);
void motorRightDrive(int16_t speed);

#endif /* MOTOR_H_ */