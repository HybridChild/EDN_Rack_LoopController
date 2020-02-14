/*
 * Segment7.cpp
 *
 * Created: 09-02-2020 14:48:32
 *  Author: Esben
 */ 

#include "Segment7.h"
#include "MCP23017.h"
#include "System.h"

volatile const unsigned char Segment7_DigitTable[128] = {
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
	(unsigned char)~(0b00000000),	/* [34]		Character: " */
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
	(unsigned char)~(0b01100100),	/* [47]		Character: / */
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
	(unsigned char)~(0b00000000),	/* [92]		Character: \ */
	(unsigned char)~(0b00001111),	/* [93]		Character: ] */
	(unsigned char)~(0b00000000),	/* [94]		Character: ^ */
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


void Segment7_WriteSingle(uint8_t dig, char chr)
{			
	if (dig == 0)
	{
		MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_0_1, OLATB, Segment7_DigitTable[(uint8_t)chr]);
	}
	else if (dig == 1)
	{
		MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_0_1, OLATA, Segment7_DigitTable[(uint8_t)chr]);
	}
	else if (dig == 2)
	{
		MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_2_3, OLATB, Segment7_DigitTable[(uint8_t)chr]);
	}
	else if (dig == 3)
	{
		MCP23017_WriteReg(MCP23017_ADDR_7SEG_DIGIT_2_3, OLATA, Segment7_DigitTable[(uint8_t)chr]);
	}
}

void Segment7_WriteAll(char c3, char c2, char c1, char c0)
{
	Segment7_WriteSingle(0, c0);
	Segment7_WriteSingle(1, c1);
	Segment7_WriteSingle(2, c2);
	Segment7_WriteSingle(3, c3);
}
