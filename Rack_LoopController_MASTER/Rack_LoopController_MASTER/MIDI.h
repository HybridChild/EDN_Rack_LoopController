
#ifndef MIDI_H_
#define MIDI_H_

#define MIDI_PC_OFF		0xFF
#define MIDI_PC_MAX		127
#define MIDI_CH_OMNI	0
#define MIDI_CH_MAX		16
#define MIDI_BUF_SIZE	3

volatile unsigned char sysMIDI_Channel;
volatile char MIDI_Buffer[MIDI_BUF_SIZE];
volatile unsigned char MIDI_PC_In;
volatile unsigned char MIDI_PC_Flag;

void MIDI_Init();
void MIDI_ReceiveIncoming();
void MIDI_TransmitProgramChange(unsigned char prg);


#endif /* MIDI_H_ */