#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define UART_BAUD  9600
#define _BAUD          (((F_CPU / (UART_BAUD * 16UL))) - 1)

static void uart_init(void)
{
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0L = (uint8_t)(_BAUD & 0xFF);
	UBRR0H = (uint8_t)((_BAUD >> 8) & 0xFF);
}

static void uart_tx(char c)
{
	while(!(UCSR0A & (1 << UDRE0))) ;
	UDR0 = c;
}

static void uart_tx_str(const char *s)
{
	register char c;
	while((c = *s++)) { uart_tx(c); }
}

static void uart_tx_str_P(const char *s)
{
	register char c;
	while((c = pgm_read_byte(s++))) { uart_tx(c); }
}

void uart_flush(void)
{
	uint8_t dummy;
	while(UCSR0A & (1 << RXC0))
	{
		dummy = UDR0;
	}

	(void)dummy;
}

char uart_rx(void)
{
	while(!(UCSR0A & (1 << RXC0))) ;
	return UDR0;
}

void uart_rx_str(char *buf, uint16_t size)
{
	size_t i;
	char c;
	i = 0;
	while((c = uart_rx()) != '\r' && c != '\n')
	{
		uart_tx(c);
		buf[i++] = c;
		if(i == size - 1)
		{
			break;
		}
	}

	uart_tx_str("\r\n");
	buf[i] = '\0';
}
