
#ifndef SP10281_3X7SEGMENT_H_
#define SP10281_3X7SEGMENT_H_

#define GPIO_SP10281_SEGMENT_DDR	DDRA
#define GPIO_SP10281_SEGMENT_PORT	PORTA
#define GPIO_SP10281_SEGMENT_PIN	PINA

#define GPIO_SP10281_DIGIT_DDR		DDRB
#define GPIO_SP10281_DIGIT_PORT		PORTB
#define GPIO_SP10281_DIGIT_PIN		PINB

#define GPIO_SP10281_DIGIT0_PIN		PINB5
#define GPIO_SP10281_DIGIT1_PIN		PINB6
#define GPIO_SP10281_DIGIT2_PIN		PINB7
#define GPIO_SP10281_DIGIT0_MASK	(1 << GPIO_SP10281_DIGIT0_PIN)
#define GPIO_SP10281_DIGIT1_MASK	(1 << GPIO_SP10281_DIGIT1_PIN)
#define GPIO_SP10281_DIGIT2_MASK	(1 << GPIO_SP10281_DIGIT2_PIN)
#define GPIO_SP10281_DIGIT_MASK		(GPIO_SP10281_DIGIT0_MASK | GPIO_SP10281_DIGIT1_MASK | GPIO_SP10281_DIGIT2_MASK)

#define SP10281_NUMBER_OF_DIGITS	3
#define SP10281_OVF_RESET 2		// 305Hz / 2 = 152Hz (6,5ms)

volatile unsigned char SP10281_OvfCnt;
volatile unsigned char SP10281_OvfFlag;
								
void SP10281_Init();
void SP10281_Update();
void SP10281_WriteChar(char d2, char d1, char d0, unsigned char dot2, unsigned char dot1, unsigned char dot0);
void SP10281_WriteDigit(unsigned char dig, unsigned char data, unsigned char dot);
void SP10281_WriteNumber(short num);

#endif /* SP10281_3X7SEGMENT_H_ */