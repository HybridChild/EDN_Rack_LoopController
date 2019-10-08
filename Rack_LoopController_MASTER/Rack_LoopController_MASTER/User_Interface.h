
#ifndef UI_H_
#define UI_H_

#define UI_WRITE_7SEG_ALL	1000
#define UI_WRITE_7SEG_ON	1001
#define UI_WRITE_7SEG_OFF	1002
#define UI_WRITE_7SEG_IN	1003
#define UI_WRITE_7SEG_OUT	1004
#define UI_WRITE_7SEG_OMN	1005

#define UI_MODE_LEDS_OFF	3

void UI_Write_7seg_MIDI_CC(unsigned char val);
void UI_Write_7seg_Display(short val);
void UI_Update_Preset_Loop_LEDs(unsigned char val);
void UI_Update_SW_Ctrl_LEDs(unsigned char val);
void UI_Update_Mode_LEDs(unsigned char val);

#endif /* UI_H_ */