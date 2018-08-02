/*
 * lab03_part2.c
 *
 * Created: 8/1/2018 2:43:26 PM
 * Author : Daniel
 */

#include <avr/io.h>
//#include "RIMS.h"

enum PORTC_states { INIT, WAIT, ADD, SUBTRACT } PORTC_State;

void PORTC_Tick()
{
	unsigned char tmpA = PINA & 0x01; //bitmask, only want tmpA	

	switch(PORTC_State) {   // Transitions
		case INIT:  // Initial transition
		PORTC_State = WAIT;
		break;
		
		case WAIT:
		if (!tmpA) {
			PORTC_State = PB0_ON;
		}
		else if (tmpA) {
			PORTC_State = PB1_ON;
		}
		break;

		case ADD:
		if (!tmpA) {
			PORTC_State = PB1_ON;
		}
		else if (tmpA) {
			PORTC_State = PB0_ON;
		}
		break;
		
		case SUBTRACT:
		if (){
			//idk
		}
		else if () {
			
		}
		break;
		
		default:
		PORTC_State = INIT;
		break;
	} // Transitions

	switch(PORTC_State) {   // State actions
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
	DDRC = 0xF0; PORTC = 0x0F;              // Initialize outputs
	PORTC_State = INIT; // Indicates initial call

	while(1) {
		PORTC_Tick();
	}
}

