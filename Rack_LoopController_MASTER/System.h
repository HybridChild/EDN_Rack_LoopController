/*
 * System.h
 *
 * Created: 03-02-2020 20:56:21
 *  Author: Esben
 */ 

#ifndef SYSTEM_H_
#define SYSTEM_H_

typedef enum System_State
{
	BOOT_UP,
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
	ENTER_EDIT_LOOPS_SW,
	EDIT_LOOPS_SW,
	ENTER_EDIT_MIDI_PC,
	EDIT_MIDI_PC,
	EDIT_MIDI_IN_OUT
} System_State;

typedef enum System_UI_Mode
{
	MODE_RUN			= 0,
	MODE_EDIT_PRESET	= 1,
	MODE_EDIT_MIDI		= 2
} System_UI_Mode;

volatile extern System_State SystemState;

void System_Init();
void System_Run();


#endif /* SYSTEM_H_ */