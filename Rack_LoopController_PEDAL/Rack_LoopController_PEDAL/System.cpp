/*
 * System.cpp
 *
 * Created: 12-02-2020 21:35:59
 *  Author: Esben
 */ 

#include <avr/io.h>
#include "System.h"
#include "Segment7.h"
#include "MCP23017.h"

volatile System_State SystemState = EDITING;

void System_HandleFootswitchInput(Footswitch_State state, uint8_t sw)
{
	if (state == SHORT_PRESS)
	{
		MasterCom_QueueCommand(FootswitchShortPress, 1, (uint8_t *)&sw);
	}
	else if (state == LONG_PRESS)
	{
		MasterCom_QueueCommand(FootswitchLongPress, 1, (uint8_t *)&sw);
	}
	else if (state == LONG_LONG_PRESS)
	{
		MasterCom_QueueCommand(FootswitchLongLongPress, 1, (uint8_t *)&sw);
		
		/* Only write Tuner in 7-segment display if Not in tuner mode AND if not Loop Ctrl bottom row footswitch was pressed */
		if ((SystemState != TUNER) && !(SystemState == RUN_LOOP_CTRL && (sw & 0x0F)))
		{
			Segment7_WriteAll('T', 'u', 'n', 'r');
		}
	}
}

unsigned char System_HandleMasterCommand(CMD cmd, uint8_t length, uint8_t *dat)
{
	unsigned char response = ACK_BYTE;
	
	if (cmd == UpdateSystemState && length == 1)
	{
		SystemState = (System_State)dat[0];
	}
	else if (cmd == Update7segments && length == 4)
	{
		Segment7_WriteAll((char)dat[0], (char)dat[1], (char)dat[2], (char)dat[3]);
	}
	else if (cmd == UpdatePresetLoopLEDs && length == 1)
	{
		MCP23017_WriteReg(MCP23017_ADDR_SWITCH_INDICATOR, OLATA, dat[0]);
	}
	else if (cmd == UpdateTunerLEDs && length == 2)
	{
		MCP23017_WriteReg(MCP23017_ADDR_TUNER_DISPLAY, OLATA, dat[0]);
				
		if (dat[1])
		{
			TUNER_GREEN_LED_PORT |= TUNER_GREEN_LED_MASK;
		}
		else
		{
			TUNER_GREEN_LED_PORT &= ~TUNER_GREEN_LED_MASK;
		}
	}
	else
	{
		response = NACK_BYTE;
	}
	
	return response;
}
