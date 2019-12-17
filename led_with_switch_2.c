/*
 * led_with_switch_2.c
 *
 * Created: 17.12.2019 18:00:00
 * Author : Jan
 * This program changes the state of an LED (ON/OFF) by pressing a button. Releasing the button does not change the state!
 */ 

#define F_CPU 16000000L // Specify oscillator frequency; Needed for _delay_ms()
#include <avr/io.h>		// The two header files come with the drivers for the Arduino
#include <util/delay.h>	// They include the _delay_ms() function and some defines (Right click --> Go to implementation)
#include <avr/sleep.h>
#include <avr/interrupt.h>


static void led_init(void)
{
	DDRB = (1 << 2); // Configures Pin 2 of PORTB as output (should be Pin 10 on arduino); Code: 1 gets shifted by 2
}

static void button_init(void)
{
	DDRB = ~(1 << 4); // Configures Pin 4 of PORTB as input (should be Pin 12 on arduino; must be connected to GND via a switch)
	
	PINB |= (1 << 4);	// Pull input Pin at 5V as default
	
	PCICR |= (1 << PCIE0);	// enables Pin Change 0 interrupt
	PCMSK0 |= (1 << PCINT4);	// makes Pin 4 to interrupt pin
}

static void led_on(void)
{
	PORTB |= (1 << 2);
}


static void led_off(void)
{
	PORTB &= ~(1 << 2);
}

ISR(PCINT0_vect) /* pin change interrupt service routine */
{
	if (PINB & (1 << 4))	// If input pin is at 5V (default state) --> Output is OFF
	{
		led_off();
	}
	
	if (!(PINB & (1 << 4)))	// If input pin is pulled down to 0V (via the physical switch) --> Output is ON
	{
		led_on();
	}
}




int main(void)
{
	led_init();
	button_init();
	sei();	// enables interrupts globally
	
	while (1)
	{
		sleep_mode();
	}
}