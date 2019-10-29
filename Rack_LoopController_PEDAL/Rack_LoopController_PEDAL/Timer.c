
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "MasterCom.h"
#include "Footswitch.h"

void Timer0_Init()
{
	/* Setup Timer0 */
	TCCR0B |= (1 << CS02);		// Set clock prescaler to 256 (16MHz / 256 / 256 = 244Hz (4,1ms))
	TIMSK0 |= (1 << TOIE0);		// Overflow interrupt enable
	TCNT0 = 0;
	
	Timer0_TickFlag = 0;
}

ISR(TIMER0_OVF_vect)
{
	//Timer0_TickFlag = 1;
	
	if (MasterCom_OvfCnt)
	{
		if (++MasterCom_OvfCnt > MASTER_COM_TIMEOUT)
		{
			MasterCom_TimeoutFlag = 1;
		}
	}
	
	if (Footswitch_OvfCnt)
	{
		Footswitch_OvfCnt++;
		
		if (Footswitch_OvfCnt > FOOTSWITCH_LONG_LONG_PRESS_OVF && Footswitch_PressState == PRESSED)
		{
			Footswitch_TimerFlag = 1;
			Footswitch_PressState = STILL_PRESSED;
		}
		else if (Footswitch_OvfCnt > FOOTSWITCH_LONG_PRESS_OVF  && Footswitch_PressState == WAITING)
		{
			Footswitch_PressState = PRESSED;
			Footswitch_TimerFlag = 1;
		}
		else if (Footswitch_OvfCnt > FOOTSWITCH_SHORT_PRESS_OVF && Footswitch_PressState == PRESS_SENSED)
		{
			Footswitch_PressState = WAITING;
			Footswitch_TimerFlag = 1;
		}
	}
}