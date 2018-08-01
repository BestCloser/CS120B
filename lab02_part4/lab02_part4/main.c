/*
 * lab02_part4.c
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
    DDRA = 0x00; PORTA = 0xFF; //PA[7:0] is input
	DDRB = 0x0F; PORTB = 0xF0; //PB[3:0] is output
	DDRC = 0xF0; PORTC = 0x0F; //PC[7:4] is output
	
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	
	while (1) 
    {
		tmpA = PINA;
		//PA7>PB3, PA6>PB2, PA5>PB1, PA4>PB0, PA3>PC7, PA2>PC6, PA1>PC5, PA0>PC4
		tmpB = SetBit(tmpB, 3, GetBit(tmpA, 7));
		tmpB = SetBit(tmpB, 2, GetBit(tmpA, 6));
		tmpB = SetBit(tmpB, 1, GetBit(tmpA, 5));
		tmpB = SetBit(tmpB, 0, GetBit(tmpA, 4));
		
		tmpC = SetBit(tmpC, 7, GetBit(tmpA, 3));
		tmpC = SetBit(tmpC, 6, GetBit(tmpA, 2));
		tmpC = SetBit(tmpC, 5, GetBit(tmpA, 1));
		tmpC = SetBit(tmpC, 4, GetBit(tmpA, 0));
		
		PORTB = tmpB & 0x0F;
		PORTC = tmpC & 0xF0;
    }
}

