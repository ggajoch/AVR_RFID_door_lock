#ifndef __MAIN_H__
#define __MAIN_H__

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __AVR__
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/pgmspace.h>
	#include <avr/eeprom.h>
	//#include <util/delay.h>
	
	#ifdef PROGMEM
	#undef PROGMEM
	#define PROGMEM __attribute__((section(".progmem.data")))
	#endif
#else
	#define sei(); 
	#define PSTR(a) (a)
#endif
//#define __AVR__


#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define sbi(value, bit) ((value) |= (1UL << (bit)))
#define cbi(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? sbi(value, bit) : cbi(value, bit))


class Person
{
private:
public:
	//uint32_t Hash;
	uint8_t data1 : 4;
	uint8_t data2 : 4;
	uint8_t data3 : 4;
	uint8_t data4 : 4;
	uint8_t data5 : 4;
	uint8_t data6 : 4;
	uint8_t data7 : 4;
	uint8_t data8 : 4;
	uint8_t data9 : 4;
	uint8_t data10 : 4;
	void calculate( char * text );
	Person();
	Person(char * text);
	bool operator==(Person b)
	{
		return ((this->data1 == b.data1) && (this->data2 == b.data2) && (this->data3 == b.data3) && (this->data4 == b.data4) && (this->data5 == b.data5) && (this->data6 == b.data6) && (this->data7 == b.data7) && (this->data8 == b.data8) && (this->data9 == b.data9) && (this->data10 == b.data10));
	}
	void getID(char * dest);
};

struct BaseElement
{
	bool empty	  :  1;
	Person who;
};

//extern BaseElement EEMEM baseEEPROM[MAXSIZE];

class CardBase_
{
private:
	static const uint16_t MAXSIZE = 82;
	BaseElement baseSRAM[82];
	
  
  void save_empty_to_EEPROM(uint16_t pos);
  void save_position_to_EEPROM(uint16_t pos);
  uint16_t first_empty_pos();
  bool empty_pos(uint16_t pos);
  void place_person(uint16_t pos, Person person);
  uint16_t find_pos_of(Person person);
public:
	void read_base_from_EEPROM();
  bool authorised_card(Person person);
  bool add_card(Person person);
  bool delete_card(Person person);
  void list_base();
  void clean_base();
};
extern CardBase_ CardBase;


class Interface_
{
private:
	char buff[15];
	volatile uint8_t iterBeg, iterEnd;
	uint8_t SIZE;
public:
  void add_character(char x);
  void clear_buffer();
  char receive_byte();
  char receive_byte_wait();
  char * readline(bool show = true);
  uint8_t size();
  void putch(char x);
  void putstring(const char * x);
  void putstring_P(const char * x);
  void init();
  Interface_();
};
extern Interface_ Interface;


class Buffer_
{
private:
	static const uint8_t MAXSIZE = 15;
	char buff[MAXSIZE];
	volatile uint8_t iter;
	bool Completed;
public:
  void add_character(char x);
  void clear_buffer();
  void clear_buffer_interrupt();
  char * receive_data();
  bool new_data_available();
  Buffer_();
};
extern Buffer_ Buffer;



class LowLevelDevice_
{
private:
	
  void beepOpened();
  void beepDenied();
  void relay_on();
  void relay_off();
public:
	void init();
	void open_door();
	void access_denied();
};
extern LowLevelDevice_ LowLevelDevice;


class Parser_
{
private: 
  enum command 
    {
      add_card = 1,
      delete_card = 2,
		show_list = 3,
		clean_base = 4
    };
public:
  void parse_data(char * data);
};


class Authenticator_
{
private:
  bool access_granted(Person person, char * card_number);
  bool access_denied(Person person, char * card_number);
public:
  bool process_card(char * data);
};
extern Authenticator_ Authenticator;

class Login_
{
private:
public:
	char SRAMpassword[16];
	void refreshPassword();
	bool askForPassword();
	void writeToEEPROM(uint8_t len);
};
extern  Login_  Login;


enum ClockPresc
{
	NOCLOCK = 0,
	Presc1 = 1,
	Presc8 = 0b10,
	Presc64 = 0b11,
	Presc256 = 0b100,
	Presc1024 = 0b101,
	T0Falling = 0b110,
	T0Rising = 0b111
};

struct Timer0_
{
	void begin(ClockPresc prescaler);
	void end();
};
extern Timer0_ Timer0;

struct Timer2_
{
	void begin(ClockPresc prescaler);
	void end();
};
extern Timer2_ Timer2;


#endif
