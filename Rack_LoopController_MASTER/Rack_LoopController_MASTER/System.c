
#include <avr/io.h>
#include <avr/eeprom.h>
#include "System.h"
#include "User_Interface.h"
#include "RotaryEncoder.h"

#define EEPROM_MIDI_CH_ADDR			  0
#define EEPROM_PRESETS_BASE_ADDR	100

#define PRESET_BYTE_SIZE			  4
#define NUMBER_OF_PRESETS			  8

#define MIDI_CC_OFF					0xFF

enum SystemState {
	RUN_PRESET_CTRL,
	RUN_LOOP_CTRL,
	TUNER,
	EDIT_UI_MODE,
	ENTER_EDIT_LOOPS_SW,
	EDIT_LOOPS_SW,
	ENTER_EDIT_MIDI_CC,
	EDIT_MIDI_CC,
	EDIT_MIDI_IN_OUT };

enum SystemPreset {
	PRESET_1 = 0,
	PRESET_2 = 1,
	PRESET_3 = 2,
	PRESET_4 = 3,
	PRESET_5 = 4,
	PRESET_6 = 5,
	PRESET_7 = 6,
	PRESET_8 = 7
	};

enum System_UI_Mode {
	MODE_RUN			= 0,
	MODE_EDIT_LOOPS_SW	= 1,
	MODE_EDIT_MIDI_CC	= 2
	};
	
enum System_SetMIDI_Edit {
	MIDI_IN		= UI_WRITE_7SEG_IN,
	MIDI_OUT	= UI_WRITE_7SEG_OUT
	};

typedef struct Preset
{
	unsigned char Active_Loops;
	unsigned char Active_SW_Ctrl;
	unsigned char MIDI_CC_in;
	unsigned char MIDI_CC_out;
} Preset;

volatile enum SystemState Present_SystemState;
volatile enum SystemState Next_SystemState;
volatile enum SystemPreset sysPreset;
volatile enum System_UI_Mode sysUI_Mode;
volatile Preset PresetTable[8];

void System_Init()
{
	Present_SystemState = RUN_PRESET_CTRL;
	Next_SystemState = RUN_PRESET_CTRL;
	sysPreset = PRESET_1;
	sysUI_Mode = MODE_RUN;
	
	eeprom_read_block((void*)PresetTable, (void*)EEPROM_PRESETS_BASE_ADDR, PRESET_BYTE_SIZE * NUMBER_OF_PRESETS);
	
	UI_Write_7seg_Display((short)(sysPreset + 1));
	UI_Update_Preset_Loop_LEDs(PresetTable[sysPreset].Active_Loops);
	UI_Update_SW_Ctrl_LEDs(PresetTable[sysPreset].Active_SW_Ctrl);
	UI_Update_Mode_LEDs((unsigned char)sysUI_Mode);
	
	UI_UserAction_TimoutFlag = 0;
	UI_MarkSelection_TimoutFlag = 0;
	
	// Send preset data to pedal
}

