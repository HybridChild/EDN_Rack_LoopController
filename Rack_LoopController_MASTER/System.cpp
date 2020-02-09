/*
 * System.cpp
 *
 * Created: 03-02-2020 20:56:34
 *  Author: Esben
 */ 

#include <avr/eeprom.h>
#include <math.h>
#include "System.h"
#include "MCP23017.h"
#include "SP10281_3x7segment.h"
#include "RotaryEncoder.h"
#include "PedalComm.h"
#include "UI.h"
#include "util.h"

void System_UpdateUI_LEDs();
void System_UpdateUI_Relays();
void System_ChangePreset();

volatile uint16_t System_TempSelectOvfCnt = 0;
volatile bool System_TempSelectFlag = false;
volatile uint16_t System_MarkSelectionOvfCnt = 0;
volatile bool System_MarkSelectionFlag = false;

volatile System_State SystemState = BOOT_UP;
volatile System_State LastSystemState = BOOT_UP;
volatile System_Preset ActivePreset = PRESET_1;
volatile System_Preset LastActivePreset = PRESET_1;
volatile System_Preset TempSelectPreset = PRESET_1;
volatile System_Preset LastTempSelectPreset = PRESET_1;
volatile System_RunMode SystemRunMode = RUN_PRESET_MODE;
volatile MIDI_Channel TempSelectMidiChannel = MIDI_CH_OMNI;
volatile MIDI_Channel LastTempSelectMidiChannel = MIDI_CH_OMNI;
volatile MIDI_Channel System_MidiChannel = MIDI_CH_OMNI;
volatile System_UI_Mode SystemUIMode = MODE_RUN;
volatile System_UI_Mode LastSystemUIMode = MODE_RUN;
volatile System_SetMidiEdit SetMidiEdit = MIDI_IN;
volatile uint8_t TempSelectMidiPC = 0;
volatile uint8_t LastTempSelectMidiPC = 0;

Preset PresetTable[NUMBER_OF_PRESETS];
Preset LoopModePreset = {0, 0, MIDI_PC_OFF, MIDI_PC_OFF};
Preset LastLoopModePreset = {0, 0, MIDI_PC_OFF, MIDI_PC_OFF};

volatile uint8_t UserEditSelection = 0;
volatile uint8_t LastUserEditSelection = 0;

