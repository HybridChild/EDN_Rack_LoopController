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

volatile Footswitch_State Footswitch_PressState = IDLE;
volatile bool Footswitch_PressFlag = false;
volatile uint8_t Footswitch_PortState = 0;
volatile uint8_t Footswitch_InterruptMask = 0;
volatile uint16_t Footswitch_TimerOvfCnt = 0;
volatile bool Footswitch_TimerFlag = false;

void Footswitch_Init()
{
	Footswitch_PortState = MCP23017_ReadReg(MCP23017_ADDR_SWITCH_INDICATOR, INTCAPB);	// Read state of Port when interrupt occurred (Clear interrupt B)
	
	DDRC &= ~(1 << PORTC1);		// Set PortC 1 as input
	PCMSK1 |= (1 << PCINT9);	// Enable PCINT[9] PortC1 for interrupt
	PCICR |= (1 << PCIE1);		// Enable Pin Change Interrupt 1 (PCINT[14:8])
	
	Footswitch_EnableInterrupt();
}

void Footswitch_PressDetected()
{
	uint8_t tmp = 0;
	
	tmp = MCP23017_ReadReg(MCP23017_ADDR_SWITCH_INDICATOR, INTFB);		// Read what pin caused the interrupt
	
	Footswitch_PortState = MCP23017_ReadReg(MCP23017_ADDR_SWITCH_INDICATOR, INTCAPB);	// Read state of Port when interrupt occurred (Clear interrupt B)
	
	if (Footswitch_PortState != 0x00)		// Only react on rising-edge
	{
		Footswitch_InterruptMask = tmp;
		Footswitch_PressState = PRESS_SENSED;
		Footswitch_TimerOvfCnt = 1;		// Start Timer
	}
	else
	{
		Footswitch_EnableInterrupt();
	}	
}

void Footswitch_HandleTimer()
{
	Footswitch_PortState = MCP23017_ReadReg(MCP23017_ADDR_SWITCH_INDICATOR, GPIOB);		// Read current state of Port
	
	if (Footswitch_PressState == WAITING)
	{
		/* Check if switch is no longer pressed */
		if (Footswitch_PortState != Footswitch_InterruptMask)
		{
			Footswitch_PressState = SHORT_PRESS;
			Footswitch_TimerOvfCnt = 0;	// Stop timer
		}
	}
	else if (Footswitch_PressState == PRESSED)
	{
		/* Check if switch is no longer pressed */
		if (Footswitch_PortState != Footswitch_InterruptMask)
		{
			Footswitch_TimerOvfCnt = 0;	// Stop timer
			Footswitch_PressState = SHORT_PRESS;
		}
		else
		{
			/* Write appropriate message in 7-segment display based on system state */
			Segment7_WriteAll('m', 'o', 'd', 'E', 0, 0, 0, 0);
		}
	}
	else if (Footswitch_PressState == STILL_PRESSED)
	{
		/* Check if switch is no longer pressed */
		if (Footswitch_PortState != Footswitch_InterruptMask)
		{
			Footswitch_PressState = LONG_PRESS;
		}
		else
		{
			Footswitch_PressState = LONG_LONG_PRESS;
		}
		
		Footswitch_TimerOvfCnt = 0;	// Stop timer
	}
}

void Footswitch_HandlePress()
{
	/* Transmit Press State and responsible Footswitch index to Master */

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