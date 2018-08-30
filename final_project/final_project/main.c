/*
 * final_project.c
 *
 * Created: 8/26/2018 4:41:38 PM
 * Author : Daniel
 */ 

/*
WILL EDIT THIS FILE WHENEVER I NEED TO TEST SOMETHING ON BREADBOARD BECAUSE
IDK HOW TO USE MULTIPLE C FILES, SO GOING TO JUST COPY-PASTE OVER INTO THIS FILE
CURRENT CODE: TEST.C
*/

//#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
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
unsigned char x_val = ~0x00;
unsigned char y_val = 0x00;
unsigned char x_catcher = ~0x00;
unsigned char bounds = 0;
//unsigned char startGame = 0;
unsigned char gameOver = 0;
unsigned char beat_catch = 0;
unsigned char beat_miss = 0;
unsigned char miss_checker = 0;
unsigned char health = 4;
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
	if (health == 0)
	{
		gameOver = 1;
	}
}

int checkCollision()
{
	if ((x_catcher & ~0x01) == (x_val & ~0x01) ||
	(x_catcher & ~0x02) == (x_val & ~0x02) ||
	(x_catcher & ~0x04) == (x_val & ~0x04) ||
	(x_catcher & ~0x08) == (x_val & ~0x08) ||
	(x_catcher & ~0x10) == (x_val & ~0x10) ||
	(x_catcher & ~0x20) == (x_val & ~0x20) ||
	(x_catcher & ~0x40) == (x_val & ~0x40) ||
	(x_catcher & ~0x80) == (x_val & ~0x80) )
	{
		return 1;
	}
	else {
		return 0;
	}
}

void CLEAR_PLAYER()
{
	x_catcher = ~0x00;
	bounds = 0;
//	x_catcher = ~(0b00011000);
//	bounds = 0x04;
}

void CLEAR_BEATS()
{
	i = 0;
	j = 0;
	x_val = ~0x00;
	y_val = 0x00;
}

void Reset_Catch()
{
	beat_catch = 0;
	combo = 0;
	score = 0;
}

void Reset_Miss()
{
	beat_miss = 0;
}

void CLEAR_MATRIX()
{
	PORTA =   0b00000000; //y
	PORTB = ~(0b00000000); //x
	PORTD = 0xFF;	
}

void Reset_gameOver()
{
	unsigned char D3 = ~PIND & 0x04;
	if (D3 == 0x04){
		health = 4;
		gameOver = 0;
	}
}


int startGame()
{
	unsigned char tmp = PIND;
	tmp = tmp & 0x03;
	if (tmp == 0x02 || tmp == 0x01)
	{
		return 1;
	}
	else {
		return 0;
	}
}

void updateLCD()
{
/*	if (gameOver == 0)
	{
		LCD_ClearScreen();
		LCD_DisplayString(1, combo + \0);
	}
	else if (gameOver == 1)
	{
		LCD_ClearScreen();
		LCD_DisplayString(1, "this is gay");
	}
	*/
	unsigned char tmpScore = score;
	unsigned char tmpCombo = combo;
	if(gameOver != 1)
	{
//		LCD_DisplayString(1, "Player 1");
		LCD_ClearScreen();
		LCD_Cursor(1);
		LCD_WriteData('S');
		LCD_Cursor(2);
		LCD_WriteData('C');
		LCD_Cursor(3);
		LCD_WriteData('O');
		LCD_Cursor(4);
		LCD_WriteData('R');
		LCD_Cursor(5);
		LCD_WriteData('E');
		LCD_Cursor(6);
		LCD_WriteData(':');
		//Current Score
		LCD_Cursor(7);
		LCD_WriteData( tmpScore / 100  + '0'); //hundreds place
		tmpScore = tmpScore - (tmpScore / 100) * 100;
		LCD_Cursor(8);
		LCD_WriteData(tmpScore / 10 + '0'); //tens place
		LCD_Cursor(9);
		LCD_WriteData(tmpScore % 10 + '0'); //ones place*/

		//combo
		LCD_Cursor(17);
		LCD_WriteData( tmpCombo / 100  + '0'); //hundreds place
		tmpCombo = tmpCombo - (tmpCombo / 100) * 100;
		LCD_Cursor(18);
		LCD_WriteData(tmpCombo / 10 + '0'); //tens place
		LCD_Cursor(19);
		LCD_WriteData(tmpCombo % 10 + '0'); //ones place*/
		LCD_Cursor(20);
		LCD_WriteData('x');
		}
		else {
			LCD_ClearScreen();
			LCD_DisplayString(1, "press reset to play again");
		}
}





