
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "MasterCom.h"
#include "Footswitch.h"


void Timer0_Init()
{
	/* Setup Timer */
	TCCR0A |= (1 << WGM01);		// Set Timer to CTC mode (Clear Timer on Compare match)
	TCNT0 = 0;					// Clear counter register
	OCR0A = 124;				// Set Output Compare value (62,5kHz / 125 = 500Hz (2ms))
	TIMSK0 |= (1 << OCIE0A);	// Output Compare A Match Interrupt Enable
	
	/* Start Timer */
	TCCR0B |= (1 << CS02);		// Set clock prescaler to 256 (16MHz / 256 = 62,5kHz)
}

uint32_t Timer0_ms2cnt(uint32_t ms)
{
	ms *= 1000;
	return (uint32_t)(ms / TIMER0_PERIOD_US);
}

/* Timer0 Compare A Match Interrupt Service Routine */
ISR(TIMER0_COMPA_vect)
{
	/* Overflow counter for delaying transmit to Master */
	if (MasterCom_DelayTXOvfCnt)
	{
		if (++MasterCom_DelayTXOvfCnt > MASTERCOM_DELAY_TX_TIMEOUT)
		{
			MasterCom_DelayTXOvfCnt = 0;	// Stop overflow counter
			MasterCom_DelayTxFlag = true;
		}
	}

	/* Overflow counter for Master response timeout */
	if (MasterCom_ResponseTimeoutOvfCnt)
	{
		if (++MasterCom_ResponseTimeoutOvfCnt > MASTERCOM_RESPONSE_TIMEOUT)
		{
			MasterCom_ResponseTimeoutOvfCnt = 1;	// Reset overflow counter
			MasterCom_ResponseTimeoutFlag = true;
		}
	}
	
	/* Overflow counter for receiving full command frame */
	if (MasterCom_FullFrameTimeoutOvfCnt)
	{
		if (++MasterCom_FullFrameTimeoutOvfCnt > MASTERCOM_FULL_FRAME_TIMEOUT)
		{
			MasterCom_FullFrameTimeoutOvfCnt = 0;	// Stop counter
			MasterCom_FullFrameTimeoutFlag = true;
		}
	}
	
	/* Overflow counter for handling Footswitch press timing */
	if (Footswitch_TimerOvfCnt)
	{
		Footswitch_TimerOvfCnt++;
			
		if (Footswitch_TimerOvfCnt > FOOTSWITCH_LONG_LONG_PRESS_TIMEOUT && Footswitch_PressState == PRESSED)
		{
			Footswitch_PressState = STILL_PRESSED;
			Footswitch_TimerFlag = true;
		}
		else if (Footswitch_TimerOvfCnt > FOOTSWITCH_LONG_PRESS_TIMEOUT  && Footswitch_PressState == PRESS_SENSED)
		{
			Footswitch_PressState = PRESSED;
			Footswitch_TimerFlag = true;
		}
	}
}
