
#include "User_Interface.h"
#include "Util.h"
#include "MCP23017.h"
#include "SP10281_3x7segment.h"

void UI_Write_7seg_Display(short val)
{
	if (val == UI_WRITE_7SEG_ALL)
	{
		SP10281_WriteChar('A', 'L', 'L', 0, 0, 0);
	}
	else if (val == UI_WRITE_7SEG_OFF)
	{
		SP10281_WriteChar('o', 'F', 'F', 0, 0, 0);
	}
	else if (val == UI_WRITE_7SEG_ON)
	{
		SP10281_WriteChar('o', 'n', ' ', 0, 0, 0);
	}
	else if (val == UI_WRITE_7SEG_IN)
	{
		SP10281_WriteChar('i', 'n', ' ', 0, 0, 0);
	}
	else if (val == UI_WRITE_7SEG_OUT)
	{
		SP10281_WriteChar('o', 'u', 't', 0, 0, 0);
	}
	else
	{
		SP10281_WriteNumber(val);
	}
}

void UI_Write_7seg_MIDI_CC(unsigned char val)
{
	if (val > 127)
	{
		UI_Write_7seg_Display(UI_WRITE_7SEG_OFF);
	}
	else
	{
		UI_Write_7seg_Display(val);
	}
}

void UI_Update_Preset_Loop_LEDs(unsigned char val)
{
	val = Reverse_Byte(val);
	
	MCP23017_WriteReg(MCP23017_ADDR_UI_LEDS, OLATB, val);
}

void UI_Update_SW_Ctrl_LEDs(unsigned char val)
{
	unsigned char tmp = 0;
	tmp = MCP23017_ReadReg(MCP23017_ADDR_UI_LEDS, GPIOA);
	tmp &= 0x07;
	
	val &= 0x0F;
	
	val = Reverse_Byte(val);
	tmp |= val;
	MCP23017_WriteReg(MCP23017_ADDR_UI_LEDS, OLATA, tmp);
}

void UI_Update_Mode_LEDs(unsigned char val)
{
	unsigned char tmp = 0;
	tmp = MCP23017_ReadReg(MCP23017_ADDR_UI_LEDS, GPIOA);
	tmp &= 0xF8;
	
	if (val == UI_MODE_LEDS_OFF)
	{
		val = 0;
	}
	else
	{
		val += 5;
		val = (1 << val);
	}
	
	val = Reverse_Byte(val);
	tmp |= val;
	
	MCP23017_WriteReg(MCP23017_ADDR_UI_LEDS, OLATA, tmp);
}