#include <avr/io.h>
#include "io.c"

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	// Initializes the LCD display
	LCD_init();
	LCD_ClearScreen();
//	LCD_Cursor(1);
//	LCD_WriteData(9 + '0'); 
	// Starting at position 1 on the LCD screen, writes Hello World
	LCD_DisplayString(1, "WTF");
	
	while(1) {continue;}
}
