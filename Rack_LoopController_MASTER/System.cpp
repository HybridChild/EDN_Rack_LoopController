/*
 * System.cpp
 *
 * Created: 03-02-2020 20:56:34
 *  Author: Esben
 */ 

#include "System.h"
#include "RotaryEncoder.h"

volatile extern System_State SystemState = BOOT_UP;

void System_Init()
{
	
}

void System_Run()
{
	switch (SystemState)
	{
		case BOOT_UP:
		break;
		
		case EDIT_MIDI_CHANNEL:
		break;
		
		case INITIALIZE_SYSTEM:
		break;
		
		case ENTER_RUN_PRESET_CTRL:

		case RUN_PRESET_CTRL:
		break;
		
		case ENTER_RUN_LOOP_CTRL:
		
		case RUN_LOOP_CTRL:
		break;
		
		case ENTER_TUNER:
		
		case TUNER:
		break;
		
		case ENTER_EDIT_UI_MODE:
		
		case EDIT_UI_MODE:
		break;
		
		case ENTER_EDIT_LOOPS_SW:
		
		case EDIT_LOOPS_SW:
		
		case ENTER_EDIT_MIDI_PC:
		
		case EDIT_MIDI_PC:
		break;
		
		case EDIT_MIDI_IN_OUT:
		break;
		
		default:
		break;
	}
}