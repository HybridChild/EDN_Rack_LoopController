
#ifndef __MCP23017_h_included__
#define __MCP23017_h_included__

#include <stdint-gcc.h>

#define START_BYTE_MASK    0x20

#define IODIRA    0x00
#define IODIRB    0x01
#define IPOLA     0x02
#define IPOLB     0x03
#define GPINTENA  0x04
#define GPINTENB  0x05
#define DEFVALA   0x06
#define DEFVALB   0x07
#define INTCONA   0x08
#define INTCONB   0x09
#define IOCONA    0x0A
#define IOCONB    0x0B
#define GPPUA     0x0C
#define GPPUB     0x0D
#define INTFA     0x0E
#define INTFB     0x0F
#define INTCAPA   0x10
#define INTCAPB   0x11
#define GPIOA     0x12
#define GPIOB     0x13
#define OLATA     0x14
#define OLATB     0x15

void MCP23017_WriteReg(unsigned char chip_addr, unsigned char reg, unsigned char val);
unsigned char MCP23017_ReadReg(unsigned char chip_addr, unsigned char reg);

typedef enum MCP23017_Port {PortA, PortB} MCP23017_Port;
typedef enum MCP23017_Pin {Pin0, Pin1, Pin2, Pin3, Pin4, Pin5, Pin6, Pin7} MCP23017_Pin;

class MCP_Output
{
public:
	typedef enum Output_State {CLEARED = 0, SET, AUTO_TOGGLE} Output_State;
	
	volatile static uint16_t AutoToggle_OvfCnt;
	volatile static uint16_t AutoToggle_Timeout;
	volatile static uint8_t AutoToggle_Flag;
	volatile static uint8_t AutoToggle_PortA;
	volatile static uint8_t AutoToggle_PortB;
	
	MCP_Output();
	MCP_Output(uint8_t adrr, MCP23017_Port port, MCP23017_Pin pin);
	uint8_t getState();
	void Set();
	void Clear();
	void Toggle();
	void AutoToggle();
	static void SetAutoToggleSpeed(uint16_t ms);
	static void PerformAutoToggle(uint8_t addr);
	
private:
	uint8_t Addr;
	MCP23017_Port Port;
	MCP23017_Pin Pin;
	Output_State State;
};

class MCP_Input
{
public:
	MCP_Input();
	MCP_Input(uint8_t adrr, MCP23017_Port port, MCP23017_Pin pin);
	bool Read();
private:
	uint8_t Addr;
	MCP23017_Port Port;
	MCP23017_Pin Pin;
};

#endif  // __MCP23017_h_included__