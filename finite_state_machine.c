/*
 * finite_state_machine.c
 *
 * Created: 18.12.2019 13:00:00
 * Author : Jan
 *
 * Simple cyclic finite state machine (FSM) with an LED
 * Uses Interrupts for the state changes
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

#define LED_ON		PORTB |= (1 << 2)
#define LED_OFF		PORTB &= ~(1 << 2)


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


void PWM_init(void)		// Use Timer1 as this one can be used for 
{
	TCCR1B |= (1 << WGM12);		// CTC timer mode
	TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B);	// Output compare values
	OCR1A = 800;
	OCR1B = 100;	// Changing the two values changes duty cycle!
}


void fading_init(uint8_t milliS)	// Use Timer0 for changing the duty cycle
{
	TCCR0A |= (1 << WGM01);	// set to CTC mode
	// =milliS*7.8125-1    works out with formular from video with pre-scalar = 1024
	OCR0A =	milliS*7.8125-1;		// changes the top value
	
	TIMSK0 |= (1 << OCIE0A);	// enable interrupt
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


void led_dim(void)
{
	TCCR1B |= (1 << CS10);	// starts PWM (with timer with no pre-scalar)
	TCCR0B |= (1 << CS02) | (1 << CS00);	// starts fading Timer; sets pre-scalar to 1024
}

ISR(PCINT0_vect) /* pin change interrupt service routine */
{
	if (!(PINB & (1 << 4)))	// Button pressed
	{
		b++;
		_delay_ms(50);	// SW entprellen! Keep it short, so that one can press the button very quickly!
	}
}


ISR(TIMER1_COMPA_vect)	// Timer1 used for PWM
{
	LED_ON;
}


ISR(TIMER1_COMPB_vect)	// Timer1 used for PWM
{
	LED_OFF;
}


ISR(TIMER0_COMPA_vect)	// Timer0 for fading (changing duty cycle)
{
	uint16_t period = OCR1A;
	uint16_t duty = OCR1B;
	
	if (duty < period)
	{
		duty++;
	} 
	else
	{
		duty = 0;
	}
	OCR1B = duty;
}


int main(void)
{
	led_init();
	button_init();
	PWM_init();
	fading_init(2);	// wait 2ms for each step (up to 800 --> ~1.6s in total)
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
			TCCR1B &= ~(1 << CS10);	// stops timer!
			LED_OFF;
			break;
			
			case 2:
			LED_ON;		// Could add an additional if to only execute led_on() if the LED is OFF.
			break;
			
			case 3:
			led_blink_1s();	// 1s blink; it seems that _delay_ms() does not work with a variable as input, that's why I wrote seperate functions!
			break;
			
			case 4:
			led_blink_250ms();
			break;
			
			case 5:
			led_dim();
			break;
			
			default:
			LED_OFF;
			b = 1;
			break;
		}
	}
}
