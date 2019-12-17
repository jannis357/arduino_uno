/*
 * Arduino_Uno.c
 *
 * Created: 30.11.2019 17:42:13
 * Author : Jan
 */ 

#define F_CPU 16000000L // Specify oscillator frequency; Needed for _delay_ms()
#include <avr/io.h>		// The two header files come with the drivers for the Arduino
#include <util/delay.h>	// They include the _delay_ms() function and some defines (Right click --> Go to implementation)

int main(void)
{
    DDRB = (1 << 2); // Configure Pin 2 of PORTB as output (should be Pin 10 on arduino); Code: 1 gets shifted by 2
	while (1) 
    {
		PORTB |= (1 << 2);	// ver-odert
		_delay_ms(500); // Needs F_CPU
		PORTB &= ~(1 << 2);	// ver-undet
		_delay_ms(100);
    }
}