/*
 * Rack_LoopController_PEDAL.cpp
 *
 * Created: 09-02-2020 14:04:05
 * Author : Esben
 */ 

#define F_CPU 16000000UL

/* Includes */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint-gcc.h>
#include "Timer.h"
#include "i2cmaster.h"
#include "UART.h"
#include "MasterCom.h"
#include "MCP23017.h"
#include "Segment7.h"
#include "Footswitch.h"
#include "System.h"

/* Function prototypes */
void Init_ioExpanders();

/* Main application */
int main(void)
{
	/* Initialization */
	TUNER_GREEN_LED_DDR |= TUNER_GREEN_LED_MASK;	// Setup output pin for Tuner display green LED (PortB 0)
	Timer0_Init();
	i2c_init();
	UART_Init(MASTER_UART_BAUDRATE, UART_2_STOP_BITS, UART_NO_PARITY);
	MasterCom_Init();
	
	Init_ioExpanders();
	Footswitch_Init();	// Should come after initialization of io-expanders
	
	/* Global Enable Interrupt */
	sei();
	
	Segment7_WriteAll('r', 'i', 'f', 'f', 0, 0, 0, 0);
	
    /* Main loop */
    while (1) 
    {
		/* Handle incoming data from Master */
		while (UART_Available())
		{
			MasterCom_Receive();
		}
				
		/* Handle command received from Master. (Only once per program loop) */
		if (MasterCom_RxAvailable())
		{
			MasterCom_HandleReceived();
		}
				
		/* Handle TX Command queue */
		if (MasterCom_TxAvailable())
		{
			MasterCom_PutCommand();
		}
				
		/* Transmit data to Master */
		if (MasterCom_DelayTxFlag)
		{
			MasterCom_DelayTxFlag = false;
			
			if (!UART_QueueIsEmpty())
			{
				MasterCom_Transmit();
			}
		}
		
		/* Retransmit command if no response was received */
		if (MasterCom_ResponseTimeoutFlag)
		{
			MasterCom_ResponseTimeoutFlag = false;
			MasterCom_PrepareRetransmit();
		}
		
		/* Send NACK if full command frame was not received before timeout */
		if (MasterCom_FullFrameTimeoutFlag)
		{
			MasterCom_FullFrameTimeoutFlag = false;
			UART_QueueChar(NACK_BYTE);
			MasterCom_DelayTxFlag = true;
		}
		
		/* Footswitch press detected */
		if (Footswitch_PressFlag)
		{
			Footswitch_PressFlag = 0;
			Footswitch_PressDetected();
		}
				
		/* Handle Footswitch timing */
		if (Footswitch_TimerFlag)
		{
			Footswitch_TimerFlag = false;
			Footswitch_HandleTimer();
		}
		
		/* Handle Footswitch user input (press) */
		if (Footswitch_PressState == SHORT_PRESS ||
			Footswitch_PressState == LONG_PRESS ||
			Footswitch_PressState == LONG_LONG_PRESS)
		{			
			Footswitch_HandlePress();
			
			/* Clear and reload for new user input */
			Footswitch_PressState = IDLE;
			Footswitch_EnableInterrupt();
		}
    }
}

void Init_ioExpanders()
{
	/* Footswitch and Indicator		- Addr: 000
		- Port A: Indicator lights	- Outputs
		- Port B: Footswitches		- Inputs */
	MCP23017_WriteReg(MCP23017_ADDR_SWITCH_INDICATOR, IODIRA, 0x00);	// Set Port A to output
	
	MCP23017_WriteReg(MCP23017_ADDR_SWITCH_INDICATOR, IODIRB, 0xFF);	// Set Port B to input
	MCP23017_WriteReg(MCP23017_ADDR_SWITCH_INDICATOR, GPINTENB, 0xFF);	// Enables all pins on Port B for interrupt-on-change event
	MCP23017_WriteReg(MCP23017_ADDR_SWITCH_INDICATOR, INTCONB, 0x00);	// Set Port B to compare to previous value for interrupts
	MCP23017_WriteReg(MCP23017_ADDR_SWITCH_INDICATOR, GPPUB, 0xFF);		// Enable pull up resistors for Port B
	MCP23017_ReadReg(MCP23017_ADDR_SWITCH_INDICATOR, INTCAPB);			// Clear interrupt B
	
	/* 7-segment digit 0 & 1		- Addr: 010
		- Port A: Digit 0			- Outputs
		- Port B: Digit 1			- Outputs */
	MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_0_1, IODIRA, 0x00);		// Set Port A to output
	MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_0_1, IODIRB, 0x00);		// Set Port B to output
	
	/* 7-segment digit 2 & 3		- Addr: 001
		- Port A: Digit 2			- Outputs
		- Port B: Digit 3			- Inputs */
	MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_2_3, IODIRA, 0x00);		// Set Port A to output
	MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_2_3, IODIRB, 0x00);		// Set Port B to output
	
	/* Tuner display				- Addr: 100
		- Port A: Red lights		- Outputs
		- Port B: Unused			- --- */
	MCP23017_WriteReg(MCP23017_ADDR_TUNER_DISPLAY, IODIRA, 0x00);    // Set Port A to output
}
