/*
 * lab03_part3.c
 *
 * Created: 8/2/2018 2:07:50 AM
 * Author : Daniel
 */ 


#include <avr/io.h>
//#include "RIMS.h"
#include <string.h>

enum LOCK_States { INIT, MAIN_WAIT, X, X_WAIT, Y, Y_WAIT, HASH, HASH_WAIT } LOCK_State;

void LOCK_Tick()
{
	unsigned char A0 = PINA & 0x01; //bitmasks, want PA0(X) and PA1(Y) and PA2(#/HASH)
	unsigned char A1 = PINA & 0x02;
	unsigned char A2 = PINA & 0x04;
	char code[2];

	switch(LOCK_State) {   // Transitions
		case INIT:  // Initial transition
		PORTB = 0x00;
		LOCK_State = MAIN_WAIT;
		break;
		
		case MAIN_WAIT:
		if ( A0 && !A1 && !A2 ) {
			LOCK_State = X;
		}
		else if ( !A0 && A1 && !A2 ) {
			LOCK_State = Y;
		}
		else if ( !A0 && !A1 && A2 ) {
			LOCK_State = HASH;
		}
		else {
			LOCK_State = MAIN_WAIT;
		}
		break;

		case X:
		LOCK_State = X_WAIT;
		break;

		case X_WAIT:
		if ( !A0 && !A1 && !A2) { 
			LOCK_State = MAIN_WAIT;
		}
		else {
			LOCK_State = X_WAIT;
		}
		break;

		case Y:
		LOCK_State = Y_WAIT;
		break;

		case Y_WAIT:
		if ( !A0 && !A1 && !A2) { 
			LOCK_State = MAIN_WAIT;
		}
		else {
			LOCK_State = Y_WAIT;
		}
		break;
		
		case HASH:
		LOCK_State = HASH_WAIT;
		break;

		case HASH_WAIT:
		if ( !A0 && !A1 && !A2) { 
			LOCK_State = MAIN_WAIT;
		}
		else {
			LOCK_State = HASH_WAIT;
		}
		break;
		
		default:
		LOCK_State = INIT;
		break;
	} // Transitions

	switch(LOCK_State) {   // State actions
		case INIT:
		PORTB = 0x00;
		break;
		
		case MAIN_WAIT:
		if (code[0] == '#' && code[1] == 'Y') {
			PORTB = 0x01;
		}
		break;
		
		case X:
		code = code + 'X';
		break;

		case Y:
		code = code + 'Y';
		break;

		case HASH:
		code[0] = '#';
		break;

		default:
		break;
	} // State actions
}


int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;              // Initialize outputs
	LOCK_State = INIT; // Indicates initial call

	while(1) {
		LOCK_Tick();
	}
}
