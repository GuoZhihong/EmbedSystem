// this program enables SPI communication and
// Sets the AVR into Slave mode

#include <avr/io.h>
#include <avr/interrupt.h>

volatile data;

int main (void)
{
    DDRB &= ~((1<<2)|(1<<3)|(1<<5));   // SCK, MOSI and SS as inputs
    DDRB |= (1<<4);                    // MISO as output

    SPCR &= ~(1<<MSTR);                // Set as slave 
    SPCR |= (1<<SPR0)|(1<<SPR1);       // divide clock by 128
    SPCR |= (1<<SPIE);                 // Enable SPI Interrupt
    SPCR |= (1<<SPE);                  // Enable SPI

    sei();

    while(1)
    {
        ;
    }
}

ISR (SPI_STC_vect)
{
    data = SPDR;
    // do something with the received data
	
}