/*
 * System.h
 *
 * Created: 03-02-2020 20:56:21
 *  Author: Esben
 */ 

#ifndef SYSTEM_H_
#define SYSTEM_H_

/* Includes */
#include "PedalCom.h"
#include "MIDI.h"

/* Defines */
#define SYSTEM_TEMP_SELECT_TIMEOUT	1500	// 1500*2ms = 3 seconds
#define SYSTEM_MARK_SELECTION_TIMEOUT	150	// 150*2ms = 300ms

#define EEPROM_MIDI_CH_ADDR			  0
#define EEPROM_PRESETS_BASE_ADDR	100

#define NUMBER_OF_PRESETS			8
#define NUMBER_OF_LOOPS				8
#define NUMBER_OF_CTRL_SWITCHES		4

/* Enumerations */
typedef enum System_State
{
	BOOT_UP = 0,
	EDIT_MIDI_CHANNEL,
	INITIALIZE_SYSTEM,
	ENTER_RUN_PRESET_CTRL,
	RUN_PRESET_CTRL,
	ENTER_RUN_LOOP_CTRL,
	RUN_LOOP_CTRL,
	ENTER_TUNER,
	TUNER,
	ENTER_EDIT_UI_MODE,
	EDIT_UI_MODE,
	ENTER_EDIT_PRESET,
	EDIT_PRESET,
	ENTER_EDIT_MIDI_IN_OUT,
	EDIT_MIDI_IN_OUT,
	ENTER_EDIT_MIDI_PC,
	EDIT_MIDI_PC
} System_State;

typedef enum System_UI_Mode
{
	MODE_RUN			= 0,
	MODE_EDIT_PRESET	= 1,
	MODE_EDIT_MIDI		= 2
} System_UI_Mode;

typedef enum System_RunMode
{
	RUN_PRESET_MODE,
	RUN_LOOP_MODE
} System_RunMode;

typedef enum System_SetMidiEdit
{
	MIDI_IN,
	MIDI_OUT
} System_SetMidiEdit;

typedef enum System_Preset
{
	PRESET_1 = 0,
	PRESET_2 = 1,
	PRESET_3 = 2,
	PRESET_4 = 3,
	PRESET_5 = 4,
	PRESET_6 = 5,
	PRESET_7 = 6,
	PRESET_8 = 7
} System_Preset;

/* Structures */
typedef struct Preset
{
	uint8_t Active_Loops;
	uint8_t Active_CtrlSwitches;
	uint8_t MIDI_PC_In;
	uint8_t MIDI_PC_Out;
} Preset;

inline bool operator==(const Preset& lhs, const Preset& rhs)
{
	return	(	lhs.Active_Loops == rhs.Active_Loops
			&&	lhs.Active_CtrlSwitches == rhs.Active_CtrlSwitches
			&&	lhs.MIDI_PC_In == rhs.MIDI_PC_In
			&&	lhs.MIDI_PC_Out == rhs.MIDI_PC_Out);
}

inline bool operator!=(const Preset& lhs, const Preset& rhs)
{
	return	(	lhs.Active_Loops != rhs.Active_Loops
			||	lhs.Active_CtrlSwitches != rhs.Active_CtrlSwitches
			||	lhs.MIDI_PC_In != rhs.MIDI_PC_In
			||	lhs.MIDI_PC_Out != rhs.MIDI_PC_Out);
}

/* Global external variables */
volatile extern System_State SystemState;
volatile extern MIDI_Channel System_MidiChannel;
volatile extern uint16_t System_TempSelectOvfCnt;
volatile extern bool System_TempSelectFlag;
volatile extern uint16_t System_MarkSelectionOvfCnt;
volatile extern bool System_MarkSelectionFlag;

/* Prototypes for global external functions */
void System_Run();
void System_HandleRotaryEncoderInput();
void System_HandleTempSelectTimeout();
unsigned char System_HandlePedalCommand(CMD cmd, uint8_t length, uint8_t *dat);
void System_HandleMIDIProgramChange();
void System_HandleMarkSelection();

#endif /* SYSTEM_H_ */