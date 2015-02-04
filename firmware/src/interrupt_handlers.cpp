#include<inttypes.h>
#include "main.h"


volatile uint8_t MODE = 1; 
// 0 - computer mode
// 1 - normal mode

#ifdef __AVR2560__
ISR(USART_RX_vect) {
	//Interface.putstring("FJSALFJA\n\r");
	if( MODE )
	{
		Buffer.add_character(UDR0);
	}
	else
	{
		Interface.add_character(UDR0);
	}
}

#else

	#ifdef __AVR__
	ISR(USART_RXC_vect) {
	#else
	void UASRTinterrupt() {
	  char UDR = 'a';
	#endif
		if( MODE )
		{
			Buffer.add_character(UDR);
		}
		else
		{
			Interface.add_character(UDR);
		}
	}

#endif



volatile uint8_t remainingTicksTIM0 = 250;

#ifdef __AVR__
ISR(TIMER0_OVF_vect)
#else
void TIMinterrupt()
#endif
{
	--remainingTicksTIM0;
	if( remainingTicksTIM0 == 0 )
	{
	//	Interface.putstring("b");
		Buffer.clear_buffer_interrupt();
		remainingTicksTIM0 = 50;
	}
}