void System_Run()
{
	switch (SystemState)
	{
		case BOOT_UP:
			/* Fill preset table from EEPROM */
			eeprom_read_block((void*)PresetTable, (void*)EEPROM_PRESETS_BASE_ADDR, sizeof(Preset) * NUMBER_OF_PRESETS);
			
			/* Get device MIDI channel from EEPROM */
			System_MidiChannel = (MIDI_Channel)eeprom_read_byte((uint8_t*)EEPROM_MIDI_CH_ADDR);
			
			if (System_MidiChannel > MIDI_CH_MAX)
			{
				System_MidiChannel = MIDI_CH_OMNI;
				eeprom_write_byte((uint8_t*)EEPROM_MIDI_CH_ADDR, System_MidiChannel);
			}
			
			/* Check if button pressed for MIDI channel edit */
			if (RotaryEncoder_IsPressed())
			{
				/* Update UI mode LEDs */
				ModeRunLED.Clear();
				ModeEditPresetLED.Clear();
				ModeEditMidiLED.Set();
				
				if (System_MidiChannel == MIDI_CH_OMNI)
				{
					SP10281_WriteChar('o', 'm', 'n', 0, 0, 0);
				} 
				else
				{
					SP10281_WriteNumber(System_MidiChannel);
				}
				
				SystemState = EDIT_MIDI_CHANNEL;
			} 
			else
			{
				SystemState = INITIALIZE_SYSTEM;
			}
			
			break;
		
		case EDIT_MIDI_CHANNEL:
			/* If user is selecting new MIDI channel */
			if (TempSelectMidiChannel != LastTempSelectMidiChannel)
			{
				/* Write selected MIDI channel to 7-segment display */
				if (TempSelectMidiChannel == MIDI_CH_OMNI)
				{
					SP10281_WriteChar('o', 'm', 'n', 0, 0, 0);
				}
				else
				{
					SP10281_WriteNumber(TempSelectMidiChannel);
				}
				
				/* Start timer */
				System_TempSelectOvfCnt = 1;
			}
			
			LastTempSelectMidiChannel = TempSelectMidiChannel;
			
			break;
		
		case INITIALIZE_SYSTEM:			
			/* Write active preset to 7-segment display */
			SP10281_WriteNumber(ActivePreset+1);
			
			/* Update mode LEDs */
			ModeRunLED.Set();
			ModeEditPresetLED.Clear();
			ModeEditMidiLED.Clear();
			
			/* Set appropriate System State */
			if (SystemRunMode == RUN_PRESET_MODE)
			{
				SystemState = ENTER_RUN_PRESET_CTRL;
			}
			else if (SystemRunMode == RUN_LOOP_MODE)
			{
				SystemState = ENTER_RUN_LOOP_CTRL;
			}
			
			break;
		
		case ENTER_RUN_PRESET_CTRL:
			/* Update state variables */
			SystemRunMode = RUN_PRESET_MODE;
			SystemState = RUN_PRESET_CTRL;
		
			/* Change LastActivePreset to force update system */
			if (ActivePreset > PRESET_1)
			{
				LastActivePreset = System_Preset(LastActivePreset - 1);
			}
			else if (ActivePreset < PRESET_8)
			{
				LastActivePreset = System_Preset(LastActivePreset + 1);
			}
			
		case RUN_PRESET_CTRL:
			/* If active preset has changed since last cycle then update system */
			if (ActivePreset != LastActivePreset)
			{
				System_ChangePreset();
			}
			
			/* If user is browsing presets */
			if (TempSelectPreset != LastTempSelectPreset)
			{
				/* Update 7-segment display */
				SP10281_WriteNumber(TempSelectPreset + 1);
								
				/* Update UI LEDs */
				LoopModePreset = PresetTable[TempSelectPreset];
				System_UpdateUI_LEDs();
				
				/* Start timer */
				System_TempSelectOvfCnt = 1;
			}
			
			break;
		
		case ENTER_RUN_LOOP_CTRL:
			/* Update Pedal display */
			PedalComm_QueueCommand(UpdateTunerLEDs, 1, 0);
			PedalComm_QueueCommand(Update7segments, 4, (uint8_t*)"Loop");
			PedalComm_QueueCommand(UpdatePresetLoopLEDs, 1, (uint8_t*)&LoopModePreset.Active_Loops);
			
			/* Update UI LEDs */
			System_UpdateUI_LEDs();
			
			/* Update 7-segment display */
			SP10281_WriteChar('M', 'L', 'C', 0, 0, 0);
			
			/* Update state variables */
			SystemRunMode = RUN_LOOP_MODE;
			SystemState = RUN_LOOP_CTRL;
			
		case RUN_LOOP_CTRL:
			/* If active loops or ctrl switches has changed since last cycle */
			if (LoopModePreset != LastLoopModePreset)
			{
				/* Update relays */
				//System_UpdateUI_Relays();
				
				/* Update UI LEDs */
				System_UpdateUI_LEDs();
			}
			
			/* If user is browsing presets */
			if (TempSelectPreset != LastTempSelectPreset)
			{
				/* Update 7-segment display */
				SP10281_WriteNumber(TempSelectPreset + 1);
							
				/* Update UI LEDs */
				LoopModePreset = PresetTable[TempSelectPreset];
				System_UpdateUI_LEDs();
				
				/* Start timer */
				System_TempSelectOvfCnt = 1;
			}
			
			break;
		
		case ENTER_TUNER:
		
		case TUNER:
			break;
		
		case ENTER_EDIT_UI_MODE:
			/* Change LastSystemUIMode to force update mode LEDs */
			if (SystemUIMode < MODE_EDIT_MIDI)
			{
				LastSystemUIMode = System_UI_Mode(SystemUIMode + 1);
			}
			else if (SystemUIMode > MODE_RUN)
			{
				LastSystemUIMode = System_UI_Mode(SystemUIMode - 1);
			}
			
			System_ChangePreset();
			
			SystemState = EDIT_UI_MODE;
			
		case EDIT_UI_MODE:
			/* If user is changing the UI mode then update mode LEDs*/
			if (SystemUIMode != LastSystemUIMode)
			{
				if (SystemUIMode == MODE_RUN)
				{
					ModeRunLED.AutoToggle();
					ModeEditPresetLED.Clear();
					ModeEditMidiLED.Clear();
				}
				else if (SystemUIMode == MODE_EDIT_PRESET)
				{
					ModeRunLED.Clear();
					ModeEditPresetLED.AutoToggle();
					ModeEditMidiLED.Clear();
				}
				else if (SystemUIMode == MODE_EDIT_MIDI)
				{
					ModeRunLED.Clear();
					ModeEditPresetLED.Clear();
					ModeEditMidiLED.AutoToggle();
				}
			}
			
			LastSystemUIMode = SystemUIMode;
			
			break;
		
		case ENTER_EDIT_PRESET:
			/* Disable Auto toggle */
			ModeRunLED.Clear();
			ModeEditPresetLED.Set();
			ModeEditMidiLED.Clear();
			
			LoopModePreset = PresetTable[ActivePreset];
			
			UserEditSelection = 0;
			LastUserEditSelection = 1;
			
			SystemState = EDIT_PRESET;
			
		case EDIT_PRESET:
			/* If user is selecting */
			if (UserEditSelection != LastUserEditSelection)
			{
				/* Blank LEDs except from selection */
				if (UserEditSelection <= 7)
				{
					MCP23017_WriteReg(MCP23017_UI_LEDS_ADDR, OLATA, (1 << UserEditSelection));
					MCP23017_WriteReg(MCP23017_UI_LEDS_ADDR, OLATB, (1 << UI_MODE_PRESET_LED_PIN));
				}
				else
				{
					MCP23017_WriteReg(MCP23017_UI_LEDS_ADDR, OLATA, 0x00);
					MCP23017_WriteReg(MCP23017_UI_LEDS_ADDR, OLATB, ((1 << UI_MODE_PRESET_LED_PIN) | (1 << (UserEditSelection - 8))));
				}
				
				/* Start timer */
				System_MarkSelectionOvfCnt = 1;
			}
			
			LastUserEditSelection = UserEditSelection;
			break;
			
		case ENTER_EDIT_MIDI_IN_OUT:
			/* Disable Auto toggle */
			ModeRunLED.Clear();
			ModeEditPresetLED.Clear();
			ModeEditMidiLED.Set();
			
			/* Update 7-segment display */
			if (SetMidiEdit == MIDI_IN)
			{
				SP10281_WriteChar('i', 'n', ' ', 0, 0, 0);
			}
			else
			{
				SP10281_WriteChar('o', 'u', 't', 0, 0, 0);
			}
			
			SystemState = EDIT_MIDI_IN_OUT;
			
		case EDIT_MIDI_IN_OUT:
			break;
		
		case ENTER_EDIT_MIDI_PC:
			/* Get setup for active preset */
			if (SetMidiEdit == MIDI_IN)
			{
				TempSelectMidiPC = PresetTable[ActivePreset].MIDI_PC_In;
				LastTempSelectMidiPC = PresetTable[ActivePreset].MIDI_PC_In;
			}
			else
			{
				TempSelectMidiPC = PresetTable[ActivePreset].MIDI_PC_Out;
				LastTempSelectMidiPC = PresetTable[ActivePreset].MIDI_PC_Out;
			}
			
			/* Update 7-segment display */
			if (TempSelectMidiPC == MIDI_PC_OFF)
			{
				SP10281_WriteChar('o', 'f', 'f', 0, 0, 0);
			}
			else
			{
				SP10281_WriteNumber(TempSelectMidiPC);
			}
			
			SystemState = EDIT_MIDI_PC;
			
		case EDIT_MIDI_PC:
			
			break;
		
		default:
			break;
	}
	
	LastActivePreset = ActivePreset;
	LastSystemState = SystemState;
	LastLoopModePreset = LoopModePreset;
	LastTempSelectPreset = TempSelectPreset;
}

