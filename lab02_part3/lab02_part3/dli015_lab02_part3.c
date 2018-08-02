/*
 * lab02_part3.c
 *
 * Created: 7/31/2018 3:17:30 PM
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
    DDRA = 0x00; PORTA = 0xFF; //PA3 ... PA0 value between 0 and 15 (full)
	DDRB = 0xFF; PORTC = 0x00; //PC5 ... PC0 LED for fuel level, lv1 or 2 = PC5; lv3 or 4, PC5 and PC4; lv5 or 6, PC5,4,3 ; etc ; PC6 is LOW FUEL, lights up if level is 4 or less
    unsigned char tmpA = 0x00;
	unsigned char tmpA_2 = 0x00;
	unsigned char tmpC = 0x00;

	while(1) 
    {
		tmpA = PINA & 0x0F; //bitmask for PA3,2,1,0 ; PA4 is now [if key is in the ignition], PA5 is [if driver is seated], PA6 is [if seatbelt is fastened]
		tmpC = 0x00;	
		if (tmpA == 0) { //PC6 on, nothing else
			tmpC = SetBit(tmpC, 6, 1);
		}
		else if ( (tmpA == 1) || (tmpA == 2) ) {
			tmpC = SetBit(tmpC, 6, 1);
			tmpC = SetBit(tmpC, 5, 1);
		}
		else if ( (tmpA == 3) || (tmpA == 4) ) {
			tmpC = SetBit(tmpC, 6, 1);
			tmpC = SetBit(tmpC, 5, 1);
			tmpC = SetBit(tmpC, 4, 1);
		}
		else if ( (tmpA == 5) || (tmpA == 6) ) {
			tmpC = SetBit(tmpC, 6, 0);
			tmpC = SetBit(tmpC, 5, 1);
			tmpC = SetBit(tmpC, 4, 1);
			tmpC = SetBit(tmpC, 3, 1);
		}
		else if ( (tmpA == 7) || (tmpA == 8) ||(tmpA == 9) ) {
			tmpC = SetBit(tmpC, 6, 0);
			tmpC = SetBit(tmpC, 5, 1);
			tmpC = SetBit(tmpC, 4, 1);
			tmpC = SetBit(tmpC, 3, 1);
			tmpC = SetBit(tmpC, 2, 1);
		}
		else if ( (tmpA == 10) || (tmpA == 11) || (tmpA == 12) ) {
			tmpC = SetBit(tmpC, 6, 0);
			tmpC = SetBit(tmpC, 5, 1);
			tmpC = SetBit(tmpC, 4, 1);
			tmpC = SetBit(tmpC, 3, 1);
			tmpC = SetBit(tmpC, 2, 1);
			tmpC = SetBit(tmpC, 1, 1);
		}
		else if ( (tmpA == 13) || (tmpA == 14) || (tmpA == 15) ) {
			tmpC = SetBit(tmpC, 6, 0);
			tmpC = SetBit(tmpC, 5, 1);
			tmpC = SetBit(tmpC, 4, 1);
			tmpC = SetBit(tmpC, 3, 1);
			tmpC = SetBit(tmpC, 2, 1);
			tmpC = SetBit(tmpC, 1, 1);
			tmpC = SetBit(tmpC, 0, 1);
		}
		
		tmpA_2 = PINA & 0x70; //bitmask for PA6,5,4
		if ( GetBit(tmpA_2, 4) && GetBit(tmpA_2, 5) && !GetBit(tmpA_2, 6) ) {
			tmpC = SetBit(tmpC, 7, 1);
		}

		PORTC = tmpC;
    }
}
