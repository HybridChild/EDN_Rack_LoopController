/*
 * MasterCom.h
 *
 * Created: 12-02-2020 18:37:19
 *  Author: Esben
 */ 


#ifndef MASTERCOM_H_
#define MASTERCOM_H_

#define MASTER_UART_BAUDRATE	19200

#define MASTERCOM_DELAY_TX_TIMEOUT	3		// 3*2ms = 6ms
#define MASTERCOM_RESPONSE_TIMEOUT	1000	// 1000*2ms = 2 seconds

#define ACK_BYTE			0xAC
#define NACK_BYTE			0x2A

/* Enumerations */
enum Tuner_Note
{
	Unknown = 0,
	C	,
	Db	,
	D	,
	Eb	,
	E	,
	F	,
	Gb	,
	G	,
	Ab	,
	A	,
	Bb	,
	B
};

enum Tuner_SideBar
{
	Sidebar_Off	= 0	,
	Sidebar_Sharp	,
	Sidebar_Flat	,
	Sidebar_Spot_on
};

typedef enum CMD
{
	Empty					= 0x00,
	FootswitchShortPress	= 0xB1,
	FootswitchLongPress		= 0xB2,
	FootswitchLongLongPress	= 0xB4,
	UpdatePresetLoopLEDs	= 0xD0,
	UpdateTunerLEDs			= 0xD3,
	UpdateSystemState		= 0xD5,
	Update7segments			= 0xD7,
	UpdateTuner				= 0xDE
} CMD;

struct CommandStruct
{
	CMD command;
	uint8_t length;
	uint8_t data[4];
	bool sent;
	bool acked;
};

void MasterCom_Init();
uint8_t MasterCom_RxAvailable();
uint8_t MasterCom_TxAvailable();
void MasterCom_FlushTxQueue();
void MasterCom_Receive();
void MasterCom_HandleReceived();
bool MasterCom_QueueCommand(CMD cmd, uint8_t datLen, uint8_t *dat);
void MasterCom_PutCommand();
void MasterCom_Transmit();

volatile extern bool MasterCom_DelayTxFlag;
volatile extern uint8_t MasterCom_DelayTXOvfCnt;
volatile extern uint16_t MasterCom_ResponseTimeoutOvfCnt;
volatile extern bool MasterCom_ResponseTimeoutFlag;


#endif /* MASTERCOM_H_ */