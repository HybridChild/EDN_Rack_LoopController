/*
 * UART.cpp
 *
 * Created: 26-01-2020 22:13:14
 *  Author: Esben
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "UART.h"


#ifndef F_CPU
#define F_CPU 20000000UL
#endif

/* Size of the circular receive/transmit buffers, must be power of 2 */
#define UART0_RX_BUFFER_SIZE 64
#define UART0_TX_BUFFER_SIZE 64
#define UART1_RX_BUFFER_SIZE 64
#define UART1_TX_BUFFER_SIZE 64

/* size of RX/TX buffers */
#define UART0_RX_BUFFER_MASK	( UART0_RX_BUFFER_SIZE - 1)
#define UART0_TX_BUFFER_MASK	( UART0_TX_BUFFER_SIZE - 1)
#define UART1_RX_BUFFER_MASK	( UART1_RX_BUFFER_SIZE - 1)
#define UART1_TX_BUFFER_MASK	( UART1_TX_BUFFER_SIZE - 1)

/* module global variables */
static volatile unsigned char UART0_TxBuf[UART0_TX_BUFFER_SIZE];
static volatile unsigned char UART0_RxBuf[UART0_RX_BUFFER_SIZE];
static volatile unsigned char UART0_TxHead;
static volatile unsigned char UART0_TxTail;
static volatile unsigned char UART0_RxHead;
static volatile unsigned char UART0_RxTail;
static volatile unsigned char UART0_LastRxError;

static volatile unsigned char UART1_TxBuf[UART1_TX_BUFFER_SIZE];
static volatile unsigned char UART1_RxBuf[UART1_RX_BUFFER_SIZE];
static volatile unsigned char UART1_TxHead;
static volatile unsigned char UART1_TxTail;
static volatile unsigned char UART1_RxHead;
static volatile unsigned char UART1_RxTail;
static volatile unsigned char UART1_LastRxError;


/*************************************************************************
Function: UART0_Init()
Purpose:  initialize UART0 and set baudrate
Input:    baudrate, stop bits, parity
Returns:  none
**************************************************************************/
void UART0_Init(unsigned int baudrate, unsigned char stopbit, unsigned char parity)
{
	DDRD |= (1 << PORTD1);
	PORTD |= (1 << PORTD1);
	
	UART0_TxHead = 0;
	UART0_TxTail = 0;
	UART0_RxHead = 0;
	UART0_RxTail = 0;
	
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
		UCSR0C &= ~(1 << UPM00);
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
Function: UART0_GetChar()
Purpose:  return byte from ringbuffer  
Returns:  lower byte:  received byte from ringbuffer
          higher byte: last receive error
**************************************************************************/
unsigned int UART0_GetChar(void)
{
	unsigned char tmptail;
	unsigned char data;

	if ( UART0_RxHead == UART0_RxTail )
	{
		return UART_NO_DATA;   /* no data available */
	}
    
	/* calculate /store buffer index */
	tmptail = (UART0_RxTail + 1) & UART0_RX_BUFFER_MASK;
	UART0_RxTail = tmptail;
    
	/* get data from receive buffer */
	data = UART0_RxBuf[tmptail];
    
	return (UART0_LastRxError << 8) + data;
}


/*************************************************************************
Function: UART0_PutChar()
Purpose:  write byte to ringbuffer for transmitting via UART
Input:    byte to be transmitted
Returns:  none
**************************************************************************/
void UART0_PutChar(unsigned char data)
{
	unsigned char tmphead;
    
	/* calculate buffer index */
	tmphead  = (UART0_TxHead + 1) & UART0_TX_BUFFER_MASK;
    
	/* wait for free space in buffer */
	while ( tmphead == UART0_TxTail );
    
	UART0_TxBuf[tmphead] = data;
	UART0_TxHead = tmphead;

	/* enable UDRE interrupt */
	UCSR0B |= (1 << UDRIE0);
}


/*************************************************************************
Function: UART0_PutString()
Purpose:  transmit string to UART
Input:    string to be transmitted
Returns:  none
**************************************************************************/
void UART0_PutString(const char *s )
{
    while (*s)
	{
		UART0_PutChar(*s++);
	}
}


/*************************************************************************
Function: UART0_PutString_p()
Purpose:  transmit string from program memory to UART
Input:    program memory string to be transmitted
Returns:  none
**************************************************************************/
void UART0_PutString_p(const char *progmem_s )
{
	register char c;
    
	while ( (c = pgm_read_byte(progmem_s++)) )
	{
		UART0_PutChar(c);
	}
}


/*************************************************************************
Function: UART0_Available()
Purpose:  Determine the number of bytes waiting in the receive buffer
Input:    None
Returns:  Integer number of bytes in the receive buffer
**************************************************************************/
int UART0_Available(void)
{
	//return (UART0_RX_BUFFER_MASK + UART0_RxHead - UART0_RxTail) % UART0_RX_BUFFER_MASK;
	return (UART0_RxHead != UART0_RxTail);
}


/*************************************************************************
Function: UART0_Flush()
Purpose:  Flush bytes waiting the receive buffer. Acutally ignores them.
Input:    None
Returns:  None
**************************************************************************/
void UART0_Flush(void)
{
	UART0_RxHead = UART0_RxTail;
}


int	UART0_QueueChar(unsigned char data)
{
	unsigned char tmphead;
	
	/* calculate buffer index */
	tmphead = (UART0_TxHead + 1) & UART0_TX_BUFFER_MASK;
	
	/* check for free space in buffer */
	if ( tmphead == UART0_TxTail )
		return 0;
	
	UART0_TxBuf[tmphead] = data;
	UART0_TxHead = tmphead;
	
	return 1;
}


void UART0_QueueArray(const char *array, unsigned char length)
{
	unsigned char i = 0;
	
	for (i = 0; i < length; i++)
	{
		UART0_QueueChar(array[i]);
	}
}


void UART0_PutQueue(void)
{
	/* enable UDRE interrupt */
	UCSR0B |= (1 << UDRIE0);
}


bool UART0_QueueIsEmpty(void)
{
	return (UART0_TxHead == UART0_TxTail);
}


/*************************************************************************
Function: UART0 Receive Complete interrupt
Purpose:  called when the UART has received a character
**************************************************************************/
ISR(USART0_RX_vect)
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
	tmphead = ( UART0_RxHead + 1) & UART0_RX_BUFFER_MASK;
    
	if ( tmphead == UART0_RxTail )
	{
		/* error: receive buffer overflow */
		lastRxError = UART_BUFFER_OVERFLOW >> 8;
	}
	else
	{
		/* store new index */
		UART0_RxHead = tmphead;
		/* store received data in buffer */
		UART0_RxBuf[tmphead] = data;
	}

	UART0_LastRxError = lastRxError;
}


