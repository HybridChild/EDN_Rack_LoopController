#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
#include "i2cmaster.h"
#include "MCP23017.h"
#include "UART.h"
#include "Timer.h"
#include "MasterCom.h"
#include "Footswitch.h"
#include "Segment_7.h"

#define MASTER_UART_BAUDRATE	19200

void Init_IO_Expander();

int main(void)
{
	DDRB |= (1 << PORTB0);	// Setup output pin for Tuner display green LED (PortB 0)
	
	i2c_init();
	UART_Init(MASTER_UART_BAUDRATE, UART_2_STOP_BITS, UART_EVEN_PARITY);
	Timer0_Init();
	
	MasterCom_Init();
	Init_IO_Expander();
	Footswitch_Init();
	
	sei();	// Global interrupt enable
	
	Segment_7_WriteAll('r', 'i', 'f', 'f', 0, 0, 0, 0);
	MCP23017_WriteReg(TUNER_DISPLAY_ADDR, OLATA, 0x00);
	
	StateOfSystem = PRESET_CTRL;
	
	/* Main loop */
    while (1)
    {
		/* Handle incoming data from Master */
		if (UART_RX_Flag)
		{
			UART_RX_Flag = 0;
			MasterCom_Receive();
		}
		
		/* Handle received command from Master */
		if (MasterCom_CommandReceivedFlag)
		{
			MasterCom_CommandReceivedFlag = 0;
			MasterCom_HandleReceived();
		}
		
		if (MasterCom_TimeoutFlag)
		{
			MasterCom_TimeoutFlag = 0;
			MasterCom_OvfCnt = 0;	// Stop timeout timer
			MasterCom_Retransmit();
		}
		
		/* Handle footswitch press */
		if (Footswitch_PressFlag)
		{
			Footswitch_PressFlag = 0;
			Footswitch_HandlePress();
		}
		
		/* Handle footswitch timing */
		if (Footswitch_TimerFlag)
		{
			Footswitch_TimerFlag = 0;
			Footswitch_HandleTimer();
		}
		
		if (Footswitch_PressState == SHORT_PRESS ||
			Footswitch_PressState == LONG_PRESS ||
			Footswitch_PressState == LONG_LONG_PRESS ||
			Footswitch_PressState == ABORTED)
		{
			if (Footswitch_PressState == LONG_PRESS)
			{
				if (StateOfSystem == PRESET_CTRL)
				{
					StateOfSystem = LOOP_CTRL;
					Segment_7_WriteAll('L', 'o', 'o', 'P', 0, 0, 0, 0);
				}
				else if (StateOfSystem == LOOP_CTRL || StateOfSystem == TUNER)
				{
					StateOfSystem = PRESET_CTRL;
					Segment_7_WriteAll('P', 'r', 'E', ' ', 0, 0, 0, 0);
				}
			}
			else if (Footswitch_PressState == LONG_LONG_PRESS)
			{
				StateOfSystem = TUNER;
				Segment_7_WriteAll('t', 'u', 'n', 'r', 0, 0, 0, 0);
			}
			
			if (Footswitch_PressState != ABORTED)
			{
				MasterCom_FootswitchPress(Footswitch_MCP_IntMask, (unsigned char)Footswitch_PressState);
			}
			
			Footswitch_PressState = IDLE;
			Footswitch_EnableInterrupt();
		}
    }
}

void Init_IO_Expander()
{
	/* Footswitch and Indicator		- Addr: 000
		- Port A: Indicator lights	- Outputs
		- Port B: Footswitches		- Inputs */
	MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, IODIRA, 0x00);		// Set Port A to output
	
	MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, IODIRB, 0xFF);		// Set Port B to input
	MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, GPINTENB, 0xFF);	// Enables all pins on Port B for interrupt-on-change event
	MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, INTCONB, 0x00);	// Set Port B to compare to previous value for interrupts
	MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, GPPUB, 0xFF);		// Enable pull up resistors for Port B
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