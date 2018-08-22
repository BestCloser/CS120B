/*
 * lab10_part2.c
 *
 * Created: 8/21/2018 5:25:17 PM
 * Author : Daniel
 */  

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "timer.h"
#include <stdio.h>
#include "io.h"
#include "io.c"
#include "scheduler.h"



unsigned char pause = 0;

enum Scroll_SM_States { Scroll_display };
const unsigned char array[] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'S', '1', '2', '0', 'B', ' ', 'i', 's', ' ', 'L', 'e', 'g', 'e', 'n', 'd', '.', '.', '.', ' ', 'w', 'a', 'i', 't', ' ', 'f', 'o', 'r', ' ', 'i', 't', ' ', 'D', 'A', 'R', 'Y', '!' };
unsigned char scroll_index = 0;

int ScrollSM(int state) {

	switch(state) {
		case Scroll_display:	
		if (scroll_index >= 52) {
			scroll_index = 0;
		}
		else {
			scroll_index++;
		}
		break;
		
		default:
		break;
	}
	
	return state;
}



enum Cursor_SM{ init, wait };

int CursorSM(int state){
	static unsigned char i, j;

	switch(state){
		case init:
		i = 1;
		j = scroll_index;
		LCD_Cursor(1);
		state = wait;
		break;
		
		case wait:
		state = wait;
		if(j >= sizeof(array)/sizeof(char)){
			LCD_WriteData(' ');
		}
		else {
			LCD_WriteData(array[j]);
		}
			
		if(i >= 16){
			state = Scroll_display;
		}
		else {
			i++;
			j++;
		}
		break;
	}
	
	
	return state;
}

int main()
{

	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	// Period for the tasks
	unsigned long int Scroll_calc = 400;
	unsigned long int Cursor_calc = 5;

	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(Scroll_calc, Cursor_calc);

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int Scroll_period = Scroll_calc/GCD;
	unsigned long int Cursor_period = Cursor_calc/GCD;

	//Declare an array of tasks 
	static task ScrollTask, CursorTask;
	task *tasks[] = { &ScrollTask, &CursorTask };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	ScrollTask.state = Scroll_display;
	ScrollTask.period = Scroll_period;
	ScrollTask.elapsedTime = Scroll_period;
	ScrollTask.TickFct = &ScrollSM;
	
	// Task 2
	CursorTask.state = init;
	CursorTask.period = Cursor_period;
	CursorTask.elapsedTime = Cursor_period;
	CursorTask.TickFct = &CursorSM;

	// Set the timer and turn it on
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_WriteData(0 + '0');
	
	TimerSet(GCD);
	TimerOn();

	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
}
