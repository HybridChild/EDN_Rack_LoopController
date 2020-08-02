/*
 * InputCapture.h
 *
 * Created: 09-05-2020 20:01:51
 *  Author: Esben
 */ 

#ifndef INPUT_CAPTURE_H_
#define INPUT_CAPTURE_H_

void InputCapture_Init();
void InputCapture_Enable();
void InputCapture_Disable();
bool InputCapture_Available();
uint32_t InputCapture_GetCount();

#endif /* INPUT_CAPTURE_H_ */
