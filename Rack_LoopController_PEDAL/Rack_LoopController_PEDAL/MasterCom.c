
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "MasterCom.h"
#include "UART.h"
#include "MCP23017.h"
#include "Segment_7.h"
#include "main.h"

#define SELECT_RX_TX_DDR				DDRD
#define SELECT_RX_TX_PORT				PORTD
#define SELECT_RX_PORT					PORTB5
#define SELECT_RX_MASK					(1 << SELECT_RX_PORT)
#define SELECT_TX_PORT					PORTB6
#define SELECT_TX_MASK					(1 << SELECT_TX_PORT)

#define MASTER_COM_TX_BUF_SIZE			6
#define MASTER_COM_RX_BUF_SIZE			9
#define MASTER_COM_SET_RX				0
#define MASTER_COM_SET_TX				1

#define TUNER_GREEN_LED					(1 << 7)
#define TUNER_NOTE_MASK					0x0F
#define TUNER_NOTE_UNKNOWN				0x00
#define TUNER_NOTE_C					0x01
#define TUNER_NOTE_Db					0x02
#define TUNER_NOTE_D					0x03
#define TUNER_NOTE_Eb					0x04
#define TUNER_NOTE_E					0x05
#define TUNER_NOTE_F					0x06
#define TUNER_NOTE_Gb					0x07
#define TUNER_NOTE_G					0x08
#define TUNER_NOTE_Ab					0x09
#define	TUNER_NOTE_A					0x0A
#define TUNER_NOTE_Bb					0x0B
#define TUNER_NOTE_B					0x0C
#define TUNER_SIDE_BAR_MASK				0x30
#define TUNER_SIDE_BAR_OFF				0x00
#define TUNER_SIDE_BAR_SHARP			0x10
#define TUNER_SIDE_BAR_FLAT				0x20
#define TUNER_SIDE_BAR_SPOT_ON			0x30

void MasterCom_Select_RX_TX(unsigned char dir);
void MasterCom_Transmit(unsigned char length);

volatile char MasterCom_TX_Buffer[MASTER_COM_TX_BUF_SIZE];
volatile char MasterCom_RX_Buffer[MASTER_COM_RX_BUF_SIZE];
volatile unsigned char MasterCom_RX_ByteCnt;
volatile unsigned char MasterCom_RX_Length;


void MasterCom_Init()
{
	/* Set RX/TX Select GPIOs to output */
	SELECT_RX_TX_DDR |= (SELECT_RX_MASK | SELECT_TX_MASK);
	
	/* Activate receive */
	MasterCom_Select_RX_TX(MASTER_COM_SET_RX);
	
	/* Enable TX Complete Interrupt */
	UCSR0B |= (1 << TXCIE0);
	
	/* Clear buffers */
	memset((void*)MasterCom_TX_Buffer, 0, MASTER_COM_TX_BUF_SIZE);
	memset((void*)MasterCom_RX_Buffer, 0, MASTER_COM_RX_BUF_SIZE);
	memset((void*)MasterCom_RX_Data, 0, MASTER_COM_RX_DATA_MAX_SIZE);
	
	MasterCom_RX_ByteCnt = 0;
	MasterCom_RX_Length = 0;
	MasterCom_NackCnt = 0;
	MasterCom_OvfCnt = 0;
	MasterCom_TimeoutFlag = 0;
	MasterCom_CommandReceivedFlag = 0;
	MasterCom_Data_Length_Byte = 0;
	MasterCom_CMD_Byte = 0;
}


