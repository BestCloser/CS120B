/*
 * lab01_challenge.c
 *
 * Created: 7/31/2018 12:22:10 PM
 * Author : Daniel
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A/B/C's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs,
	// initialize to 0s
	unsigned char tmpA = 0x00; 	// Temporary variable to hold the value of A,B,C,D
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	unsigned char tmpD = 0x00;
	while(1)
	{
		// (1) Read input
		tmpA = PINA;
		tmpB = PINB;
		tmpC = PINC;
		
		// (2) Perform computation
		if ( (tmpA + tmpB + tmpC) > 140 ) {
			tmpD = 0x01; //PD0 = 1
		}
		else {
			tmpD = 0x00;
		}
		signed short tmpAC = tmpA - tmpC;
		if ( abs(tmpAC) > 80 ) {
			tmpD = tmpD | 0x02 ; //PD1 = 1
		}
		
		
		// (3) Write output
		PORTD = tmpD;
	}
	return 0;
}