void System_Run()
{
	static Preset TempEdit_Preset = {0, 0, 0};
	static enum SystemPreset TempSelect_Preset = PRESET_1;
	static enum System_UI_Mode TempSelect_UI_Mode = MODE_RUN;
	static enum System_SetMIDI_Edit TempSetMIDI_Edit = MIDI_IN;
	static unsigned char TempSelect_MIDI_CC = MIDI_CC_OFF;
	static unsigned char TempSelect_PresetLoopSW_Ctrl = 0;
	static unsigned char Blink_var = 0;
	
	
	if (Next_SystemState != Present_SystemState
		|| (RotEnc_State != IDLE && RotEnc_State != PRESSED)
		|| UI_UserAction_TimoutFlag
		|| UI_MarkSelection_TimoutFlag /*
		|| MIDI_RX_Flag
		|| Pedal_RX_Flag */)
	{
		Present_SystemState = Next_SystemState;
		
		switch (Present_SystemState)
		{
			case RUN_PRESET_CTRL:
				if (UI_UserAction_TimoutFlag)
				{
					TempSelect_Preset = sysPreset;
					UI_Write_7seg_Display((short)(TempSelect_Preset + 1));
					UI_UserAction_TimoutFlag = 0;
				}
			
				if (RotEnc_State == SHORT_PRESS)
				{
					sysPreset = TempSelect_Preset;
					UI_Update_Preset_Loop_LEDs(PresetTable[sysPreset].Active_Loops);
					UI_Update_SW_Ctrl_LEDs(PresetTable[sysPreset].Active_SW_Ctrl);
				}
				else if (RotEnc_State == LONG_PRESS)
				{
					sysPreset = TempSelect_Preset;
					UI_Update_Preset_Loop_LEDs(PresetTable[sysPreset].Active_Loops);
					UI_Update_SW_Ctrl_LEDs(PresetTable[sysPreset].Active_SW_Ctrl);
					Next_SystemState = EDIT_UI_MODE;
					UI_MarkSelection_OvfCnt = 1;
				}
				else if (RotEnc_State == ROT_RIGHT)
				{
					if (TempSelect_Preset < PRESET_8)
					{
						TempSelect_Preset++;
					}
					
					UI_Write_7seg_Display((short)(TempSelect_Preset + 1));
					UI_UserAction_OvfCnt = 1;
				}
				else if (RotEnc_State == ROT_LEFT)
				{
					if (TempSelect_Preset > PRESET_1)
					{
						TempSelect_Preset--;
					}
					
					UI_Write_7seg_Display((short)(TempSelect_Preset + 1));
					UI_UserAction_OvfCnt = 1;
				}
				
				RotEnc_State = IDLE;
				RotaryEncoder_EnableInterrupt();
							
				break;
			case RUN_LOOP_CTRL:
		
				break;
			case TUNER:
		
				break;
			case EDIT_UI_MODE:
				if (UI_MarkSelection_TimoutFlag)
				{
					UI_MarkSelection_TimoutFlag = 0;
					
					if (Blink_var)
					{
						Blink_var = 0;
						UI_Update_Mode_LEDs(UI_MODE_LEDS_OFF);
					}
					else
					{
						Blink_var = 1;
						UI_Update_Mode_LEDs(TempSelect_UI_Mode);
					}
					
					UI_MarkSelection_OvfCnt = 1;
				}
				
				if (RotEnc_State == SHORT_PRESS)
				{
					sysUI_Mode = TempSelect_UI_Mode;
					
					if (TempSelect_UI_Mode == MODE_RUN)
					{
						Next_SystemState = RUN_PRESET_CTRL;
					}
					else if (TempSelect_UI_Mode == MODE_EDIT_LOOPS_SW)
					{
						Next_SystemState = ENTER_EDIT_LOOPS_SW;
					}
					else if (TempSelect_UI_Mode == MODE_EDIT_MIDI_CC)
					{
						Next_SystemState = ENTER_EDIT_MIDI_CC;
					}
					
					UI_Update_Mode_LEDs(TempSelect_UI_Mode);
				}
				else if (RotEnc_State == LONG_PRESS)
				{
					
				}
				else if (RotEnc_State == ROT_RIGHT)
				{
					if (TempSelect_UI_Mode < MODE_EDIT_MIDI_CC)
					{
						TempSelect_UI_Mode++;
						UI_Update_Mode_LEDs(TempSelect_UI_Mode);
						UI_MarkSelection_OvfCnt = 1;
					}
				}
				else if (RotEnc_State == ROT_LEFT)
				{
					if (TempSelect_UI_Mode > MODE_RUN)
					{
						TempSelect_UI_Mode--;
						UI_Update_Mode_LEDs(TempSelect_UI_Mode);
						UI_MarkSelection_OvfCnt = 1;
					}
				}
				
				RotEnc_State = IDLE;
				RotaryEncoder_EnableInterrupt();
				
				break;
			case ENTER_EDIT_LOOPS_SW:
				TempEdit_Preset = PresetTable[sysPreset];
				TempSelect_PresetLoopSW_Ctrl = 0;
				
				UI_Write_7seg_Display((short)sysPreset);
				UI_Update_Preset_Loop_LEDs(1 << TempSelect_PresetLoopSW_Ctrl);
				UI_Update_SW_Ctrl_LEDs(0);
				UI_MarkSelection_OvfCnt = 1;
				
				Next_SystemState = EDIT_LOOPS_SW;
				
				break;
			case EDIT_LOOPS_SW:
				
				if (UI_MarkSelection_TimoutFlag)
				{
					UI_MarkSelection_TimoutFlag = 0;
					UI_Update_Preset_Loop_LEDs(TempEdit_Preset.Active_Loops);
					UI_Update_SW_Ctrl_LEDs(TempEdit_Preset.Active_SW_Ctrl);
				}
				
				if (RotEnc_State == SHORT_PRESS)
				{
					if (TempSelect_PresetLoopSW_Ctrl < NUMBER_OF_PRESETS)
					{
						TempEdit_Preset.Active_Loops ^= (1 << TempSelect_PresetLoopSW_Ctrl);
					}
					else
					{
						TempEdit_Preset.Active_SW_Ctrl ^= (1 << (TempSelect_PresetLoopSW_Ctrl - NUMBER_OF_PRESETS));
					}
					
					UI_Update_Preset_Loop_LEDs(TempEdit_Preset.Active_Loops);
					UI_Update_SW_Ctrl_LEDs(TempEdit_Preset.Active_SW_Ctrl);
				}
				else if (RotEnc_State == LONG_PRESS)
				{
					eeprom_write_block((void*)&TempEdit_Preset, (void*)(EEPROM_PRESETS_BASE_ADDR+(sysPreset*PRESET_BYTE_SIZE)), 2);
					PresetTable[sysPreset].Active_Loops = TempEdit_Preset.Active_Loops;
					PresetTable[sysPreset].Active_SW_Ctrl = TempEdit_Preset.Active_SW_Ctrl;
					Next_SystemState = EDIT_UI_MODE;
					UI_MarkSelection_OvfCnt = 1;
				}
				else if (RotEnc_State == ROT_RIGHT)
				{
					if (TempSelect_PresetLoopSW_Ctrl < 11)
					{
						TempSelect_PresetLoopSW_Ctrl++;
					}
					
					if (TempSelect_PresetLoopSW_Ctrl < NUMBER_OF_PRESETS)
					{
						UI_Update_Preset_Loop_LEDs(1 << TempSelect_PresetLoopSW_Ctrl);
						UI_Update_SW_Ctrl_LEDs(0);
					}
					else
					{
						UI_Update_Preset_Loop_LEDs(0);
						UI_Update_SW_Ctrl_LEDs(1 << (TempSelect_PresetLoopSW_Ctrl - NUMBER_OF_PRESETS));
					}
						
					UI_MarkSelection_OvfCnt = 1;
				}
				else if (RotEnc_State == ROT_LEFT)
				{
					if (TempSelect_PresetLoopSW_Ctrl > 0)
					{
						TempSelect_PresetLoopSW_Ctrl--;
					}
										
					if (TempSelect_PresetLoopSW_Ctrl < NUMBER_OF_PRESETS)
					{
						UI_Update_Preset_Loop_LEDs(1 << TempSelect_PresetLoopSW_Ctrl);
						UI_Update_SW_Ctrl_LEDs(0);
					}
					else
					{
						UI_Update_Preset_Loop_LEDs(0);
						UI_Update_SW_Ctrl_LEDs(1 << (TempSelect_PresetLoopSW_Ctrl - NUMBER_OF_PRESETS));
					}
										
					UI_MarkSelection_OvfCnt = 1;
				}
						
				RotEnc_State = IDLE;
				RotaryEncoder_EnableInterrupt();
						
				break;
			case ENTER_EDIT_MIDI_CC:
				UI_Write_7seg_Display(TempSetMIDI_Edit);
				Next_SystemState = EDIT_MIDI_CC;
				break;
			case EDIT_MIDI_CC:
				if (RotEnc_State == SHORT_PRESS)
				{
					if (TempSetMIDI_Edit == MIDI_IN)
					{
						TempSelect_MIDI_CC = PresetTable[sysPreset].MIDI_CC_in;
					}
					else
					{
						TempSelect_MIDI_CC = PresetTable[sysPreset].MIDI_CC_out;
					}
					
					UI_Write_7seg_MIDI_CC(TempSelect_MIDI_CC);
					Next_SystemState = EDIT_MIDI_IN_OUT;
				}
				else if (RotEnc_State == LONG_PRESS)
				{
					TempSetMIDI_Edit = MIDI_IN;
					UI_Write_7seg_Display(sysPreset + 1);
					Next_SystemState = EDIT_UI_MODE;
				}
				else if (RotEnc_State == ROT_RIGHT)
				{
					if (TempSetMIDI_Edit == MIDI_IN)
					{
						TempSetMIDI_Edit = MIDI_OUT;
					}
					else
					{
						TempSetMIDI_Edit = MIDI_IN;
					}
					
					UI_Write_7seg_Display(TempSetMIDI_Edit);
				}
				else if (RotEnc_State == ROT_LEFT)
				{
					if (TempSetMIDI_Edit == MIDI_IN)
					{
						TempSetMIDI_Edit = MIDI_OUT;
					}
					else
					{
						TempSetMIDI_Edit = MIDI_IN;
					}
										
					UI_Write_7seg_Display(TempSetMIDI_Edit);
				}
				
				RotEnc_State = IDLE;
				RotaryEncoder_EnableInterrupt();
			break;
		case EDIT_MIDI_IN_OUT:
			if (RotEnc_State == SHORT_PRESS)
			{
				if (TempSetMIDI_Edit == MIDI_IN)
				{
					PresetTable[sysPreset].MIDI_CC_in = TempSelect_MIDI_CC;
					eeprom_write_block((void*)&TempSelect_MIDI_CC, (void*)(EEPROM_PRESETS_BASE_ADDR+(sysPreset*PRESET_BYTE_SIZE) + 2), 1);
				}
				else
				{
					PresetTable[sysPreset].MIDI_CC_out = TempSelect_MIDI_CC;
					eeprom_write_block((void*)&TempSelect_MIDI_CC, (void*)(EEPROM_PRESETS_BASE_ADDR+(sysPreset*PRESET_BYTE_SIZE) + 3), 1);
				}
				
				Next_SystemState = ENTER_EDIT_MIDI_CC;
			}
			else if (RotEnc_State == LONG_PRESS)
			{
				
			}
			else if (RotEnc_State == ROT_RIGHT)
			{
				if (TempSelect_MIDI_CC != 127)
				{
					TempSelect_MIDI_CC++;
				}
				
				UI_Write_7seg_MIDI_CC(TempSelect_MIDI_CC);
			}
			else if (RotEnc_State == ROT_LEFT)
			{
				if (TempSelect_MIDI_CC > 127 || TempSelect_MIDI_CC == 0)
				{
					TempSelect_MIDI_CC = MIDI_CC_OFF;
				}
				else if (TempSelect_MIDI_CC > 0)
				{
					TempSelect_MIDI_CC--;
				}
								
				UI_Write_7seg_MIDI_CC(TempSelect_MIDI_CC);
			}
			
			RotEnc_State = IDLE;
			RotaryEncoder_EnableInterrupt();
			
			break;
			
			default:
				break;
		}
	}
}