void System_HandleRotaryEncoderInput()
{
	switch (SystemState)
	{
		case EDIT_MIDI_CHANNEL:
			if (RotEnc_State == SHORT_PRESS)
			{
				System_MidiChannel = TempSelectMidiChannel;
			}
			else if (RotEnc_State == LONG_PRESS)
			{
				/* Store selected MIDI channel and continue */
				System_MidiChannel = TempSelectMidiChannel;
				eeprom_write_byte((uint8_t*)EEPROM_MIDI_CH_ADDR, (uint8_t)System_MidiChannel);
				SystemState = INITIALIZE_SYSTEM;
			}
			else if (RotEnc_State == ROT_RIGHT)
			{
				/* Increment selected MIDI channel */
				if (TempSelectMidiChannel < MIDI_CH_MAX)
				{
					TempSelectMidiChannel = MIDI_Channel(TempSelectMidiChannel + 1);
				}
			}
			else if (RotEnc_State == ROT_LEFT)
			{
				/* Decrement selected MIDI channel */
				if (TempSelectMidiChannel > MIDI_CH_OMNI)
				{
					TempSelectMidiChannel = MIDI_Channel(TempSelectMidiChannel - 1);
				}
			}

			break;
		
		case RUN_PRESET_CTRL:
		case RUN_LOOP_CTRL:
			if (RotEnc_State == SHORT_PRESS)
			{
				ActivePreset = TempSelectPreset;
				
				if (SystemRunMode == RUN_LOOP_MODE)
				{
					SystemState = ENTER_RUN_PRESET_CTRL;
				}
			}
			else if (RotEnc_State == LONG_PRESS)
			{
				ActivePreset = TempSelectPreset;
				
				SystemState = ENTER_EDIT_UI_MODE;
			}
			else if (RotEnc_State == ROT_RIGHT)
			{
				/* Increment temporary selected preset */
				if (TempSelectPreset < PRESET_8)
				{
					TempSelectPreset = System_Preset(TempSelectPreset + 1);
				}
			}
			else if (RotEnc_State == ROT_LEFT)
			{
				/* Decrement temporary selected preset */
				if (TempSelectPreset > PRESET_1)
				{
					TempSelectPreset = System_Preset(TempSelectPreset - 1);
				}
			}

			break;
			
		case EDIT_UI_MODE:
			if (RotEnc_State == SHORT_PRESS)
			{
				if (SystemUIMode == MODE_RUN)
				{
					SystemState = INITIALIZE_SYSTEM;
				}
				else if (SystemUIMode == MODE_EDIT_PRESET)
				{
					SystemState = ENTER_EDIT_PRESET;
				}
				else if (SystemUIMode == MODE_EDIT_MIDI)
				{
					SystemState = ENTER_EDIT_MIDI_IN_OUT;
				}
			}
			else if (RotEnc_State == LONG_PRESS)
			{
			}
			else if (RotEnc_State == ROT_RIGHT)
			{
				/* Increment UI mode */
				if (SystemUIMode < MODE_EDIT_MIDI)
				{
					SystemUIMode = System_UI_Mode(SystemUIMode + 1);
				}
			}
			else if (RotEnc_State == ROT_LEFT)
			{
				/* Increment UI mode */
				if (SystemUIMode > MODE_RUN)
				{
					SystemUIMode = System_UI_Mode(SystemUIMode - 1);
				}
			}
			
			break;
		
		case EDIT_PRESET:
			if (RotEnc_State == SHORT_PRESS)
			{
				/* Update temporary preset */
				if (UserEditSelection < NUMBER_OF_LOOPS)
				{
					LoopModePreset.Active_Loops ^= (1 << UserEditSelection);
				}
				else
				{
					LoopModePreset.Active_CtrlSwitches ^= (1 << (UserEditSelection - 8));
				}
				
				/* Update relays */
				//System_UpdateUI_Relays();
				
				/* Update UI LEDs */
				System_UpdateUI_LEDs();
				
			}
			else if (RotEnc_State == LONG_PRESS)
			{
				/* Store changes in EEPROM */
				eeprom_write_block((void*)&LoopModePreset, (void*)(EEPROM_PRESETS_BASE_ADDR+(ActivePreset * sizeof(Preset))), 2);
				
				/* Update PresetTable */
				PresetTable[ActivePreset].Active_Loops = LoopModePreset.Active_Loops;
				PresetTable[ActivePreset].Active_CtrlSwitches = LoopModePreset.Active_CtrlSwitches;
				
				/* Return to Edit UI mode */
				SystemState = ENTER_EDIT_UI_MODE;
			}
			else if (RotEnc_State == ROT_RIGHT)
			{
				if (UserEditSelection < 11)
				{
					UserEditSelection++;
				}
			}
			else if (RotEnc_State == ROT_LEFT)
			{
				if (UserEditSelection > 0)
				{
					UserEditSelection--;
				}
			}
			break;
			
		case EDIT_MIDI_IN_OUT:
			if (RotEnc_State == SHORT_PRESS)
			{
				SystemState = ENTER_EDIT_MIDI_PC;
			}
			else if (RotEnc_State == LONG_PRESS)
			{
				/* Return to Edit UI mode */
				SystemState = ENTER_EDIT_UI_MODE;
			}
			else if (RotEnc_State == ROT_RIGHT)
			{
				SetMidiEdit = MIDI_OUT;
				SP10281_WriteChar('o', 'u', 't', 0, 0, 0);
			}
			else if (RotEnc_State == ROT_LEFT)
			{
				SetMidiEdit = MIDI_IN;
				SP10281_WriteChar('i', 'n', ' ', 0, 0, 0);
			}
			
			break;
			
		case EDIT_MIDI_PC:
			if (RotEnc_State == SHORT_PRESS)
			{
				/* Stop timeout timer */
				System_TempSelectOvfCnt = 0;
				
				LastTempSelectMidiPC = TempSelectMidiPC;
			}
			else if (RotEnc_State == LONG_PRESS)
			{
				if (SetMidiEdit == MIDI_IN)
				{
					PresetTable[ActivePreset].MIDI_PC_In = TempSelectMidiPC;
					eeprom_write_byte((uint8_t*)(EEPROM_PRESETS_BASE_ADDR+(ActivePreset * sizeof(Preset)) + 2), TempSelectMidiPC);
				}
				else
				{
					PresetTable[ActivePreset].MIDI_PC_Out = TempSelectMidiPC;
					eeprom_write_byte((uint8_t*)(EEPROM_PRESETS_BASE_ADDR+(ActivePreset * sizeof(Preset)) + 3), TempSelectMidiPC);
				}
				
				SystemState = ENTER_EDIT_MIDI_IN_OUT;
			}
			else if (RotEnc_State == ROT_RIGHT)
			{
				if (TempSelectMidiPC != MIDI_PC_MAX)
				{
					TempSelectMidiPC++;
				}
				
				/* Update 7-segment display */
				if (TempSelectMidiPC == MIDI_PC_OFF)
				{
					SP10281_WriteChar('o', 'f', 'f', 0, 0, 0);
				}
				else
				{
					SP10281_WriteNumber(TempSelectMidiPC);
				}
				
				/* Start timeout timer */
				System_TempSelectOvfCnt = 1;
			}
			else if (RotEnc_State == ROT_LEFT)
			{
				if (TempSelectMidiPC > MIDI_PC_MAX || TempSelectMidiPC == 0)
				{
					TempSelectMidiPC = MIDI_PC_OFF;
				}
				else if (TempSelectMidiPC > 0)
				{
					TempSelectMidiPC--;
				}
				
				/* Update 7-segment display */
				if (TempSelectMidiPC == MIDI_PC_OFF)
				{
					SP10281_WriteChar('o', 'f', 'f', 0, 0, 0);
				}
				else
				{
					SP10281_WriteNumber(TempSelectMidiPC);
				}
				
				/* Start timeout timer */
				System_TempSelectOvfCnt = 1;
			}
			
			break;
		
		default:
			break;
	}
}

