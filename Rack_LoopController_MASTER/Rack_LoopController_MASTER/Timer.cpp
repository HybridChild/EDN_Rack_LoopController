
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "RotaryEncoder.h"
#include "SP10281_3x7segment.h"
#include "MCP23017.h"
#include "PedalCom.h"
#include "System.h"

void Timer0_Init()
{
	/* Setup Timer */
	TCCR0A |= (1 << WGM01);		// Set Timer to CTC mode (Clear Timer on Compare match)
	TCNT0 = 0;					// Clear counter register
	OCR0A = 155;				// Set Output Compare value (78125Hz / 156 = 500,8Hz (1,997ms))
	TIMSK0 |= (1 << OCIE0A);	// Output Compare A Match Interrupt Enable
	
	/* Start Timer */
	TCCR0B |= (1 << CS02);		// Set clock prescaler to 256 (20MHz / 256 = 78125Hz)
}

uint32_t Timer0_ms2cnt(uint32_t ms)
{
	ms *= 1000;
	return (uint32_t)(ms / TIMER0_PERIOD_US);
}

/* Timer0 Compare A Match Interrupt Service Routine */
ISR(TIMER0_COMPA_vect)
{
	/* Overflow counter to reset parameter if user doesn't commit */
	if (System_TempSelectOvfCnt)
	{
		if (++System_TempSelectOvfCnt > SYSTEM_TEMP_SELECT_TIMEOUT)
		{
			System_TempSelectOvfCnt = 0;
			System_TempSelectFlag = true;
		}
	}
	
	/* Overflow counter to shortly blank LEDs to mark selection */
	if (System_MarkSelectionOvfCnt)
	{
		if (++System_MarkSelectionOvfCnt > SYSTEM_MARK_SELECTION_TIMEOUT)
		{
			System_MarkSelectionOvfCnt = 0;
			System_MarkSelectionFlag = true;
		}
	}
	
	/* Overflow counter for delaying transmit to pedal */
	if (PedalCom_DelayTXOvfCnt)
	{
		if (++PedalCom_DelayTXOvfCnt > PEDALCOM_DELAY_TX_TIMEOUT)
		{
			PedalCom_DelayTXOvfCnt = 0;	// Stop overflow counter
			PedalCom_DelayTxFlag = true;
		}
	}

	/* Overflow counter for Pedal response timeout and heartbeat */
	if (++PedalCom_ResponseTimeoutOvfCnt > PEDALCOM_RESPONSE_TIMEOUT)
	{
		PedalCom_ResponseTimeoutOvfCnt = 1;	// Reset overflow counter
		PedalCom_ResponseTimeoutFlag = true;
	}
	
	/* Overflow counter for multiplexing 3x7-segment display */
	if (++SP10281_OvfCnt >= SP10281_OVF_RESET)
	{
		SP10281_OvfCnt = 0;		// Stop overflow counter
		SP10281_OvfFlag = 1;
	}
	
	/* Overflow counter for blinking UI LEDs */
	if (++(MCP_Output::AutoToggle_OvfCnt) > MCP_Output::AutoToggle_Timeout)
	{
		MCP_Output::AutoToggle_OvfCnt = 0;		// Reset overflow counter
		
		if (MCP_Output::AutoToggle_PortA || MCP_Output::AutoToggle_PortB)
		{
			MCP_Output::AutoToggle_Flag = 1;
		}
	}
	
	/* Overflow counter for rotary encoder handling */
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
				RotEnc_State = IDLE;
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