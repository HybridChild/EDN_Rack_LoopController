/*
 * Footswitch.h
 *
 * Created: 12-02-2020 18:40:06
 *  Author: Esben
 */ 


#ifndef FOOTSWITCH_H_
#define FOOTSWITCH_H_


#define FOOTSWITCH_LONG_PRESS_TIMEOUT		 550	// 550*2ms = 1100ms
#define FOOTSWITCH_LONG_LONG_PRESS_TIMEOUT	1100	// 1200*2ms = 2200ms

enum Footswitch_State
{
	IDLE,
	PRESS_SENSED,
	WAITING,
	SHORT_PRESS,
	PRESSED,
	STILL_PRESSED,
	LONG_PRESS,
	LONG_LONG_PRESS
};

void Footswitch_Init();
void Footswitch_PressDetected();
void Footswitch_HandleTimer();
void Footswitch_HandlePress();
void Footswitch_EnableInterrupt();

volatile extern Footswitch_State Footswitch_PressState;
volatile extern bool Footswitch_PressFlag;
volatile extern uint16_t Footswitch_TimerOvfCnt;
volatile extern bool Footswitch_TimerFlag;

#endif /* FOOTSWITCH_H_ */