#define CPU_16MHz 0x00
#define F_CPU 16000000L

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

/*the first 3 defines indicate the Teensy frequency, 
  which will allow the clock functions to scale */

int main(void){

CPU_PRESCALE(CPU_16MHz);

DDRB |= (1<<PB5); //INITIALIZE PB5 TO OUTPUT

while(1){

	PORTB |= (1<<PB5);		//POWER ON PB5
	
	_delay_ms(10000);
	
	PORTB &= ~(1<<PB5);		//POWER OFF ALL OF PORT B

	_delay_ms(10000);
	}
}