/*
 * lab07_part3.c
 * Author : Daniel
 */ 

#include <avr/io.h>
//#include "RIMS.h"
#include "timer.c"

enum ThreeLED_States { ONE, TWO, THREE } ThreeLED_State;
	
void ThreeLED_Tick() {
	switch(ThreeLED_State) {
		
		case ONE:
		PORTB = (PORTB & 0x08) | 0x01;
		ThreeLED_State = TWO;
		break;
		
		case TWO:
		PORTB = (PORTB & 0x08) | 0x02;
		ThreeLED_State = THREE;
		break;
		
		case THREE:
		PORTB = (PORTB & 0x08) | 0x04;
		ThreeLED_State = ONE;
		break;
	}
}

enum BlinkingLED_States { Blink_OFF, Blink_ON } BlinkingLED_State;
void BlinkingLED_Tick() {
	switch(BlinkingLED_State) {
		
		case Blink_OFF:
		PORTB = PORTB & 0x07;
		BlinkingLED_State = Blink_ON;
		break;
		
		case Blink_ON:
		PORTB = PORTB | 0x08;
		BlinkingLED_State = Blink_OFF;
		break;
	}
}

enum Speaker_States { Speaker, Speaker_OFF } Speaker_State;
	
void Speaker_Tick() {
	switch(Speaker_State) {
		case Speaker:
		if ((~PINA & 0x04) == 0x00) {
			PORTB = PORTB & 0x0F; //PB4 = 0;
			Speaker_State = Speaker;
		}
		else if ((~PINA & 0x04) == 0x04) { //PA2 is on
			PORTB = PORTB | 0x10;
			Speaker_State = Speaker_OFF;
		}
		break;
		
		case Speaker_OFF:
		PORTB = PORTB & 0x0F; //speaker 2ms on/off
		Speaker_State = Speaker;
		break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; //*NEW: PA2 is button for sound
	DDRB = 0xFF; PORTB = 0x00;
	 
	unsigned long ThreeLED_elapsedTime = 0;
	unsigned long BlinkingLED_elapsedTime = 0;
//	unsigned long Speaker_elapsedTime = 0;
	const unsigned long timerPeriod = 1;
	//B = 0;
	TimerSet(1);
	TimerOn();
	ThreeLED_State = ONE;
	BlinkingLED_State = Blink_OFF;
	Speaker_State = Speaker;
	while (1) {
		if (ThreeLED_elapsedTime >= 38) { //125 is 1000ms, 300ms is 125*.3 = 37.5 => 38 
			ThreeLED_Tick();
			ThreeLED_elapsedTime = 0;
		}
		if (BlinkingLED_elapsedTime >= 125) {
			BlinkingLED_Tick();
			BlinkingLED_elapsedTime = 0;
		}
		Speaker_Tick();
/*		if ((~PINA & 0x04) == 0x04) { //PA2 == 1;
			Speaker_elapsedTime += timerPeriod;
		}
*/		
		while (!TimerFlag){} // Wait for timer period
		TimerFlag = 0; // Lower flag raised by timer

		ThreeLED_elapsedTime += timerPeriod;
		BlinkingLED_elapsedTime += timerPeriod;
	}
}