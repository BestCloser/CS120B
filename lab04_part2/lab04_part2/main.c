/*
 * lab04_part2.c
*/

#include <avr/io.h>
//#include "RIMS.h"

enum PORTC_states { INIT, MAIN_WAIT, ADD, ADD_WAIT, SUBTRACT, SUB_WAIT, RESET, RESET_WAIT } PORTC_State;

void PORTC_Tick()
{
	unsigned char A0 = ~PINA & 0x01; //bitmasks, want PA0 and PA1	
	unsigned char A1 = ~PINA & 0x02;
	unsigned char tmpC = PORTC & 0x0F; //max 9, min 0
	switch(PORTC_State) {   // Transitions
		case INIT:  // Initial transition
		PORTC = 0x07;
		PORTC_State = MAIN_WAIT;
		break;
		
		case MAIN_WAIT:
		if ( !A0 && !A1 ) {
			PORTC_State = MAIN_WAIT;
		}
		else if ( A0 && !A1 ) {
			PORTC_State = ADD;
		}
		else if ( !A0 && A1 ) {
			PORTC_State = SUBTRACT;
		}
		else if ( A0 && A1 ) {
			PORTC_State = RESET;
		}
		break;

		case ADD:
		PORTC_State = ADD_WAIT;
		break;

		case ADD_WAIT:
		if ( !A0 ) { //both (!A0 && !A1) and (!A0 && A1)
			PORTC_State = MAIN_WAIT;
		}
		else if ( A0 && !A1 ) {
			PORTC_State = ADD_WAIT;
		}
		else if ( A0 && A1 ) {
			PORTC_State = RESET;
		}
		break;

		case SUBTRACT:
		PORTC_State = SUB_WAIT;
		break;

		case SUB_WAIT:
		if ( !A1 ) { //includes (!A0 && !A1) and (A0 && !A1)
			PORTC_State = MAIN_WAIT;
		}
		else if ( !A0 && A1 ) {
			PORTC_State = SUB_WAIT;
		}
		else if ( A0 && A1 ) {
			PORTC_State = RESET;
		}
		break;
		
		case RESET:
		PORTC_State = RESET_WAIT;
		break;

		case RESET_WAIT:
		if ( !A0 && !A1 ) {
			PORTC_State = MAIN_WAIT;
		}
		else {
			PORTC_State = RESET_WAIT;
		}
		break;
		
		default:
		PORTC_State = INIT;
		break;
	} // Transitions

	switch(PORTC_State) {   // State actions
		case INIT:
		tmpC = 0x07;
		PORTC = tmpC; //init = 7
		break;

		case ADD:
		if (tmpC < 9) {
			tmpC = tmpC + 1;
		} //else tmpC is already 9
		PORTC = tmpC;
		break;

		case SUBTRACT:
		if (tmpC > 0) {
			tmpC = tmpC - 1;
		} //else tmpC is already 0
		PORTC = tmpC;
		break;

		case RESET:
		//tmpC = 0x00;
		PORTC = 0x00;
		break;

		default:
		break;
	} // State actions
}


int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;              // Initialize outputs
	PORTC_State = INIT; // Indicates initial call

	while(1) {
		PORTC_Tick();
	}
}

