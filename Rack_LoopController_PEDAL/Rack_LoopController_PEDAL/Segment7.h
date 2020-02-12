/*
 * Segment7.h
 *
 * Created: 09-02-2020 14:48:17
 *  Author: Esben
 */ 

#ifndef SEGMENT7_H_
#define SEGMENT7_H_

#include <stdint-gcc.h>

void Segment7_WriteDigit(uint8_t dig, char data, uint8_t dot);
void Segment7_WriteAll(char d3, char d2, char d1, char d0, uint8_t dot3, uint8_t dot2, uint8_t dot1, uint8_t dot0);


#endif /* SEGMENT7_H_ */