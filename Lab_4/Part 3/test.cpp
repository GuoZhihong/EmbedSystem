#include <avr/io.h>
#define F_CPU 10000000  // system clock is 20 MHz
#include <util/delay.h>  // uses F_CPU to achieve us and ms delays
 
// Motor Control Functions -- pwm is an 8-bit value
//  (i.e. ranges from 0 to 255)
 
void M1_forward(unsigned char pwm)
{
  OCR0A = 0;
  OCR2A = pwm;
}
 
void M1_reverse(unsigned char pwm)
{
  OCR0B = 0;
  OCR0A = pwm;
}
 
void M2_forward(unsigned char pwm)
{
  OCR2A = 0;
  OCR2B = pwm;
}
 
void M2_reverse(unsigned char pwm)
{
  OCR2B = 0;
  OCR2A = pwm;
}
 
 
// Motor Initialization routine -- this function must be called
//  before you use any of the above functions
void motors_init()
{
    // configure for inverted PWM output on motor control pins:
    //  set OCxx on compare match, clear on timer overflow
    //  Timer0 and Timer2 count up from 0 to 255
    TCCR0A = TCCR2A = 0xF3;
 
    // use the system clock/8 (=2.5 MHz) as the timer clock
    TCCR0B = TCCR2B = 0x01; //start the clock
 
    // initialize all PWMs to 0% duty cycle (braking)
    OCR0A = OCR0B = OCR2A = OCR2B = 0;
 
    // set PWM pins as digital outputs (the PWM signals will not
    // appear on the lines if they are digital inputs)
   // DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    //DDRB |= (1 << PB3) | (1<< PB0);
	DDRB |= (1<< PB0); //| (1 << PB3) | (1 << PB4);
	DDRD |= (1 << PD7) | (1 << PD4) | (1 << PD2) | (1 << PD3) | (1 << PD6) | (1 << PD0);
	DDRB |= (1 << PB3) | (1 << PB4) | (1 << PB5);
}

 
// delay for time_ms milliseconds by looping
//  time_ms is a two-byte value that can range from 0 - 65535
//  a value of 65535 (0xFF) produces an infinite delay
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
 
int main()
{
    motors_init();
	
	PORTB |= 0b00110001;
	PORTD |= 0b01000000;
	//PORTD = 0b00000000; //PD7 should be on off
	//PORTD=(PORTD & 0b11111100) | 0b10 ;
    M1_forward(255);  // motor 1 forward at half speed
   // M2_reverse(25);  // motor 2 reverse at 10% speed
 
    delay_ms(5000);  // delay for 2s while motors run
 
    //M1_reverse(64);  // motor 1 reverse at 25% speed
    //M2_forward(0);  // motor 2 stop/brake
 
    // loop here forever to keep the program counter from
    //  running off the end of our program
    while (1)
        ;
 
    return 0;
}