/*************************************************************************
Function: UART0 Data Register Empty interrupt
Purpose:  called when the UART is ready to transmit the next byte
**************************************************************************/
ISR(USART0_UDRE_vect)
{
	unsigned char tmptail;
    
	if ( UART0_TxHead != UART0_TxTail)
	{
		/* calculate and store new buffer index */
		tmptail = (UART0_TxTail + 1) & UART0_TX_BUFFER_MASK;
		UART0_TxTail = tmptail;

		/* get one byte from buffer and write it to UART */
		UDR0 = UART0_TxBuf[tmptail];  /* start transmission */
	}
	else
	{
		/* tx buffer empty, disable UDRE interrupt */
		UCSR0B &= ~(1 << UDRIE0);
	}
}


/*************************************************************************
Function: UART1_Init()
Purpose:  initialize UART1 and set baudrate
Input:    baudrate, stop bits, parity
Returns:  none
**************************************************************************/
void UART1_Init(unsigned int baudrate, unsigned char stopbit, unsigned char parity)
{
	DDRD |= (1 << PORTD3);
	PORTD |= (1 << PORTD3);
	
	UART1_TxHead = 0;
	UART1_TxTail = 0;
	UART1_RxHead = 0;
	UART1_RxTail = 0;
	
	baudrate = UART_BAUD_SELECT(baudrate, F_CPU);
	
	/*Set baud rate */
	UBRR1H = (unsigned char)(baudrate >> 8);
	UBRR1L = (unsigned char)baudrate;
		
	/* Set USART Mode */
	UCSR1C &= ~((1 << UMSEL11) | (1 << UMSEL10));		// Asynchronous USART
		
	/* Set Stop Bit(s) */
	if (stopbit == UART_2_STOP_BITS)
		UCSR1C |= (1 << USBS1);							// 2 Stop bits
	else
		UCSR1C &= ~(1 << USBS1);						// 1 Stop bit

	/* Set Parity */
	if (parity == UART_EVEN_PARITY)
	{
		UCSR1C |= (1 << UPM11);							// Even parity
	}
	else if (parity == UART_ODD_PARITY)
	{
		UCSR1C |= ((1 << UPM11) | (1 << UPM10));		// Odd parity
	}
	else
		UCSR1C &= ~((1 << UPM11) | (1 << UPM10));		// No parity
		
	/* Set Data Bits */
	UCSR1C |= ((1 << UCSZ11) | (1 << UCSZ10));			// 8 Data bits

	/* Enable interrupts */
	UCSR1B |= ((1 << RXCIE1) | (1 << UDRIE1));
	
	/*Enable receiver and transmitter */
	UCSR1B |= ((1 << RXEN1) | (1 << TXEN1));
}


