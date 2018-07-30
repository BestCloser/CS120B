/*
 * lab01_part1.c
 *
 * Created: 7/30/2018 2:49:05 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs,
	// initialize to 0s
	unsigned char tmpB = 0x00; 	// Temporary variable to hold the value of B
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	while(1)
	{
		// (1) Read input
		tmpA = PINA & 0x0F;
		// (2) Perform computation
		// if PA0 is 1, set PB1PB0=01, else=10
		if (tmpA == 0x01) { // True if PA0 is 1, PA1 is 0
			tmpB = 0x01; // (clear rightmost 2 bits, then set to 1)
		}
		else {
			tmpB = 0x00; // (clear rightmost 2 bits, then set to 0)
		}
		// (3) Write output
		PORTB = tmpB;
	}
	return 0;
}


