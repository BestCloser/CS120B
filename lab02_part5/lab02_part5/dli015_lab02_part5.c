/*
 * lab02_part5.c
 *
 * Created: 8/1/2018 12:29:53 PM
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
    DDRD = 0x00; PORTD = 0xFF; //PD[7:0] is input
	DDRB = 0xFE; PORTB = 0x01; //PB0 is input, PB1 and PB2 are outputs
	
	unsigned char tmpD = 0x00;
	unsigned short weight = 0x000;
	unsigned char tmpB = 0x00;
    while (1) 
    {
		tmpD = PIND;
		tmpB = PINB & 0x01; //only want PB0
		weight = (tmpB << 8) & 0x100; //make weight 9 bits
		weight = weight + tmpD;
		if (weight >= 70) {
			tmpB = SetBit(tmpB, 1, 1); //keep PB0 and change PB1 to 1 
		} 
		else if ( (weight > 5) && (weight < 70) ) {
			tmpB = SetBit(tmpB, 1, 0);
			tmpB = SetBit(tmpB, 2, 1); //keep PB0 and change PB2 to 1 and PB1 stays/becomes 0
		}
		else {
			//tmpB = tmpB | 0x01; //do nothing tbh this part is useless
		}
		PORTB = tmpB & 0x07;
    }
}

