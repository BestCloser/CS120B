/*
 * lab10_part1.c
 *
 * Created: 8/21/2018 1:20:01 PM
 * Author : Daniel
 */ 
#include <avr/io.h>
#include "keypad.h"
#include "scheduler.h"

enum Keypad_States { init, wait };
task Keypad_Task;

int Keypad_SM(int state) {
	unsigned char input;

	switch(state) {
		case init:
		state = wait;
		break;
		
		case wait:
		input = GetKeypadKey();     // get current keypad press

		switch(input) {
			case '\0': PORTB = 0x1F; break;
			case '0': PORTB = 0x00; break;
			case '1': PORTB = 0x01; break;
			case '2': PORTB = 0x02; break;
			case '3': PORTB = 0x03; break;
			case '4': PORTB = 0x04; break;
			case '5': PORTB = 0x05; break;
			case '6': PORTB = 0x06; break;
			case '7': PORTB = 0x07; break;
			case '8': PORTB = 0x08; break;
			case '9': PORTB = 0x09; break;
			case 'A': PORTB = 0x0A; break;
			case 'B': PORTB = 0x0B; break;
			case 'C': PORTB = 0x0C; break;
			case 'D': PORTB = 0x0D; break;
			case '*': PORTB = 0x0E; break;
			case '#': PORTB = 0x0F; break;
			default: PORTB = 0x1B; break;
		}
		break;
		default: break;
	}

	return state;
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;	// B has outputs
	DDRC = 0xF0; PORTC = 0x0F;	// C has inputs/outputs

	Keypad_Task.state = init;
//	Keypad_Task.period = idk;
//	Keypad_Task.elapsedTime = idk;
	Keypad_Task.TickFct = &Keypad_SM;
	
//	TimerSet(GCD);
//	TimerOn();

	while(1)
	{
		Keypad_Task.state = Keypad_Task.TickFct(Keypad_Task.state);
	}
}