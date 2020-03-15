/*
 * MCP23017.cpp
 *
 * Created: 19-01-2020 21:25:51
 *  Author: Esben
 */ 

#include "MCP23017.h"
#include "i2cmaster.h"
#include "Timer.h"

#define START_BYTE_MASK    0x20

volatile uint16_t MCP_Output::AutoToggle_OvfCnt = 0;
volatile uint16_t MCP_Output::AutoToggle_Timeout = 100;
volatile uint8_t MCP_Output::AutoToggle_Flag = 0;
volatile uint8_t MCP_Output::AutoToggle_PortA = 0;
volatile uint8_t MCP_Output::AutoToggle_PortB = 0;

void MCP23017_WriteReg(unsigned char chip_addr, unsigned char reg, unsigned char val)
{
	unsigned char start_byte = (((START_BYTE_MASK | chip_addr) << 1) + I2C_WRITE);
	
	i2c_start(start_byte);		// Set device address and write mode
	i2c_write(reg);             // Set MCP23017 memory pointer
	i2c_write(val);             // Write value to register
	i2c_stop();                 // Set stop condition = release bus
}

unsigned char MCP23017_ReadReg(unsigned char chip_addr, unsigned char reg)
{
	unsigned char start_byte = (((START_BYTE_MASK | chip_addr) << 1) + I2C_WRITE);

	i2c_start(start_byte);		// Set device address and write mode
	i2c_write(reg);             // Set MCP23017 memory pointer
	i2c_stop();                 // Set stop condition = release bus

	start_byte |= I2C_READ;

	i2c_start(start_byte);		// Set device address and write mode
	return i2c_readNak();       // Read byte and send stop condition
}

MCP_Output::MCP_Output()
{

}

MCP_Output::MCP_Output(uint8_t adrr, MCP23017_Port port, MCP23017_Pin pin)
{
	/* Initialize members */
	Addr = adrr;
	Port = port;
	Pin = pin;
	
	/* Set Port pin to output */
	unsigned char val;
	unsigned char reg;
	
	if (Port == PortA)
	{
		reg = IODIRA;
	}
	else
	{
		reg = IODIRB;
	}
	
	val = MCP23017_ReadReg((unsigned char)Addr, reg);
	val &= ~(1 << Pin);
	MCP23017_WriteReg(Addr, reg, val);
	
	this->Clear();
}

uint8_t MCP_Output::getState()
{
	return (uint8_t)State;
}

void MCP_Output::Set()
{
	unsigned char val;
	unsigned char reg;
	
	if (Port == PortA)
	{
		reg = OLATA;
		MCP_Output::AutoToggle_PortA &= ~(1 << Pin);
	}
	else
	{
		reg = OLATB;
		MCP_Output::AutoToggle_PortB &= ~(1 << Pin);
	}
	
	val = MCP23017_ReadReg((unsigned char)Addr, reg);
	val |= (1 << Pin);
	MCP23017_WriteReg((unsigned char)Addr,  reg, val);
	
	State = SET;
}

void MCP_Output::Clear()
{
	unsigned char val;
	unsigned char reg;
		
	if (Port == PortA)
	{
		reg = OLATA;
		MCP_Output::AutoToggle_PortA &= ~(1 << Pin);
	}
	else
	{
		reg = OLATB;
		MCP_Output::AutoToggle_PortB &= ~(1 << Pin);
	}
		
	val = MCP23017_ReadReg((unsigned char)Addr, reg);
	val &= ~(1 << Pin);
	MCP23017_WriteReg((unsigned char)Addr,  reg, val);
		
	State = CLEARED;
}

void MCP_Output::Toggle()
{
	if (State == CLEARED)
	{
		this->Set();
	} 
	else if (State == SET)
	{
		this->Clear();
	}
}

void MCP_Output::AutoToggle()
{
	State = AUTO_TOGGLE;
	
	if (Port == PortA)
	{
		MCP_Output::AutoToggle_PortA |= (1 << Pin);
	}
	else
	{
		MCP_Output::AutoToggle_PortB |= (1 << Pin);
	}
}

void MCP_Output::SetAutoToggleSpeed(uint16_t ms)
{
	AutoToggle_Timeout = (uint16_t)Timer0_ms2cnt(ms);
}

void MCP_Output::PerformAutoToggle(uint8_t addr)
{
	uint8_t valA = MCP23017_ReadReg(addr, OLATA);
	uint8_t valB = MCP23017_ReadReg(addr, OLATB);
	
	valA ^= MCP_Output::AutoToggle_PortA;
	valB ^= MCP_Output::AutoToggle_PortB;
	
	MCP23017_WriteReg(addr, OLATA, valA);
	MCP23017_WriteReg(addr, OLATB, valB);
}

MCP_Input::MCP_Input()
{
	
}

MCP_Input::MCP_Input(uint8_t adrr, MCP23017_Port port, MCP23017_Pin pin)
{
	/* Initialize members */
	Addr = adrr;
	Port = port;
	Pin = pin;
	
	/* Set Port pin to output */
	unsigned char val;
	unsigned char reg;
	
	if (Port == PortA)
	{
		reg = IODIRA;
	}
	else
	{
		reg = IODIRB;
	}
	
	val = MCP23017_ReadReg((unsigned char)Addr, reg);
	val |= (1 << Pin);
	MCP23017_WriteReg(Addr, reg, val);
}

bool MCP_Input::Read()
{
	unsigned char val;
	unsigned char reg;
	
	if (Port == PortA)
	{
		reg = GPIOA;
	}
	else
	{
		reg = GPIOB;
	}
	
	val = MCP23017_ReadReg((unsigned char)Addr, reg);
	val &= (1 << Pin);
	
	return (bool)val;
}
