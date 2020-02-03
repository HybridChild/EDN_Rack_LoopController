/*
 * ActionHandler.h
 *
 * Created: 03-02-2020 19:13:36
 *  Author: Esben
 */ 


#ifndef ACTIONHANDLER_H_
#define ACTIONHANDLER_H_

#include "PedalComm.h"

void Handle_MIDIProgramChange();
unsigned char Handle_PedalCommand(CMD cmd, uint8_t length, uint8_t *dat);
void Handle_RotaryEncoderAction();



#endif /* ACTIONHANDLER_H_ */