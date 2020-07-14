/*
 * PedalComunication.cpp
 *
 * Created: 27-01-2020 19:35:30
 *  Author: Esben
 */ 

/* Includes */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "PedalCom.h"
#include "UART.h"
#include "System.h"

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

#define CMD_HEADER_SIZE		4
#define SOF_BYTE			0x5F
#define EOF_BYTE			0xEF

#define SOF_BYTE_IDX		0
#define LENGTH_BYTE_IDX		1
#define CMD_BYTE_IDX		2
#define DATA_BYTE_IDX		3

#define RX_BUFFER_SIZE		16

#define DIR_TRANSMIT		0
#define DIR_RECEIVE			1

#define MAX_RETRANSMITS		5


/* Function prototypes */
void Select_RX_TX(uint8_t dir);

/* Global variables */
volatile bool PedalCom_DelayTxFlag = false;
volatile uint8_t PedalCom_DelayTXOvfCnt = 0;
volatile uint16_t PedalCom_ResponseTimeoutOvfCnt = 0;
volatile bool PedalCom_ResponseTimeoutFlag = false;
volatile uint16_t PedalCom_FullFrameTimeoutOvfCnt = 0;
volatile bool PedalCom_FullFrameTimeoutFlag = false;
volatile uint16_t PedalCom_HeartbeatOvfCnt = 1;
volatile bool PedalCom_HeartbeatFlag = false;
volatile bool PedalCom_ConnectionOpen = true;

/* Local variables */
volatile unsigned char RX_Buffer[RX_BUFFER_SIZE] = {0};

volatile CommandStruct RX_CommandQueue[RX_QUEUE_SIZE] = {Empty, 0, {0,0,0,0}, false, false};
volatile CommandStruct TX_CommandQueue[TX_QUEUE_SIZE] = {Empty, 0, {0,0,0,0}, false, true};
volatile uint8_t RxHead = 0;
volatile uint8_t RxTail = 0;
volatile uint8_t TxHead = 0;
volatile uint8_t TxTail = 0;

volatile uint8_t RetransmitCnt = 0;

/* Function implementations */
void PedalCom_Init()
{
	/* Set RX/TX Select GPIOs to output */
	SELECT_RX_TX_DDR |= (SELECT_RX_MASK | SELECT_TX_MASK);
	
	/* Activate receive */
	Select_RX_TX(DIR_RECEIVE);
	
	/* Enable TX Complete Interrupt */
	UCSR0B |= (1 << TXCIE0);
}

uint8_t PedalCom_RxAvailable()
{
	return (RxHead != RxTail);
}

uint8_t PedalCom_TxAvailable()
{
	return (TxHead != TxTail);
}

void PedalCom_FlushTxQueue()
{
	TxTail = TxHead;
	TX_CommandQueue[TxHead].acked = true;
}


void PedalCom_Receive()
{
	static uint8_t RX_ByteCnt = 0;
	
	/* Start/reset Delay TX timer. Both in case a NACK needs to be sent, and also to
	   prevent response to previous command being sent while still receiving new data. */
	if (PedalCom_DelayTXOvfCnt != 0)
	{
		PedalCom_DelayTXOvfCnt = 1;
	}
	
	/* Fetch new byte from UART buffer */
	RX_Buffer[RX_ByteCnt] = UART0_GetChar();
	RX_ByteCnt++;
	
	/* Handle new byte */
	if ((RX_ByteCnt - 1) == SOF_BYTE_IDX)
	{
		PedalCom_ResponseTimeoutOvfCnt = 0;		// Stop counter
		
		if (RX_Buffer[SOF_BYTE_IDX] == SOF_BYTE)
		{
			PedalCom_FullFrameTimeoutOvfCnt = 1;
		}
		else
		{
			RX_ByteCnt = 0;
			PedalCom_DelayTXOvfCnt = 1;	// Start/reset Delay TX counter in case a new message is in queue to be sent
				
			if (RX_Buffer[SOF_BYTE_IDX] == ACK_BYTE)
			{
				RetransmitCnt = 0;	// Reset retransmit counter
				TX_CommandQueue[TxTail].acked = true;
				
				if (!PedalCom_ConnectionOpen)
				{
					PedalCom_ConnectionOpen = true;
					
					/* Set system state to initialize Pedal */
					SystemState = INITIALIZE_SYSTEM;
				}
			}
			else if (RX_Buffer[SOF_BYTE_IDX] == NACK_BYTE)
			{
				PedalCom_PrepareRetransmit();
			}
		}
	}
	else if ((RX_ByteCnt - 1) == LENGTH_BYTE_IDX)
	{
		RX_CommandQueue[RxHead].length = RX_Buffer[LENGTH_BYTE_IDX];
	}
	else if (RX_ByteCnt == RX_CommandQueue[RxHead].length)
	{
		PedalCom_FullFrameTimeoutOvfCnt = 0;
		
		/* If full frame received */
		if (RX_Buffer[RX_CommandQueue[RxHead].length - 1] == EOF_BYTE)
		{
			RX_CommandQueue[RxHead].command = (CMD)RX_Buffer[CMD_BYTE_IDX];
			
			for (uint8_t i = 0; i < (RX_CommandQueue[RxHead].length - CMD_HEADER_SIZE); i++)
			{
				RX_CommandQueue[RxHead].data[i] = RX_Buffer[DATA_BYTE_IDX + i];
			}
			
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
			PedalCom_DelayTXOvfCnt = 1;	// Start/reset Delay TX timer
		}
			
		RX_ByteCnt = 0;
	}	
}


