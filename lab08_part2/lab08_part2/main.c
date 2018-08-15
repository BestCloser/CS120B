/*
 * lab08_part2.c
 *
 * Created: 8/15/2018 2:32:08 PM
 * Author : Daniel
 */ 

#include <avr/io.h>


int main(void)
{

    /* Replace with your application code */
	DDRA = 0xFF; PORTA = 0x00;
		
    while (1) 
    {
		PORTA = 0xFF;
    }
}