void System_HandleTempSelectTimeout()
{
	switch (SystemState)
	{
		case EDIT_MIDI_CHANNEL:
			TempSelectMidiChannel = System_MidiChannel;
			break;
			
		case RUN_PRESET_CTRL:
			if (RotaryEncoder_IsPressed())
			{
				ActivePreset = TempSelectPreset;
			}
			else
			{
				TempSelectPreset = ActivePreset;
				LastTempSelectPreset = ActivePreset;
			}

			SystemState = ENTER_RUN_PRESET_CTRL;
			break;
			
		case RUN_LOOP_CTRL:
			if (RotaryEncoder_IsPressed())
			{
				ActivePreset = TempSelectPreset;
				SystemState = ENTER_RUN_PRESET_CTRL;
			}
			else
			{
				TempSelectPreset = ActivePreset;
				LastTempSelectPreset = ActivePreset;
				SystemState = ENTER_RUN_LOOP_CTRL;
			}
			
			break;
			
		case EDIT_MIDI_PC:
			TempSelectMidiPC = LastTempSelectMidiPC;
			
			/* Update 7-segment display */
			if (TempSelectMidiPC == MIDI_PC_OFF)
			{
				SP10281_WriteChar('o', 'f', 'f', 0, 0, 0);
			}
			else
			{
				SP10281_WriteNumber(TempSelectMidiPC);
			}
			
			break;
			
		default:
			break;
	}
}

