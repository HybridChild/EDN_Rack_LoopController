/*
 * UART.h
 *
 * Created: 26-01-2020 22:14:34
 *  Author: Esben
 */ 


#ifndef UART_H_
#define UART_H_

#define UART_NO_PARITY		0
#define UART_EVEN_PARITY	2
#define UART_ODD_PARITY		3

#define UART_1_STOP_BIT		1
#define UART_2_STOP_BITS	2

/* high byte error return code of uart_getc() */
#define UART_FRAME_ERROR		0x0800              /* Framing Error by UART       */
#define UART_OVERRUN_ERROR		0x0400              /* Overrun condition by UART   */
#define UART_BUFFER_OVERFLOW	0x0200              /* receive ringbuffer overflow */
#define UART_NO_DATA			0x0100              /* no receive data available   */

/** @brief  UART Baudrate Expression
 *  @param  xtalcpu  system clock in Mhz, e.g. 4000000L for 4Mhz          
 *  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600     
 */
#define UART_BAUD_SELECT(baudRate,xtalCpu) ((xtalCpu)/((baudRate)*16l)-1)

void			UART0_Init(unsigned int baudrate, unsigned char stopbit, unsigned char parity);
unsigned int	UART0_GetChar(void);
void			UART0_PutChar(unsigned char data);
void			UART0_PutString(const char *s );
void			UART0_PutString_p(const char *progmem_s );
int				UART0_Available(void);
void			UART0_Flush(void);
int				UART0_QueueChar(unsigned char data);
void			UART0_QueueArray(const char *array, unsigned char length);
void			UART0_PutQueue(void);

void			UART1_Init(unsigned int baudrate, unsigned char stopbit, unsigned char parity);
unsigned int	UART1_GetChar(void);
void			UART1_PutChar(unsigned char data);
void			UART1_PutString(const char *s );
void			UART1_PutString_p(const char *progmem_s );
int				UART1_Available(void);
void			UART1_Flush(void);


#endif /* UART_H_ */