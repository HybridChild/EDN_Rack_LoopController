
#define F_CPU 20000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "RotaryEncoder.h"
#include "SP10281_3x7segment.h"
#include "i2cmaster.h"
#include "MCP23017.h"
#include "System.h"

void Init_IO_Expanders();

int main(void)
{
	Timer0_Init();
	RotaryEncoder_Init();
	SP10281_Init();
	i2c_init();
	
	sei();
	
	Init_IO_Expanders();
	System_Init();
	
    /* Main loop */
    while (1) 
    {
		if (Timer0_TickFlag)
		{
			Timer0_TickFlag = 0;
		}
		
		/* Service 7-segment display */
		if (SP10281_OvfFlag)
		{
			SP10281_OvfFlag = 0;
			SP10281_Update();
		}
		
		System_Run();
    }
	
	return 0;
}

void Init_IO_Expanders()
{
	MCP23017_WriteReg(MCP23017_ADDR_UI_LEDS, IODIRA, 0x00);    // Set Port A to output
	MCP23017_WriteReg(MCP23017_ADDR_UI_LEDS, IODIRB, 0x00);    // Set Port B to output
}