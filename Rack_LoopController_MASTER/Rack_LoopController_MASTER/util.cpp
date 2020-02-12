/*
 * util.cpp
 *
 * Created: 26-01-2020 19:36:30
 *  Author: Esben
 */ 

#include "Util.h"

uint8_t Util_ReverseByte(uint8_t b)
{
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

uint8_t Util_Bit2Int(uint8_t val)
{
	uint8_t i = 0;
	
	while (val)
	{
		val = (val >> 1);
		i++;
	}
	
	return i;
}