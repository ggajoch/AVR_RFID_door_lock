#include "main.h"

Authenticator_ Authenticator;


bool Authenticator_::access_granted(Person person, char * card_number)
{
	LowLevelDevice.open_door();
	Interface.putstring_P(PSTR("ACCESS GRANTED for "));
	Interface.putstring(card_number);
	Interface.putstring_P(PSTR(" with Hash "));
	char buf[12];
	person.getID(buf);
	Interface.putstring(buf);
	Interface.putstring_P(PSTR("\n\r"));
}


bool Authenticator_::access_denied(Person person, char * card_number)
{
	Interface.putstring_P(PSTR("ACCESS DENIED for "));
	Interface.putstring(card_number);
	Interface.putstring_P(PSTR(" with Hash "));
	char buf[12];
	person.getID(buf);
	Interface.putstring(buf);
	Interface.putstring_P(PSTR("\n\r"));
}


bool Authenticator_::process_card(char * data)
{
	Interface.putstring_P(PSTR("Readed card: "));
	Interface.putstring(data);
	Person now(data);
	if( CardBase.authorised_card(now) )
	{
		Interface.putstring_P(PSTR(", acces granted\n\r"));
		this->access_granted(now, data);
	}
	else
	{
		Interface.putstring_P(PSTR(", acces denied\n\r"));
		this->access_denied(now, data);
	}
}

