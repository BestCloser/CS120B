/*
 * lab03_part1.c
 *
 * Created: 8/1/2018 1:51:51 PM
 * Author : Daniel
 */ 

#include <avr/io.h>
//#include "RIMS.h"

enum LED_states { INIT, PB0_ON, PB1_ON } LED_State;

void LED_Tick()
{
	unsigned char tmpA = PINA & 0x01; //bitmask, only want tmpA	

	switch(LED_State) {   // Transitions
		case INIT:  // Initial transition
		LED_State = PB0_ON;
		break;

		case PB0_ON:
		if (!tmpA) {
			LED_State = PB0_ON;
		}
		else if (tmpA) {
			LED_State = PB1_ON;
		}
		break;

		case PB1_ON:
		if (!tmpA) {
			LED_State = PB1_ON;
		}
		else if (tmpA) {
			LED_State = PB0_ON;
		}
		break;

		default:
		LED_State = INIT;
		break;
	} // Transitions

	switch(LED_State) {   // State actions
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

