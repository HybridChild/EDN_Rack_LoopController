/*
 * SP10281_3x7segment.cpp
 *
 * Created: 26-01-2020 20:02:12
 *  Author: Esben
 */ 

#include <avr/io.h>
#include "SP10281_3x7segment.h"

#define SP10281_DIGIT_0	0
#define SP10281_DIGIT_1	1
#define SP10281_DIGIT_2	2

volatile unsigned char SP10281_DigitData[3] = {0};
volatile unsigned char SP10281_DigitCnt = 0;

volatile unsigned char SP10281_OvfCnt = 0;
volatile bool SP10281_OvfFlag = false;

volatile const unsigned char SP10281_DigitTable[38] = {
	(unsigned char)~(0b00111111),	/* [0]		Character: 0 */
	(unsigned char)~(0b00000110),	/* [1]		Character: 1 */
	(unsigned char)~(0b01011011),	/* [2]		Character: 2 */
	(unsigned char)~(0b01001111),	/* [3]		Character: 3 */
	(unsigned char)~(0b01100110),	/* [4]		Character: 4 */
	(unsigned char)~(0b01101101),	/* [5]		Character: 5 */
	(unsigned char)~(0b01111101),	/* [6]		Character: 6 */
	(unsigned char)~(0b00000111),	/* [7]		Character: 7 */
	(unsigned char)~(0b01111111),	/* [8]		Character: 8 */
	(unsigned char)~(0b01101111),	/* [9]		Character: 9 */
	(unsigned char)~(0b01110111),	/* [10]		Character: A */
	(unsigned char)~(0b01111100),	/* [11]		Character: b */
	(unsigned char)~(0b00111001),	/* [12]		Character: C */
	(unsigned char)~(0b01011110),	/* [13]		Character: d */
	(unsigned char)~(0b01111001),	/* [14]		Character: E */
	(unsigned char)~(0b01110001),	/* [15]		Character: F */
	(unsigned char)~(0b00111101),	/* [16]		Character: G */
	(unsigned char)~(0b01110100),	/* [17]		Character: h */
	(unsigned char)~(0b00000100),	/* [18]		Character: i */
	(unsigned char)~(0b00011110),	/* [19]		Character: J */
	(unsigned char)~(0b01110101),	/* [20]		Character: k */
	(unsigned char)~(0b00111000),	/* [21]		Character: L */
	(unsigned char)~(0b00010101),	/* [22]		Character: M */
	(unsigned char)~(0b01010100),	/* [23]		Character: n */
	(unsigned char)~(0b01011100),	/* [24]		Character: o */
	(unsigned char)~(0b01110011),	/* [25]		Character: P */
	(unsigned char)~(0b01100111),	/* [26]		Character: q */
	(unsigned char)~(0b01010000),	/* [27]		Character: r */
	(unsigned char)~(0b00101101),	/* [28]		Character: S */
	(unsigned char)~(0b01111000),	/* [29]		Character: t */
	(unsigned char)~(0b00011100),	/* [30]		Character: U */
	(unsigned char)~(0b00011100),	/* [31]		Character: v */
	(unsigned char)~(0b00111110),	/* [32]		Character: W */
	(unsigned char)~(0b00010100),	/* [33]		Character: x */
	(unsigned char)~(0b01101110),	/* [34]		Character: y */
	(unsigned char)~(0b00011011),	/* [35]		Character: Z */
	(unsigned char)~(0b01000000),	/* [36]		Character: - */
	(unsigned char)~(0b00000000)	/* [37]		Character:   */
};

void SP10281_Init()
{
	/* Setup GPIOs */
	GPIO_SP10281_DIGIT_DDR |= GPIO_SP10281_DIGIT_MASK;	// Set GPIO output pins for digit control
	GPIO_SP10281_SEGMENT_DDR = 0xFF;					// Set GPIO output pins for individual segments
	
	SP10281_WriteNumber(123);
}

void SP10281_Update()
{
	static unsigned char tmp = 0;

	/* Increment digit counter */
	if (++SP10281_DigitCnt >= SP10281_NUMBER_OF_DIGITS)
		SP10281_DigitCnt = 0;

	/* Activate next digit */
	tmp = (1 << (SP10281_DigitCnt + GPIO_SP10281_DIGIT0_PIN));
	GPIO_SP10281_DIGIT_PORT |= GPIO_SP10281_DIGIT_MASK;
	GPIO_SP10281_DIGIT_PORT &= ~tmp;

	/* Update segments */
	GPIO_SP10281_SEGMENT_PORT = SP10281_DigitData[SP10281_DigitCnt];
}

void SP10281_WriteDigit(unsigned char dig, char data, unsigned char dot)
{
	if (data >= '0' && data <= '9')
	{
		data -= '0';
	}
	else if (data >= 'A' && data <= 'Z')
	{
		data -= 'A';
		data += 10;
	}
	else if (data >= 'a' && data <= 'z')
	{
		data -= 'a';
		data += 10;
	}
	else if (data == '-')
	{
		data = 36;
	}
	else if (data == ' ')
	{
		data = 37;
	}
	else
	{
		return;
	}
	
	SP10281_DigitData[dig] = SP10281_DigitTable[(unsigned char)data];
	
	/* Set or clear dot */
	if (dot)
	SP10281_DigitData[dig] &= 0b01111111;
	else
	SP10281_DigitData[dig] |= 0b10000000;
}

void SP10281_WriteChar(char d2, char d1, char d0, unsigned char dot2, unsigned char dot1, unsigned char dot0)
{
	SP10281_WriteDigit(SP10281_DIGIT_0, d0, dot0);
	SP10281_WriteDigit(SP10281_DIGIT_1, d1, dot1);
	SP10281_WriteDigit(SP10281_DIGIT_2, d2, dot2);
}

/* Writes a number between -99 and 999 to the display */
void SP10281_WriteNumber(short num)
{
	short tmp = 0;
	
	if (num >= -99 && num <= 999)
	{
		if (num < 0)
		{
			SP10281_WriteDigit(SP10281_DIGIT_2, '-', 0);
			num = -num;
		}
		else
		{
			tmp = num / 100;
			tmp += '0';
			SP10281_WriteDigit(SP10281_DIGIT_2, tmp, 0);
			num %= 100;
		}
		
		tmp = num / 10;
		tmp += '0';
		SP10281_WriteDigit(SP10281_DIGIT_1, tmp, 0);
		num %= 10;
		
		tmp = num + '0';
		SP10281_WriteDigit(SP10281_DIGIT_0, tmp, 0);
	}
}
