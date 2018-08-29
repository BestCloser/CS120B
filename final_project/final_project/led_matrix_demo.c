
//sample LED code from LED matrix pre-lab
/* 
   the synchSM should display a demo on the LED matrix
   where a single illuminated LED scrolls from left to 
   right, top to bottom, then resets. The synchSM period 
   should be at least 50-100 ms in order for the demo
   to be visible. 
*/

#include <avr/io.h>
#include "timer.h"
#include "bit.h"
#include "scheduler.h"
#include "io.h"
#include "io.c"
// ====================
// SM1: DEMO LED matrix
// ====================
enum SM1_States {sm1_display};
int SM1_Tick(int state) {

    // === Local Variables ===
    static unsigned char column_val = 0x01; // sets the pattern displayed on columns
    static unsigned char column_sel = 0x7F; // grounds column to display pattern
    
    // === Transitions ===
    switch (state) {
   	 case sm1_display:    break;
   	 default:   	        state = sm1_display;
   			        break;
    }
    
    // === Actions ===
    switch (state) {
   	 case sm1_display:   // If illuminated LED in bottom right corner
        if (column_sel == 0xFE && column_val == 0x80) {
   				column_sel = 0x7F; // display far left column
				column_val = 0x01; // pattern illuminates top row
   			        }
       // else if far right column was last to display (grounded)	
			        else if (column_sel == 0xFE) {
   				column_sel = 0x7F; // resets display column to far left column
   				column_val = column_val << 1; // shift down illuminated LED one row
   			        }
			        // else Shift displayed column one to the right
   			        else {
   				column_sel = (column_sel >> 1) | 0x80;
   			        }
   			        break;
   	 default:   	        break;
    }
    
    PORTA = column_val; // PORTA displays column pattern
    PORTB = column_sel; // PORTB selects column to display pattern

    return state;    
};

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	
	// Period for the tasks
	unsigned long int SM1_calc = 100;

	//Calculating GCD
	unsigned long tempGCD = 1;
	tempGCD = findGCD(SM1_calc, SM1_calc);

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tempGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SM1_period = SM1_calc/GCD;
	
	//Declare an array of tasks
	static task SM1_Task;
	task *tasks[] = { &SM1_Task };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	SM1_Task.state = sm1_display;//Task initial state.
	SM1_Task.period = SM1_period;//Task Period.
	SM1_Task.elapsedTime = SM1_period;//Task current elapsed time.
	SM1_Task.TickFct = &SM1_Tick;//Function pointer for the tick.

	// Set the timer and turn it on
/*
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_WriteData(0 + '0');
*/
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

