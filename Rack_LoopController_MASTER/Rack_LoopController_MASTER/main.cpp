/*
 * Cpp_Test.cpp
 *
 * Created: 19-01-2020 17:20:54
 * Author : Esben
 */ 

#define F_CPU 20000000UL

/* Includes */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint-gcc.h>
#include "Timer.h"
#include "i2cmaster.h"
#include "UART.h"
#include "RotaryEncoder.h"
#include "SP10281_3x7segment.h"
#include "MCP23017.h"
#include "UI.h"
#include "PedalCom.h"
#include "MIDI.h"
#include "System.h"

/* Main application */
int main(void)
{
	/* Initialization */
	Timer0_Init();
	i2c_init();
	UART0_Init(PEDAL_UART_BAUDRATE, UART_2_STOP_BITS, UART_NO_PARITY);
	UART1_Init(MIDI_UART_BAUDRATE, UART_1_STOP_BIT, UART_NO_PARITY);
	PedalCom_Init();
	
	RotaryEncoder_Init();
	SP10281_Init();
	UI_Init();
	
	/* Global Enable Interrupt */
	sei();
	
	/* Main loop */
    while (1) 
    {
		/* Handle incoming MIDI */
		if (UART1_Available())
		{
			MIDI_ReceiveIncoming();
		}
		
		if (MIDI_ProgramChangeFlag)
		{
			MIDI_ProgramChangeFlag = false;
			System_HandleMIDIProgramChange();
		}
		
		/* Handle incoming data from Pedal */
		while (UART0_Available())
		{
			PedalCom_Receive();
		}
		
		/* Handle command received from Pedal. (Only once per program loop) */
		if (PedalCom_RxAvailable())
		{
			PedalCom_HandleReceived();
		}
		
		/* Handle TX Command queue */
		if (PedalCom_TxAvailable())
		{
			PedalCom_PutCommand();
		}
		
		/* Transmit data to Pedal */
		if (PedalCom_DelayTxFlag)
		{
			PedalCom_DelayTxFlag = false;
			PedalCom_Transmit();
		}
		
		/* Send current System State to Pedal (Heartbeat) */
		if (PedalCom_ResponseTimeoutFlag)
		{
			PedalCom_ResponseTimeoutFlag = false;
			
			/* If last command did not get an ACK */
			if (PedalCom_TxAvailable())
			{
				PedalCom_ConnectionOpen = false;
				PedalCom_FlushTxQueue();
			}
			else
			{
				PedalCom_QueueCommand(UpdateSystemState, 1, (uint8_t *)SystemState);
			}
		}
		
		/* Service 7-segment display */
		if (SP10281_OvfFlag)
		{
			SP10281_OvfFlag = 0;
			SP10281_Update();
		}
		
		/* Blink UI LEDs */
		if (MCP_Output::AutoToggle_Flag)
		{
			MCP_Output::AutoToggle_Flag = 0;
			MCP_Output::PerformAutoToggle(MCP23017_UI_LEDS_ADDR);
		}
		
		/* Handle input from rotary encoder */
		if (RotEnc_State != IDLE && RotEnc_State != PRESSED)
		{
			System_HandleRotaryEncoderInput();
			
			RotEnc_State = IDLE;
			RotaryEncoder_EnableInterrupt();
		}
		
		/* Handle temp select timeout condition */
		if (System_TempSelectFlag)
		{
			System_TempSelectFlag = false;
			System_HandleTempSelectTimeout();
		}
		
		/* Reset UI LEDs after selection mark */
		if (System_MarkSelectionFlag)
		{
			System_MarkSelectionFlag = false;
			System_HandleMarkSelection();
		}
		
		/* The grand state machine */
		System_Run();
    }
}
