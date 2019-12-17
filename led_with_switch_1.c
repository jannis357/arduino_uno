/*
 * led_with_switch_1.c
 *
 * Created: 08.12.2019 21:44:00
 * Author : Jan
 * This program turns on an LED when a switch is pressed
 */ 

#define F_CPU 16000000L // Specify oscillator frequency; Needed for _delay_ms()
#include <avr/io.h>		// The two header files come with the drivers for the Arduino
#include <util/delay.h>	// They include the _delay_ms() function and some defines (Right click --> Go to implementation)

int main(void)
{
    DDRB = (1 << 2); // Configures Pin 2 of PORTB as output (should be Pin 10 on arduino); Code: 1 gets shifted by 2
	DDRB = ~(1 << 4); // Configures Pin 4 of PORTB as input (should be Pin 12 on arduino; must be connected to GND via a switch)
	
	PINB |= (1 << 4);	// Pull input Pin at 5V as default
	
	while (1) 
    {
		if (PINB & (1 << 4))	// If input pin is at 5V (default state) --> Output is OFF
		{
			PORTB &= ~(1 << 2);
		}
		
		if (!(PINB & (1 << 4)))	// If input pin is pulled down to 0V (via the physical switch) --> Output is ON
		{
			PORTB |= (1 << 2);
		}
    }
}

