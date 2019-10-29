#include <avr/io.h>
#include <avr/interrupt.h>
#include "Footswitch.h"
#include "main.h"
#include "MCP23017.h"
#include "MasterCom.h"
#include "Segment_7.h"

void Footswitch_Init()
{
	Footswitch_MCP_PortState = MCP23017_ReadReg(SWITCH_INDICATOR_ADDR, INTCAPB);	// Read state of Port when interrupt occurred (Clear interrupt B)
	
	DDRC &= ~(1 << PORTC1);		// Set PortC 1 as input
	PCMSK1 |= (1 << PCINT9);	// Enable PCINT[9] PortC1 for interrupt
	PCICR |= (1 << PCIE1);		// Enable Pin Change Interrupt 1 (PCINT[14:8])
	
	Footswitch_EnableInterrupt();
	
	Footswitch_OvfCnt = 0;
	Footswitch_PressFlag = 0;
	Footswitch_TimerFlag = 0;
	Footswitch_PressState = IDLE;
	Footswitch_MCP_PortState = 0;
	Footswitch_MCP_IntMask = 0;
}

void Footswitch_HandlePress()
{
	unsigned char tmp = 0;
	
	tmp = MCP23017_ReadReg(SWITCH_INDICATOR_ADDR, INTFB);		// Read what pin caused the interrupt
	
	Footswitch_MCP_PortState = MCP23017_ReadReg(SWITCH_INDICATOR_ADDR, INTCAPB);	// Read state of Port when interrupt occurred (Clear interrupt B)
	
	if (Footswitch_MCP_PortState != 0x00)		// Only react on rising-edge
	{
		Footswitch_MCP_IntMask = tmp;
		Footswitch_PressState = PRESS_SENSED;
		Footswitch_OvfCnt = 1;		// Start Timer
		
		MCP23017_WriteReg(TUNER_DISPLAY_ADDR, OLATA, Footswitch_MCP_IntMask);
	}
	else
	{
		Footswitch_EnableInterrupt();
	}
}

void Footswitch_HandleTimer()
{
	Footswitch_MCP_PortState = MCP23017_ReadReg(SWITCH_INDICATOR_ADDR, GPIOB);		// Read current state of Port
	
	if (Footswitch_PressState == WAITING)
	{
		/* Check if switch is no longer pressed */
		if (Footswitch_MCP_PortState != Footswitch_MCP_IntMask)
		{
			Footswitch_PressState = SHORT_PRESS;
			Footswitch_OvfCnt = 0;	// Stop timer
		}
	}
	else if (Footswitch_PressState == PRESSED)
	{
		/* Check if switch is no longer pressed */
		if (Footswitch_MCP_PortState != Footswitch_MCP_IntMask)
		{
			Footswitch_OvfCnt = 0;	// Stop timer
			Footswitch_PressState = ABORTED;
		}
		else
		{
			Segment_7_WriteAll('w', 'a', 'i', 't', 0, 0, 0, 0);
		}
	}
	else if (Footswitch_PressState == STILL_PRESSED)
	{
		/* Check if switch is no longer pressed */
		if (Footswitch_MCP_PortState != Footswitch_MCP_IntMask)
		{
			Footswitch_PressState = LONG_PRESS;
		}
		else
		{
			Footswitch_PressState = LONG_LONG_PRESS;
		}
		
		Footswitch_OvfCnt = 0;	// Stop timer
	}
}

void Footswitch_EnableInterrupt()
{
	PCIFR |= (1 << PCIF1);		// Clear Pin Change Interrupt Flag 1
	PCICR |= (1 << PCIE1);		// Enable Pin Change Interrupt 1 (PCINT[14:8])
}

ISR (PCINT1_vect)	// Pin Change Interrupt 1 (PCINT1) Service Routine
{
	if (!(PINC & (1 << PINC1)))		// Only react on falling-edge
	{
		PCICR &= ~(1 << PCIE1);		// Disable Pin Change Interrupt 1 (PCINT[14:8])
		Footswitch_PressFlag = 1;
	}
}