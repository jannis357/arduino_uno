# arduino_uno
Test files to learn C and push some codes on the Arduino Uno with Atmel Studio

The following will give a short description of the programs (normally further down is newer and better optimized:

blink_led --> LED blinks

led_with_switch_1 --> LED is on only when button is pressed
led_with_switch_1a --> same but with Interrupt

led_with_switch_2 --> changes state of LED by pressing a button; releasing it does not change the state! With interrupt
led_with_switch_2a --> same but with XOR switch
led_with_switch_2b --> same but without interrupt (button service routine in main function)

finite_state machine --> FSM with different LED options (with interrupt to toggle between states); Use this version
finite_state_machine --> Archived FSM without interrupts (button service routine in main); Do not use anymore!
