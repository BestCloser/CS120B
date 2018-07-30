/*
 * lab01_part3.c
 *
 * Created: 7/30/2018 4:31:10 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	// initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char cntavail;
	while(1)
	{
		// (1) Read input
		tmpA = PINA & 0x0F;
		// (2) Perform computation

		if ( (tmpA == 0x00) ) {
			cntavail = 0x04;
		}
		else if ( (tmpA == 0x01) || (tmpA == 0x02) || (tmpA == 0x04) || (tmpA == 0x08) ) {
			cntavail = 0x03;
		}
		else if ( (tmpA == 0x03) || (tmpA == 0x05) || (tmpA == 0x06) || (tmpA == 0x09) || (tmpA == 0x0A) || (tmpA == 0x0C) ) {
			cntavail = 0x02;
		}
		else if ( (tmpA == 0x07) || (tmpA == 0x0B) || (tmpA == 0x0D) || (tmpA == 0x0E) ) {
			cntavail = 0x01;
		}
		else {
			cntavail = 0x40;
		}
		// (3) Write output
		
		PORTC = cntavail;
		
	}
	return 0;
}