void System_HandleMarkSelection()
{
	if (SystemState == EDIT_PRESET)
	{
		MCP23017_WriteReg(MCP23017_UI_LEDS_ADDR, OLATA, LoopModePreset.Active_Loops);
		MCP23017_WriteReg(MCP23017_UI_LEDS_ADDR, OLATB, ((1 << UI_MODE_PRESET_LED_PIN) | (LoopModePreset.Active_CtrlSwitches & 0x0F)));
	}
}

unsigned char System_HandlePedalCommand(CMD cmd, uint8_t length, uint8_t *dat)
{
	unsigned char response = ACK_BYTE;
	
	if (cmd == FootswitchShortPress)
	{
		if (SystemState == RUN_PRESET_CTRL)
		{
			ActivePreset = (System_Preset)(Util_Bit2Int(dat[0]) - 1);
		} 
		else if (SystemState == RUN_LOOP_CTRL)
		{
			LoopModePreset.Active_Loops ^= dat[0];
		}
		else if (SystemState == TUNER)
		{
			SystemState = INITIALIZE_SYSTEM;
		}
	}
	else if (cmd == FootswitchLongPress)
	{
		if (SystemState == RUN_PRESET_CTRL)
		{
			SystemRunMode = RUN_LOOP_MODE;
			SystemState = INITIALIZE_SYSTEM;
		}
		else if (SystemState == RUN_LOOP_CTRL)
		{
			/* Determine if top or bottom row of pedals was pressed */
			if (dat[0] & 0x0F)	// If bottom row then (de)activate Ctrl switches
			{
				LoopModePreset.Active_CtrlSwitches ^= dat[0];
			}
			else				// If top row then change to Preset mode
			{
				SystemRunMode = RUN_PRESET_MODE;
				SystemState = INITIALIZE_SYSTEM;
			}
		}
		else if (SystemState == TUNER)
		{
			SystemState = INITIALIZE_SYSTEM;
		}
	}
	else if (cmd == FootswitchLongLongPress)
	{
		if (SystemState == RUN_PRESET_CTRL || SystemState == RUN_LOOP_CTRL)
		{
			SystemState = TUNER;
		}
		else if (SystemState == TUNER)
		{
			SystemState = INITIALIZE_SYSTEM;
		}
	}
	else
	{
		response = NACK_BYTE;
	}
	
	return response;
}

