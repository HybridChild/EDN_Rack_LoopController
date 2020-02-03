/*
 * util.cpp
 *
 * Created: 26-01-2020 19:36:30
 *  Author: Esben
 */ 

#include "Util.h"

unsigned char Reverse_Byte(unsigned char b)
{
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}