/*
 * Segment7.h
 *
 * Created: 09-02-2020 14:48:17
 *  Author: Esben
 */ 

#ifndef SEGMENT7_H_
#define SEGMENT7_H_

#include <stdint-gcc.h>

void Segment7_WriteDigit(uint8_t dig, char chr);
void Segment7_WriteAll(char c3, char c2, char c1, char c0);


#endif /* SEGMENT7_H_ */