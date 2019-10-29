
#ifndef FOOTSWITCH_H_
#define FOOTSWITCH_H_

#define FOOTSWITCH_SHORT_PRESS_OVF		 75	// 244Hz / 36 = 6,77Hz (147,5ms) 
#define FOOTSWITCH_LONG_PRESS_OVF		244	// 244Hz / 244 = 1Hz (1000ms)
#define FOOTSWITCH_LONG_LONG_PRESS_OVF	488	// 244 / 488 = 0,5Hz (2000ms)

enum Footswitch_State
{
	IDLE			= 0,
	PRESS_SENSED	= 3,
	WAITING			= 5,
	SHORT_PRESS		= 1,
	PRESSED			= 6,
	ABORTED			= 7,
	STILL_PRESSED	= 8,
	LONG_PRESS		= 2,
	LONG_LONG_PRESS = 4
};

volatile unsigned char Footswitch_PressFlag;
volatile unsigned char Footswitch_TimerFlag;
volatile unsigned short Footswitch_OvfCnt;
volatile enum Footswitch_State Footswitch_PressState;
volatile unsigned char Footswitch_MCP_PortState;
volatile unsigned char Footswitch_MCP_IntMask;

void Footswitch_Init();
void Footswitch_EnableInterrupt();
void Footswitch_HandlePress();
void Footswitch_HandleTimer();

#endif /* FOOTSWITCH_H_ */