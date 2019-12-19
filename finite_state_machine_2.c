/*
 * finite_state_machine_2.c
 *
 * Created: 18.12.2019 13:00:00
 * Author : Jan
 *
 * Simple cyclic finite state machine (FSM) with an LED
 * Uses a timer in the main routine
 * States:
 * 1) LED off
 * 2) LED on
 * 3) LED blinks (1 s on, 1 s off)  
 * 4) LED blinks (250 ms on, 250 ms off)
 * 5) LED dims (going on from 0-100% and going of from 0-100%)
 *
 * Connections:
 * --> LED to Pin 10 on Arduino  
 * --> Pin 12 --> Button --> GND
 */ 

#define F_CPU 16000000L // Specify oscillator frequency; Needed for _delay_ms()
#include <avr/io.h>		// The header files come with the drivers for the Arduino
#include <avr/sleep.h>
#include <util/delay.h>	// They include the _delay_ms() function and some defines (Right click --> Go to implementation)

static void led_init(void)
{
	DDRB = (1 << 2); // Configures Pin 2 of PORTB as output (should be Pin 10 on arduino); Code: 1 gets shifted by 2
}


static void button_init(void)
{
	DDRB = ~(1 << 4); // Configures Pin 4 of PORTB as input (should be Pin 12 on arduino; must be connected to GND via a switch)
	
	PINB |= (1 << 4);	// Pull input Pin at 5V as default 
}


void led_on(void)
{
	PORTB |= (1 << 2);
}


void led_off(void)
{
	PORTB &= ~(1 << 2);
}


void led_blink_1s(void)
{
	PORTB ^= (1 << 2); // changes state of bit
	_delay_ms(1000);
}


int main(void)
{
	led_init();
	button_init();
	
	int a = 0;	// toggle variable; gets the value 1 if button is pressed; returns to 0 if button is released
	int b = 1;  // cyclic switch variable; gets raised by 1 everytime the button is pressed (and back to 1 at the end)
	int number_of_states = 3;
	
	while (1)
	{
		/* --------------------------Button service routine---------------------------------- */
		if (a == 0)					// prevents that status of the led is toggled when the button is released!
		{
			if (!(PINB & (1 << 4)))	// Button pressed
			{
				a = 1;				// prevents that status of led is toggled when the button is released!
				b++;
			}
		}
		
		if (PINB & (1 << 4))		// Button NOT pressed
		{
			a = 0;					// prevents that status of led is toggled when the button is released!
		}
		
		if (b > number_of_states || b <= 0)					// resets switch variable b to initial value
		{
			b = 1;
		}
		/* --------------------------Button service routine END---------------------------------- */
		
		switch (b)		// Main switch routine that selects the different states of the FSM
		{
			case 1:
			led_off();
			_delay_ms(50);	// SW entprellen! Keep it short, so that one can press the button very quickly!
			break;
			
			case 2:
			led_on();		// Could add an additional if to only execute led_on() if the LED is OFF.
			_delay_ms(50);
			break;
			
			case 3:
			led_blink_1s();	// 1s blink; it seems that _delay_ms() does not work with a variable as input!
			_delay_ms(50);
			
			break;
			
			default:
			led_off();
			_delay_ms(50);
			break;
		}
	}
}
