/*
 * MasterCom.h
 *
 * Created: 12-02-2020 18:37:19
 *  Author: Esben
 */ 


#ifndef MASTERCOM_H_
#define MASTERCOM_H_

/* Defines */
#define MASTER_UART_BAUDRATE	19200

#define MASTERCOM_DELAY_TX_TIMEOUT		2		// 2*2ms = 4ms
#define MASTERCOM_RESPONSE_TIMEOUT		16		// 16*2ms = 32ms
#define MASTERCOM_FULL_FRAME_TIMEOUT	5		// 5*2ms = 10ms

#define ACK_BYTE			0xAC
#define NACK_BYTE			0x2A

/* Enumerations */
typedef enum CMD
{
	Empty					= 0x00,
	FootswitchShortPress	= 0xB1,
	FootswitchLongPress		= 0xB2,
	FootswitchLongLongPress	= 0xB4,
	UpdatePresetLoopLEDs	= 0xD0,
	UpdateTunerLEDs			= 0xD3,
	UpdateSystemState		= 0xD5,
	Update7segments			= 0xD7
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
void MasterCom_PrepareRetransmit();

volatile extern bool MasterCom_DelayTxFlag;
volatile extern uint8_t MasterCom_DelayTXOvfCnt;
volatile extern uint16_t MasterCom_ResponseTimeoutOvfCnt;
volatile extern bool MasterCom_ResponseTimeoutFlag;
volatile extern uint16_t MasterCom_FullFrameTimeoutOvfCnt;
volatile extern bool MasterCom_FullFrameTimeoutFlag;


#endif /* MASTERCOM_H_ */