#include "Segment_7.h"
#include "MCP23017.h"
#include "main.h"

volatile const unsigned char Segment_7_DigitTable[38] = {
	~(0b00111111),	/* [0]		Character: 0 */
	~(0b00000110),	/* [1]		Character: 1 */
	~(0b01011011),	/* [2]		Character: 2 */
	~(0b01001111),	/* [3]		Character: 3 */
	~(0b01100110),	/* [4]		Character: 4 */
	~(0b01101101),	/* [5]		Character: 5 */
	~(0b01111101),	/* [6]		Character: 6 */
	~(0b00000111),	/* [7]		Character: 7 */
	~(0b01111111),	/* [8]		Character: 8 */
	~(0b01101111),	/* [9]		Character: 9 */
	~(0b01110111),	/* [10]		Character: A */
	~(0b01111100),	/* [11]		Character: b */
	~(0b00111001),	/* [12]		Character: C */
	~(0b01011110),	/* [13]		Character: d */
	~(0b01111001),	/* [14]		Character: E */
	~(0b01110001),	/* [15]		Character: F */
	~(0b00111101),	/* [16]		Character: G */
	~(0b01110100),	/* [17]		Character: h */
	~(0b00000100),	/* [18]		Character: i */
	~(0b00011110),	/* [19]		Character: J */
	~(0b01110101),	/* [20]		Character: k */
	~(0b00111000),	/* [21]		Character: L */
	~(0b00010101),	/* [22]		Character: M */
	~(0b01010100),	/* [23]		Character: n */
	~(0b01011100),	/* [24]		Character: o */
	~(0b01110011),	/* [25]		Character: P */
	~(0b01100111),	/* [26]		Character: q */
	~(0b01010000),	/* [27]		Character: r */
	~(0b00101101),	/* [28]		Character: S */
	~(0b01111000),	/* [29]		Character: t */
	~(0b00011100),	/* [30]		Character: U */
	~(0b00011100),	/* [31]		Character: v */
	~(0b00111110),	/* [32]		Character: W */
	~(0b01110110),	/* [33]		Character: X */
	~(0b01101110),	/* [34]		Character: y */
	~(0b00011011),	/* [35]		Character: Z */
	~(0b01000000),	/* [36]		Character: - */
	~(0b00000000)	/* [37]		Character:   */
};

void Segment_7_WriteDigit(unsigned char dig, char data, unsigned char dot)
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
		MCP23017_WriteReg(DIGIT_0_1_ADDR, OLATB, data);
	}
	else if (dig == 1)
	{
		MCP23017_WriteReg(DIGIT_0_1_ADDR, OLATA, data);
	}
	else if (dig == 2)
	{
		MCP23017_WriteReg(DIGIT_2_3_ADDR, OLATB, data);
	}
	else if (dig == 3)
	{
		MCP23017_WriteReg(DIGIT_2_3_ADDR, OLATA, data);
	}
}

void Segment_7_WriteAll(char d3, char d2, char d1, char d0, unsigned char dot3, unsigned char dot2, unsigned char dot1, unsigned char dot0)
{
	Segment_7_WriteDigit(0, d0, dot0);
	Segment_7_WriteDigit(1, d1, dot1);
	Segment_7_WriteDigit(2, d2, dot2);
	Segment_7_WriteDigit(3, d3, dot3);
}
