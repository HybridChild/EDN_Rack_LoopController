/*
 * Footswitch.cpp
 *
 * Created: 12-02-2020 18:40:20
 *  Author: Esben
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Footswitch.h"
#include "MCP23017.h"
#include "Segment7.h"
#include "System.h"

/* Variable initialization */
volatile Footswitch_State Footswitch_PressState = IDLE;
volatile bool Footswitch_PressFlag = false;
volatile uint8_t Footswitch_PortState = 0;
volatile uint8_t Footswitch_InterruptMask = 0;
volatile uint16_t Footswitch_TimerOvfCnt = 0;
volatile bool Footswitch_TimerFlag = false;

/* Function implementations */
void Footswitch_Init()
{	
	DDRC &= ~(1 << PORTC1);		// Set PortC 1 as input
	PCMSK1 |= (1 << PCINT9);	// Enable PCINT[9] PortC1 for interrupt

	Footswitch_EnableInterrupt();
}

void Footswitch_PressDetected()
{
	uint8_t tmp = 0;
	
	// Read what pin caused the interrupt
	tmp = MCP23017_ReadReg(MCP23017_ADDR_SWITCH_INDICATOR, INTFB);
	
	// Read state of Port when interrupt occurred (Clear interrupt B)
	Footswitch_PortState = MCP23017_ReadReg(MCP23017_ADDR_SWITCH_INDICATOR, INTCAPB);

	if (Footswitch_PortState != 0x00)		// Footswitch pressed
	{
		Footswitch_InterruptMask = tmp;
		Footswitch_PressState = PRESS_SENSED;
		Footswitch_TimerOvfCnt = 1;		// Start Timer
	}
	else									// Footswitch released
	{
		if (Footswitch_PressState == PRESS_SENSED)
		{
			Footswitch_PressState = SHORT_PRESS;
		}
		else if (Footswitch_PressState == PRESSED)
		{
			Footswitch_PressState = LONG_PRESS;
		}

		Footswitch_TimerOvfCnt = 0;	// Stop timer
	}

	Footswitch_EnableInterrupt();
}

void Footswitch_HandleTimer()
{
	/* Assume switch is still pressed */

	if (Footswitch_PressState == PRESSED)
	{
		/* Write appropriate message in 7-segment display based on system state */
		if (SystemState == EDITING)
		{
		}
		else if (SystemState == RUN_PRESET_CTRL)
		{
			Segment7_WriteAll('L', 'o', 'o', 'P', 0, 0, 0, 0);
		}
		else if (SystemState == RUN_LOOP_CTRL)
		{
			if (Footswitch_InterruptMask & 0xF0)
			{
				Segment7_WriteAll('P', 'r', 'e', ' ', 0, 0, 0, 0);
			}
			else
			{
				Segment7_WriteAll('^', '^', '^', '^', 0, 0, 0, 0);
			}
		}
		else if (SystemState == TUNER)
		{
		}
	}
	else if (Footswitch_PressState == STILL_PRESSED)
	{
		Footswitch_PressState = LONG_LONG_PRESS;
		
		Footswitch_TimerOvfCnt = 0;	// Stop timer
	}
}

void Footswitch_HandlePress()
{	
	System_HandleFootswitchInput(Footswitch_PressState, Footswitch_InterruptMask);
}

void Footswitch_EnableInterrupt()
{
	PCIFR |= (1 << PCIF1);		// Clear Pin Change Interrupt Flag 1
	PCICR |= (1 << PCIE1);		// Enable Pin Change Interrupt 1 (PCINT[14:8])
}

/* Pin Change Interrupt 1 (PCINT1) Service Routine */
ISR (PCINT1_vect)
{
	if (!(PINC & (1 << PINC1)))		// Only react on falling-edge
	{
		PCICR &= ~(1 << PCIE1);		// Disable Pin Change Interrupt 1 (PCINT[14:8])
		Footswitch_PressFlag = true;
	}
}
