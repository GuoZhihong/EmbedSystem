

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

//#define F_CPU 1000000UL 
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#define CPU_16MHz 0x00
#define F_CPU 16000000L

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

//what should be compelted: I/O console input and reply. Speed of one wheel. 
//TODO: debugging. update it to work on on more than one wheel. And go in reverse
//if char does not work, and the input is uin8_t, then it means that the buffer must be created to an array of chars. 

void initWheels()
{
	//DDRB |= (1 << PB1); //Enablers (aka Accelerator) 
	DDRD |= (1 << PD2);
	DDRB |= (1 << PD3); //A inputs (directions of wheels) [PD2: Back] [PD3: front]
	
    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	TCCR0B |= (1 << CS00);
	OCR0A = OCR2A = 0;

	//PORTB |= (0 << PB5) | (1 << PB4);
}

void initUart()
{
	 UCSR0B |= (1<<RXEN0)  | (1<<TXEN0); 
 	 UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
	 UBRR0H  = (BAUD_PRESCALE >> 8);
	 UBRR0L  = BAUD_PRESCALE;
}

 char receiveByte()
 {
	  char received_byte;
 	  // wait until a byte is ready to read
	  while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 ){;}
	  
	  // grab the byte from the serial port
	  received_byte = UDR0;
	 
	  return received_byte;
 } 

void sendByte(char sent_byte)
{
	  // wait until the port is ready to be written to
	  while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){;}
	 
	  // write the byte to the serial port
    	  UDR0 = sent_byte;
}

int main(void){
	initWheels();
	//initUart();
	//volatile char m8;

	//OCR1A = 255;
	//PORTB |= (1 << 1);
	int acc = 255;

    	while(1)
   	    {
			
			//_delay_ms(5000);

			goForward(acc);
			_delay_ms(5000);
		
			//goBackward(acc);
			//_delay_ms(5000);

			acc = acc + 50;

			if (acc = 1050){
				acc = 0;
			}
    	}
    	return 0;   
}

goForward(int PWM)
{
     //PORTD |= (1 << 3) | ~(1 << 2); 
	OCR0A = 0;
	OCR2A = PWM;
}

goBackward(int PWM)
{
	OCR0A = PWM;
	OCR2A = 0;
}
