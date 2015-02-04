#include "main.h"

void Interface_::add_character(char x)
{
  this->buff[this->iterEnd] = x;
  ++iterEnd;
  while( this->iterEnd >= SIZE ) this->iterEnd -= SIZE;
}

void Interface_::clear_buffer()
{
  this->iterEnd = this->iterBeg;
}


char Interface_::receive_byte()
{
	char x = (this->buff[this->iterBeg]);
	++this->iterBeg;
	while( this->iterBeg >= SIZE ) this->iterBeg -= SIZE;
	return x;
}


char Interface_::receive_byte_wait()
{	
	while(this->size() == 0);
	char x = (this->buff[this->iterBeg]);
	++this->iterBeg;
	while( this->iterBeg >= SIZE ) this->iterBeg -= SIZE;
	return x;
}

char * Interface_::readline(bool show)
{
#ifdef __AVR__
	char * tmp = (char *)malloc(15);
	uint8_t it = 0;
	while(1)
	{
		tmp[it] = this->receive_byte_wait();
		if( show ) this->putch(tmp[it]);
		if( tmp[it] == '\b' ) 
		{
			--it;
			--it;
		}
		if( tmp[it] == '\r' || tmp[it] == '\n' || tmp[it] == ')' )
		{
			tmp[it] = 0;
			return tmp;
		}
		++it;
	}
#else
	char * tmp = (char *)malloc(15);
	scanf("%s",tmp);
	return tmp;
#endif
}

uint8_t Interface_::size()
{
	int8_t res = ( iterEnd - iterBeg );
	while( res < 0 ) res += this->SIZE;
	return res;
}

void Interface_::putch(char x)
{
#ifdef __AVR2560__
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = x;
#else
#ifdef __AVR__
	while (!(UCSRA & (1 << UDRE)));
	/* Put data into buffer, sends the data */
	UDR = x;
#endif
#endif
	printf("%c",x);
}

void Interface_::putstring(const char * x)
{
	while(*x)
	{
		this->putch(*x);
		++x;
	}
}

void Interface_::putstring_P(const char * x)
{
#ifdef __AVR__
	char now;
	while( (now = pgm_read_byte(x)) != 0x00)
	{
		this->putch(now);
		++x;
	}
#else
	this->putstring(x);
#endif
}


Interface_::Interface_()
{
  this->iterBeg = this->iterEnd = 0;
  
  
}

#define USART_BAUDRATE 9600 
#define BAUD_PRESCALE (((10000000UL / (USART_BAUDRATE))) - 1) 

void Interface_::init()
{
	this->SIZE = 15;
#ifdef __AVR2560__ 
	uint32_t ubrr = ((F_CPU / 16) / (4800)) - 1;
	/* Set baud rate */
	
	UBRR0H = (unsigned char)(ubrr>>8); 
	UBRR0L = (unsigned char)ubrr; 

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   // Turn on the transmission and reception circuitry 
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // Use 8-bit character sizes 
	
	/*UBRR0H = (uint8_t) (ubrr >> 8);
	UBRR0L = (uint8_t) ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);*/
	
	UCSR0B |= (1 << RXCIE0);

#else
#ifdef __AVR__ 
	uint32_t ubrr = ((F_CPU / 16) / (9600)) - 1;
	/* Set baud rate */
	UBRRH = (uint8_t) (ubrr >> 8);
	UBRRL = (uint8_t) ubrr;
	/* Enable receiver and transmitter */
	UCSRB = (1 << RXEN) | (1 << TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
	
	UCSRB |= (1 << RXCIE);
#endif
#endif
}

Interface_ Interface;