enum Player_states { Player_init, Player_start, initialization, main_state , right, left, Release } Player_state;
int Player_Tick(int Player_state)
{
	unsigned char left_press = PIND & 0x02;
	unsigned char right_press = PIND & 0x01;
	
	switch (Player_state) //State transitions
	{
		case Player_init:
		Player_state = Player_start;
		break;
		
		case Player_start:
		if(startGame() == 1 && gameOver == 0)
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
		if (gameOver == 1) {
			CLEAR_PLAYER();
			Player_state = Player_init;
		}
		
		else if (left_press == 0x02)
		{
			Player_state = left;
		}
		else if(right_press == 0x01)
		{
			Player_state = right;
		}
		else {
			Player_state = main_state;
		}
		break;
		
		case Release:
		if(right_press == 0x01 && left_press == 0x00)
		{
			Player_state = right;
		}
		else if(left_press == 0x02 && right_press == 0x00)
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
		}
		break;
		
		case right:
		if (bounds < 0x07)
		{
			bounds++;
			x_catcher = ((x_catcher >> 1) ^ 0x80);
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
	
	switch (Beats_state) //State transitions
	{
		case Beats_start:
		if(startGame() == 1 && gameOver == 0)
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
			CLEAR_BEATS();
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



//SM2 IS SUPPOSED TO BE TESTED FOR SHIFT REGISTER, LITERALLY DOES NOTHING ATM //BASED OFF OF THE LED_MATRIX_DEMO CODE
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
//		x_player = 0x01;
	}
	else {
//		x_player = (x_player << 1) | 0x01;
	}
	break;
	default:   	        break;
}

//	
//	transmit_data(x_player);
    return state;    
};



