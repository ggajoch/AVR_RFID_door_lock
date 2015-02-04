#include "main.h"

void Timer0_::begin(ClockPresc prescaler)
{
#ifdef __AVR2560__
	TCCR0B |= prescaler;
	sbi(TIMSK0, TOIE0);
	sei();
#else
#ifdef __AVR__
	TCCR0 |= prescaler;
	sbi(TIMSK, TOIE0);
	sei();
#else
	printf("TIMER0 begin\n");
#endif
#endif
}
void Timer0_::end()
{
#ifdef __AVR2560__
	TCCR0B = NOCLOCK;
	cbi(TIMSK0, TOIE0);
#else
#ifdef __AVR__
	TCCR0 = NOCLOCK;
	cbi(TIMSK, TOIE0);
#else
	printf("TIMER0 end");
#endif
#endif
}
Timer0_ Timer0;



void Timer2_::begin(ClockPresc prescaler)
{

#ifdef __AVR2560__
	TCCR2A = 0;
	TCCR2B |= prescaler;
	sbi(TIMSK2, TOIE2);
	sei();
#else
#ifdef __AVR__
	TCCR2 |= prescaler;
	sbi(TIMSK, TOIE2);
	sei();
#else
	printf("TIMER2 begin\n");
#endif
#endif
}
void Timer2_::end()
{
#ifdef __AVR2560__
	TCCR2B = NOCLOCK;
	cbi(TIMSK2, TOIE2);
#else
#ifdef __AVR__
	TCCR2 = NOCLOCK;
	cbi(TIMSK, TOIE2);
#else
	printf("TIMER0 end");
#endif
#endif
}
Timer2_ Timer2;