void MasterCom_HandleReceived()
{
	if (MasterCom_CMD_Byte == MASTER_COM_CMD_SET_SYSTEM_STATE)
	{
		StateOfSystem = MasterCom_RX_Data[0];
		
		if (StateOfSystem == PRESET_CTRL)
		{
			Segment_7_WriteAll('P', 'r', 'E', ' ', 0, 0, 0, 0);
		}
		else if (StateOfSystem == LOOP_CTRL)
		{
			Segment_7_WriteAll('L', 'o', 'o', 'P', 0, 0, 0, 0);
		}
		else if (StateOfSystem == TUNER)
		{
			Segment_7_WriteAll('T', 'u', 'n', 'r', 0, 0, 0, 0);
		}
	}
	else if (MasterCom_CMD_Byte == MASTER_COM_CMD_PRE_LOOP_LEDS)
	{
		MCP23017_WriteReg(SWITCH_INDICATOR_ADDR, OLATA, MasterCom_RX_Data[0]);
	}
	else if (MasterCom_CMD_Byte == MASTER_COM_CMD_TUNER_LEDS)
	{
		MCP23017_WriteReg(TUNER_DISPLAY_ADDR, OLATA, MasterCom_RX_Data[0]);
					
		if (MasterCom_RX_Data[1])
		{
			PORTB |= (1 << PORTB0);
		}
		else
		{
			PORTB &= ~(1 << PORTB0);
		}
	}
	else if (MasterCom_CMD_Byte == MASTER_COM_CMD_7_SEGMENTS)
	{
		MCP23017_WriteReg(DIGIT_0_1_ADDR, OLATB, MasterCom_RX_Data[0]);
		MCP23017_WriteReg(DIGIT_0_1_ADDR, OLATA, MasterCom_RX_Data[1]);
		MCP23017_WriteReg(DIGIT_2_3_ADDR, OLATB, MasterCom_RX_Data[2]);
		MCP23017_WriteReg(DIGIT_2_3_ADDR, OLATA, MasterCom_RX_Data[3]);
	}
	else if (MasterCom_CMD_Byte == MASTER_COM_CMD_TUNER)
	{
		MCP23017_WriteReg(TUNER_DISPLAY_ADDR, OLATA, MasterCom_RX_Data[0]);
		
		if (MasterCom_RX_Data[1] & TUNER_GREEN_LED)
		{
			PORTB |= (1 << PORTB0);
		}
		else
		{
			PORTB &= ~(1 << PORTB0);
		}
		
		switch (MasterCom_RX_Data[1] & TUNER_NOTE_MASK)
		{
			case TUNER_NOTE_UNKNOWN:
				Segment_7_WriteDigit(2, ' ', 0);
				Segment_7_WriteDigit(1, ' ', 0);
			break;
			case TUNER_NOTE_C:
				Segment_7_WriteDigit(2, 'C', 0);
				Segment_7_WriteDigit(1, ' ', 0);
			break;
			case TUNER_NOTE_Db:
				Segment_7_WriteDigit(2, 'D', 0);
				Segment_7_WriteDigit(1, 'b', 0);
			break;
			case TUNER_NOTE_D:
				Segment_7_WriteDigit(2, 'D', 0);
				Segment_7_WriteDigit(1, ' ', 0);
			break;
			case TUNER_NOTE_Eb:
				Segment_7_WriteDigit(2, 'E', 0);
				Segment_7_WriteDigit(1, 'b', 0);
			break;
			case TUNER_NOTE_E:
				Segment_7_WriteDigit(2, 'E', 0);
				Segment_7_WriteDigit(1, ' ', 0);
			break;
			case TUNER_NOTE_F:
				Segment_7_WriteDigit(2, 'F', 0);
				Segment_7_WriteDigit(1, ' ', 0);
			break;
			case TUNER_NOTE_Gb:
				Segment_7_WriteDigit(2, 'G', 0);
				Segment_7_WriteDigit(1, 'b', 0);
			break;
			case TUNER_NOTE_G:
				Segment_7_WriteDigit(2, 'G', 0);
				Segment_7_WriteDigit(1, ' ', 0);
			break;
			case TUNER_NOTE_Ab:
				Segment_7_WriteDigit(2, 'A', 0);
				Segment_7_WriteDigit(1, 'b', 0);
			break;
			case TUNER_NOTE_A:
				Segment_7_WriteDigit(2, 'A', 0);
				Segment_7_WriteDigit(1, ' ', 0);
			break;
			case TUNER_NOTE_Bb:
				Segment_7_WriteDigit(2, 'B', 0);
				Segment_7_WriteDigit(1, 'b', 0);
			break;
			case TUNER_NOTE_B:
				Segment_7_WriteDigit(2, 'B', 0);
				Segment_7_WriteDigit(1, ' ', 0);
			break;
			default:
			break;
		}
		
		switch (MasterCom_RX_Data[1] & TUNER_SIDE_BAR_MASK)
		{
			case TUNER_SIDE_BAR_OFF:
				Segment_7_WriteDigit(3, ' ', 0);
				Segment_7_WriteDigit(0, ' ', 0);
			break;
			case TUNER_SIDE_BAR_SHARP:
				Segment_7_WriteDigit(3, ' ', 0);
				Segment_7_WriteDigit(0, '-', 0);
			break;
			case TUNER_SIDE_BAR_FLAT:
				Segment_7_WriteDigit(3, '-', 0);
				Segment_7_WriteDigit(0, ' ', 0);
			break;
			case TUNER_SIDE_BAR_SPOT_ON:
				Segment_7_WriteDigit(3, '-', 0);
				Segment_7_WriteDigit(0, '-', 0);
			break;
			default:
			break;
		}
	}
}