enum Matrix_Output_states {Start_Matrix_Output, Matrix_Output_catcher,  Matrix_Output_beats, End_Game} Matrix_Output_state;
int Matrix_Output(int Matrix_Output_state)
{	
	switch (Matrix_Output_state) //State transitions
	{
		case Start_Matrix_Output:
		if (startGame() == 1 && gameOver == 0)
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
			CLEAR_MATRIX();
			Matrix_Output_state = End_Game;
		}
		break;
		
		case Matrix_Output_beats:
		Matrix_Output_state = Matrix_Output_catcher;
		break;
		
		case End_Game:
		if (gameOver == 1) {
			while (gameOver == 1) {
				Reset_gameOver();
			}
			Matrix_Output_state = End_Game;
		}
		else {
			Matrix_Output_state = Start_Matrix_Output;
		}
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
//		transmit_data(something); //idk how to do this yet
		
		if(y_val == 0b00000001 && checkCollision() == 1)
		{
			beat_catch = 1;
			
			
		}
		else if (y_val == 0b00000001 && checkCollision() == 0)
		{
			
			check_health();
			
			beat_miss = 1;
			
			PORTA = 0xFF;
			PORTB = 0x00;
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


enum Catch_states {Start_Catch, Record_Catch} Catch_state;
int Catch_Output(int Catch_state)
{
	switch (Catch_state) //State transitions
	{
		case Start_Catch:
		if (beat_catch == 1)
		{
			/*beat_catch = 0;
			combo = combo + 1;
			score = score + combo;
			updateLCD();*/
			Catch_state = Record_Catch;
		}
		else {
			Catch_state = Start_Catch;
		}
		break;
		
		case Record_Catch:
		if (gameOver != 1) {
			beat_catch = 0;
			combo = combo + 1;
			score = score + combo;
			updateLCD();
		}
		else if (gameOver == 1) {
			Reset_Catch();
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


enum Miss_states {Start_Miss, Record_Miss} Miss_state;
int Miss_Output(int Miss_state)
{
	switch (Miss_state) //State transitions
	{
		case Start_Miss:
		if (beat_miss == 1)
		{
			/*beat_miss = 0;
			health = health - 1;
			combo = 0;
			updateLCD();*/
			Miss_state = Record_Miss;
		}
		else
		{
			Miss_state = Start_Miss;
		}
		break;
		
		case Record_Miss:
		if (gameOver != 1){
			beat_miss = 0;
			health = health - 1;
			combo = 0;
			updateLCD();
		}
		else if (gameOver == 1) {
			Reset_Miss();
		}
		Miss_state = Start_Miss;
		break;
		
		default:
		Miss_state = Start_Miss;
		break;
	}

	switch (Miss_state) //State actions
	{
		case Start_Miss:
		break;
		
		case Record_Miss:
		miss_checker++;
		break;
		
		default:
		break;
	}
	return Miss_state;
}


/*
enum LCD_states {display_init, display_start, display_game, display_end } LCD_state;
int LCD_Display(int LCD_state)
{
	switch(LCD_state)
	{
		case display_init:
		LCD_state = display_start;
		break;
		
		case display_start:
		LCD_init();
		LCD_ClearScreen();
		LCD_DisplayString(1, "Press any button to start");
		if (startGame() == 1 && gameOver == 0)
		{
			LCD_state = display_game;
		}
		else
		{
				LCD_state = display_start;
		}
		break;
		
		case display_game:
//		LCD_DisplayString(1, combo + 'x' + "    Score: " + score  );
		LCD_ClearScreen();
		LCD_DisplayString(1, "Is this working");
		if (gameOver == 1) {
			LCD_ClearScreen();
			LCD_DisplayString(1, "Final Score: 0");
			LCD_state = display_end;
		}
		else {
			LCD_state = display_game;
		}
		break;
		
		case display_end:
		if (gameOver == 0)
		{
			LCD_state = display_start;
		}
		else {
			LCD_state = display_end;
		}
		break;
	}
	
	switch(LCD_state)
	{
		case display_init:
		break;
		
		case display_start:
		break;
		
		case display_game:
		break;
		
		case display_end:
		break;
		
	}
	
	return LCD_state;
}
*/
int main(void)
{
	DDRA = 0xFF; PORTA = 0x00; //outputs
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xC0; PORTD = 0x3F; //inputs
		
	// Period for the tasks
	unsigned long int Beats_calc = 200;
	unsigned long int SM2_calc = 200;
	unsigned long int Player_calc = 50;
	unsigned long int Matrix_calc = 1;
	unsigned long int Catch_calc = 100;
	unsigned long int Miss_calc = 100;
//	unsigned long int LCD_calc = 100;
	
	
	//Calculating GCD
	unsigned long tempGCD = 1;
	tempGCD = findGCD(Beats_calc, SM2_calc);
	tempGCD = findGCD(tempGCD, Player_calc);
	tempGCD = findGCD(tempGCD, Matrix_calc);
	tempGCD = findGCD(tempGCD, Catch_calc);
	tempGCD = findGCD(tempGCD, Miss_calc);
//	tempGCD = findGCD(tempGCD, LCD_calc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tempGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int Beats_period = Beats_calc/GCD;
	unsigned long int SM2_period = SM2_calc/GCD;
	unsigned long int Player_period = Player_calc/GCD;
	unsigned long int Matrix_period = Matrix_calc/GCD;
	unsigned long int Catch_period = Catch_calc/GCD;
	unsigned long int Miss_period = Miss_calc/GCD;
//	unsigned long int LCD_period = LCD_calc/GCD;
	
	//Declare an beats of tasks
	static task Beats_Task, SM2_Task, Player_Task, Matrix_Task, Catch_Task, Miss_Task;//, LCD_Task;
	task *tasks[] = { &Beats_Task, &SM2_Task, &Player_Task, &Matrix_Task, &Catch_Task, &Miss_Task};// &LCD_Task };
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

	// Task 6 //Miss states
	Miss_Task.state = Start_Miss;//Task initial state.
	Miss_Task.period = Miss_period;//Task Period.
	Miss_Task.elapsedTime = Miss_period;//Task current elapsed time.
	Miss_Task.TickFct = &Miss_Output;//Function pointer for the tick.

/*
	// Task 7 //LCD states
	LCD_Task.state = display_init;//Task initial state.
	LCD_Task.period = LCD_period;//Task Period.
	LCD_Task.elapsedTime = LCD_period;//Task current elapsed time.
	LCD_Task.TickFct = &LCD_Display;//Function pointer for the tick.
*/
	// Set the timer and turn it on

//	LCD_init();
//	LCD_ClearScreen();
//	LCD_Cursor(1);
//	LCD_WriteData(0 + '0');
//	LCD_DisplayString(1, "Hello World");



	LCD_init();
	LCD_ClearScreen();
	LCD_DisplayString(1, "Press any button to start");
	
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