void PedalCom_HandleReceived()
{
	unsigned char response;
	response = System_HandlePedalCommand(RX_CommandQueue[RxTail].command, RX_CommandQueue[RxTail].length, (uint8_t *)RX_CommandQueue[RxTail].data);
		
	/* Calculate and store new queue index */
	RxTail = (RxTail + 1) & RX_QUEUE_MASK;

	/* Prepare Transmit response */
	UART0_QueueChar(response);
	
	/* Start/reset Delay TX timer */
	PedalCom_DelayTXOvfCnt = 1;
}


bool PedalCom_QueueCommand(CMD cmd, uint8_t datLen, uint8_t *dat)
{
	/* Calculate new queue head index */
	uint8_t tmpHead = (TxHead + 1) & TX_QUEUE_MASK;
	
	/* Check for free space in queue */
	if ( tmpHead == TxTail )
		return false;
	
	/* Copy command to queue */
	TX_CommandQueue[TxHead].command = cmd;
	TX_CommandQueue[TxHead].length = datLen + CMD_HEADER_SIZE;
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


void PedalCom_PutCommand()
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
		
		for (uint8_t i = 0; i < (TX_CommandQueue[TxTail].length - CMD_HEADER_SIZE); i++)
		{
			UART0_QueueChar((unsigned char)TX_CommandQueue[TxTail].data[i]);
		}
		
		UART0_QueueChar(EOF_BYTE);
		
		/* If no data is waiting to be transmitted */
		if (PedalCom_DelayTXOvfCnt == 0)
		{
			/* Signal that data is waiting to be transmitted without delay */
			PedalCom_DelayTxFlag = true;
		}
		
		TX_CommandQueue[TxTail].sent = true;
		
		/* Start response timer */
		PedalCom_ResponseTimeoutOvfCnt = 1;
	}
}


void PedalCom_PrepareRetransmit()
{
	if (RetransmitCnt < MAX_RETRANSMITS)
	{
		RetransmitCnt++;
		
		/* Prepare retransmit latest command */
		TX_CommandQueue[TxTail].sent = false;
	}
}


void PedalCom_Transmit()
{
	/* Activate Transmit */
	Select_RX_TX(DIR_TRANSMIT);
	
	/* Start transmit */
	UART0_PutQueue();
}


void Select_RX_TX(uint8_t dir)
{
	if (dir == DIR_TRANSMIT)
	{
		SELECT_RX_TX_PORT &= ~(SELECT_RX_MASK);
		SELECT_RX_TX_PORT |= SELECT_TX_MASK;
	}
	else
	{
		SELECT_RX_TX_PORT |= SELECT_RX_MASK;
		SELECT_RX_TX_PORT &= ~(SELECT_TX_MASK);
	}
}

/* TX Complete Interrupt ISR */
ISR(USART0_TX_vect)
{
	/* Deactivate Transmit */
	Select_RX_TX(DIR_RECEIVE);
}