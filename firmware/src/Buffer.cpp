#include "main.h"

void Buffer_::add_character(char x)
{
		Interface.putch(x);	
	if( x == '\r' || x == '\n' )
	{
		if( iter >= 10 )
			Completed = true;
	}
	else if( iter < 10 && ((x >= 'A' && x <= 'Z' ) || (x >= 'a' && x <= 'z' ) || (x >= '0' && x <= '9' ) ))
	{
		if( iter == 0 )
		{
			Timer0.begin(Presc1024);
		}
		if( iter < this->MAXSIZE )
		{
			this->buff[iter] = x;
			++iter;
		}
	}
}

void Buffer_::clear_buffer()
{
  this->iter = 0;
  Completed = false;
}

extern volatile uint8_t MODE;
void Buffer_::clear_buffer_interrupt()
{
	Timer0.end();
	this->iter = 0;
	Completed = false;
	if( this->buff[0] == 'x' && this->buff[1] == 'x' )
	{
		MODE = 0;
	}
}

char * Buffer_::receive_data()
{
  return this->buff;
}

bool Buffer_::new_data_available()
{
  return ( this->Completed );
}

Buffer_::Buffer_()
{
  this->iter = 0;
  this->buff[10] = 0;
  Completed = false;
}
Buffer_ Buffer;