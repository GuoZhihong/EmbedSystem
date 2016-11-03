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
	
	DDRB |= (1<<PB7) | (1<<PB5);
	
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	
	TCCR0B |= (1 << CS00);
	
	OCR0A = 128; 

	while (1)
	{
		PORTB |= (1 << PB5); //Why is PB7 on?
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