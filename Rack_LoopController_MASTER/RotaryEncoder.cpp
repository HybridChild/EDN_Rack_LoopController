/*
 * CPPFile1.cpp
 *
 * Created: 26-01-2020 20:31:31
 *  Author: Esben
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "RotaryEncoder.h"

volatile unsigned short RotaryEncoder_OvfCnt = 0;
volatile enum RotaryEncoder_State RotEnc_State = IDLE;

void RotaryEncoder_Init()
{
	/* Setup input GPIO pins */
	GPIO_ROT_ENC_DDR &= ~(GPIO_ROT_ENC_PRESS_MASK | GPIO_ROT_ENC_INDICATOR_MASK | GPIO_ROT_ENC_TRIGGER_MASK);
	
	/* Setup Pin Change interrupt */
	PCMSK1 |= (1 << PCINT8_BIT);	// Enable Pin Change interrupt for Pin 1 (PCINT8)
	PCICR |= (1 << PCIE1);			// Enable Pin Change interrupt PCINT[15:8]
	
	/* Setup Interrupt Sense (INT2) */
	EICRA |= ((1 << ISC21) | (1 << ISC20));		// Rising edge trigger for INT2
	EIMSK |= (1 << INT2);						// Enable External Interrupt Request (INT2)
}

void RotaryEncoder_EnableInterrupt()
{
	/* Clear and reenable interrupt */
	EIFR |= (1 << INTF2);		// Clear External interrupt (INT2) Flag
	PCIFR |= (1 << PCIF1);		// Clear Pin Change interrupt Flag
	EIMSK |= (1 << INT2);		// Enable External Interrupt Request (INT2)
	PCICR |= (1 << PCIE1);		// Enable Pin Change interrupt (PCI1) PCINT[15:8]
}


/* Pin Change Interrupt Request 1 Service Routine */
/* To handle Rotary Encoder Rotation */
ISR(PCINT1_vect)
{
	if (GPIO_ROT_ENC_PIN & GPIO_ROT_ENC_TRIGGER_MASK)	// Only react on rising edge
	{
		/* Disable External interrupts */
		EIMSK &= ~(1 << INT2);		// Disable External Interrupt Request (INT2)
		PCICR &= ~(1 << PCIE1);		// Disable Pin Change interrupt (PCI1) PCINT[15:8]
		
		/* Determine Left/Right turn */
		if (GPIO_ROT_ENC_PIN & GPIO_ROT_ENC_INDICATOR_MASK)
			RotEnc_State = ROT_RIGHT;
		else
			RotEnc_State = ROT_LEFT;
	}
}

/* External Interrupt Request 2 Service routine */
/* To handle Rotary Encoder Press */
ISR(INT2_vect)
{
	RotaryEncoder_OvfCnt = 1;		// Start overflow counter
	
	/* Disable External interrupts */
	EIMSK &= ~(1 << INT2);		// Disable External Interrupt Request (INT2)
	PCICR &= ~(1 << PCIE1);		// Disable Pin Change interrupt (PCI1) PCINT[15:8]
}