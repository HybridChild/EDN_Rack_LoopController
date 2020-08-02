#include <avr/io.h>
#include <avr/interrupt.h>
#include "InputCapture.h"

/* Defines */
#define TIMER1_TOP 0xFFFF

/* Size of the circular count buffer, must be power of 2 */
#define INPUT_CAPTURE_BUFFER_SIZE   8
/* Mask for calculating count buffer head/tail */
#define INPUT_CAPTURE_BUFFER_MASK	( INPUT_CAPTURE_BUFFER_SIZE - 1)

/* Local variables */
volatile uint32_t CountBuffer[INPUT_CAPTURE_BUFFER_SIZE] = {0};
volatile uint8_t BufHead = 0;
volatile uint8_t BufTail = 0;
volatile uint32_t newCnt = 0;
volatile uint32_t lastCnt = 0;
volatile uint32_t Timer1_OvfCnt = 0;

/* Function implementations */

void InputCapture_Init()
{
	DDRD &= ~(1 << PIND6);		// Set PortD 6 (ICP1) to input
	TCCR1B &= ~(1 << ICES1);	// Input Capture - Falling-edge trigger
	TCCR1B |= (1 << ICNC1);		// Input Capture - Enable Noise Canceler

	TIMSK1 |= (1 << TOIE1);		// Enable Counter/Timer1 Overflow interrupt
}

void InputCapture_Enable()
{
	TCCR1B |= (1 << CS10);		// Counter/Timer1 Clock source - No prescaling
	TIFR1 |= ((1 << ICF1) | (1 << TOV1));		// Clear pending interrupt
	TIMSK1 |= (1 << ICIE1);		// Enable Input Capture interrupt
}

void InputCapture_Disable()
{
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));	// Counter/Timer1 Clock source - None (Counter/Timer1 stopped)
	TIMSK1 &= ~(1 << ICIE1);	// Disable Input Capture interrupt
}

bool InputCapture_Available()
{
	return BufHead != BufTail;
}

uint32_t InputCapture_GetCount()
{
	uint8_t tmpTail = BufTail;
	BufTail = (BufTail + 1) & INPUT_CAPTURE_BUFFER_MASK;
	return CountBuffer[tmpTail];
}

/* Input Capture Interrupt Service Routine */
ISR(TIMER1_CAPT_vect)
{
	/* Read count register */
	newCnt = ICR1;

	/* Store counts since last input event */
	CountBuffer[BufHead] = newCnt + (Timer1_OvfCnt*TIMER1_TOP) - lastCnt;

	lastCnt = newCnt;
	Timer1_OvfCnt = 0;

	/* Calculate new buffer head index */
	BufHead = (BufHead + 1) & INPUT_CAPTURE_BUFFER_MASK;

	/* Check for buffer overflow */
	if ( BufHead == BufTail )
	{
		/* Calculate new buffer tail index (ignore overflow)*/
		BufTail = (BufTail + 1) & INPUT_CAPTURE_BUFFER_MASK;
	}
}

/* Timer1 overflow Interrupt Service Routine */
ISR(TIMER1_OVF_vect)
{
	Timer1_OvfCnt++;
}
