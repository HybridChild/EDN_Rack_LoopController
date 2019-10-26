
#include <avr/interrupt.h>
#include "MCP23017.h"
#include "i2cmaster.h"

void MCP23017_WriteReg(unsigned char chip_addr, unsigned char reg, unsigned char val)
{
    unsigned char start_byte = (((START_BYTE_MASK | chip_addr) << 1) + I2C_WRITE);
    
    i2c_start(start_byte);		// Set device address and write mode
    i2c_write(reg);             // Set MCP23017 memory pointer
    i2c_write(val);             // Write value to register
    i2c_stop();                 // Set stop conditon = release bus
}

unsigned char MCP23017_ReadReg(unsigned char chip_addr, unsigned char reg)
{
    unsigned char start_byte = (((START_BYTE_MASK | chip_addr) << 1) + I2C_WRITE);

    i2c_start(start_byte);		// Set device address and write mode
    i2c_write(reg);             // Set MCP23017 memory pointer
    i2c_stop();                 // Set stop conditon = release bus

    start_byte |= I2C_READ;

    i2c_start(start_byte);		// Set device address and write mode
    return i2c_readNak();       // Read byte and send stop condition
}