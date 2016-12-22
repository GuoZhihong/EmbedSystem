#include<avr/io.h>
#define F_CPU 1000000UL //1 MHz 
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

int ADCCounter = 0; //number of times the interrupt handler is called
int maxCount = 0;

//SPI: initialize SPI as slave
void initSPI()
{
	SPCR |= (1 << SPIE) | (1 << SPE); //set SPI interrupt and SPI operations
}

//ADC: Initialization for the ADC
void initADC()
{
	ADMUX |= (1 << REFS0); //Voltage reference is AVcc. multiplexer set to read pin PC0 by default
	ADCSRA |= (1 << ADEN); //This enables the ADC 
}

//ADC: Method which reads from an analog source and returns an int
int readADC()
{
	ADCSRA |= (1 << ADSC);		 	//Start read
	while(!(ADCSRA & (1<<ADIF)));	//Do nothing while data is being read
	ADCSRA |= (1 << ADIF); 			//reset interrupt flag
	return (ADC); 					//return the read value
}

//Timer: initialize timer which will be used to take ADC values
void initADCTimer()
{
	TCCR2A = 0;											//enable timer2
	TCCR2B = 0;								
	TCCR2A |= (1 << WGM21); 							//set timer to CTC
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);	//set prescaler to 1024
	TCNT2 = 0;											//initialize counter register
	OCR2A = 98; 										//set value to which to count up to. this is roughly 0.1 seconds
	TIMSK2 = (1 << OCIE2A);								//enable interrupt on compare match with register OCR2A
}


//USART: Initialization for serial communication for testing
void initUsart()
{
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0)  | (1 << TXEN0); //Enable RX, TX
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); 				//This sets data size to 8 bit
	UBRR0H  = (BAUD_PRESCALE >> 8);
	UBRR0L  = BAUD_PRESCALE;
}

//USART: Returns a char that was typed/ received
char receiveByte()
{
	while( !( UCSR0A & ( 1 << RXC0 ) )  ){}; //do nothing until transmission ends
	return UDR0;
}

//USART: Returns a char that was transmitted
void transmitByte(char data)
{
	while( !( UCSR0A & ( 1 << UDRE0 ) ) ){};
	UDR0 = data;
}

//USART: Method to print text to console, it is done by transmitting each char of a string
void printStr(char const*s)
{
	while(*s)
	{
		transmitByte(*s);
		s++;
	}
}

//SPI: sends SPI data
void SPIsend(int reads)
{
    SPDR = reads;                 // send the data
    while(!(SPSR & (1<<SPIF)));  // wait until transmission is complete
}

//Reads and sends over SPI the smallest value of multiple ADC values and the id of the sensor that took it.
//
void readerFct()
{
	//char tempbuff[128];
	//int tempIndex = 0;
	//int holder = 0;
	int sense1 = 0;
	int sense2 = 0;
	
	ADMUX = 00000000;
	sense1 = readADC();
	ADMUX = 00000001;
	sense2 = readADC();/*
	ADMUX = 00000010;
	sense3 = readADC();
	ADMUX = 00000011;
	sense4 = readADC();
	ADMUX = 00000500;
	sense5 = readADC();*/
	
	if(sense1 < sense2) //object detected by sensor 1 is closer
	{
		printStr("sensor 1 is closer!\n\r");
		SPIsend(1);
		SPIsend(sense1);
	}
	else				//object detected by sensor 2 is closer
	{
		printStr("sensor 2 is closer!\n\r");
		SPIsend(2);
		SPIsend(sense2);
	}
}


int main(void)
{
	
	//initializers
	initADC();
	initADCTimer();
	initUsart();
	initSPI();
	
	//sets the number clock timer interrupts before scanning and sending data over to master. 
	//maxCount = 1 * 10;
	maxCount = 5; //about 0.5 seconds.
	
	sei();
	
	while(1) //main loop
	{	

	}
    
    return 0;
}

ISR(TIMER2_COMPA_vect)  // timer2 compare interrupt
{
	ADCCounter++;
	if(ADCCounter >= maxCount) 
	{	
		readerFct();
		ADCCounter = 0;
	}	
}