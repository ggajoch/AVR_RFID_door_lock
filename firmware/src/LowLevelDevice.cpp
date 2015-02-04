#include "main.h"

LowLevelDevice_ LowLevelDevice;

/*#define RELAY_PORT PORTC
#define RELAY_DDR PORTC
#define RELAY_PIN 1*/

#define RELAY_PORT PORTD
#define RELAY_DDR DDRD
#define RELAY_PIN 2

#define BEEP_PORT PORTB
#define BEEP_DDR DDRB
#define BEEP_PIN 6

#define GLUE(a,b) (a ## b)
//DDRC
//PORTC

void LowLevelDevice_::init()
{
#ifdef __AVR__
	sbi(RELAY_DDR,RELAY_PIN);
	sbi(BEEP_DDR,BEEP_PIN);
#endif
}

volatile uint16_t ovfs;
volatile uint8_t countFlag, leftToChange;
#ifdef __AVR__
ISR(TIMER2_OVF_vect)
{
	--leftToChange;
	++ovfs;
	if( leftToChange == 0 )
	{
		BEEP_PORT ^= (1 << BEEP_PIN);
		leftToChange = countFlag;
	}
}
#endif

void LowLevelDevice_::beepOpened()
{
	ovfs = 0;
	countFlag = 4;
	Timer2.begin(Presc1);
#ifdef __AVR__
	while( ovfs < 62500 ){} //2 secs
	ovfs = 0;
	while( ovfs < 31250 ){} //1 sec
	Timer2.end();
#endif
}

void LowLevelDevice_::beepDenied()
{
ovfs = 0;
	countFlag = 1;
	Timer2.begin(Presc8);
#ifdef __AVR__
	if( ovfs > 6000 ) //~1 sec
	{
		Timer2.end();
	}
#endif
}


void LowLevelDevice_::relay_on()
{
#ifdef __AVR__
	sbi(RELAY_PORT,RELAY_PIN);
#endif
}

void LowLevelDevice_::relay_off()
{
#ifdef __AVR__
	cbi(RELAY_PORT,RELAY_PIN);
#endif
}

void LowLevelDevice_::open_door()
{
	this->relay_on();
	this->beepOpened();
	this->relay_off();
}

void LowLevelDevice_::access_denied()
{
	this->beepDenied();
}
