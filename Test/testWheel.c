#define F_CPU 1000000L

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

//what should be compelted: I/O console input and reply. Speed of one wheel. 
//TODO: debugging. update it to work on on more than one wheel. And go in reverse
//if char does not work, and the input is uin8_t, then it means that the buffer must be created to an array of chars. 
//void initWheels(void);



void initWheels()
{
	DDRD |= (1 << PD6) | (1 << PD7); //Enablers (aka Accelerator) 

	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2)| (1 << PB6) | (1 << PB7); //A inputs (directions of wheels) [PD2: Back] [PD3: front]

    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	TCCR0B |= (1 << CS00);

	OCR0A = 0;

	//PORTB |= (0 << PB5) | (1 << PB4);
}

int main(void){
	
	initWheels();
	

    	while(1)
   	    {
			
			//_delay_ms(5000);
			OCR0A = 100;
			goForward();
			_delay_ms(1500);

			OCR0A = 255;
			goBackward();
			_delay_ms(1500);
    	}
    	return 0;   
}

goForward()
{
			PORTD |= (1 << PD7);
			PORTB &= ~(1 << PB0);

			PORTB |= (1<< PB6);
			PORTB &= ~(1<< PB7);

			PORTB |= (1<< PB2);
			PORTB &= ~(1 << PB1);
}

goBackward()
{
			PORTD &= ~(1 << PD7);
			PORTB |= (1 << PB0);

			PORTB &= ~(1<< PB6);
			PORTB |= (1<< PB7);

			PORTB &= ~(1<< PB2);
			PORTB |= (1 << PB1);
}
