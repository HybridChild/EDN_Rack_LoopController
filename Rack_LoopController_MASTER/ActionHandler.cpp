/*
 * ActionHandler.cpp
 *
 * Created: 03-02-2020 19:13:51
 *  Author: Esben
 */ 

#include <stdint-gcc.h>
#include "ActionHandler.h"
#include "RotaryEncoder.h"
#include "UI.h"
#include "SP10281_3x7segment.h"

void Handle_MIDIProgramChange()
{
	// Do something with MIDI_ReceivedProgram
}

unsigned char Handle_PedalCommand(CMD cmd, uint8_t length, uint8_t *dat)
{
	unsigned char response = ACK_BYTE;
	
	/* Handle command */
	switch (cmd)
	{
		case FootswitchShortPress:
			SP10281_WriteNumber(1);
			break;
		case FootswitchLongPress:
			SP10281_WriteNumber(2);
			break;
		case FootswitchLongLongPress:
			SP10281_WriteNumber(3);
			break;
		default:
			response = NACK_BYTE;
			break;
	}
	
	return response;
}


void Handle_RotaryEncoderAction()
{
	volatile static uint8_t rotCnt = 0;
	
	switch (RotEnc_State)
	{
		case ROT_LEFT:
			if (rotCnt < 7)
			{
				LoopPreset_LEDs[rotCnt].Clear();
				rotCnt++;
				LoopPreset_LEDs[rotCnt].Set();
			}
			break;
		case ROT_RIGHT:
			if (rotCnt > 0)
			{
				LoopPreset_LEDs[rotCnt].Clear();
				rotCnt--;
				LoopPreset_LEDs[rotCnt].Set();
			}
			break;
		case SHORT_PRESS:
			SP10281_WriteChar('s', 'h', 'r', 0, 0, 0);
			break;
		case LONG_PRESS:
			SP10281_WriteChar('l', 'o', 'n', 0, 0, 0);
			break;
		default:
			break;
	}
	
	RotEnc_State = IDLE;
	RotaryEncoder_EnableInterrupt();}