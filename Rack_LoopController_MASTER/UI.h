/*
 * UI.h
 *
 * Created: 19-01-2020 17:23:13
 *  Author: Esben
 */ 

#ifndef UI_H_
#define UI_H_

#include "MCP23017.h"

#define MCP23017_UI_LEDS_ADDR	0b000

#define UI_MODE_LEDS_OFF	3

extern MCP_Output ModeRun_LED;
extern MCP_Output ModePreset_LED;
extern MCP_Output ModeMIDI_LED;
extern MCP_Output LoopPreset_LEDs[8];
extern MCP_Output SW_Ctrl_LEDs[4];

void UI_Init();

#endif /* UI_H_ */