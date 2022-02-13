	
	/*main program*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "util.h"
#include <util/delay.h>
#include "port_definitions.h"
#include "testing.h"
#include "LCD.h"
#include "dec.h"
#include "globals.h"



int main (void)

{
		LCD_Init();
		keypad_init();
		USART_init();
		LCD_WriteText("   WILLKOMMEN");
		_delay_ms(1000);
	
	while (1)
	{	
		
		LCD_WriteByte(0x01,0);
		LCD_WriteText("1TESTA");
		LCD_Goto(9,0);
		LCD_WriteText("2TESTE");
		LCD_Goto(0,1);
		LCD_WriteText("3BAT");
		LCD_Goto(9,1);
		LCD_WriteText("4SETLCD");
		sei();     /*przerwania*/
		key = -1;

		while  (key <1 || key > 4){LCD_Goto(0,0);};
		
		switch (key)
		{
			case 1:
			{
			LCD_ClrLine(0);
			LCD_ClrLine(1);
			LCD_Goto(0,0);
			LCD_WriteText("WybranoTest A-B");
			wire_test_init();
			testing ('A');
			_delay_ms(1);
			LCD_Goto(0,1);
			LCD_WriteText("1Show     2Print");
			key = -1;
			while  (key < 1 || key > 2) {LCD_Goto(0,0);}
				switch (key)
				{
				case 1:
				LCD_port_state('A');
				break;
				case 2:
				USART_port_state('A');
				break;
				}
			break;
			}
		
			case 2:
			{
			LCD_ClrLine(0);
			LCD_ClrLine(1);
			LCD_Goto(0,0);
			LCD_WriteText("WybranoTest A-E");
			wire_test_init();
			testing ('E');
			LCD_Goto(0,1);
			LCD_WriteText("1Show     2Print");
			key = -1;
			while  (key < 1 || key > 2){LCD_Goto(0,0);}
				switch (key)
				{
				case 1:
				LCD_port_state('E');
				break;
				case 2:
				USART_port_state('E');
				break;
				}
			break;
			}
			
			case 3:
			{
			LCD_ClrLine(0);
			LCD_ClrLine(1);
			LCD_Goto(0,0);
			LCD_WriteText("WybranoTest BAT");
			battery_test ();
			_delay_ms(100);
			break;
			}
			
			case 4:
			{
			int i = OCR0;
	//		int m = OCR2;
			LCD_ClrLine(0);
			LCD_ClrLine(1);
			LCD_Goto(0,0);
			LCD_WriteText("WybranoSetLCD");
			_delay_ms(500);
		//	LCD_Goto(0,1);
		//	LCD_WriteText("1Jasnosc");
			key = -1;
		/*	while  (key < 1 || key > 2) {LCD_Goto(0,0);};
		
			switch(key)
			{
				
				case 1:
				{	*/
					LCD_ClrLine(0);
					LCD_ClrLine(1);
					LCD_Goto(0,0);
					LCD_WriteText("RegJasnosci");
					LCD_Goto(0,1);
					LCD_WriteText("4--   5 OK   6++");
						
						while (key != 5)
						{
						LCD_Goto(0,0);
						if (i<7) i=7;
						if (i>229) i=230;
						
						switch (key)
						{
						case 4:
						i-=10;
						key =-1;
						break;
						case 6:
						i+=10;
						key=-1;
						break;
						}
						OCR0=i;
						}
			/*	break;
				}
				
				case 2:
				{	
					LCD_ClrLine(0);
					LCD_ClrLine(1);
					LCD_Goto(0,0);
					LCD_WriteText("RegKontrastu");
					LCD_Goto(0,1);
					LCD_WriteText("4--   5 OK   6++");
						
				while (key != 5)
						{
						LCD_Goto(0,0);
						if (m<3) m=3;
						if (m>229) m=230;
						
						switch (key)
						{
						case 4:
						m-=10;
						key =-1;
						break;
						case 6:
						m+=10;
						key=-1;
						break;
						}
						OCR2=m;
						}
					break;
					}*/
			}
			break;
			}
}
}

