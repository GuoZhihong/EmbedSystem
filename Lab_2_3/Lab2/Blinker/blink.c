#include <avr/io.h> 
#include <avr/interrupt.h>

int main(void)
{
	DDRB |= (1 << PB0); //set PB0 as output	
	
	
	TCCR1B = 0;
	TCNT1 = 0;
	
	TCNT1 = 51200; //set counter to 51200 
	TCCR1B |= (1<< WGM12); //Enable CTC
	TCCR1B |= (1<< CS12); //set up timer with prescaler = 256
	TIMSK1 |= (1 << TOIE1); //Enable timer flow interrupt for Timer 0
	
	//This means each state will last for 256/51200

    sei();         //enable global interrupts
	
	
    while (1)
    {
        //main loop
    }
}



ISR(TIMER1_OVF_vect)
{
	cli();
	TCNT1 = 51200; //Resets value to 51200
    PORTB ^= (1 << 0); //toggle LED
    sei();
}