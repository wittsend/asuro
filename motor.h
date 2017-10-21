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
	int16_t		speedRpm;		//Desired speed in RPM
	uint8_t		minSpeed;		//Minimum PWM speed to get robot moving
	uint16_t	*speedIn;		//Pointer to odometer speed data
	float		kP;				//P error constant
} MotorData;

///////////[Global Variables]///////////////////////////////////////////////////////////////////////
extern MotorData motorLeft;
extern MotorData motorRight;

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
void motorInit(void);
void motorLeftDrive(int16_t speed);
void motorRightDrive(int16_t speed);
void steerRobot(int16_t speed, int16_t turnRatio);
int16_t motorLeftDriveSpeed(int16_t speedRPM);
int16_t motorRightDriveSpeed(int16_t speedRPM);

#endif /* MOTOR_H_ */