/*************************************************************************
Function: UART1_GetChar()
Purpose:  return byte from ringbuffer  
Returns:  lower byte:  received byte from ringbuffer
          higher byte: last receive error
**************************************************************************/
unsigned int UART1_GetChar(void)
{
	unsigned char tmptail;
	unsigned char data;

	if ( UART1_RxHead == UART1_RxTail )
	{
		return UART_NO_DATA;   /* no data available */
	}
		
	/* calculate /store buffer index */
	tmptail = (UART1_RxTail + 1) & UART1_RX_BUFFER_MASK;
	UART1_RxTail = tmptail;
		
	/* get data from receive buffer */
	data = UART1_RxBuf[tmptail];
		
	return (UART1_LastRxError << 8) + data;
}


/*************************************************************************
Function: UART1_PutChar()
Purpose:  write byte to ringbuffer for transmitting via UART
Input:    byte to be transmitted
Returns:  none
**************************************************************************/
void UART1_PutChar(unsigned char data)
{
	unsigned char tmphead;
		
	/* calculate buffer index */
	tmphead  = (UART1_TxHead + 1) & UART1_TX_BUFFER_MASK;
		
	/* wait for free space in buffer */
	while ( tmphead == UART1_TxTail );
		
	UART1_TxBuf[tmphead] = data;
	UART1_TxHead = tmphead;

	/* enable UDRE interrupt */
	UCSR1B |= (1 << UDRIE1);
}


/*************************************************************************
Function: UART1_PutString()
Purpose:  transmit string to UART
Input:    string to be transmitted
Returns:  none
**************************************************************************/
void UART1_PutString(const char *s )
{
	while (*s)
	{
		UART1_PutChar(*s++);
	}
}


/*************************************************************************
Function: UART1_PutString_p()
Purpose:  transmit string from program memory to UART
Input:    program memory string to be transmitted
Returns:  none
**************************************************************************/
void UART1_PutString_p(const char *progmem_s )
{
	register char c;
		
	while ( (c = pgm_read_byte(progmem_s++)) )
	{
		UART1_PutChar(c);
	}
}


/*************************************************************************
Function: UART1_Available()
Purpose:  Determine the number of bytes waiting in the receive buffer
Input:    None
Returns:  Integer number of bytes in the receive buffer
**************************************************************************/
int UART1_Available(void)
{
	//return (UART1_RX_BUFFER_MASK + UART1_RxHead - UART1_RxTail) % UART1_RX_BUFFER_MASK;
	return (UART1_RxHead != UART1_RxTail);
}


/*************************************************************************
Function: UART1_Flush()
Purpose:  Flush bytes waiting the receive buffer. Acutally ignores them.
Input:    None
Returns:  None
**************************************************************************/
void UART1_Flush(void)
{
	UART1_RxHead = UART1_RxTail;
}


/*************************************************************************
Function: UART1 Receive Complete interrupt
Purpose:  called when the UART has received a character
**************************************************************************/
ISR(USART1_RX_vect)
{
	unsigned char tmphead;
	unsigned char data;
	unsigned char usr;
	unsigned char lastRxError;
	
	
	/* read UART status register and UART data register */
	usr  = UCSR1A;
	data = UDR1;
	
	/* */
	lastRxError = (usr & (FE1 | DOR1));
	
	/* calculate buffer index */
	tmphead = ( UART1_RxHead + 1) & UART1_RX_BUFFER_MASK;
	
	if ( tmphead == UART1_RxTail )
	{
		/* error: receive buffer overflow */
		lastRxError = UART_BUFFER_OVERFLOW >> 8;
	}
	else
	{
		/* store new index */
		UART1_RxHead = tmphead;
		/* store received data in buffer */
		UART1_RxBuf[tmphead] = data;
	}

	UART1_LastRxError = lastRxError;
}


/*************************************************************************
Function: UART1 Data Register Empty interrupt
Purpose:  called when the UART is ready to transmit the next byte
**************************************************************************/
ISR(USART1_UDRE_vect)
{
	unsigned char tmptail;
	
	if ( UART1_TxHead != UART1_TxTail)
	{
		/* calculate and store new buffer index */
		tmptail = (UART1_TxTail + 1) & UART1_TX_BUFFER_MASK;
		UART1_TxTail = tmptail;

		/* get one byte from buffer and write it to UART */
		UDR1 = UART1_TxBuf[tmptail];  /* start transmission */
	}
	else
	{
		/* tx buffer empty, disable UDRE interrupt */
		UCSR1B &= ~(1 << UDRIE1);
	}
}