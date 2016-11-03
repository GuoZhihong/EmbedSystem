#define CPU_16MHz 0x00
#define F_CPU 16000000L

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usb_serial.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

/*the first 3 defines indicate the Teensy frequency, 
  which will allow the clock functions to scale */

/*This file cycles the brigthness by cyclcing the frequency of our thing. */
int main(void)
{
	usb_init();
	char buffer [32];
	CPU_PRESCALE(CPU_16MHz);
	uint8_t ch; 
	ch = ch&0b00000111;
	
	//set multiplexer to read. REFS0 is the reference voltage.
	ADMUX |= (1<<REFS0); 
	//sets up Teensy frequency to adjust to hardware
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  
	ADMUX |= ch;   
	
	while(1)
	{
		ADCSRA|=(1<<ADSC); //gets ready to read
		while(!(ADCSRA && (1<<ADIF))); //waits until reading is done
		itoa(ADC, buffer, 5);
		usb_serial_write(buffer, 5);
		_delay_ms(500);
		ADCSRA |= (1<<ADIF); //resets interrrupt flag
	}
}