/*
 * MIDI.cpp
 *
 * Created: 03-02-2020 18:20:33
 *  Author: Esben
 */ 

#include <avr/eeprom.h>
#include "MIDI.h"
#include "UART.h"

volatile uint8_t System_MIDI_Channel = MIDI_CH_OMNI;
volatile char MIDI_RxBuffer[MIDI_BUF_SIZE] = {0};
volatile char MIDI_TxBuffer[MIDI_BUF_SIZE] = {0};
volatile uint8_t MIDI_ReceivedProgram = 0;
volatile bool MIDI_ProgramChangeFlag = false;

void MIDI_Init()
{
	System_MIDI_Channel = eeprom_read_byte((uint8_t*)EEPROM_MIDI_CH_ADDR);
	
	if (System_MIDI_Channel > MIDI_CH_MAX)
	{
		System_MIDI_Channel = MIDI_CH_OMNI;
		eeprom_write_byte((uint8_t*)EEPROM_MIDI_CH_ADDR, System_MIDI_Channel);
	}
}


void MIDI_ReceiveIncoming()
{
	static uint8_t RX_ByteCnt = 0;
	
	MIDI_RxBuffer[RX_ByteCnt] = UART1_GetChar();
	
	if (RX_ByteCnt == 0)
	{
		/* Check if command is a Program Change */
		if ( (MIDI_RxBuffer[RX_ByteCnt] & 0xF0) == 0xC0 )
		{
			/* Check if command is relevant to this device */
			if ( (System_MIDI_Channel == MIDI_CH_OMNI) || ( (MIDI_RxBuffer[RX_ByteCnt] & 0x0F) != (System_MIDI_Channel-1) ) )
			{
				RX_ByteCnt++;
			}
		}
	}
	else if (RX_ByteCnt > 0)
	{
		/* Check if data byte contains a valid MIDI program (is between 0 and 127) */
		if (MIDI_RxBuffer[RX_ByteCnt] <= MIDI_PC_MAX)
		{
			MIDI_ReceivedProgram = MIDI_RxBuffer[RX_ByteCnt];
			MIDI_ProgramChangeFlag = true;
		}
		
		RX_ByteCnt = 0;
	}
}

void MIDI_TransmitProgramChange(uint8_t prg)
{
	MIDI_TxBuffer[0] = 0xC0;
	
	if (System_MIDI_Channel != MIDI_CH_OMNI)
	{
		MIDI_TxBuffer[0] |= (System_MIDI_Channel-1);
	}
	
	MIDI_TxBuffer[1] = (char)prg;
	MIDI_TxBuffer[2] = 0;
	
	UART1_PutString((char*)MIDI_TxBuffer);
}