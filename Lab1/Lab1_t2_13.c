#define CPU_16MHz 0x00
#define F_CPU 16000000L
#define BAUD 9600

#include <stdio.h>
#include <stdlib.h>
#include <util/setbaud.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usb_serial.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

/*the first 3 defines indicate the Teensy frequency, 
  which will allow the clock functions to scale */

/*This file cycles and prints the frequency of our thing. */


	


int main(void){
	
	usb_init();
	char buffer [32];

	CPU_PRESCALE(CPU_16MHz);
	
	DDRB |= (1<<PB7)| (1<<PB5);
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	TCCR0B |= (1 << CS00);

	//usb_serial_write("test",4);

	while (1)
	{		
		OCR0A = 255;
		itoa(OCR0A, buffer, 10);
		usb_serial_write(buffer,4);
		_delay_ms(1000);
		
		OCR0A = 128;
		itoa(OCR0A, buffer, 10);
		usb_serial_write(buffer,4);
		_delay_ms(1000);
		
		OCR0A = 64;
		itoa(OCR0A, buffer, 10);
		usb_serial_write(buffer,4);
		_delay_ms(1000);
		
		OCR0A = 32;
		itoa(OCR0A, buffer, 10);
		usb_serial_write(buffer,4);
		_delay_ms(1000);
		
		OCR0A = 16;
		itoa(OCR0A, buffer, 10);
		usb_serial_write(buffer,4);
		_delay_ms(1000);


	}
	

}