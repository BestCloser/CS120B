/*
 * lab9_part2.c
 *
 */ 

#include <avr/io.h>
#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)
#define A2 (~PINA & 0x04)

unsigned char freq_choice = 0;
enum bool {false, true} music_on;
	
/*
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
*/

//siwon? what's the difference
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



enum switchstates {init_switch, off_switch, on_switch, wait_switchON, wait_switchOFF}switch_state;
void switch_tick(){
	switch(switch_state){
		case init_switch:
		set_PWM(0);
		switch_state = off_switch;
		break;
		
		case off_switch:
		music_on = false;
		switch_state = wait_switchOFF;
		
		break;
		
		case wait_switchOFF:
		if (A0)
		{
			switch_state = on_switch;
		}
		else{
			switch_state = wait_switchOFF;
		};
		break;
		
		case on_switch:
		music_on = true;
		switch_state = wait_switchON;
		break;
		
		case wait_switchON:
		if (A0)
		{
			switch_state = wait_switchON;
		}
		else{
			switch_state = off_switch;
		};
		break;
		default:
		set_PWM(0);
		break;
	}
};

void freq_option(){
	if (freq_choice == 0)
	{
		set_PWM(261.63*(1000/125));
	}
	else if (freq_choice == 1)
	{
		set_PWM(293.66*(1000/125));
	}
	else if (freq_choice == 2)
	{
		set_PWM(329.63*(1000/125));
	}
	else if (freq_choice == 3)
	{
		set_PWM(349.23*(1000/125));
	}
	else if (freq_choice == 4)
	{
		set_PWM(392.00*(1000/125));
	}
	else if (freq_choice == 5)
	{
		set_PWM(440.00*(1000/125));
	}
	else if (freq_choice == 6)
	{
		set_PWM(493.88*(1000/125));
	}
	else if (freq_choice == 7)
	{
		set_PWM(523.25*(1000/125));
	}
}

enum freq_states{init_freq, setCurr_freq, inc_freq, wait_incFreq, dec_freq, wait_decFreq} freq_state;
void setFreq_tick(){
	switch(freq_state){
		case init_freq:
		set_PWM(0);
		freq_state = setCurr_freq;
		break;
		
		case setCurr_freq:
		if (A2 && ~A1 && (music_on == true))
		{
			freq_state = inc_freq;
		}
		else if (~A2 && A1 && (music_on == true))
		{
			freq_state = dec_freq;
		}
		else{
			freq_state = setCurr_freq;
		}
		break;
		
		case inc_freq:
		if (freq_choice < 9){
			freq_choice++;
		}
		freq_option();
		freq_state = wait_incFreq;
		break;
		
		case wait_incFreq:
		if (A2)
		{
			freq_state = wait_incFreq;
		}
		else{
			freq_state = setCurr_freq;
		}
		break;
		
		case dec_freq:
		if (freq_choice > 0){
			freq_choice--;
		}
		freq_option();
		freq_state = wait_decFreq;
		break;
		
		case wait_decFreq:
		if (A1)
		{
			freq_state = wait_decFreq;
		}
		else{
			freq_state = setCurr_freq;
		}
		break;
	}
}


int main(void)
{
	PWM_on();
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	/* Replace with your application code */
	while (1)
	{
		switch_tick();
		setFreq_tick();
	}
}


