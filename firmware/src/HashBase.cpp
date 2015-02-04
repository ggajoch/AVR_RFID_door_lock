#include "main.h"

//BaseElement EEMEM baseEEPROM[100];

void CardBase_::read_base_from_EEPROM()
{
//TODO
	eeprom_read_block(this->baseSRAM, (void *)0, sizeof(BaseElement)*this->MAXSIZE);
}


void CardBase_::save_position_to_EEPROM(uint16_t pos)
{
	eeprom_write_dword( (uint32_t *)(sizeof(BaseElement)*pos), *((uint32_t *)(&this->baseSRAM[pos])  ));
}

void CardBase_::save_empty_to_EEPROM(uint16_t pos)
{
	eeprom_write_byte( (uint8_t *)(sizeof(BaseElement)*pos), *((uint8_t *)(&this->baseSRAM[pos])  ));
}


uint16_t CardBase_::first_empty_pos()
{
	uint16_t it = 0;
	while( it < this->MAXSIZE && !empty_pos(it) ) ++it;
	return it;
}

bool CardBase_::empty_pos(uint16_t pos)
{
	return (baseSRAM[pos].empty);
}

void CardBase_::place_person(uint16_t pos, Person person)
{
	this->baseSRAM[pos].empty = false;
	this->baseSRAM[pos].who = person;
	//this->baseSRAM[pos].Hash = person.Hash;
	
	this->save_position_to_EEPROM(pos);
}


uint16_t CardBase_::find_pos_of(Person person)
{
	for(uint8_t i = 0; i < this->MAXSIZE; ++i)
	{
		if( this->baseSRAM[i].empty == 0 && this->baseSRAM[i].who == person )
		{
			return i;
		}
	}
	return this->MAXSIZE;

}

bool CardBase_::authorised_card(Person person)
{
	if(this->find_pos_of(person) != this->MAXSIZE )
		return true;
	else
		return false;
}

bool CardBase_::add_card(Person person)
{
	if( authorised_card(person) ) return false;
	uint16_t pos = this->first_empty_pos();
	baseSRAM[pos].empty = 0;
	baseSRAM[pos].who = person;
	this->save_position_to_EEPROM(pos);
}

bool CardBase_::delete_card(Person person)
{
	uint16_t pos = this->find_pos_of(person);
	if( pos != this->MAXSIZE )
	{
		baseSRAM[pos].empty = true;
		this->save_empty_to_EEPROM(pos);
	}
}

void CardBase_::list_base()
{
	Interface.putstring_P(PSTR("List of slots:\n\r"));
	char buf[12];
	
	uint8_t last = this->MAXSIZE - 1;
	for(int8_t i = this->MAXSIZE - 1; i >= 0; --i)
	{
		if( this->empty_pos(i) )
		{
			last = i;
		}
		else
			break;
	}
	//sprintf(buf,"\n\r%d\n\r\n\r",last);
	//Interface.putstring(buf);
	uint8_t taken = 0;
	for(uint8_t i = 0; i < last; ++i)
	{
		sprintf(buf,"%d:\t",i+1);
		Interface.putstring(buf);
		if( this->empty_pos(i) )
			Interface.putstring_P(PSTR("empty"));
		else
		{
			//Interface.putstring_P(PSTR("non-empty"));
			//sprintf(buf,"%ld",this->baseSRAM[i].Hash);   //TODO
			//Interface.putstring(buf);
			++taken;
		}
		Interface.putstring_P(PSTR("\n\r"));
	}
	Interface.putstring_P(PSTR("Left: "));
	sprintf(buf,"%d",this->MAXSIZE-taken);
	Interface.putstring(buf);
	Interface.putstring_P(PSTR(" places.\n\r"));
}

void CardBase_::clean_base()
{
	char buf[10];
	for(uint8_t i = 0; i < this->MAXSIZE; ++i)
	{
		if( !empty_pos(i) )
		{
			Interface.putstring_P(PSTR("Cleaned position "));
			sprintf(buf,"%d",i);
			Interface.putstring(buf);
			Interface.putstring_P(PSTR("\n\r"));
			this->baseSRAM[i].empty = true;
			this->save_position_to_EEPROM(i);
		}
	}
}

CardBase_ CardBase;
