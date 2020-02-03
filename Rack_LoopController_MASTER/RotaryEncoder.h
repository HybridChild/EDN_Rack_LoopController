/*
 * RotaryEncoder.h
 *
 * Created: 26-01-2020 20:30:44
 *  Author: Esben
 */ 


#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#define ROT_ENC_PRESS_OVF		60		// 60*2ms = 120ms
#define ROT_ENC_LONG_PRESS_OVF	600		// 600*2ms = 1200ms

#define GPIO_ROT_ENC_DDR				PCINT8_DDR
#define GPIO_ROT_ENC_PORT				PORTB
#define GPIO_ROT_ENC_PIN				PINB
#define GPIO_ROT_ENC_TRIGGER_PIN		PINB0
#define GPIO_ROT_ENC_INDICATOR_PIN		PINB1
#define GPIO_ROT_ENC_PRESS_PIN			PINB2
#define GPIO_ROT_ENC_TRIGGER_MASK		(1 << GPIO_ROT_ENC_TRIGGER_PIN)
#define GPIO_ROT_ENC_INDICATOR_MASK		(1 << GPIO_ROT_ENC_INDICATOR_PIN)
#define GPIO_ROT_ENC_PRESS_MASK			(1 << GPIO_ROT_ENC_PRESS_PIN)

typedef enum RotaryEncoder_State
{
	IDLE,
	ROT_RIGHT,
	ROT_LEFT,
	SHORT_PRESS,
	PRESSED,
	ABORTED,
	LONG_PRESS
} RotaryEncoder_State;

extern volatile unsigned short RotaryEncoder_OvfCnt;
extern volatile enum RotaryEncoder_State RotEnc_State;

void RotaryEncoder_Init();
void RotaryEncoder_EnableInterrupt();

#endif /* ROTARYENCODER_H_ */