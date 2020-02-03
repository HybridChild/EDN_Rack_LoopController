/*
 * Cpp_Test.cpp
 *
 * Created: 19-01-2020 17:20:54
 * Author : Esben
 */ 

#define F_CPU 20000000UL

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
#include "PedalComm.h"
#include "MIDI.h"
#include "ActionHandler.h"

#define PEDAL_UART				0
#define MIDI_UART				1
#define PEDAL_UART_BAUDRATE		19200
#define MIDI_UART_BAUDRATE		31250

int main(void)
{
	/* Initialization */
	Timer0_Init();
	i2c_init();
	UART0_Init(PEDAL_UART_BAUDRATE, UART_2_STOP_BITS, UART_NO_PARITY);
	UART1_Init(MIDI_UART_BAUDRATE, UART_1_STOP_BIT, UART_NO_PARITY);
	
	RotaryEncoder_Init();
	SP10281_Init();
	UI_Init();
	MIDI_Init();
	
	sei();	// Global Enable Interrupt
	
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
			Handle_MIDIProgramChange();
		}
		
		/* Handle incoming data from Pedal */
		while (UART0_Available())
		{
			PedalComm_Receive();
		}
		
		/* Handle command received from pedal. (Only once per program loop) */
		if (PedalComm_RxAvailable())
		{
			PedalComm_HandleReceived();
		}
		
		/* Transmit command to Pedal if any are queued up */
		if (PedalComm_TxAvailable())
		{
			PedalComm_PutCommand();
		}
		
		/* Transmit data to Pedal */
		if (PedalComm_DelayTxFlag)
		{
			PedalComm_DelayTxFlag = false;
			PedalComm_Transmit();
		}
		
		/* Handle Rotary Encoder input */
		if (RotEnc_State != IDLE && RotEnc_State != PRESSED && RotEnc_State != ABORTED)
		{
			Handle_RotaryEncoderAction();
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
    }
}

