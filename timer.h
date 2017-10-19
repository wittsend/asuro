/*
 * timer.h
 *
 * Created: 20/10/2017 10:29:38 AM
 *  Author: Fiona
 */ 


#ifndef TIMER_H_
#define TIMER_H_

/////////////[Defines]//////////////////////////////////////////////////////////////////////////////
#define TIMER_INTS_PER_MS	72		//Number of interrupts per ms

//////////////[Type Definitions]////////////////////////////////////////////////////////////////////
//typedef enum DelayStates {DLY_START, DLY_WAIT, DLY_STOP} DelayStates;
//
//typedef struct DelayInstance
//{
	//uint32_t startTime;
	//uint32_t timeToWait;
	//DelayStates state;
//} DelayInstance;

/////////////[Functions]////////////////////////////////////////////////////////////////////////////
void timerInit(void);
void timerDelayMs(uint16_t timeMs);


#endif /* TIMER_H_ */