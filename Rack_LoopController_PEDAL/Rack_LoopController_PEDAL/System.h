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

#define TUNER_GREEN_LED_DDR				DDRB
#define TUNER_GREEN_LED_PORT			PORTB
#define TUNER_GREEN_LED_PORT_IDX		PORTB0
#define TUNER_GREEN_LED_MASK			(1 << TUNER_GREEN_LED_PORT_IDX)

#define MCP23017_ADDR_SWITCH_INDICATOR	0b000
#define MCP23017_ADDR_7SEG_DIGIT_0_1	0b001
#define MCP23017_ADDR_7SEG_DIGIT_2_3	0b010
#define MCP23017_ADDR_TUNER_DISPLAY		0b100

typedef enum System_State
{
	EDITING			= 0,
	RUN_PRESET_CTRL	= 1,
	RUN_LOOP_CTRL	= 2,
	TUNER			= 3
} System_State;

/* Global external variables */
volatile extern System_State SystemState;

/* Prototypes for global external functions */
void System_HandleFootswitchInput(Footswitch_State state, uint8_t sw);
unsigned char System_HandleMasterCommand(CMD cmd, uint8_t length, uint8_t *dat);


#endif /* SYSTEM_H_ */