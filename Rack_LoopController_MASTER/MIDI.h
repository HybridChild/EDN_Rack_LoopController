/*
 * MIDI.h
 *
 * Created: 03-02-2020 18:20:16
 *  Author: Esben
 */ 


#ifndef MIDI_H_
#define MIDI_H_

#define EEPROM_MIDI_CH_ADDR			  0
#define EEPROM_PRESETS_BASE_ADDR	100

#define MIDI_PC_MAX		127
#define MIDI_CH_OMNI	0
#define MIDI_CH_MAX		16
#define MIDI_BUF_SIZE	3

volatile extern uint8_t System_MIDI_Channel;
volatile extern uint8_t MIDI_ReceivedProgram;
volatile extern bool MIDI_ProgramChangeFlag;

void MIDI_Init();
void MIDI_ReceiveIncoming();
void MIDI_TransmitProgramChange(uint8_t prg);

#endif /* MIDI_H_ */