
#ifndef MASTERCOM_H_
#define MASTERCOM_H_

#define MASTER_COM_TIMEOUT	20		// 244Hz / 20 = 12,2Hz (82ms)

#define MASTER_COM_SOF					0x5F
#define MASTER_COM_EOF					0xEF
#define MASTER_COM_ACK					0xAC
#define MASTER_COM_NACK					0x2A

#define MASTER_COM_CMD_SET_SYSTEM_STATE	0xD5
#define MASTER_COM_CMD_PRE_LOOP_LEDS	0xD0
#define MASTER_COM_CMD_TUNER_LEDS		0xD3
#define MASTER_COM_CMD_7_SEGMENTS		0xD7
#define MASTER_COM_CMD_TUNER			0xDE
#define MASTER_COM_CMD_SHORT_PRESS		0xB1
#define MASTER_COM_CMD_LONG_PRESS		0xB2
#define MASTER_COM_CMD_LONG_LONG_PRESS	0xB3

#define MASTER_COM_RX_DATA_MAX_SIZE		4

enum MasterCom_Tuner_Note {
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

enum MasterCom_Tuner_SideBar {
	Sidebar_Off		,
	Sidebar_Sharp	,
	Sidebar_Flat	,
	Sidebar_Spot_on	};
	
volatile unsigned short MasterCom_OvfCnt;
volatile unsigned char MasterCom_TimeoutFlag;
volatile unsigned char MasterCom_NackCnt;
volatile unsigned char MasterCom_CommandReceivedFlag;
volatile unsigned char MasterCom_CMD_Byte;
volatile unsigned char MasterCom_Data_Length_Byte;
volatile unsigned char MasterCom_RX_Data[MASTER_COM_RX_DATA_MAX_SIZE];

void MasterCom_Init();
void MasterCom_FootswitchPress(unsigned char switch_num, unsigned char duration);
void MasterCom_Retransmit();
void MasterCom_Receive();
void MasterCom_HandleReceived();

#endif /* MASTERCOM_H_ */