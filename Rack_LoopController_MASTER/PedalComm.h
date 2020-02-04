/*
 * PedalComm.h
 *
 * Created: 27-01-2020 19:35:11
 *  Author: Esben
 */ 


#ifndef PEDALCOMM_H_
#define PEDALCOMM_H_

#include <stdint-gcc.h>

#define PEDALCOMM_DELAY_TX_TIMEOUT	3		// 3*2ms = 6ms
#define PEDALCOMM_RESPONSE_TIMEOUT	1000	// 1000*2ms = 2 seconds

#define ACK_BYTE			0xAC
#define NACK_BYTE			0x2A

typedef enum CMD
{
	Empty					= 0x00,
	FootswitchShortPress	= 0xB1,
	FootswitchLongPress		= 0xB2,
	FootswitchLongLongPress	= 0xB4,
	UpdatePresetLoopLEDs	= 0xD0,
	UpdateTunerLEDs			= 0xD3,
	Update7segments			= 0xD7,
	UpdateTuner				= 0xDE,
	Heartbeat				= 0xEB
} CMD;

struct CommandStruct
{
	CMD command;
	uint8_t length;
	uint8_t data[4];
	bool sent;
	bool acked;
};

void PedalComm_Init();
uint8_t PedalComm_RxAvailable();
uint8_t PedalComm_TxAvailable();
void PedalComm_FlushTxQueue();
void PedalComm_Receive();
void PedalComm_HandleReceived();
bool PedalComm_QueueCommand(CMD cmd, uint8_t datLen, uint8_t *dat);
void PedalComm_PutCommand();
void PedalComm_Transmit();

volatile extern bool PedalComm_DelayTxFlag;
volatile extern uint8_t PedalComm_DelayTXOvfCnt;
volatile extern uint16_t PedalComm_ResponseTimeoutOvfCnt;
volatile extern bool PedalComm_ResponseTimeoutFlag;
volatile extern bool PedalComm_ConnectionOpen;

#endif /* PEDALCOMM_H_ */