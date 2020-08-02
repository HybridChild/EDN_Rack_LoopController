/*
 * Tuner.h
 *
 * Created: 09-05-2020 20:01:51
 *  Author: Esben
 */ 

#ifndef TUNER_H_
#define TUNER_H_

#define TUNER_TRANSMIT_TIMEOUT  40  // 40*2ms = 80ms

volatile extern uint8_t Tuner_LED_Data[2];
volatile extern uint8_t Tuner_7seg_Data[4];
volatile extern uint8_t Tuner_TransmitTimerOvfCnt;
volatile extern bool Tuner_TransmitTimerFlag;

void Tuner_Init();
void Tuner_Enable();
void Tuner_Disable();
bool Tuner_ResultAvailable();
void Tuner_FetchCounts();
void Tuner_DetectFrequency();
void Tuner_GenerateDisplayData();

#endif /* TUNER_H_ */