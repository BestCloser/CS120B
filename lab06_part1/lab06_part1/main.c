/*
 * lab06_part1.c
 *
 * Created: 8/9/2018 2:14:54 PM
 * Author : Daniel
 */ 


#include <avr/io.h>
#include "io.c"
#include "timer.c"

void Tick(unsigned char *prev);

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	DDRA = 0x00; PORTA = 0xFF; //A0 and A1 are inputs
	unsigned char cnt = 0;
	// Initializes the LCD display
	LCD_init();
	
	// Starting at position 1 on the LCD screen, writes Hello World
	//LCD_DisplayString(1, "0");
	//LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_WriteData(0 + '0');
	
	TimerSet(125);
	TimerOn();
	
	while(1) {
		Tick(&cnt);
//		LCD_WriteData(cnt + '0');
		while (!TimerFlag);
		TimerFlag = 0;
	}
}


void Tick(unsigned char *prev) {
	unsigned char A0 = ~PINA & 0x01;
	unsigned char A1 = ~PINA & 0x02;
	
	if (A0 && !A1) {
		if (*prev < 9) {
			*prev = *prev + 1;
		}
	}
	else if (!A0 && A1) {
		if (*prev > 0) {
			*prev = *prev - 1;
		}
	}
	else if (A0 && A1) {
		*prev = 0;
	}
	else { // if (!A0 && !A1)
		//do nothing;
	}
	LCD_Cursor(1);
	LCD_WriteData(*prev + '0');
}