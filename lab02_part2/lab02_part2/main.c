/*
 * lab02_part2.c
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
	DDRC = 0xFF; PORTC = 0x00; //PC5 ... PC0 LED for fuel level, lv1 or 2 = PC5; lv3 or 4, PC5 and PC4; lv5 or 6, PC5,4,3 ; etc ; PC6 is LOW FUEL, lights up if level is 4 or less
    unsigned char tmpA = 0x00;
	unsigned char tmpC = 0x00;
	while (1) 
    {
		tmpA = PINA & 0x0F; //bitmask, only want PA3,2,1,0
		tmpC = 0x00;
		if (tmpA == 0) { //check PC6 on or off
			SetBit(tmpC, 0, 1);
		}
		tmpC = (tmpC >> 1); //shift left
		if ( (tmpA > 0) && (tmpA <= 2) ) { //5
			SetBit(tmpC, 0, 1);
			SetBit(tmpC, 1, 1);
		}
		else {
			SetBit(tmpC, 0, 1);
		}
		tmpC = (tmpC >> 1);
		if ( (tmpA > 2) && (tmpA <= 4) ) { //4
			SetBit(tmpC, 0, 1);
			SetBit(tmpC, 1, 1);
			SetBit(tmpC, 2, 1);
		}
		else {
			SetBit(tmpC, 0, 1);
		}
		tmpC = (tmpC >> 1);
		if ( (tmpA > 4) && (tmpA <= 6) ) { //3
			SetBit(tmpC, 0, 1);
			SetBit(tmpC, 1, 1);
			SetBit(tmpC, 2, 1);
			SetBit(tmpC, 3, 0);
		}
		else {
			SetBit(tmpC, 0, 1);
		}
		tmpC = (tmpC >> 1);
		if ( (tmpA > 6) && (tmpA <= 9) ) { //2
			SetBit(tmpC, 0, 1);
			SetBit(tmpC, 1, 1);
			SetBit(tmpC, 2, 1);
			SetBit(tmpC, 3, 1);
			SetBit(tmpC, 4, 0);
		}
		else {
			SetBit(tmpC, 0, 1);
		}
		tmpC = (tmpC >> 1);
		if ( (tmpA > 9) && (tmpA <= 12) ) { //1
		SetBit(tmpC, 0, 1);
			SetBit(tmpC, 1, 1);
			SetBit(tmpC, 2, 1);
			SetBit(tmpC, 3, 1);
			SetBit(tmpC, 4, 1);
			SetBit(tmpC, 5, 0);
		}
		else {
			SetBit(tmpC, 0, 1);
		}
		tmpC = (tmpC >> 1);
		if ( (tmpA > 12) && (tmpA <= 15) ) {  //0
			SetBit(tmpC, 0, 1);
			SetBit(tmpC, 1, 1);
			SetBit(tmpC, 2, 1);
			SetBit(tmpC, 3, 1);
			SetBit(tmpC, 4, 1);
			SetBit(tmpC, 5, 1);
			SetBit(tmpC, 6, 0);
		}
		
		PORTC = tmpC & 0x7F;
    }
	return 0;
}

