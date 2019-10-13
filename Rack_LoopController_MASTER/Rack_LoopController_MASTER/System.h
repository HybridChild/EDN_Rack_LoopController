
#ifndef SYSTEM_H_
#define SYSTEM_H_

#define EEPROM_MIDI_CH_ADDR			  0
#define EEPROM_PRESETS_BASE_ADDR	100

#define UI_USER_ACTION_TIMEOUT		800		// 305Hz / 800 = 0,38Hz (2624ms)
#define UI_MARK_SELECTION_TIMOUT	100		// 305Hz / 122 = 2,5Hz (400ms)

volatile unsigned short UI_UserAction_OvfCnt;
volatile unsigned char UI_UserAction_TimoutFlag;
volatile unsigned short UI_MarkSelection_OvfCnt;
volatile unsigned char UI_MarkSelection_TimoutFlag;

void System_Init();
void System_Run();


#endif /* SYSTEM_H_ */