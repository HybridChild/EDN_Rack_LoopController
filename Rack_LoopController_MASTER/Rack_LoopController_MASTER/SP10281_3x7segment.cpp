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

volatile const unsigned char SP10281_DigitTable[128] = {
	(unsigned char)~(0b00000000),	/* [0]		Character: NULL */
	(unsigned char)~(0b00000000),	/* [1]		Character: SOH */
	(unsigned char)~(0b00000000),	/* [2]		Character: STX */
	(unsigned char)~(0b00000000),	/* [3]		Character: ETX */
	(unsigned char)~(0b00000000),	/* [4]		Character: EOT */
	(unsigned char)~(0b00000000),	/* [5]		Character: ENQ */
	(unsigned char)~(0b00000000),	/* [6]		Character: ACK */
	(unsigned char)~(0b00000000),	/* [7]		Character: BEL */
	(unsigned char)~(0b00000000),	/* [8]		Character: BS */
	(unsigned char)~(0b00000000),	/* [9]		Character: TAB */
	(unsigned char)~(0b00000000),	/* [10]		Character: LF */
	(unsigned char)~(0b00000000),	/* [11]		Character: VT */
	(unsigned char)~(0b00000000),	/* [12]		Character: FF */
	(unsigned char)~(0b00000000),	/* [13]		Character: CR */
	(unsigned char)~(0b00000000),	/* [14]		Character: SO */
	(unsigned char)~(0b00000000),	/* [15]		Character: SI */
	(unsigned char)~(0b00000000),	/* [16]		Character: DLE */
	(unsigned char)~(0b00000000),	/* [17]		Character: DC1 */
	(unsigned char)~(0b00000000),	/* [18]		Character: DC2 */
	(unsigned char)~(0b00000000),	/* [19]		Character: DC3 */
	(unsigned char)~(0b00000000),	/* [20]		Character: DC4 */
	(unsigned char)~(0b00000000),	/* [21]		Character: NAK */
	(unsigned char)~(0b00000000),	/* [22]		Character: SYN */
	(unsigned char)~(0b00000000),	/* [23]		Character: ETB */
	(unsigned char)~(0b00000000),	/* [24]		Character: CAN */
	(unsigned char)~(0b00000000),	/* [25]		Character: EM */
	(unsigned char)~(0b00000000),	/* [26]		Character: SUB */
	(unsigned char)~(0b00000000),	/* [27]		Character: ESC */
	(unsigned char)~(0b00000000),	/* [28]		Character: FS */
	(unsigned char)~(0b00000000),	/* [29]		Character: GS */
	(unsigned char)~(0b00000000),	/* [30]		Character: RS */
	(unsigned char)~(0b00000000),	/* [31]		Character: US */
	(unsigned char)~(0b00000000),	/* [32]		Character: Space */
	(unsigned char)~(0b00000000),	/* [33]		Character: ! */
	(unsigned char)~(0b00100010),	/* [34]		Character: " */
	(unsigned char)~(0b00000000),	/* [35]		Character: # */
	(unsigned char)~(0b00000000),	/* [36]		Character: $ */
	(unsigned char)~(0b00000000),	/* [37]		Character: % */
	(unsigned char)~(0b00000000),	/* [38]		Character: & */
	(unsigned char)~(0b00000000),	/* [39]		Character: ' */
	(unsigned char)~(0b00111001),	/* [40]		Character: ( */
	(unsigned char)~(0b00001111),	/* [41]		Character: ) */
	(unsigned char)~(0b00000000),	/* [42]		Character: * */
	(unsigned char)~(0b00000000),	/* [43]		Character: + */
	(unsigned char)~(0b00000000),	/* [44]		Character: ´ */
	(unsigned char)~(0b01000000),	/* [45]		Character: - */
	(unsigned char)~(0b00000000),	/* [46]		Character: . */
	(unsigned char)~(0b01010010),	/* [47]		Character: / */
	(unsigned char)~(0b00111111),	/* [48]		Character: 0 */
	(unsigned char)~(0b00000110),	/* [49]		Character: 1 */
	(unsigned char)~(0b01011011),	/* [50]		Character: 2 */
	(unsigned char)~(0b01001111),	/* [51]		Character: 3 */
	(unsigned char)~(0b01100110),	/* [52]		Character: 4 */
	(unsigned char)~(0b01101101),	/* [53]		Character: 5 */
	(unsigned char)~(0b01111101),	/* [54]		Character: 6 */
	(unsigned char)~(0b00000111),	/* [55]		Character: 7 */
	(unsigned char)~(0b01111111),	/* [56]		Character: 8 */
	(unsigned char)~(0b01101111),	/* [57]		Character: 9 */
	(unsigned char)~(0b00000000),	/* [58]		Character: : */
	(unsigned char)~(0b00000000),	/* [59]		Character: ; */
	(unsigned char)~(0b00000000),	/* [60]		Character: < */
	(unsigned char)~(0b00000000),	/* [61]		Character: = */
	(unsigned char)~(0b00000000),	/* [62]		Character: > */
	(unsigned char)~(0b00000000),	/* [63]		Character: ? */
	(unsigned char)~(0b00000000),	/* [64]		Character: @ */
	(unsigned char)~(0b01110111),	/* [65]		Character: A */
	(unsigned char)~(0b01111100),	/* [66]		Character: B */
	(unsigned char)~(0b00111001),	/* [67]		Character: C */
	(unsigned char)~(0b01011110),	/* [68]		Character: D */
	(unsigned char)~(0b01111001),	/* [69]		Character: E */
	(unsigned char)~(0b01110001),	/* [70]		Character: F */
	(unsigned char)~(0b00111101),	/* [71]		Character: G */
	(unsigned char)~(0b01110110),	/* [72]		Character: H */
	(unsigned char)~(0b00000110),	/* [73]		Character: I */
	(unsigned char)~(0b00011110),	/* [74]		Character: J */
	(unsigned char)~(0b01110101),	/* [75]		Character: K */
	(unsigned char)~(0b00111000),	/* [76]		Character: L */
	(unsigned char)~(0b00010101),	/* [77]		Character: M */
	(unsigned char)~(0b01010100),	/* [78]		Character: N */
	(unsigned char)~(0b00111111),	/* [79]		Character: O */
	(unsigned char)~(0b01110011),	/* [80]		Character: P */
	(unsigned char)~(0b01100111),	/* [81]		Character: Q */
	(unsigned char)~(0b01010000),	/* [82]		Character: R */
	(unsigned char)~(0b00101101),	/* [83]		Character: S */
	(unsigned char)~(0b01111000),	/* [84]		Character: T */
	(unsigned char)~(0b00111110),	/* [85]		Character: U */
	(unsigned char)~(0b00111110),	/* [86]		Character: V */
	(unsigned char)~(0b00101010),	/* [87]		Character: W */
	(unsigned char)~(0b01110110),	/* [88]		Character: X */
	(unsigned char)~(0b01101110),	/* [89]		Character: Y */
	(unsigned char)~(0b00011011),	/* [90]		Character: Z */
	(unsigned char)~(0b00111001),	/* [91]		Character: [ */
	(unsigned char)~(0b01100100),	/* [92]		Character: \ */
	(unsigned char)~(0b00001111),	/* [93]		Character: ] */
	(unsigned char)~(0b00000001),	/* [94]		Character: ^ */
	(unsigned char)~(0b00001000),	/* [95]		Character: _ */
	(unsigned char)~(0b00000000),	/* [96]		Character: ` */
	(unsigned char)~(0b01110111),	/* [97]		Character: a */
	(unsigned char)~(0b01111100),	/* [98]		Character: b */
	(unsigned char)~(0b01011000),	/* [99]		Character: c */
	(unsigned char)~(0b01011110),	/* [100]	Character: d */
	(unsigned char)~(0b01111001),	/* [101]	Character: e */
	(unsigned char)~(0b01110001),	/* [102]	Character: f */
	(unsigned char)~(0b00111101),	/* [103]	Character: g */
	(unsigned char)~(0b01110100),	/* [104]	Character: h */
	(unsigned char)~(0b00000100),	/* [105]	Character: i */
	(unsigned char)~(0b00011110),	/* [106]	Character: j */
	(unsigned char)~(0b01110101),	/* [107]	Character: k */
	(unsigned char)~(0b00111000),	/* [108]	Character: l */
	(unsigned char)~(0b00010101),	/* [109]	Character: m */
	(unsigned char)~(0b01010100),	/* [110]	Character: n */
	(unsigned char)~(0b01011100),	/* [111]	Character: o */
	(unsigned char)~(0b01110011),	/* [112]	Character: p */
	(unsigned char)~(0b01100111),	/* [113]	Character: q */
	(unsigned char)~(0b01010000),	/* [114]	Character: r */
	(unsigned char)~(0b00101101),	/* [115]	Character: s */
	(unsigned char)~(0b01111000),	/* [116]	Character: t */
	(unsigned char)~(0b00011100),	/* [117]	Character: u */
	(unsigned char)~(0b00011100),	/* [118]	Character: v */
	(unsigned char)~(0b00111110),	/* [119]	Character: w */
	(unsigned char)~(0b01110110),	/* [120]	Character: x */
	(unsigned char)~(0b01101110),	/* [121]	Character: y */
	(unsigned char)~(0b00011011),	/* [122]	Character: z */
	(unsigned char)~(0b00000000),	/* [123]	Character: { */
	(unsigned char)~(0b00000000),	/* [124]	Character: | */
	(unsigned char)~(0b00000000),	/* [125]	Character: } */
	(unsigned char)~(0b00000000),	/* [126]	Character: ~ */
	(unsigned char)~(0b00000000)	/* [127]	Character: DEL */
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

void SP10281_WriteSingle(unsigned char dig, char data, unsigned char dot)
{	
	SP10281_DigitData[dig] = SP10281_DigitTable[(unsigned char)data];
	
	/* Set or clear dot */
	if (dot)
		SP10281_DigitData[dig] &= 0b01111111;
	else
		SP10281_DigitData[dig] |= 0b10000000;
}

void SP10281_WriteAll(char c2, char c1, char c0, unsigned char dot2, unsigned char dot1, unsigned char dot0)
{
	SP10281_WriteSingle(SP10281_DIGIT_0, c0, dot0);
	SP10281_WriteSingle(SP10281_DIGIT_1, c1, dot1);
	SP10281_WriteSingle(SP10281_DIGIT_2, c2, dot2);
}

/* Writes a number between -99 and 999 to the display */
void SP10281_WriteNumber(short num)
{
	short tmp = 0;
	
	if (num >= -99 && num <= 999)
	{
		if (num < 0)
		{
			SP10281_WriteSingle(SP10281_DIGIT_2, '-', 0);
			num = -num;
		}
		else
		{
			tmp = num / 100;
			tmp += '0';
			SP10281_WriteSingle(SP10281_DIGIT_2, tmp, 0);
			num %= 100;
		}
		
		tmp = num / 10;
		tmp += '0';
		SP10281_WriteSingle(SP10281_DIGIT_1, tmp, 0);
		num %= 10;
		
		tmp = num + '0';
		SP10281_WriteSingle(SP10281_DIGIT_0, tmp, 0);
	}
}
