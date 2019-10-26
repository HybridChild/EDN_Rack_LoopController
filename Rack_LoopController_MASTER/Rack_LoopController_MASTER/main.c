
#define F_CPU 20000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "RotaryEncoder.h"
#include "SP10281_3x7segment.h"
#include "i2cmaster.h"
#include "MCP23017.h"
#include "System.h"
#include "UART.h"
#include "MIDI.h"
#include "PedalCom.h"

#define PEDAL_UART				0
#define MIDI_UART				1
#define PEDAL_UART_BAUDRATE		19200
#define MIDI_UART_BAUDRATE		31250

void Init_IO_Expanders();

int main(void)
{
	Timer0_Init();
	i2c_init();
	UART0_Init(PEDAL_UART_BAUDRATE, UART_2_STOP_BITS, UART_EVEN_PARITY);
	UART1_Init(MIDI_UART_BAUDRATE, UART_1_STOP_BIT, UART_NO_PARITY);
	
	RotaryEncoder_Init();
	SP10281_Init();
	
	sei();
	
	Init_IO_Expanders();
	MIDI_Init();
	PedalCom_Init();
	System_Init();
	
    /* Main loop */
    while (1) 
    {
		/* Service 7-segment display */
		if (SP10281_OvfFlag)
		{
			SP10281_OvfFlag = 0;
			SP10281_Update();
		}
		
		/* Handle incoming MIDI */
		if (UART1_RX_Flag)
		{
			UART1_RX_Flag = 0;
			MIDI_ReceiveIncoming();
		}
			
		/* Handle incoming data from Pedal */
		if (UART0_RX_Flag)
		{
			UART0_RX_Flag = 0;
			PedalCom_Receive();
		}
		
		/* If no response from pedal */
		if (PedalCom_TimeoutFlag)
		{
			PedalCom_TimeoutFlag = 0;
			
			/* Stop timeout Timer */
			PedalCom_OvfCnt = 0;
			
			PedalCom_Retransmit();
		}
		
		System_Run();
    }
	
	return 0;
}

void Init_IO_Expanders()
{
	MCP23017_WriteReg(MCP23017_ADDR_UI_LEDS, IODIRA, 0x00);    // Set Port A to output
	MCP23017_WriteReg(MCP23017_ADDR_UI_LEDS, IODIRB, 0x00);    // Set Port B to output
}