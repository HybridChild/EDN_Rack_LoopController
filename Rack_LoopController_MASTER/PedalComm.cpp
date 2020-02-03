/*
 * PedalCommunication.cpp
 *
 * Created: 27-01-2020 19:35:30
 *  Author: Esben
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint-gcc.h>
#include <string.h>
#include "PedalComm.h"
#include "UART.h"
#include "util.h"
#include "ActionHandler.h"


/* Defines */
#define SELECT_RX_TX_DDR				DDRB
#define SELECT_RX_TX_PORT				PORTB
#define SELECT_RX_PORT					PORTB3
#define SELECT_RX_MASK					(1 << SELECT_RX_PORT)
#define SELECT_TX_PORT					PORTB4
#define SELECT_TX_MASK					(1 << SELECT_TX_PORT)

/* Size of the circular receive/transmit queues, must be power of 2 */
#define RX_QUEUE_SIZE		4
#define TX_QUEUE_SIZE		8

/* Mask for calculating RX/TX queues head/tail */
#define RX_QUEUE_MASK	( RX_QUEUE_SIZE - 1)
#define TX_QUEUE_MASK	( TX_QUEUE_SIZE - 1)

#define SOF_BYTE			0x5F
#define EOF_BYTE			0xEF

#define SOF_BYTE_IDX		0
#define LENGTH_BYTE_IDX		1
#define CMD_BYTE_IDX		2
#define DATA_BYTE_IDX		3

#define PEDALCOMM_RX_BUF_SIZE		16


/* Enumerations */

typedef enum CommDirection
{
	receive = 0,
	transmit = 1
} CommDirection;

enum PedalComm_Tuner_Note
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

enum PedalComm_Tuner_SideBar
{
	Sidebar_Off	= 0	,
	Sidebar_Sharp	,
	Sidebar_Flat	,
	Sidebar_Spot_on
};


/* Function prototypes */
void Select_RX_TX(CommDirection dir);

/* Global variables */
volatile unsigned char RX_Buffer[PEDALCOMM_RX_BUF_SIZE] = {0};

volatile CommandStruct RX_CommandQueue[RX_QUEUE_SIZE] = {Empty, 0, {0,0,0,0}, false, false};
volatile CommandStruct TX_CommandQueue[TX_QUEUE_SIZE] = {Empty, 0, {0,0,0,0}, false, false};
volatile uint8_t RxHead = 0;
volatile uint8_t RxTail = 0;
volatile uint8_t TxHead = 0;
volatile uint8_t TxTail = 0;

volatile bool PedalComm_DelayTxFlag = false;
volatile uint8_t PedalComm_DelayTXOvfCnt = 0;
volatile uint16_t PedalComm_ResponseTimeout_OvfCnt = 0;
volatile uint8_t NackCnt = 0;

/* Function implementations */
void PedalComm_Init()
{
	/* Set RX/TX Select GPIOs to output */
	SELECT_RX_TX_DDR |= (SELECT_RX_MASK | SELECT_TX_MASK);
	
	/* Activate receive */
	Select_RX_TX(receive);
	
	/* Enable TX Complete Interrupt */
	UCSR0B |= (1 << TXCIE0);
}

uint8_t PedalComm_RxAvailable()
{
	return (RxHead != RxTail);
}

uint8_t PedalComm_TxAvailable()
{
	return (TxHead != TxTail);
}

void PedalComm_FlushTxQueue()
{
	TxTail = TxHead;
}


void PedalComm_Receive()
{
	static uint8_t RX_ByteCnt = 0;
	
	/* Start/reset Delay TX timer. Both in case a NACK needs to be sent, and also to
	   prevent response to previous command being sent while still receiving new data. */
	PedalComm_DelayTXOvfCnt = 1;
	
	/* Fetch new byte from UART buffer */
	RX_Buffer[RX_ByteCnt] = UART0_GetChar();
	RX_ByteCnt++;
	
	/* Handle new byte */
	if ((RX_ByteCnt - 1) == SOF_BYTE_IDX)
	{
		if (RX_Buffer[SOF_BYTE_IDX] != SOF_BYTE)
		{
			RX_ByteCnt = 0;
				
			if (RX_Buffer[SOF_BYTE_IDX] == ACK_BYTE)
			{
				PedalComm_ResponseTimeout_OvfCnt = 0;		// Stop timeout timer
				NackCnt = 0;
				
				TX_CommandQueue[TxTail].acked = true;
			}
			else if (RX_Buffer[SOF_BYTE_IDX] == NACK_BYTE)
			{
				/* Stop retransmitting command if Pedal keeps responding with NACK. */
				if (++NackCnt <= 3)
				{
					/* Prepare retransmit latest command */
					TX_CommandQueue[TxTail].sent = false;
					PedalComm_DelayTXOvfCnt = 1;	// Start/reset Delay TX timer
				}
			}
		}
	}
	else if ((RX_ByteCnt - 1) == LENGTH_BYTE_IDX)
	{
		RX_CommandQueue[RxHead].length = RX_Buffer[LENGTH_BYTE_IDX];
	}
	else if (RX_ByteCnt == RX_CommandQueue[RxHead].length)
	{
		/* If full frame received */
		if (RX_Buffer[RX_CommandQueue[RxHead].length - 1] == EOF_BYTE)
		{
			RX_CommandQueue[RxHead].command = (CMD)RX_Buffer[CMD_BYTE_IDX];
			RX_CommandQueue[RxHead].data[0] = RX_Buffer[DATA_BYTE_IDX];
			
			/* calculate new RX head index */
			uint8_t tmpHead = (RxHead + 1) & RX_QUEUE_MASK;
				
			if ( tmpHead == RxTail )
			{
				/* error: RX command queue overflow */
			}
			else
			{
				/* store new index */
				RxHead = tmpHead;
			}
		}
		else
		{
			/* Prepare Transmit NACK */
			UART0_QueueChar(NACK_BYTE);
			PedalComm_DelayTXOvfCnt = 1;	// Start/reset Delay TX timer
		}
			
		RX_ByteCnt = 0;
	}	
}


