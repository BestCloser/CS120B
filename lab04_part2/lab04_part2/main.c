/*
 * lab3p2.c
 *
 * Created: 8/1/2018 1:28:50 PM
 * Author : ucrcse
 */ 

#include <avr/io.h> 

enum c_val{init, wait0, dec_wait, decC, incC, inc_wait,clr_c}operateC;
	
	void changeC(){
	  unsigned char pa0 = PINA & 0x01;
	  unsigned char pa1 = PINA & 0x02;
 		 switch(operateC){
			case init:
				PORTC = 7;
				operateC = wait0;
			break;
			
			case wait0:
				if (~PINA == 3)   //if both on
				{
					operateC = clr_c;
				}
				else if(~PINA == 1) //inc_state
				{
					operateC = incC;
				}
				else if (~PINA == 2) //dec_state
				{
					operateC = decC;
				}
				else{
					//do nothing
				}
			break;
			
			case dec_wait:
				if (~PINA == 0)
				{
					operateC = wait0;
				}
				else if (~PINA == 3)
				{
					operateC = clr_c;
				}
				else{
					//whatever
				}
			break;
			
			case decC:
				if (PORTC > 0)
				{
					PORTC--;
				}
				operateC = dec_wait;
			break;
				
			case incC:
				if (PORTC < 9)
				{
					PORTC++;
				}
				operateC = dec_wait;
			break;
			
			case inc_wait:
				if (~PINA == 0)
				{
					operateC = wait0;
				}
				else if (~PINA == 3)
				{
					operateC = clr_c;
				}
				else{
					//whatever
				}
			break;
			
			case clr_c:
			PORTC = 0;
			if (~PINA != 3)
			{
				operateC = wait0;
			}
			
			break;
			
			default:
			operateC = wait0;
			break;
			}
		return;
		};
int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; 
	DDRC = 0xFF; PORTC = 0x00; 
    while (1) 
    {
		changeC();
    }
}