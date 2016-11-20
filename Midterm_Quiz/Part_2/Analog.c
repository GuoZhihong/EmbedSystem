#define CPU_16MHz 0x00
#define F_CPU 10000000

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
//#include <uart.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

#include <string.h>
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)


//Declaration of our functions
void USART_init(void);
unsigned char USART_receive(void);
void USART_send(unsigned char data);
void USART_putstring(char* StringPtr);
void USART_Flush(void);
int setTimer(char timerChar);
void configurePWD(void);
void setFWD(char freq_ch);
uint16_t ReadADC(uint8_t ADCchannel);
void analog_init(void);
void delay_ms(unsigned int time_ms);

void USART_init(){

	//
	//Declare variables for Serial Communication
	//
	UBRR0H  = (uint8_t)(BAUD_PRESCALE >> 8);
	UBRR0L  = (uint8_t)BAUD_PRESCALE;

    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); /* 8-bit data */
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);   /* Enable RX and TX */


}

void analog_init(){
    /*
        Declare variables for Analog
    */

     // Select Vref=AVcc
    ADMUX |= (1<<REFS0);
    //set prescaller to 128 and enable ADC register
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); 
}

int main(void){

	USART_init();

	CPU_PRESCALE(CPU_16MHz);
	
	char buffer [32];

	char timer_ch;
	int timer_val;
	int analog_Val;
	//
	//Declare variables for Timer
	//
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	TCCR0B |= (1 << CS00);
	
	PORTD |= (1 << PD6); 

	timer_ch = USART_receive(); // wait for input for Timer

	timer_val = setTimer(timer_ch); //Set time value

	while (1)
	{
            itoa(ReadADC(0),buffer,5); //Convert data from analog into char array.
            USART_send(buffer); //Prints to console.

			delay_ms(timer_val); //Wait for timer_val miliseconds

	}
}

/*

    Function reads analog from ADC0 and return Data.

*/
uint16_t ReadADC(uint8_t ADCchannel)
{
    //select ADC channel with safety mask
    ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
    //single conversion mode
    ADCSRA |= (1<<ADSC);
    // wait until ADC conversion is complete
    while( ADCSRA & (1<<ADSC) );
   return ADC;
}

/*
	Function waits for user's side to be read

*/
void USART_send(unsigned char data){
 
 while(!(UCSR0A & (1<<UDRE0))); // Wait for empty transmit buffer
 UDR0 = data; //Puts data into buffer, sends the data
 
}

/*
	Function waits for a data input
	Returns data as char
*/
unsigned char USART_receive(void){
 
 while(!(UCSR0A & (1<<RXC0))); //Wait for data to be received
 return UDR0; //Get and return received data from buffer
 
}

/*
	Flush the receiver buffer. This empties the buffer for new data.
*/
void USART_Flush(void){
	unsigned char dummy;
	while (UCSR0A & (1<< RXC0))  dummy = UDR0;
}

/*

	Function that utiilizes a switch case for Timer.
	Possible arguments consists of: [q,w,e,r]
	Each returning a unique time in miliseconds.

*/
int setTimer(char ch)
{
	int time= 0;
		switch(ch)
		{
			case 'q':
				time = 100;
				break;
			case 'w':
				time = 500;
				break;
			case 'e':
				time = 1000;
				break;
			case 'r':
				time = 2000;
				break;
		}
	
	return time;

}

/*

	Function that utiilizes a switch case for Frequency.
	Possible arguments consts of: [q,w,e,r,null]
	Each sets the OCR0A value to a unique number 

*/
void setFWD(char freq_ch)
{
			switch(freq_ch)
		{
			case 'q':
				OCR0A = 255;
				break;
			case 'w':
				OCR0A = 155;
				break;
			case 'e':
				OCR0A = 100;
				break;
			case 'r':
				OCR0A = 55;
				break;
			default:
				OCR0A = 200;
				break;
		}
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

