/*
 * lab08_part1.c
 *
 * Created: 8/15/2018 1:24:40 PM
 * Author : Daniel
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; //A[0] connected to potentiometer
	DDRB = 0xFF; PORTB = 0x00; //B[7:0] connected to LEDs
	DDRD = 0xFF; PORTD = 0x00; //D[1:0] connected to LEDs
	TimerOn();
	ADC_init(); //call function before while loop
	

	
    while (1) 
    {
/*HINT
		unsigned short x = 0xABCD;
		unsigned char my_char = (char)x; // my_char = 0xCD
		my_char = (char)(x >> 4); // my_char = 0xBC
*/

		unsigned short x = ADC;
		unsigned char my_char;
		unsigned char my_char2;	

		x = ADC; // 10 bits is 0x03FF
		my_char = (char)x; //0x03FF -> 0xFF
		
		PORTB = my_char; // B[7:0]
		
		my_char2 = (char)(x >> 8); //0x03FF -> 0x03, & 0x03 to keep the right 2 bits 
		PORTD = my_char2; // D[1:0]
		
    }
}

