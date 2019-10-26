#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "PedalCom.h"
#include "UART.h"

#define SELECT_RX_TX_DDR				DDRB
#define SELECT_RX_TX_PORT				PORTB
#define SELECT_RX_PORT					PORTB3
#define SELECT_RX_MASK					(1 << SELECT_RX_PORT)
#define SELECT_TX_PORT					PORTB4
#define SELECT_TX_MASK					(1 << SELECT_TX_PORT)

#define PEDAL_COM_TX_BUF_SIZE			9
#define PEDAL_COM_RX_BUF_SIZE			6
#define PEDAL_COM_SET_RX				0
#define PEDAL_COM_SET_TX				1

void PedalCom_Select_RX_TX(unsigned char dir);
void PedalCom_Transmit(unsigned char length);

volatile char PedalCom_TX_Buffer[PEDAL_COM_TX_BUF_SIZE];
volatile char PedalCom_RX_Buffer[PEDAL_COM_RX_BUF_SIZE];
volatile unsigned char PedalCom_RX_ByteCnt;
volatile unsigned char PedalCom_RX_Length;


void PedalCom_Init()
{
	/* Set RX/TX Select GPIOs to output */
	SELECT_RX_TX_DDR |= (SELECT_RX_MASK | SELECT_TX_MASK);
	
	/* Activate receive */
	PedalCom_Select_RX_TX(PEDAL_COM_SET_RX);
	
	/* Enable TX Complete Interrupt */
	UCSR0B |= (1 << TXCIE0);	
	
	/* Clear buffers */
	memset((void*)PedalCom_TX_Buffer, 0, PEDAL_COM_TX_BUF_SIZE);
	memset((void*)PedalCom_RX_Buffer, 0, PEDAL_COM_RX_BUF_SIZE);
	
	PedalCom_RX_ByteCnt = 0;
	PedalCom_RX_Length = 0;
	PedalCom_NackCnt = 0;
	PedalCom_OvfCnt = 0;
	PedalCom_TimeoutFlag = 0;
	PedalCom_CommandReceivedFlag = 0;
	PedalCom_CMD_Byte = 0;
	PedalCom_Data_Byte = 0;
}


void PedalCom_UpdatePresetLoopLEDs(unsigned char preset)
{
	/* Create message Frame */
	PedalCom_TX_Buffer[0] = PEDAL_COM_SOF;
	PedalCom_TX_Buffer[1] = 5;
	PedalCom_TX_Buffer[2] = PEDAL_COM_CMD_PRE_LOOP_LEDS;
	PedalCom_TX_Buffer[3] = preset;
	PedalCom_TX_Buffer[4] = PEDAL_COM_EOF;
	
	PedalCom_Transmit(PedalCom_TX_Buffer[1]);
}


void PedalCom_UpdateTunerLEDs(unsigned char red, unsigned char green)
{
	/* Create message Frame */
	PedalCom_TX_Buffer[0] = PEDAL_COM_SOF;
	PedalCom_TX_Buffer[1] = 6;
	PedalCom_TX_Buffer[2] = PEDAL_COM_CMD_TUNER_LEDS;
	PedalCom_TX_Buffer[3] = red;
	PedalCom_TX_Buffer[4] = green;
	PedalCom_TX_Buffer[5] = PEDAL_COM_EOF;
		
	PedalCom_Transmit(PedalCom_TX_Buffer[1]);
}


void PedalCom_Update7segments(char d3, char d2, char d1, char d0)
{
	/* Create message Frame */
	PedalCom_TX_Buffer[0] = PEDAL_COM_SOF;
	PedalCom_TX_Buffer[1] = 8;
	PedalCom_TX_Buffer[2] = PEDAL_COM_CMD_7_SEGMENTS;
	PedalCom_TX_Buffer[3] = d0;
	PedalCom_TX_Buffer[4] = d1;
	PedalCom_TX_Buffer[5] = d2;
	PedalCom_TX_Buffer[6] = d3;
	PedalCom_TX_Buffer[7] = PEDAL_COM_EOF;
	
	PedalCom_Transmit(PedalCom_TX_Buffer[1]);
}


void PedalCom_TunerModeUpdate(unsigned char red_LEDs, unsigned char green_LED, enum PedalCom_Tuner_Note note, enum PedalCom_Tuner_SideBar side_bar)
{
	
}


void PedalCom_Select_RX_TX(unsigned char dir)
{
	if (dir == PEDAL_COM_SET_TX)
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


void PedalCom_Receive()
{
	PedalCom_RX_Buffer[PedalCom_RX_ByteCnt] = UART0_GetChar();
	PedalCom_RX_ByteCnt++;
	
	if (PedalCom_RX_ByteCnt == 1)
	{
		if (PedalCom_RX_Buffer[0] != PEDAL_COM_SOF)
		{
			PedalCom_RX_ByteCnt = 0;
			PedalCom_NackCnt = 0;
		}
		
		if (PedalCom_RX_Buffer[0] == PEDAL_COM_NACK)
		{
			PedalCom_Retransmit();
			
			/* Start timeout Timer */
			PedalCom_OvfCnt = 1;
		}
	}
	else if (PedalCom_RX_ByteCnt == 2)
	{
		PedalCom_RX_Length = PedalCom_RX_Buffer[1];
	}
	else if (PedalCom_RX_ByteCnt == PedalCom_RX_Length)
	{
		if (PedalCom_RX_Buffer[PedalCom_RX_Length - 1] == PEDAL_COM_EOF)	// If full frame received
		{
			PedalCom_CommandReceivedFlag = 1;
			PedalCom_CMD_Byte = PedalCom_RX_Buffer[2];
			PedalCom_Data_Byte = PedalCom_RX_Buffer[3];
		}
		else
		{
			/* Send Nack */
			PedalCom_TX_Buffer[0] = PEDAL_COM_NACK;
			PedalCom_Transmit(1);
		}

		PedalCom_RX_ByteCnt = 0;
	}
}
 

void PedalCom_Transmit(unsigned char length)
{
	unsigned char i = 0;
	
	/* Activate Transmit */
	PedalCom_Select_RX_TX(PEDAL_COM_SET_TX);
	
	for (i = 0; i < length; i++)
	{
		UART0_PutChar(PedalCom_TX_Buffer[i]);
	}
	
	/* Start timeout Timer */
	PedalCom_OvfCnt = 1;
}


void PedalCom_Retransmit()
{
	if (PedalCom_NackCnt < 3)
	{
		PedalCom_NackCnt++;
		
		/* Retransmit command */
		PedalCom_Transmit(PedalCom_TX_Buffer[1]);
	}
}

/* TX Complete Interrupt ISR */
ISR(USART0_TX_vect)
{
	/* Deactivate Transmit */
	PedalCom_Select_RX_TX(PEDAL_COM_SET_RX);
}
