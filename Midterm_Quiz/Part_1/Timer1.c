#define CPU_16MHz 0x00
#define F_CPU 10000000

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usb_serial.h"
#include <string.h>
//#include <uart.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

#include <string.h>
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)


//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

void USART_init(){

	//
	//Declare variables for Serial Communication
	//
	//UCSR1A = (1<<U2X1);
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); /* 8-bit data */
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);   /* Enable RX and TX */
	UBRR0H  = (uint8_t)BAUD_PRESCALE >> 8);
	UBRR0L  = (uint8_t)BAUD_PRESCALE;

}

void delay_ms(unsigned int time_ms)
{
    // _delay_ms() comes from <util/delay.h> and can only
    //  delay for a max of around 13 ms when the system
    //  clock is 20 MHz, so we define our own longer delay
    //  routine based on _delay_ms()
 
    unsigned int i;
 
    for (i = 0; i < time_ms; i++)
        _delay_ms(1);       
}

int main(void){

	usb_init();
	CPU_PRESCALE(CPU_16MHz);
	
	char buffer [32];

	DDRD |= (1<<PD6) | (1<<PD7); 

	int freq_val, timer_val;
	
	//
	//Declare variables for Timer
	//
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	TCCR0B |= (1 << CS00);
	
	OCR0A = 128; 
	PORTD |= (1 << PD6); 

	//usb_serial_write('frequency', 5);
	//buffer = 'frequency';

	while (1)
	{
		// wait until a byte is ready to read
		while(( UCSR0A & ( 1 << RXC0 )) == 0 ){}

		  // grab the byte from the serial port
		buffer = UDR0;

//		while(( UCSR0A & ( 1 << RXC0 )) == 0 ){}

//		timer_val = UDR0;
		
		// wait until the port is ready to be written to
		while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
		
		OCR0A = freq_val;

		PORTD |= (1 << PD7);
		delay_ms(timer_val);
		PORTD &= ~(1 << PD7);
		delay_ms(timer_val); 		


	}
}

void USART_send( unsigned char data){
 
 while(!(UCSR0A & (1<<UDRE0)));
 UDR0 = data;
 
}

unsigned char USART_receive(void){
 
 while(!(UCSR0A & (1<<RXC0)));
 return UDR0;
 
}

void USART_putstring(char* StringPtr){
 
while(*StringPtr != 0x00){    //Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
 USART_send(*StringPtr);    //Using the simple send function we send one char at a time
 StringPtr++;}        //We increment the pointer so we can read the next char
 
}

