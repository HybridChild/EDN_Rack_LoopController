
#ifndef PEDALCOM_H_
#define PEDALCOM_H_

#define PEDAL_COM_TIMEOUT	25		// 305Hz / 25 = 12,5Hz (82ms)

#define PEDAL_COM_SOF					0x5F
#define PEDAL_COM_EOF					0xEF
#define PEDAL_COM_ACK					0xAC
#define PEDAL_COM_NACK					0x2A

#define PEDAL_COM_CMD_PRE_LOOP_LEDS		0xD0
#define PEDAL_COM_CMD_TUNER_LEDS		0xD3
#define PEDAL_COM_CMD_7_SEGMENTS		0xD7
#define PEDAL_COM_CMD_TUNER				0xDE
#define PEDAL_COM_CMD_SHORT_PRESS		0xB1
#define PEDAL_COM_CMD_LONG_PRESS		0xB2
#define PEDAL_COM_CMD_LONG_LONG_PRESS	0xB3

enum PedalCom_Tuner_Note {
	Unknown	,
	C		,
	Db		,
	D		,
	Eb		,
	E		,
	F		,
	Gb		,
	G		,
	Ab		,
	A		,
	Bb		,
	B		};
	
enum PedalCom_Tuner_SideBar {
	Sidebar_Off		,
	Sidebar_Sharp	,
	Sidebar_Flat	,
	Sidebar_Spot_on	};

volatile unsigned short PedalCom_OvfCnt;
volatile unsigned char PedalCom_TimeoutFlag;
volatile unsigned char PedalCom_NackCnt;
volatile unsigned char PedalCom_CommandReceivedFlag;
volatile unsigned char PedalCom_CMD_Byte;
volatile unsigned char PedalCom_Data_Byte;

void PedalCom_Init();
void PedalCom_UpdatePresetLoopLEDs(unsigned char preset);
void PedalCom_UpdateTunerLEDs(unsigned char red, unsigned char green);
void PedalCom_Update7segments(char d3, char d2, char d1, char d0);
void PedalCom_TunerModeUpdate(unsigned char red_LEDs, unsigned char green_LED, enum PedalCom_Tuner_Note note, enum PedalCom_Tuner_SideBar side_bar);
void PedalCom_Retransmit();
void PedalCom_Receive();

#endif /* PEDALCOM_H_ */