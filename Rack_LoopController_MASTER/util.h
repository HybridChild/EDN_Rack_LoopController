/*
 * util.h
 *
 * Created: 26-01-2020 19:36:10
 *  Author: Esben
 */ 


#ifndef UTIL_H_
#define UTIL_H_

#include <stdint-gcc.h>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

uint8_t Util_ReverseByte(uint8_t b);
uint8_t Util_Bit2Int(uint8_t val);

#endif /* UTIL_H_ */