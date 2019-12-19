/*
 * finite_state_machine.c
 *
 * Created: 18.12.2019 13:00:00
 * Author : Jan
 *
 * Simple cyclic finite state machine (FSM) with an LED
 * Uses Interrupts to 'sense' the state change
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
#include <avr/interrupt.h>

int b = 1;  // cyclic switch variable; gets raised by 1 everytime the button is pressed (and back to 1 at the end); needs to be global, so that it can be changed by the interrupt!


static void led_init(void)
{
	DDRB = (1 << 2); // Configures Pin 2 of PORTB as output (should be Pin 10 on arduino); Code: 1 gets shifted by 2
}


static void button_init(void)
{
	DDRB = ~(1 << 4); // Configures Pin 4 of PORTB as input (should be Pin 12 on arduino; must be connected to GND via a switch)
	
	PINB |= (1 << 4);	// Pull input Pin at 5V as default 
	
	PCICR |= (1 << PCIE0);	// PCICR = Pin Change Interrupt Control Register; see OneNote for details, short: selects which Port is used for the interrupt
	PCMSK0 |= (1 << PCINT4);	// makes Pin 4 to interrupt pin; PCMSK = Pin Change Enable Mask Register
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


void led_blink_250ms(void)
{
	PORTB ^= (1 << 2); // changes state of bit
	_delay_ms(250);
}


void led_dim(void)	// TO_DO (probably with timer and/or PWM)
{
	PORTB ^= (1 << 2); // changes state of bit
	_delay_ms(100);
}


ISR(PCINT0_vect) /* pin change interrupt service routine */
{
	if (!(PINB & (1 << 4)))	// Button pressed
	{
		b++;
		_delay_ms(50);	// SW entprellen! Keep it short, so that one can press the button very quickly!
	}
}



int main(void)
{
	led_init();
	button_init();
	sei();	// enables interrupts globally
	
	int number_of_states = 5;
	
	while (1)
	{	
		if (b > number_of_states || b <= 0)					// resets switch variable b to initial value
		{
			b = 1;
		}
		
		switch (b)		// Main switch routine that selects the different states of the FSM
		{
			case 1:
			led_off();
			break;
			
			case 2:
			led_on();		// Could add an additional if to only execute led_on() if the LED is OFF.
			break;
			
			case 3:
			led_blink_1s();	// 1s blink; it seems that _delay_ms() does not work with a variable as input, that's why I wrote seperate functions!
			break;
			
			case 4:
			led_blink_250ms();
			break;
			
			case 5:
			led_dim();	// TO_DO
			break;
			
			default:
			led_off();
			b = 1;
			break;
		}
	}
}