void System_HandleMIDIProgramChange()
{
	for (uint8_t i = 0; i < NUMBER_OF_PRESETS; i++)
	{
		if (PresetTable[i].MIDI_PC_In == MIDI_ReceivedProgram)
		{
			ActivePreset = (System_Preset)i;
		}
	}
}

void System_UpdateUI_LEDs()
{
	for (uint8_t i = 0; i < NUMBER_OF_LOOPS; i++)
	{
		if (LoopModePreset.Active_Loops & (1 << i))
		{
			if (LoopLEDs[i].getState() != MCP_Output::SET)
			{
				LoopLEDs[i].Set();
			}
		}
		else
		{
			if (LoopLEDs[i].getState() != MCP_Output::CLEARED)
			{
				LoopLEDs[i].Clear();	
			}
		}
	}
				
	for (uint8_t i = 0; i < NUMBER_OF_CTRL_SWITCHES; i++)
	{
		if (LoopModePreset.Active_CtrlSwitches & (1 << i))
		{
			if (CtrlSwitchLEDs[i].getState() != MCP_Output::SET)
			{
				CtrlSwitchLEDs[i].Set();
			}
		}
		else
		{
			if (CtrlSwitchLEDs[i].getState() != MCP_Output::CLEARED)
			{
				CtrlSwitchLEDs[i].Clear();
			}
		}
	}
}

