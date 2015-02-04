#include "main.h"

__attribute__((always_inline)) inline uint8_t hexASCIItoValue(char a)
{
	if( a <= '9' ) // enought, because '9' < 'A'
		return (uint8_t)(a - '0');
	else
		return ((uint8_t)(a - 'A') + 10);
}

__attribute__((always_inline)) inline char valueToHexASCII(uint8_t a)
{
	if( a <= 9 )
		return (char)(a + '0');
	else
		return ((char)(a - 10) + 'A');
}

void Person::calculate(char * text)
{
	this->data1 = hexASCIItoValue(text[0]);
	this->data2 = hexASCIItoValue(text[1]);
	this->data3 = hexASCIItoValue(text[2]);
	this->data4 = hexASCIItoValue(text[3]);
	this->data5 = hexASCIItoValue(text[4]);
	this->data6 = hexASCIItoValue(text[5]);
	this->data7 = hexASCIItoValue(text[6]);
	this->data8 = hexASCIItoValue(text[7]);
	this->data9 = hexASCIItoValue(text[8]);
	this->data10 = hexASCIItoValue(text[9]);
}

void Person::getID(char * dest)
{
	dest[0] = valueToHexASCII(this->data1);
	dest[1] = valueToHexASCII(this->data2);
	dest[2] = valueToHexASCII(this->data3);
	dest[3] = valueToHexASCII(this->data4);
	dest[4] = valueToHexASCII(this->data5);
	dest[5] = valueToHexASCII(this->data6);
	dest[6] = valueToHexASCII(this->data7);
	dest[7] = valueToHexASCII(this->data8);
	dest[8] = valueToHexASCII(this->data9);
	dest[9] = valueToHexASCII(this->data10);
	dest[10] = NULL;
}

Person::Person()
{
}

Person::Person(char * text)
{
  this->calculate(text);
}

