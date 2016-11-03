#include<avr/io.h>
#define F_CPU 1000000UL 
#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

//what should be compelted: I/O console input and reply. Speed of one wheel. 
//TODO: debugging. update it to work on on more than one wheel. And go in reverse
//if char does not work, and the input is uin8_t, then it means that the buffer must be created to an array of chars. 

void initWheels()
{
	DDRB |= (1<<0); 
	DDRD |= (1<<0); 
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1<< WGM00);
	TCCR0B |= (1 << CS00);
	OCR0A = 255;

	PORTB |= (0 << PB5) | (1 << PB4);
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
	initUart();
	volatile char m8;

    	while(1)
   	 {
		m8=receiveByte();
	
		if(m8 == 1)	
		{
			OCCR0A=0;
			m8 = 4; 
		}
		else if(m8 == 2)
		{
			OCCR0A =125;
			m8 = 4; 
		}
		else if(m8 == 3)
		{
			OCCR0A = 255;
			m8 = 4; 
		}
		else //invalid input
		{
			m8 = 0;
		}
		
		sendByte(m8);
			

    	}
    	return 0;   
}

