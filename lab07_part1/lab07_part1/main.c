/*
 * lab07_part1.c
 *
 * Cated: 8/13/2018 1:47:27 PM
 * Author : Daniel
 */ 

#include <avr/io.h>
//#include "RIMS.h"
#include "timer.c"

enum ThreeLED_States { ThreeLED_Start, ONE, TWO, THREE } ThreeLED_State;
	
void ThreeLED_Tick() {
	switch(ThreeLED_State) {
		case ThreeLED_Start:
		PORTB = 0x00;
		ThreeLED_State = ONE;
		break;
		
		case ONE:
		PORTB = 0x01;
		ThreeLED_State = TWO;
		break;
		
		case TWO:
		PORTB = 0x02;
		ThreeLED_State = THREE;
		break;
		
		case THREE:
		PORTB = 0x04;
		ThreeLED_State = ONE;
		break;
	}
}

enum BlinkingLED_States { BlinkingLED_Start, OFF, ON } BlinkingLED_State;
void BlinkingLED_Tick() {
	switch(BlinkingLED_State) {
		case BlinkingLED_Start:
		//PORTB = 0;
		ThreeLED_State = OFF;
		break;
		
		case OFF:
		PORTB = PORTB | 0x00;
		BlinkingLED_State = ON;
		break;
		
		case ON:
		PORTB = PORTB | 0x08;
		BlinkingLED_State = OFF;
		break;
	}
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	 
	unsigned long ThreeLED_elapsedTime = 0;
	unsigned long BlinkingLED_elapsedTime = 0;
	const unsigned long timerPeriod = 125;
	//B = 0;
	TimerSet(125);
	TimerOn();
	ThreeLED_State = ONE;
	BlinkingLED_State = OFF;
	while (1) {
		if (ThreeLED_elapsedTime >= 125) {
			ThreeLED_Tick();
			ThreeLED_elapsedTime = 0;
		}
		if (BlinkingLED_elapsedTime >= 125) {
			
			BlinkingLED_Tick();
			BlinkingLED_elapsedTime = 0;
		}
		while (!TimerFlag){} // Wait for timer period
		TimerFlag = 0; // Lower flag raised by timer

		ThreeLED_elapsedTime += timerPeriod;
		BlinkingLED_elapsedTime += timerPeriod;
	}
}