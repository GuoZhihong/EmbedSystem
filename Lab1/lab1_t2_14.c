#define CPU_16MHz 0x00
#define F_CPU 16000000L

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

/*the first 3 defines indicate the Teensy frequency, 
  which will allow the clock functions to scale */

/*This file cycles the brigthness by cyclcing the frequency of our thing. */
int main(void){

	CPU_PRESCALE(CPU_16MHz);

	/*set up timer operations. Too complciated for this course
	//TTCR0A is the timer control register. Will be used for waveform generation
	//TCCR0B is another timer control register. Will be used for clock select
	//WGM0x set up the mode of operation (fast PWM in our case)
	//COM0ax set up the compare output mode (reserved for another bit in our case) 
	//CS0x set up the timer prescaler. No prescalling in our case.
	*/
	
	DDRB |= (1<<PB7) | (1<<PB5); //PB7 is the OC0A (output compare) pin. 
	
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	
	TCCR0B |= (1 << CS00);

	TIMSK0 |= (1 << TOIE0);	//timer interrupt manager
	sei();			//enable interrupt
	
	OCR0A = 128; 

	while (1)
	{
		PORTB |= (1 << PB5); 
		_delay_ms(1000);
		
		OCR0A = 128;
		_delay_ms(1000);
		
		OCR0A = 64;
		_delay_ms(1000);
		
		OCR0A = 32;
		_delay_ms(1000);
		
		OCR0A = 16;
	}
}