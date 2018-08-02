	/*
 * lab03_part1.c
 *
 * Created: 8/1/2018 1:51:51 PM
 * Author : Daniel
 */ 

#include <avr/io.h>
//#include "RIMS.h"

enum LED_states { INIT, WAIT_01, PB1_ON, WAIT_10, WAIT_11, PB0_ON, WAIT_00 } LED_State;

void LED_Tick()
{
	unsigned char tmpA = PINA & 0x01; //bitmask, only want tmpA	

	switch(LED_State) {   // Transitions
		case INIT:  // Initial transition
		PORTB = 0x01; //init condition
		LED_State = WAIT_01;
		break;

		case WAIT_01:
		if (!tmpA) {
			LED_State = WAIT_01;
		}
		else if (tmpA) {
			LED_State = PB1_ON;
		}
		break;

		case PB1_ON:

		LED_State = WAIT_10;

		break;

		case WAIT_10:
		if (!tmpA) {
			LED_State = WAIT_11;
		}
		else if (tmpA) {
			LED_State = WAIT_10;
		}
		break;

		case WAIT_11:
		if (!tmpA) {
			LED_State = WAIT_11;
		}
		else if (tmpA) {
			LED_State = PB0_ON;
		}
		break;

		case PB0_ON:
		LED_State = WAIT_00;
		break;

		case WAIT_00:
		if (!tmpA) {
			LED_State = WAIT_01;
		}
		else if (tmpA) {
			LED_State = WAIT_00;
		}
		break;

		default:
		LED_State = INIT;
		break;
	} // Transitions

	switch(LED_State) {   // State actions

		case INIT:
		PORTB = 0x01; //init PB0 = 1; PB1 = 0;
		break;
		
		case PB0_ON:
		//PB0 = 1, PB1 = 0
		PORTB = 0x01;
		break;

		case PB1_ON:
		//PB0 = 0, PB1 = 1
		PORTB = 0x02;
		break;

		default:
		break;
	} // State actions
}


int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;              // Initialize outputs
	LED_State = INIT; // Indicates initial call

	while(1) {
		LED_Tick();
	}
}

