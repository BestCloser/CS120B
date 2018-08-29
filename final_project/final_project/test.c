/*
	TEST.C
*/



#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "bit.h"
#include "scheduler.h"
#include "io.h"
#include "io.c"




// ====================
// VARIABLES AND STUFF
// ====================

unsigned short x_beats[8] = {0b01111111, 0b10111111, 0b11011111, 0b11101111, 0b11110111, 0b11111011, 0b11111101, 0b11111110};
//unsigned short y_beats[8] = {0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000};
unsigned short y_beats[8] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001};

unsigned char i = 1;
unsigned char j = 1;
unsigned char x_val = 0;
unsigned char y_val = 0;
unsigned char x_catcher = 0;
unsigned char bounds = 0;
unsigned char startGame = 0;
unsigned char gameOver = 0;
unsigned char beat_catch = 0;
unsigned char beat_miss = 0;
unsigned char health = 100;
unsigned char combo = 0;
unsigned char score = 0;

// =========================
// FUNCTIONS
// =========================

void transmit_data(unsigned char data) {
	int i;
	for (i = 0; i < 8 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTC |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
}

void check_health()
{
	if (health <= 0)
	{
		gameOver = 1;
	}
}

void CLEAR_ALL()
{
	PORTA =   0b00000000;
	PORTB = ~(0b00000000);
}


enum Player_states { Player_start, Release, main_state, initialization, right, left} Player_state;
int Player_Tick(int Player_state)
{
	unsigned char left_press = PIND & 0x02;
	unsigned char right_press = PIND & 0x01;
	startGame = PIND & 0x80;
	switch (Player_state) //State transitions
	{
		
		case Player_start:
		if(startGame == 0x80 && gameOver == 0)
		{
			Player_state = initialization;
		}
		else
		{
			Player_state = Player_start;
		}
		break;
		
		case initialization:
		Player_state = main_state;
		break;
		
		case main_state:
		if (left_press == 0x02)
		{
			Player_state = left;
//			_delay_ms(100);
		}
		else if(right_press == 0x01)
		{
			Player_state = right;
//			_delay_ms(100);
		}
		else {
			Player_state = main_state;
		}
		break;
		
		case Release:
		if(right_press == 0x01 && left_press == 0x00) //If A0 is 1(pressed) and A1 is 0(not pressed) increment
		{
			Player_state = right;
		}
		else if(left_press == 0x02 && right_press == 0x00) //If A0 is 1(pressed) and A1 is 0(not pressed) increment
		{
			Player_state = left;
		}
		break;
		
		case left:
		Player_state = Release;
		break;
		
		case right:
		Player_state = Release;
		break;
		
		default:
		Player_state = Player_start;
		break;
	}

	switch (Player_state) //State actions
	{
		case Player_start:
		break;
		
		case initialization:
		x_catcher = ~(0b00011000);
		bounds = 0x04;
		break;
		
		case main_state:
		break;
		
		case left:
		if (bounds > 0x01)
		{
			bounds--;
			x_catcher = ((x_catcher << 1) ^ 0x01);
//			_delay_ms(10);
		}
		break;
		
		case right:
		if (bounds < 0x07)
		{
			bounds++;
			x_catcher = ((x_catcher >> 1) ^ 0x80);
//			_delay_ms(10);
		}
		
		break;
		
		default:
		break;
	}
	return Player_state;
}




enum Beats_states {Beats_start, Beats_initialization, Beats_drop} Beats_state;
int Beats_Tick(int Beats_state)
{
	startGame = PIND & 0x80;
	
	switch (Beats_state) //State transitions
	{
		case Beats_start:
		if(startGame == 0x80 && gameOver == 0)
		{
			Beats_state = Beats_initialization;
		}
		else
		{
			Beats_state = Beats_start;
		}
		break;
		
		case Beats_initialization:
		i = 0;
		j = rand() % 8;
		Beats_state = Beats_drop;
		break;
		
		case Beats_drop:
		if (gameOver == 0)
		{
			if (i == 8) {
				i = 0;
				y_val = y_beats[i];
				x_val = x_beats[j];
				i++;
				j = rand() % 8;
			}
			else {
				y_val = y_beats[i];//y_val << 1; // shift down illuminated LED one row
				i++;
			}
		}
		else if(gameOver == 1)
		{
			CLEAR_ALL();
			Beats_state = Beats_start;
		}
		break;
		
		default:
		Beats_state = Beats_start;
		break;
	}

	switch (Beats_state) //State actions
	{
		case Beats_start:
		break;
		
		case Beats_initialization:
		break;
		
		case Beats_drop:
		break;

		default:
		break;
	}
	return Beats_state;
}


/*
enum Beats_States {Beats_display};
int Beats_Tick(int state) {

	
	// === Transitions ===
    switch (state) {
		case Beats_display:
		break;

   		default:
		state = Beats_display;
   		break;
    }

// === Actions === << EDITED >>
switch (state) {
	case Beats_display:   // If illuminated LED in bottom right corner
		if (i == 8) {
			i = 0;
			y_val = y_beats[i];
			x_val = x_beats[j];
			i++;
			j = rand() % 8;
		}
		else {
			y_val = y_beats[i];//y_val << 1; // shift down illuminated LED one row
			i++;
		}
	break;
	
	default:
	break;
}

//	
	
    
//  PORTA = y_val; // PORTA displays column pattern
//  PORTB = x_val; // PORTB selects column to display pattern
//	transmit_data(x_catcher);
    return state;    
};
*/
enum SM2_States {sm2_display};
int SM2_Tick(int state) {

    // === Local Variables ===
//    static unsigned char y_val = 0b00000001; // sets the pattern displayed on columns
//    static unsigned char x_val = 0b01111111; // grounds column to display pattern
      static unsigned char x_player = 0x03;
	
	// === Transitions ===
    switch (state) {
   	 case sm2_display:    break;
   	 default:   	        state = sm2_display;
   			        break;
    }



// === Actions === << EDITED >>
switch (state) {
	case sm2_display:   // If illuminated LED in bottom right corner	
	if (x_player == 0x0F) {
		x_player = 0x01;
	}
	else {
		x_player = (x_player << 1) | 0x01;
	}
	break;
	default:   	        break;
}

//	
//	transmit_data(x_player);
    return state;    
};



enum Matrix_Output_states {Start_Matrix_Output, Matrix_Output_catcher,  Matrix_Output_beats} Matrix_Output_state;
int Matrix_Output(int Matrix_Output_state)
{
	startGame = PIND & 0x80;
	switch (Matrix_Output_state) //State transitions
	{
		
		case Start_Matrix_Output:
		if (startGame == 0x80 && gameOver == 0)
		{
			Matrix_Output_state = Matrix_Output_catcher;
		}
		else
		{
			Matrix_Output_state = Start_Matrix_Output;
		}
		break;
		
		
		case Matrix_Output_catcher:
		Matrix_Output_state = Matrix_Output_beats;
		if(gameOver == 1)
		{
			CLEAR_ALL();
			Matrix_Output_state = Start_Matrix_Output;
		}
		break;
		
		case Matrix_Output_beats:
		Matrix_Output_state = Matrix_Output_catcher;
		break;
		
		default:
		Matrix_Output_state = Start_Matrix_Output;
		break;
	}

	switch (Matrix_Output_state) //State actions
	{
		case Matrix_Output_catcher:
		PORTA = 0b00000001;
		PORTB = x_catcher;
		
		
		if(y_val == 0b00000001 && ( (x_catcher & ~0x01) == (x_val & ~0x01) ||
		(x_catcher & ~0x02) == (x_val & ~0x02) ||
		(x_catcher & ~0x04) == (x_val & ~0x04) ||
		(x_catcher & ~0x08) == (x_val & ~0x08) ||
		(x_catcher & ~0x10) == (x_val & ~0x10) ||
		(x_catcher & ~0x20) == (x_val & ~0x20) ||
		(x_catcher & ~0x40) == (x_val & ~0x40) ||
		(x_catcher & ~0x80) == (x_val & ~0x80) ))
		{
			beat_catch = 1;
		}
		else if (y_val == 0b00000001)
		{
			beat_miss = 1;
		}	
		break;
		
		case Matrix_Output_beats:
		PORTA = y_val;
		PORTB = x_val;
		break;
		
		default:
		break;
	}
	
	return Matrix_Output_state;
}


enum Catch_states {Start_Catch} Catch_state;
int Catch_Output(int Catch_state)
{
	switch (Catch_state) //State transitions
	{
		case Start_Catch:
		if (beat_catch == 1)
		{
			combo = combo + 1;
			score = score + combo;
			beat_catch = 0;
		}
		else if (beat_miss == 1) {
			health--;
			combo = 0;
			beat_miss = 0;
			check_health();
		}
		Catch_state = Start_Catch;
		break;
		
		default:
		Catch_state = Start_Catch;
		break;
	}

	switch (Catch_state) //State actions
	{
		case Start_Catch:
		break;
		
		default:
		break;
	}
	return Catch_state;
}



int main(void)
{
	DDRA = 0xFF; PORTA = 0x00; //outputs
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0x00; PORTD = 0xFF; //inputs
	
		
	// Period for the tasks
	unsigned long int Beats_calc = 200;
	unsigned long int SM2_calc = 200;
	unsigned long int Player_calc = 50;
	unsigned long int Matrix_calc = 1;
	unsigned long int Catch_calc = 1;
	
	
	//Calculating GCD
	unsigned long tempGCD = 1;
	tempGCD = findGCD(Beats_calc, SM2_calc);
	tempGCD = findGCD(tempGCD, Player_calc);
	tempGCD = findGCD(tempGCD, Matrix_calc);
	tempGCD = findGCD(tempGCD, Catch_calc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tempGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int Beats_period = Beats_calc/GCD;
	unsigned long int SM2_period = SM2_calc/GCD;
	unsigned long int Player_period = Player_calc/GCD;
	unsigned long int Matrix_period = Matrix_calc/GCD;
	unsigned long int Catch_period = Catch_calc/GCD;
	
	//Declare an beats of tasks
	static task Beats_Task, SM2_Task, Player_Task, Matrix_Task, Catch_Task;
	task *tasks[] = { &Beats_Task, &SM2_Task, &Player_Task, &Matrix_Task, &Catch_Task };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	Beats_Task.state = Beats_start;//Task initial state.
	Beats_Task.period = Beats_period;//Task Period.
	Beats_Task.elapsedTime = Beats_period;//Task current elapsed time.
	Beats_Task.TickFct = &Beats_Tick;//Function pointer for the tick.

	// Task 2
	SM2_Task.state = sm2_display;//Task initial state.
	SM2_Task.period = SM2_period;//Task Period.
	SM2_Task.elapsedTime = SM2_period;//Task current elapsed time.
	SM2_Task.TickFct = &SM2_Tick;//Function pointer for the tick.

	// Task 3 //Player control
	Player_Task.state = Player_start;//Task initial state.
	Player_Task.period = Player_period;//Task Period.
	Player_Task.elapsedTime = Player_period;//Task current elapsed time.
	Player_Task.TickFct = &Player_Tick;//Function pointer for the tick.

	// Task 4 //matrix display
	Matrix_Task.state = Start_Matrix_Output;//Task initial state.
	Matrix_Task.period = Matrix_period;//Task Period.
	Matrix_Task.elapsedTime = Matrix_period;//Task current elapsed time.
	Matrix_Task.TickFct = &Matrix_Output;//Function pointer for the tick.

	// Task 5 //Catch states
	Catch_Task.state = Start_Catch;//Task initial state.
	Catch_Task.period = Catch_period;//Task Period.
	Catch_Task.elapsedTime = Catch_period;//Task current elapsed time.
	Catch_Task.TickFct = &Catch_Output;//Function pointer for the tick.



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

