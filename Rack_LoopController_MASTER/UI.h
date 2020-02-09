/*
 * UI.h
 *
 * Created: 19-01-2020 17:23:13
 *  Author: Esben
 */ 

#ifndef UI_H_
#define UI_H_

#include "MCP23017.h"
#include "System.h"

#define MCP23017_UI_LEDS_ADDR	0b000
#define MCP23017_RELAYS_ADDR	0b111

#define UI_MODE_RUN_LED_PIN			5
#define UI_MODE_PRESET_LED_PIN		6
#define UI_MODE_MIDI_LED_PIN		7

extern MCP_Output ModeRunLED;
extern MCP_Output ModeEditPresetLED;
extern MCP_Output ModeEditMidiLED;
extern MCP_Output LoopLEDs[NUMBER_OF_LOOPS];
extern MCP_Output CtrlSwitchLEDs[NUMBER_OF_CTRL_SWITCHES];

extern MCP_Output LoopRelays[NUMBER_OF_LOOPS];
extern MCP_Output CtrlSwitchRelays[NUMBER_OF_CTRL_SWITCHES];

void UI_Init();

#endif /* UI_H_ */