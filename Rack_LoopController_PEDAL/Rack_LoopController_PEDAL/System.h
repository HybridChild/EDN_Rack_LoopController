/*
 * System.h
 *
 * Created: 12-02-2020 19:06:02
 *  Author: Esben
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint-gcc.h>
#include "Footswitch.h"
#include "MasterCom.h"

#define MCP23017_ADDR_SWITCH_INDICATOR	0b000
#define MCP23017_ADDR_7SEG_DIGIT_0_1	0b001
#define MCP23017_ADDR_7SEG_DIGIT_2_3	0b010
#define MCP23017_ADDR_TUNER_DISPLAY		0b100

void System_HandleFootswitchInput(Footswitch_State state, uint8_t sw);
unsigned char System_HandleMasterCommand(CMD cmd, uint8_t length, uint8_t *dat);


#endif /* SYSTEM_H_ */