void PedalComm_HandleReceived()
{
	unsigned char response;
	response = Handle_PedalCommand(RX_CommandQueue[RxTail].command, RX_CommandQueue[RxTail].length, (uint8_t *)RX_CommandQueue[RxTail].data);
		
	/* Calculate and store new queue index */
	RxTail = (RxTail + 1) & RX_QUEUE_MASK;

	/* Prepare Transmit response */
	UART0_QueueChar(response);
	
	/* Start/reset Delay TX timer */
	PedalComm_DelayTXOvfCnt = 1;
}


bool PedalComm_QueueCommand(CMD cmd, uint8_t datLen, uint8_t *dat)
{
	/* Calculate new queue head index */
	uint8_t tmpHead = (TxHead + 1) & TX_QUEUE_MASK;
	
	/* Check for free space in queue */
	if ( tmpHead == TxTail )
		return false;
	
	/* Copy command to queue */
	TX_CommandQueue[TxHead].command = cmd;
	TX_CommandQueue[TxHead].length = datLen + 4;
	TX_CommandQueue[TxHead].sent = false;
	TX_CommandQueue[TxHead].acked = false;
	
	for (uint8_t i = 0; i < datLen; i++)
	{
		TX_CommandQueue[TxHead].data[i] = dat[i];
	}
	
	/* Update queue head */
	TxHead = tmpHead;
	
	return true;
}

void PedalComm_PutCommand()
{
	/* Only transmit new command if previous one got an ACK */
	if (TX_CommandQueue[TxTail].acked)
	{
		/* Calculate and store new queue tail index */
		TxTail = (TxTail + 1) & TX_QUEUE_MASK;
	}
	else if (!(TX_CommandQueue[TxTail].sent))
	{
		/* Send command frame to UART TX buffer */
		UART0_QueueChar(SOF_BYTE);
		UART0_QueueChar((unsigned char)TX_CommandQueue[TxTail].length);
		UART0_QueueChar((unsigned char)TX_CommandQueue[TxTail].command);
		
		for (uint8_t i = 0; i < (TX_CommandQueue[TxTail].length - 4); i++)
		{
			UART0_QueueChar((unsigned char)TX_CommandQueue[TxTail].data[i]);
		}
		
		UART0_QueueChar(EOF_BYTE);
		
		/* If no data is waiting to be transmitted */
		if (PedalComm_DelayTXOvfCnt == 0)
		{
			/* Signal that data is waiting to be transmitted without delay */
			PedalComm_DelayTxFlag = true;
		}
		
		TX_CommandQueue[TxTail].sent = true;
		
		/* Start response timeout timer */
		PedalComm_ResponseTimeout_OvfCnt = 1;
	}
}


void PedalComm_Transmit()
{
	/* Activate Transmit */
	Select_RX_TX(transmit);
	
	/* Start transmit */
	UART0_PutQueue();
}


void Select_RX_TX(CommDirection dir)
{
	if (dir == transmit)
	{
		SELECT_RX_TX_PORT &= ~(SELECT_RX_MASK);
		SELECT_RX_TX_PORT |= SELECT_TX_MASK;
	}
	else
	{
		SELECT_RX_TX_PORT &= ~(SELECT_TX_MASK);
		SELECT_RX_TX_PORT |= SELECT_RX_MASK;
	}
}

/* TX Complete Interrupt ISR */
ISR(USART0_TX_vect)
{
	/* Deactivate Transmit */
	Select_RX_TX(receive);
}