void MasterCom_FootswitchPress(unsigned char switch_num, unsigned char press_type)
{
	MasterCom_TX_Buffer[0] = MASTER_COM_SOF;
	MasterCom_TX_Buffer[1] = 5;
	MasterCom_TX_Buffer[2] = (char)(0xB0 | press_type);
	MasterCom_TX_Buffer[3] = (char)switch_num;
	MasterCom_TX_Buffer[4] = MASTER_COM_EOF;
	
	MasterCom_Transmit(MasterCom_TX_Buffer[1]);
}


void MasterCom_Receive()
{
	MasterCom_RX_Buffer[MasterCom_RX_ByteCnt] = UART_GetChar();
	MasterCom_RX_ByteCnt++;
	
	if (MasterCom_RX_ByteCnt == 1)
	{
		if (MasterCom_RX_Buffer[0] != MASTER_COM_SOF)
		{
			MasterCom_RX_ByteCnt = 0;
			
			if (MasterCom_RX_Buffer[0] == MASTER_COM_ACK)
			{
				MasterCom_NackCnt = 0;
				MasterCom_OvfCnt = 0;	// Stop timeout timer
			}
		}
		
		if (MasterCom_RX_Buffer[0] == MASTER_COM_NACK)
		{
			MasterCom_Retransmit();
			
			MasterCom_OvfCnt = 1;	// Start timeout Timer
		}
	}
	else if (MasterCom_RX_ByteCnt == 2)
	{
		MasterCom_RX_Length = MasterCom_RX_Buffer[1];
	}
	else if (MasterCom_RX_ByteCnt == MasterCom_RX_Length)
	{
		/* If full frame received */
		if (MasterCom_RX_Buffer[MasterCom_RX_Length - 1] == MASTER_COM_EOF)
		{
			unsigned char i = 0;
			MasterCom_CMD_Byte = MasterCom_RX_Buffer[2];
			MasterCom_Data_Length_Byte = MasterCom_RX_Length - 4;
			
			for (i = 0; i < MasterCom_Data_Length_Byte; i++)
			{
				MasterCom_RX_Data[i] = MasterCom_RX_Buffer[i + 3];
			}
			
			/* Transmit Ack */
			MasterCom_TX_Buffer[0] = MASTER_COM_ACK;
			MasterCom_Transmit(1);
			
			MasterCom_CommandReceivedFlag = 1;
		}
		else
		{
			/* Transmit Nack */
			MasterCom_TX_Buffer[0] = MASTER_COM_NACK;
			MasterCom_Transmit(1);
		}

		MasterCom_RX_ByteCnt = 0;
	}
}


void MasterCom_Transmit(unsigned char length)
{
	unsigned char i = 0;
	
	MasterCom_Select_RX_TX(MASTER_COM_SET_TX);	// Activate Transmit
	
	for (i = 0; i < length; i++)
	{
		UART_PutChar(MasterCom_TX_Buffer[i]);
	}
	
	/* Only start timeout timer if transmitted data is a command */
	if (length != 1)
	{
		/* Start timeout Timer */
		MasterCom_OvfCnt = 1;
	}
}


void MasterCom_Retransmit()
{
	if (MasterCom_NackCnt < 3)
	{
		MasterCom_NackCnt++;
			
		/* Retransmit command */
		MasterCom_Transmit(MasterCom_TX_Buffer[1]);
	}
}


void MasterCom_Select_RX_TX(unsigned char dir)
{
	if (dir == MASTER_COM_SET_TX)
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
ISR(USART_TX_vect)
{
	MasterCom_Select_RX_TX(MASTER_COM_SET_RX);	// Deactivate Transmit
}