
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "UART.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* Size of the circular receive/transmit buffers, must be power of 2 */
#define UART_RX_BUFFER_SIZE 32
#define UART_TX_BUFFER_SIZE 32

/* size of RX/TX buffers */
#define UART_RX_BUFFER_MASK	( UART_RX_BUFFER_SIZE - 1)
#define UART_TX_BUFFER_MASK	( UART_TX_BUFFER_SIZE - 1)

/* module global variables */
static volatile unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART_TxHead;
static volatile unsigned char UART_TxTail;
static volatile unsigned char UART_RxHead;
static volatile unsigned char UART_RxTail;
static volatile unsigned char UART_LastRxError;


/*************************************************************************
Function: UART_Init()
Purpose:  initialize UART0 and set baudrate
Input:    baudrate, stop bits, parity
Returns:  none
**************************************************************************/
void UART_Init(unsigned int baudrate, unsigned char stopbit, unsigned char parity)
{
	DDRD |= (1 << PORTD1);
	PORTD |= (1 << PORTD1);
	
	UART_TxHead = 0;
	UART_TxTail = 0;
	UART_RxHead = 0;
	UART_RxTail = 0;
	
	UART_RX_Flag = 0;
	
	baudrate = UART_BAUD_SELECT(baudrate, F_CPU);
	
	/*Set baud rate */
	UBRR0H = (unsigned char)(baudrate >> 8);
	UBRR0L = (unsigned char)baudrate;
	
	/* Set USART Mode */
	UCSR0C &= ~((1 << UMSEL01) | (1 << UMSEL00));		// Asynchronous USART
	
	/* Set Stop Bit(s) */
	if (stopbit == UART_2_STOP_BITS)
		UCSR0C |= (1 << USBS0);							// 2 Stop bits
	else
		UCSR0C &= ~(1 << USBS0);						// 1 Stop bit

	/* Set Parity */
	if (parity == UART_EVEN_PARITY)
	{
		UCSR0C |= (1 << UPM01);							// Even parity
	}
	else if (parity == UART_ODD_PARITY)
	{
		UCSR0C |= ((1 << UPM01) | (1 << UPM00));		// Odd parity
	}
	else
		UCSR0C &= ~((1 << UPM01) | (1 << UPM00));		// No parity
	
	/* Set Data Bits */
	UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00));			// 8 Data bits

	/* Enable interrupts */
	UCSR0B |= ((1 << RXCIE0) | (1 << UDRIE0));
	
	/*Enable receiver and transmitter */
	UCSR0B |= ((1 << RXEN0) | (1 << TXEN0));
}


/*************************************************************************
Function: UART_GetChar()
Purpose:  return byte from ringbuffer  
Returns:  lower byte:  received byte from ringbuffer
          higher byte: last receive error
**************************************************************************/
unsigned int UART_GetChar(void)
{
	unsigned char tmptail;
	unsigned char data;

	if ( UART_RxHead == UART_RxTail )
	{
		return UART_NO_DATA;   /* no data available */
	}
    
	/* calculate /store buffer index */
	tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
	UART_RxTail = tmptail;
    
	/* get data from receive buffer */
	data = UART_RxBuf[tmptail];
    
	return (UART_LastRxError << 8) + data;
}


/*************************************************************************
Function: UART_PutChar()
Purpose:  write byte to ringbuffer for transmitting via UART
Input:    byte to be transmitted
Returns:  none
**************************************************************************/
void UART_PutChar(unsigned char data)
{
	unsigned char tmphead;
    
	/* calculate buffer index */
	tmphead  = (UART_TxHead + 1) & UART_TX_BUFFER_MASK;
    
	/* wait for free space in buffer */
	while ( tmphead == UART_TxTail );
    
	UART_TxBuf[tmphead] = data;
	UART_TxHead = tmphead;

	/* enable UDRE interrupt */
	UCSR0B |= (1 << UDRIE0);
}


/*************************************************************************
Function: UART_PutString()
Purpose:  transmit string to UART
Input:    string to be transmitted
Returns:  none
**************************************************************************/
void UART_PutString(const char *s )
{
    while (*s)
	{
		UART0_PutChar(*s++);
	}
}


/*************************************************************************
Function: UART_PutString_p()
Purpose:  transmit string from program memory to UART
Input:    program memory string to be transmitted
Returns:  none
**************************************************************************/
void UART_PutString_p(const char *progmem_s )
{
	register char c;
    
	while ( (c = pgm_read_byte(progmem_s++)) )
	{
		UART_PutChar(c);
	}
}


/*************************************************************************
Function: UART_Available()
Purpose:  Determine the number of bytes waiting in the receive buffer
Input:    None
Returns:  Integer number of bytes in the receive buffer
**************************************************************************/
int UART_Available(void)
{
	return (UART_RX_BUFFER_MASK + UART_RxHead - UART_RxTail) % UART_RX_BUFFER_MASK;
}


/*************************************************************************
Function: UART_Flush()
Purpose:  Flush bytes waiting the receive buffer. Acutally ignores them.
Input:    None
Returns:  None
**************************************************************************/
void UART0_Flush(void)
{
	UART_RxHead = UART_RxTail;
}


/*************************************************************************
Function: UART Receive Complete interrupt
Purpose:  called when the UART has received a character
**************************************************************************/
ISR(USART_RX_vect)
{
	unsigned char tmphead;
	unsigned char data;
	unsigned char usr;
	unsigned char lastRxError;
    
    
	/* read UART status register and UART data register */
	usr  = UCSR0A;
	data = UDR0;
    
	/* */
	lastRxError = (usr & (FE0 | DOR0));
    
	/* calculate buffer index */
	tmphead = ( UART_RxHead + 1) & UART_RX_BUFFER_MASK;
    
	if ( tmphead == UART_RxTail )
	{
		/* error: receive buffer overflow */
		lastRxError = UART_BUFFER_OVERFLOW >> 8;
	}
	else
	{
		/* store new index */
		UART_RxHead = tmphead;
		/* store received data in buffer */
		UART_RxBuf[tmphead] = data;
	}

	UART_LastRxError = lastRxError;
	
	UART_RX_Flag = 1;
}


/*************************************************************************
Function: UART Data Register Empty interrupt
Purpose:  called when the UART is ready to transmit the next byte
**************************************************************************/
ISR(USART_UDRE_vect)
{
	unsigned char tmptail;
    
	if ( UART_TxHead != UART_TxTail)
	{
		/* calculate and store new buffer index */
		tmptail = (UART_TxTail + 1) & UART_TX_BUFFER_MASK;
		UART_TxTail = tmptail;

		/* get one byte from buffer and write it to UART */
		UDR0 = UART_TxBuf[tmptail];  /* start transmission */
	}
	else
	{
		/* tx buffer empty, disable UDRE interrupt */
		UCSR0B &= ~(1 << UDRIE0);
	}
}