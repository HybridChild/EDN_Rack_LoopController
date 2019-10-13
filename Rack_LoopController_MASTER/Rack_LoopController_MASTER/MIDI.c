
#include <string.h>
#include <avr/eeprom.h>
#include "MIDI.h"
#include "UART.h"
#include "System.h"

void MIDI_Init()
{
	memset((void*)MIDI_Buffer, 0, MIDI_BUF_SIZE);
	MIDI_PC_In = 0;
	MIDI_PC_Flag = 0;
	
	sysMIDI_Channel = eeprom_read_byte((uint8_t*)EEPROM_MIDI_CH_ADDR);
					
	if (sysMIDI_Channel > MIDI_CH_MAX)
	{
		sysMIDI_Channel = MIDI_CH_OMNI;
		eeprom_write_byte((uint8_t*)EEPROM_MIDI_CH_ADDR, sysMIDI_Channel);
	}
}

void MIDI_ReceiveIncoming()
{
	static unsigned char RX_cnt = 0;
	
	MIDI_Buffer[RX_cnt] = UART1_GetChar();
	
	if (RX_cnt == 0)
	{
		if ( (MIDI_Buffer[RX_cnt] & 0xF0) == 0xC0 )
		{
			if ( (sysMIDI_Channel == MIDI_CH_OMNI) || ( (MIDI_Buffer[RX_cnt] & 0x0F) != (sysMIDI_Channel-1) ) )
			{
				RX_cnt++;
			}
		}
	}
	else if (RX_cnt == 1)
	{
		if (MIDI_Buffer[RX_cnt] <= MIDI_PC_MAX)
		{
			MIDI_PC_In = MIDI_Buffer[RX_cnt];
			MIDI_PC_Flag = 1;
		}
		
		RX_cnt = 0;
	}
	else
		RX_cnt = 0;
}

void MIDI_TransmitProgramChange(unsigned char prg)
{
	MIDI_Buffer[0] = 0xC0;
	
	if (sysMIDI_Channel != MIDI_CH_OMNI)
	{
		MIDI_Buffer[0] |= (sysMIDI_Channel-1);
	}
	
	MIDI_Buffer[1] = (char)prg;
	MIDI_Buffer[2] = 0;
	
	UART1_PutString((char*)MIDI_Buffer);
}