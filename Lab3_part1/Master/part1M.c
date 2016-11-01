// this program enables SPI communication and
// Sets the AVR into Master mode using interrupts


#include <avr/io.h>
#include <avr/interrupt.h>

volatile data;

int main (void)
{
    char blah;

    DDRB |= (1<<2)|(1<<3)|(1<<5);    // SCK (System ClocK), MOSI(MasterOutputSlaveInput) and SS as outputs
    DDRB &= ~(1<<4);                 // MISO (MasterInputSlaveOutput) as input

    SPCR |= (1<<MSTR);               // Set as Master
    SPCR |= (1<<SPR0)|(1<<SPR1);     // divided clock by 128
    SPCR |= (1<<SPIE);               // Enable SPI Interrupt (when SPIF is set)    
    SPCR |= (1<<SPE);                // Enables control register


    sei();    

    while(1)
    {
        if ((SPSR & (1<<SPIF)) > 0)  // checks to see if the SPIF bit is clear
            data=blah;               // send the data

        // if multiple slaves, switch slave here.
    }
}


ISR (SPI_STC_vect)
{
    SPDR = data;
}