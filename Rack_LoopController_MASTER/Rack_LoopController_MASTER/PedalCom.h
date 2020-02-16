/*
 * PedalCom.h
 *
 * Created: 27-01-2020 19:35:11
 *  Author: Esben
 */ 


#ifndef PEDALCOM_H_
#define PEDALCOM_H_

#include <stdint-gcc.h>

#define PEDAL_UART_BAUDRATE		19200

#define PEDALCOM_DELAY_TX_TIMEOUT	3		// 3*2ms = 6ms
#define PEDALCOM_RESPONSE_TIMEOUT	1000	// 1000*2ms = 2 seconds

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

void PedalCom_Init();
uint8_t PedalCom_RxAvailable();
uint8_t PedalCom_TxAvailable();
void PedalCom_FlushTxQueue();
void PedalCom_Receive();
void PedalCom_HandleReceived();
bool PedalCom_QueueCommand(CMD cmd, uint8_t datLen, uint8_t *dat);
void PedalCom_PutCommand();
void PedalCom_Transmit();

volatile extern bool PedalCom_DelayTxFlag;
volatile extern uint8_t PedalCom_DelayTXOvfCnt;
volatile extern uint16_t PedalCom_ResponseTimeoutOvfCnt;
volatile extern bool PedalCom_ResponseTimeoutFlag;
volatile extern bool PedalCom_ConnectionOpen;

#endif /* PEDALCOM_H_ */