/*
 * UI.cpp
 *
 * Created: 19-01-2020 17:23:25
 *  Author: Esben
 */ 

#include <avr/io.h>
#include <stdint-gcc.h>
#include "MCP23017.h"
#include "UI.h"
#include "util.h"

#define UI_MODE_RUN_LED_PIN			5
#define UI_MODE_PRESET_LED_PIN		6
#define UI_MODE_MIDI_LED_PIN		7

MCP_Output ModeRun_LED;
MCP_Output ModePreset_LED;
MCP_Output ModeMIDI_LED;
MCP_Output LoopPreset_LEDs[8];
MCP_Output SW_Ctrl_LEDs[4];

void UI_Init()
{
	ModeRun_LED = MCP_Output(MCP23017_UI_LEDS_ADDR, PortB, (MCP23017_Pin)UI_MODE_RUN_LED_PIN);
	ModePreset_LED = MCP_Output(MCP23017_UI_LEDS_ADDR, PortB, (MCP23017_Pin)UI_MODE_PRESET_LED_PIN);
	ModeMIDI_LED = MCP_Output(MCP23017_UI_LEDS_ADDR, PortB, (MCP23017_Pin)UI_MODE_MIDI_LED_PIN);
	
	for (uint8_t i = 0; i < NELEMS(LoopPreset_LEDs); i++)
		LoopPreset_LEDs[i] = MCP_Output(MCP23017_UI_LEDS_ADDR, PortA, (MCP23017_Pin)i);
		
	for (uint8_t i = 0; i < NELEMS(SW_Ctrl_LEDs); i++)
		SW_Ctrl_LEDs[i] = MCP_Output(MCP23017_UI_LEDS_ADDR, PortB, (MCP23017_Pin)(i));
		
	MCP_Output::SetAutoToggleSpeed(400);
}
