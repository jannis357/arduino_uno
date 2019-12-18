/*
 * led_with_switch_2b.c
 *
 * Created: 18.12.2019 10:00:00
 * Author : Jan
 * This program changes the state of an LED (ON/OFF) by pressing a button. Releasing the button does not change the state!
 * Implemented without an Interrupt, directly in the main routine!
 *
 * Connections:
 * --> LED to Pin 10 on Arduino  
 * --> Pin 12 --> Button --> GND
 */ 

#include <avr/io.h>		// The header files come with the drivers for the Arduino


static void led_init(void)
{
	DDRB = (1 << 2); // Configures Pin 2 of PORTB as output (should be Pin 10 on arduino); Code: 1 gets shifted by 2
}


static void button_init(void)
{
	DDRB = ~(1 << 4); // Configures Pin 4 of PORTB as input (should be Pin 12 on arduino; must be connected to GND via a switch)
	
	PINB |= (1 << 4);	// Pull input Pin at 5V as default 
}


int main(void)
{
	led_init();
	button_init();
	
	int a = 0;
	
	while (1)
	{
		if (a == 0)					// prevents that status of the led is toggled when the button is released!
		{
			if (!(PINB & (1 << 4)))	// Button pressed
			{
				PORTB ^= (1 << 2);	// bitwise XOR operation; changes state of bit
				a = 1;				// prevents that status of led is toggled when the button is released!
			}
		}
		
		if (PINB & (1 << 4))		// Button NOT pressed
		{
			a = 0;					// prevents that status of led is toggled when the button is released!
		}
	}
}