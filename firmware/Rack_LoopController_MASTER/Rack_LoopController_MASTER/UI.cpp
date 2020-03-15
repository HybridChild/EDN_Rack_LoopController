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

MCP_Output ModeRunLED;
MCP_Output ModeEditPresetLED;
MCP_Output ModeEditMidiLED;
MCP_Output LoopLEDs[NUMBER_OF_LOOPS];
MCP_Output CtrlSwitchLEDs[NUMBER_OF_CTRL_SWITCHES];

MCP_Output LoopRelays[NUMBER_OF_LOOPS];
MCP_Output CtrlSwitchRelays[NUMBER_OF_CTRL_SWITCHES];

void UI_Init()
{
	/* Initialize outputs UI LEDs */
	ModeRunLED = MCP_Output(MCP23017_UI_LEDS_ADDR, PortB, (MCP23017_Pin)UI_MODE_RUN_LED_PIN);
	ModeEditPresetLED = MCP_Output(MCP23017_UI_LEDS_ADDR, PortB, (MCP23017_Pin)UI_MODE_PRESET_LED_PIN);
	ModeEditMidiLED = MCP_Output(MCP23017_UI_LEDS_ADDR, PortB, (MCP23017_Pin)UI_MODE_MIDI_LED_PIN);
	
	for (uint8_t i = 0; i < NUMBER_OF_LOOPS; i++)
	{
		LoopLEDs[i] = MCP_Output(MCP23017_UI_LEDS_ADDR, PortA, (MCP23017_Pin)i);
	}
		
	for (uint8_t i = 0; i < NUMBER_OF_CTRL_SWITCHES; i++)
	{
		CtrlSwitchLEDs[i] = MCP_Output(MCP23017_UI_LEDS_ADDR, PortB, (MCP23017_Pin)(i));
	}
		
	///* Initialize output relays */
	//for (uint8_t i = 0; i < NUMBER_OF_LOOPS; i++)
	//{
		//LoopRelays[i] = MCP_Output(MCP23017_RELAYS_ADDR, PortA, (MCP23017_Pin)i);
	//}
	//
	//for (uint8_t i = 0; i < NUMBER_OF_CTRL_SWITCHES; i++)
	//{
		//CtrlSwitchRelays[i] = MCP_Output(MCP23017_RELAYS_ADDR, PortB, (MCP23017_Pin)i);
	//}
	
	/* Set auto toggle speed */
	MCP_Output::SetAutoToggleSpeed(360/*ms*/);
}
