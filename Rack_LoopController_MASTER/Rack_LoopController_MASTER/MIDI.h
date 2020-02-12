/*
 * MIDI.h
 *
 * Created: 03-02-2020 18:20:16
 *  Author: Esben
 */ 


#ifndef MIDI_H_
#define MIDI_H_

#define MIDI_UART_BAUDRATE		31250

#define MIDI_PC_OFF		0xFF
#define MIDI_PC_MAX		127
#define MIDI_CH_MAX		16
#define MIDI_BUF_SIZE	3

typedef enum MIDI_Channel
{
	MIDI_CH_OMNI	= 0,
	MIDI_CH_1	= 1,
	MIDI_CH_2	= 2,
	MIDI_CH_3	= 3,
	MIDI_CH_4	= 4,
	MIDI_CH_5	= 5,
	MIDI_CH_6	= 6,
	MIDI_CH_7	= 7,
	MIDI_CH_8	= 8,
	MIDI_CH_9	= 9,
	MIDI_CH_10	= 10,
	MIDI_CH_11	= 11,
	MIDI_CH_12	= 12,
	MIDI_CH_13	= 13,
	MIDI_CH_14	= 14,
	MIDI_CH_15	= 15,
	MIDI_CH_16	= 16
} MIDI_Channel;

volatile extern uint8_t MIDI_ReceivedProgram;
volatile extern bool MIDI_ProgramChangeFlag;

void MIDI_ReceiveIncoming();
void MIDI_TransmitProgramChange(uint8_t prg);

#endif /* MIDI_H_ */