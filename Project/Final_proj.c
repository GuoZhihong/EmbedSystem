#define F_CPU 1000000L

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

//what should be compelted: I/O console input and reply. Speed of one wheel. 
//TODO: debugging. update it to work on on more than one wheel. And go in reverse
//if char does not work, and the input is uin8_t, then it means that the buffer must be created to an array of chars. 

void initWheels()
{
	/*
	Bellow we declare pins for to be used for motors.
	Pin PD6 is the enabler that controls the speed of all the motors 
	*/
	DDRD |= (1 << PD6) | (1 << PD7); 
	DDRC |= (1 << PC0) | (1 << PC1);
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2)| (1 << PB6) | (1 << PB7); //A inputs (directions of wheels) [PD2: Back] [PD3: front]

    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	TCCR0B |= (1 << CS00);

	OCR0A = 0;

	//PORTB |= (0 << PB5) | (1 << PB4);
}

int main(void){
	
	initWheels();
	
	OCR0A = 255;

    	while(1)
   	    {
			
			// //_delay_ms(5000);
			// goForward();
			// _delay_ms(1500);

			turnLeft_90();
			turnRight_90();

			// goBackward();
			// _delay_ms(1500);
    	}
    	return 0;   
}

goForward()
{			
			//BACK RIGHT wheel
			PORTD |= (1 << PD7);
			PORTB &= ~(1 << PB0);

			//BACK LEFT wheel
			PORTC |= (1 << PC1);
			PORTC &= ~(1 <<PC0);

			//FRONT RIGHT wheel
			PORTB |= (1<< PB6);
			PORTB &= ~(1<< PB7);

			//FRONT LEFT wheel
			PORTB |= (1<< PB2);
			PORTB &= ~(1 << PB1);
}

goBackward()
{
			//BACK RIGHT wheel
			PORTD &= ~(1 << PD7);
			PORTB |= (1 << PB0);

			//BACK LEFT wheel
			PORTC &= ~(1 << PC1);
			PORTC |= (1 <<PC0);

			//FRONT RIGHT wheel
			PORTB &= ~(1<< PB6);
			PORTB |= (1<< PB7);

			//FRONT LEFT wheel
			PORTB &= ~(1<< PB2);
			PORTB |= (1 << PB1);
}

/*
Set all LEFT wheels to go forward and all RIGHT wheels to go backwards
*/
turnLeft()
{
			//BACK RIGHT wheel
			PORTD &= ~(1 << PD7);
			PORTB |= (1 << PB0);

			//BACK LEFT wheel
			PORTC |= (1 << PC1);
			PORTC &= ~(1 <<PC0);

			//FRONT RIGHT wheel
			PORTB &= ~(1<< PB6);
			PORTB |= (1<< PB7);

			//FRONT LEFT wheel
			PORTB |= (1<< PB2);
			PORTB &= ~(1 << PB1);
}

/*
Set all RIGHT wheels to go farward and all LEFT wheels to go backwards
*/
turnRight()
{
			//BACK RIGHT wheel
			PORTD |= (1 << PD7);
			PORTB &= ~(1 << PB0);

			//BACK LEFT wheel
			PORTC &= ~(1 << PC1);
			PORTC |= (1 <<PC0);

			//FRONT RIGHT wheel
			PORTB |= (1<< PB6);
			PORTB &= ~(1<< PB7);

			//FRONT LEFT wheel
			PORTB &= ~(1<< PB2);
			PORTB |= (1 << PB1);
}

//Turn 90 degree RIGHT
turnRight_90()
{
    turnRight();
    _delay_ms(500);
}

//Turn 90 degree LEFT
turnLeft_90()
{
    turnLeft();
    _delay_ms(500);
}