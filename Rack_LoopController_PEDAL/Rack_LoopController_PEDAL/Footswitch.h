
#ifndef FOOTSWITCH_H_
#define FOOTSWITCH_H_

#define FOOTSWITCH_SHORT_PRESS_OVF		 75	// 244Hz / 36 = 6,77Hz (147,5ms) 
#define FOOTSWITCH_LONG_PRESS_OVF		244	// 244Hz / 244 = 1Hz (1000ms)
#define FOOTSWITCH_LONG_LONG_PRESS_OVF	488	// 244 / 488 = 0,5Hz (2000ms)

enum Footswitch_State
{
	IDLE,
	PRESS_SENSED,
	WAITING,
	SHORT_PRESS,
	PRESSED,
	ABORTED,
	STILL_PRESSED,
	LONG_PRESS,
	LONG_LONG_PRESS
};

volatile unsigned short Footswitch_OvfCnt;
volatile unsigned char Footswitch_IntFlag;
volatile enum Footswitch_State Footswitch_PressState;
volatile unsigned char Footswitch_MCP_PortState;
volatile unsigned char Footswitch_MCP_IntMask;

void Footswitch_Init();
void Footswitch_EnableInterrupt();
void Footswitch_HandlePress();

#endif /* FOOTSWITCH_H_ */