/*
 * Segment7.cpp
 *
 * Created: 09-02-2020 14:48:32
 *  Author: Esben
 */ 

#include "Segment7.h"
#include "MCP23017.h"
#include "System.h"

volatile const unsigned char Segment_7_DigitTable[38] = {
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
	(unsigned char)~(0b01110110),	/* [33]		Character: X */
	(unsigned char)~(0b01101110),	/* [34]		Character: y */
	(unsigned char)~(0b00011011),	/* [35]		Character: Z */
	(unsigned char)~(0b01000000),	/* [36]		Character: - */
	(unsigned char)~(0b00000000)	/* [37]		Character:   */
};


void Segment7_WriteDigit(uint8_t dig, char data, uint8_t dot)
{
	unsigned char i = data;
	
	if (data >= '0' && data <= '9')
	{
		i -= '0';
	}
	else if (data >= 'A' && data <= 'Z')
	{
		i -= 'A';
		i += 10;
	}
	else if (data >= 'a' && data <= 'z')
	{
		i -= 'a';
		i += 10;
	}
	else if (data == '-')
	{
		i = 36;
	}
	else if (data == ' ')
	{
		i = 37;
	}
	else
	{
		return;
	}
	
	data = Segment_7_DigitTable[i];
	
	/* Set or clear dot */
	if (dot)
	data &= 0b01111111;
	else
	data |= 0b10000000;
	
	if (dig == 0)
	{
		MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_0_1, OLATB, data);
	}
	else if (dig == 1)
	{
		MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_0_1, OLATA, data);
	}
	else if (dig == 2)
	{
		MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_2_3, OLATB, data);
	}
	else if (dig == 3)
	{
		MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_2_3, OLATA, data);
	}
}

void Segment7_WriteAll(char d3, char d2, char d1, char d0, uint8_t dot3, uint8_t dot2, uint8_t dot1, uint8_t dot0)
{
	Segment7_WriteDigit(0, d0, dot0);
	Segment7_WriteDigit(1, d1, dot1);
	Segment7_WriteDigit(2, d2, dot2);
	Segment7_WriteDigit(3, d3, dot3);
}
