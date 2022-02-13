
#include <avr/io.h>
#include "util.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "dec.h"
#include "globals.h"
#include "util.h"
#include "port_definitions.h"


void keypad_init(void) //klawiatura czeka na nacisniecie i zainicjowanie przerwania INT7
{
again:
SETBIT(DDRB,KEY_COL1);CLRBIT(PORTB,KEY_COL1);//ustawienie kolumn jako wyjscia i na "0"
SETBIT(DDRB,KEY_COL2);CLRBIT(PORTB,KEY_COL2);
SETBIT(DDRE,KEY_COL3);CLRBIT(PORTE,KEY_COL3);

CLRBIT(DDRE,KEY_ROW1);SETBIT(PORTE,KEY_ROW1);//ustawienie rzedow jako wejscia i pull-up
CLRBIT(DDRE,KEY_ROW2);SETBIT(PORTE,KEY_ROW2);
CLRBIT(DDRE,KEY_ROW3);SETBIT(PORTE,KEY_ROW3);
CLRBIT(DDRE,KEY_ROW4);SETBIT(PORTE,KEY_ROW4);

CLRBIT(DDRE,KEY_INT);SETBIT(PORTE,KEY_INT);//ustawienie pinu INI7 jako wejscie i pull-up


EIMSK = 0b10000000;      //wlaczenie INT7
EICRB = 0b00000000;		 //przerwanie INT7 aktywowane stanem niskim

}

ISR(INT7_vect)
{

		for (int i=0;i<3;i++) 
		
		{
		
		if ((i%3) == 0)
		
		{
		
				CLRBIT(PORTB,KEY_COL1);
				SETBIT(PORTB,KEY_COL2);
				SETBIT(PORTE,KEY_COL3);
				_delay_us(100);
			if(GETBIT(PINE,KEY_ROW1) == 0)
				 {
				 key = 1;
				 i=3;
				 }
			else if(GETBIT(PINE,KEY_ROW2) == 0)
				 {
				 key = 4;
				 i=3;
				 }
			else if(GETBIT(PINE,KEY_ROW3) == 0)
				 {
				 key = 7;
				 i=3;
				 }
			else if(GETBIT(PINE,KEY_ROW4) == 0)
				 {
				 key = 100;  //odpowiada znakowi * z klawiatury
				 i=3;
				 }
			else {key = -1;} 
		
			

		}	
		
	
		
		else if ((i%3) == 1)
		
		{
		
			SETBIT(PORTB,KEY_COL1);
			CLRBIT(PORTB,KEY_COL2);
			SETBIT(PORTE,KEY_COL3);
			_delay_us(100);

			
			 if(GETBIT(PINE,KEY_ROW1) == 0)
				 {
				 key = 2;
				 i=3;
				 }
			else if(GETBIT(PINE,KEY_ROW2) == 0)
				 {
				 key = 5;
				 i=3;
				 }
			else if(GETBIT(PINE,KEY_ROW3) == 0)
				 {
				 key = 8;
				 i=3;
				 }
			else if(GETBIT(PINE,KEY_ROW4) == 0)
				 {
				 key = 0;
				 i=3;
				 }	
			else {key = -1;} 
		
		}
		
		

	else if ((i%3) == 2)
			
		{
			
				SETBIT(PORTB,KEY_COL1);
				SETBIT(PORTB,KEY_COL2);
				CLRBIT(PORTE,KEY_COL3);
				_delay_us(100);


			if(GETBIT(PINE,KEY_ROW1) == 0)
				 {
				 key = 3;
				 i=3;
				 }
			else if(GETBIT(PINE,KEY_ROW2) == 0)
				 {
				 key = 6;
				 i=3;
				 }
			else if(GETBIT(PINE,KEY_ROW3) == 0)
				 {
				 key = 9;
				 i=3;
				 }
			else if(GETBIT(PINE,KEY_ROW4) == 0)
				 {
				 key = 101;   /*int 11 odpowiada znakowi '#' z klawiatury*/
				 i=3;
				 }	
			else {key = -1;} 
			 
			
		}
	
	}

keypad_init();
_delay_ms(40);

}
