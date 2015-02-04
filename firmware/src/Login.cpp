#include "main.h"

Login_ Login;

void Login_::refreshPassword()
{
	eeprom_read_block(SRAMpassword, (const void *)492, 16);
}

bool Login_::askForPassword()
{
	//eeprom_read_block(SRAMpassword, (const void *)492, 16);
	//Interface.putstring(SRAMpassword);
	
	
	//return true;
	char * xx = Interface.readline(true);
	if( !strcmp(SRAMpassword,xx) )
	{
		Interface.putstring_P(PSTR("\n\r"));
		free(xx);
		return true;
	}
	else
	{
		free(xx);
		Interface.putstring_P(PSTR("\n\rInvalid password! Get out!\n\r"));
		return false;
	}
}

void Login_::writeToEEPROM(uint8_t len)
{
	eeprom_write_block(SRAMpassword, (void *)492, len);	
}