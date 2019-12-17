/*
 * led_with_switch_2.c
 *
 * Created: 17.12.2019 18:00:00
 * Author : Jan
 * This program changes the state of an LED (ON/OFF) by pressing a button. Releasing the button does not change the state!
 * Implemented with an Interrupt
 *
 * Connections:
 * --> LED to Pin 10 on Arduino  
 * --> Pin 12 --> Button --> GND
 */ 

#include <avr/io.h>		// The header files come with the drivers for the Arduino
#include <avr/sleep.h>
#include <avr/interrupt.h>

int switch_var;		// To save state of LED (ON/OFF)


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
	switch_var = 1;
}


void led_off(void)
{
	PORTB &= ~(1 << 2);
	switch_var = 0;
}


void switch_led_status(int switch_var)
{
	if (switch_var == 0)	// LED is OFF
	{
		led_on();
	}
	
	if (switch_var == 1)	// LED is ON
	{
		led_off();
	}
}


ISR(PCINT0_vect) /* pin change interrupt service routine */
{
	if (!(PINB & (1 << 4)))	// Button pressed
	{
		switch_led_status(switch_var);
	}
}


int main(void)
{
	led_init();
	button_init();
	led_off();
	sei();	// enables interrupts globally
	
	while (1)
	{
		sleep_mode();
	}
}