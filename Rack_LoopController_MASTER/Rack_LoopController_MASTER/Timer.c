
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "RotaryEncoder.h"
#include "SP10281_3x7segment.h"
#include "System.h"
#include "PedalCom.h"

void Timer0_Init()
{
	/* Setup Timer0 */
	TCCR0B |= (1 << CS02);		// Set clock prescaler to 256 (20MHz / 256 / 256 = 305Hz (3,28ms))
	TIMSK0 |= (1 << TOIE0);		// Overflow interrupt enable
	TCNT0 = 0;
	
	Timer0_TickFlag = 0;
}

ISR(TIMER0_OVF_vect)
{
	//Timer0_TickFlag = 1;
	
	if (PedalCom_OvfCnt)
	{
		if (++PedalCom_OvfCnt > PEDAL_COM_TIMEOUT)
		{
			PedalCom_TimeoutFlag = 1;
			PedalCom_OvfCnt = 0;
		}
	}
	
	if (UI_UserAction_OvfCnt)
	{
		if (++UI_UserAction_OvfCnt > UI_USER_ACTION_TIMEOUT)
		{
			UI_UserAction_TimoutFlag = 1;
			UI_UserAction_OvfCnt = 0;
		}
	}
	
	if (UI_MarkSelection_OvfCnt)
	{
		if (++UI_MarkSelection_OvfCnt > UI_MARK_SELECTION_TIMOUT)
		{
			UI_MarkSelection_TimoutFlag = 1;
			UI_MarkSelection_OvfCnt = 0;
		}
	}
	
	if (++SP10281_OvfCnt >= SP10281_OVF_RESET)
	{
		SP10281_OvfFlag = 1;
		SP10281_OvfCnt = 0;
	}
	
	if (RotaryEncoder_OvfCnt)
	{
		RotaryEncoder_OvfCnt++;
		
		if (RotaryEncoder_OvfCnt > ROT_ENC_LONG_PRESS_OVF)
		{		
			/* If button is still pressed */
			if ( GPIO_ROT_ENC_PIN & GPIO_ROT_ENC_PRESS_MASK )
			{
				RotEnc_State = LONG_PRESS;
			}
			else
			{
				RotEnc_State = ABORTED;
			}
			
			RotaryEncoder_OvfCnt = 0;		// Stop overflow counter
		}
		else if (RotaryEncoder_OvfCnt > ROT_ENC_PRESS_OVF && RotEnc_State == IDLE)
		{
			/* If button is no longer pressed */
			if ( !(GPIO_ROT_ENC_PIN & GPIO_ROT_ENC_PRESS_MASK) )
			{
				RotEnc_State = SHORT_PRESS;
				RotaryEncoder_OvfCnt = 0;	// Stop overflow counter
			}
			else
				RotEnc_State = PRESSED;

		}
	}
}