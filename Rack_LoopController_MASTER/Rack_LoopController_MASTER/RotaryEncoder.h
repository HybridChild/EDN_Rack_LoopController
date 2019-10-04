
#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#define ROT_ENC_PRESS_OVF		48		// 305Hz / 48 = 6,35Hz (150ms)
#define ROT_ENC_LONG_PRESS_OVF	366		// 305Hz / 366 = 0,83Hz (1200ms)

#define GPIO_ROT_ENC_DDR				PCINT8_DDR
#define GPIO_ROT_ENC_PORT				PORTB
#define GPIO_ROT_ENC_PIN				PINB
#define GPIO_ROT_ENC_TRIGGER_PIN		PINB0
#define GPIO_ROT_ENC_INDICATOR_PIN		PINB1
#define GPIO_ROT_ENC_PRESS_PIN			PINB2
#define GPIO_ROT_ENC_TRIGGER_MASK		(1 << GPIO_ROT_ENC_TRIGGER_PIN)
#define GPIO_ROT_ENC_INDICATOR_MASK		(1 << GPIO_ROT_ENC_INDICATOR_PIN)
#define GPIO_ROT_ENC_PRESS_MASK			(1 << GPIO_ROT_ENC_PRESS_PIN)

enum RotaryEncoder_State
{
	IDLE,
	ROT_RIGHT,
	ROT_LEFT,
	SHORT_PRESS,
	PRESSED,
	ABORTED,
	LONG_PRESS
};

volatile unsigned short RotaryEncoder_OvfCnt;
volatile enum RotaryEncoder_State RotEnc_State;

void RotaryEncoder_Init();
void RotaryEncoder_EnableInterrupt();

#endif /* ROTARYENCODER_H_ */