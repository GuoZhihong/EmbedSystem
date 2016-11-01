#include <avr/io.h> 
#include <avr/interrupt.h>


int main(void)
{
	
	DDRD |= (0 << PD2); //set PD2 as input
	DDRB |= (1 << PB0); //set PB0 as output	

	TCCR1B = 0;
	TCNT1 = 0;
	
	TCNT1 = 51200; //set up timer
	TCCR1B |= (1<< WGM12); //enable CTC
	TCCR1B |= (1<< CS12); //set up timer with prescaler = 256
	TIMSK1 |= (1 << TOIE1); //Enable timer flow interrupt for Timer 0

	EICRA |= (1 << ISC01);    // set INT0 to trigger on falling edge
	EIMSK |= (1 << INT0);     // Turn on INT0
	
    sei();         //enable global interrupts

    while (1)
    {
        //main loop
    }
}

// Interrupts when button is pushed.
// This resets the counter
ISR(INT0_vect)
{
	cli(); //disable global interrupts
	TCNT1 = 0;
    	PORTB ^= (1 << 0); //toggle LED
    	TCNT1 = 51200;
	sei();	//reenable global interrupts
}

// Interrupts when timer overflows
ISR(TIMER1_OVF_vect)
{
	cli(); //disable global interrupts
	TCNT1 = 51200;
    PORTB ^= (1 << 0); //toggle LED
    sei();	//reenable global interrupts
}

