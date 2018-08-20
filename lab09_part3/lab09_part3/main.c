/*
 * lab09_part3.c
 * Name: Daniel Li
 * Partner: Siwon Kim
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter

		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		// prevents OCR0A from underflowing, using prescaler 64     // 31250 is largest frequency that will not result in underflow
		
		else if (frequency > 31250) { OCR3A = 0x0000; }

		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States{start, wait, on, off, realOff, wait2} state;

// twinkle twinkle little star, how I wonder what you are.
//G, G, D, D, E, E, D, C, C, B, B, A, A, G
double notes[14] = {392.00, 392.00, 587.33, 587.33, 659.25, 659.25, 587.33, 523.25, 523.25, 493.88, 493.88, 440.00, 440.00, 392.00};
unsigned short timeHeld[14] = {25, 25, 25, 25, 25, 25, 50, 25, 25, 25, 25, 25, 25, 50};
unsigned short timeBetween[14] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};


unsigned char input;
unsigned char n;	// index for notes, same one for timeHeld
unsigned char i;	// tick counter
unsigned char counter;	// counter2

void tick(){
	input = ~PINA & 0x01;
	
	
	switch(state){	// transitions
		case start:
		state = wait;
		break;
		
		case wait:
		if(input == 0x01){
			state = on;
		}
		else{
			state = wait;
		}
		break;
		
		case on:
		if(i <= timeHeld[n]){
			state = on;
		}
		else if(i > timeHeld[n]){
			counter++;
			if(counter > 13){
				state = realOff;
			}
			else{
				state = off;
				i = 0;
			}
		}
		break;
		
		case off:
		if(i <= timeBetween[counter]){
			state = off;
		}
		else if(i > timeBetween[counter]){
			state = on;
			n++;
			i = 0;
		}
		break;
		
		case realOff:
		state = wait2;
		break;
		
		case wait2:
		if(input == 0x01){
			state = wait2;
		}
		else{
			state = wait;
		}
		break;
		
		default:
		break;
	}
	
	switch(state){	//action
		case start:
		break;
		
		case wait:
		n = 0;
		i = 0;
		counter = 0;
		break;
		
		case on:
		set_PWM(notes[n]);	// increment the index every timeHeld
		i++;
		break;
		
		case off:
		set_PWM(0);
		i++;
		break;
		
		case realOff:
		set_PWM(0);
		break;
		
		case wait2:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	state = start;
	
	TimerSet(12);
	TimerOn();
	
	PWM_on();
	
	while (1)
	{
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}