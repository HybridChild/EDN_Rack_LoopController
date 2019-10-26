#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2cmaster.h"
#include "mcp23017.h"

#define SWITCH_INDICATOR_ADDR	0b000
#define DIGIT_0_1_ADDR			0b010
#define DIGIT_2_3_ADDR			0b001
#define TUNER_DISPLAY_ADDR		0b100

void Init_IO_Expander();

volatile unsigned char Footswitch_IntFlag = 0;

ISR (PCINT1_vect)	// Pin Change Interrupt 1 (PCINT1) Service Routine
{
	Footswitch_IntFlag = 1;
}

int main(void)
{
	DDRB |= (1 << PORTB0);	// Setup output pin for Tuner display green LED (PortB 0)
	
	i2c_init();
	Init_IO_Expander();

	sei();
	
	MCP23017_WriteReg(DIGIT_0_1_ADDR, OLATA, 0xAF);	// 'r'
	MCP23017_WriteReg(DIGIT_0_1_ADDR, OLATB, 0xEF);	// 'i'
	MCP23017_WriteReg(DIGIT_2_3_ADDR, OLATA, 0x8E);	// 'F'
	MCP23017_WriteReg(DIGIT_2_3_ADDR, OLATB, 0x8E);	// 'F'
	
	unsigned char i = 0;
	unsigned char Footswitch_PortState = 0;
	unsigned char Footswitch_IntMask = 0;
	
	/* Main loop */
    while (1)
    {
		if (Footswitch_IntFlag)
		{
			Footswitch_IntFlag = 0;
			Footswitch_IntMask = MCP23017_ReadReg(SWITCH_INDICATOR_ADDR, INTFB);		// Read what pin caused the interrupt
			Footswitch_PortState = MCP23017_ReadReg(SWITCH_INDICATOR_ADDR, INTCAPB);	// Read state of Port when interrupt occurred (Clear interrupt B)
			
			if (Footswitch_PortState != 0x00)	// Only react on Rising-edge
			{
				for (i = 0; i < 8; i++)
				{
					if (Footswitch_IntMask & (1 << i))
					{
						MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, OLATA, (1 << i));	// Set Indicator light
					}
				}
			}
		}

    }
}

void Init_IO_Expander()
{
	DDRC &= ~(1 << PORTC1);		// Set PortC 1 as input
	PCMSK1 |= (1 << PCINT9);	// Enable PCINT[9] PC1 for interrupt
	PCICR |= (1 << PCIE1);		// Pin Change Interrupt Enable (PCIE1) (PCINT[14:8])
	
	/* Footswitch and Indicator		- Addr: 000
		- Port A: Indicator lights	- Outputs
		- Port B: Footswitches		- Inputs */
	MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, IODIRA, 0x00);    // Set Port A to output
	MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, IODIRB, 0xFF);    // Set Port B to input
	MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, GPINTENB, 0xFF);  // Enables all pins on Port B for interrupt-on-change event
	MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, INTCONB, 0x00);   // Set Port B to compare to previous value for interrupts
	MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, GPPUB, 0xFF);     // Enable pull up resistors for Port B
	MCP23017_ReadReg(SWITCH_INDICATOR_ADDR, INTCAPB);			// Clear interrupt B
	
	/* 7-segment digit 0 & 1		- Addr: 010
		- Port A: Digit 0			- Outputs
		- Port B: Digit 1			- Outputs */
	MCP23017_WriteReg(DIGIT_0_1_ADDR, IODIRA, 0x00);		// Set Port A to output
	MCP23017_WriteReg(DIGIT_0_1_ADDR, IODIRB, 0x00);		// Set Port B to output
	
	
	/* 7-segment digit 2 & 3		- Addr: 001
		- Port A: Digit 2			- Outputs
		- Port B: Digit 3			- Inputs */
	MCP23017_WriteReg(DIGIT_2_3_ADDR, IODIRA, 0x00);		// Set Port A to output
	MCP23017_WriteReg(DIGIT_2_3_ADDR, IODIRB, 0x00);		// Set Port B to output
	
	
	/* Tuner display				- Addr: 100
		- Port A: Red lights		- Outputs
		- Port B: Unused			- --- */
	MCP23017_WriteReg(TUNER_DISPLAY_ADDR, IODIRA, 0x00);    // Set Port A to output
}