void System_UpdateUI_Relays()
{
	for (uint8_t i = 0; i < NUMBER_OF_LOOPS; i++)
	{
		if (LoopModePreset.Active_Loops & (1 << i))
		{
			if (LoopRelays[i].getState() != MCP_Output::SET)
			{
				LoopRelays[i].Set();
			}
		}
		else
		{
			if (LoopRelays[i].getState() != MCP_Output::CLEARED)
			{
				LoopRelays[i].Clear();
			}
		}
	}
	
	for (uint8_t i = 0; i < NUMBER_OF_CTRL_SWITCHES; i++)
	{
		if (LoopModePreset.Active_CtrlSwitches & (1 << i))
		{
			if (CtrlSwitchRelays[i].getState() != MCP_Output::SET)
			{
				CtrlSwitchRelays[i].Set();
			}
		}
		else
		{
			if (CtrlSwitchRelays[i].getState() != MCP_Output::CLEARED)
			{
				CtrlSwitchRelays[i].Clear();
			}
		}
	}
}

void System_ChangePreset()
{
	LoopModePreset = PresetTable[ActivePreset];
	
	/* Update relays */
	//System_UpdateUI_Relays();
				
	/* Send MIDI PC message if MIDI Out is set for preset */
	if (PresetTable[ActivePreset].MIDI_PC_Out != MIDI_PC_OFF)
	{
		MIDI_TransmitProgramChange(PresetTable[ActivePreset].MIDI_PC_Out);
	}
				
	/* Update Pedal display */
	PedalComm_QueueCommand(UpdateTunerLEDs, 1, 0);
	PedalComm_QueueCommand(Update7segments, 4, (uint8_t*)"Pre ");
	PedalComm_QueueCommand(UpdatePresetLoopLEDs, 1, (uint8_t*)&ActivePreset);
				
	/* Update 7-segment display */
	SP10281_WriteNumber(ActivePreset + 1);
				
	/* Update UI LEDs */
	System_UpdateUI_LEDs();
}
