/*
 * lab02_part1.c
 *
 * Created: 7/31/2018 2:04:27 PM
 * Author : Daniel
 */ 

#include <avr/io.h>


// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
    return ((x & (0x01 << k)) != 0);
}


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF; //inputs A, B
	DDRC = 0xFF; PORTC = 0x00; //output C

	unsigned char tmpA = 0x00; // init
	unsigned char tmpB = 0x00;
	unsigned char counter = 0x00; //counter for number of 1's
    while (1) 
    {
		tmpA = PINA;
		tmpB = PINB;
		counter = 0;
		while ( tmpA != 0x00 ) {
			if ( GetBit(tmpA, 0) ) {
				counter++;
			}
			tmpA = (tmpA >> 1);
		}
		while ( tmpB != 0x00 ) {
			if ( GetBit(tmpB, 0) ) {
				counter++;
			}
			tmpB = (tmpB >> 1);
		}

			PORTC = counter; //assign number of 1s to port c		
	}
	return 0;
}

