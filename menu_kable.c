	
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
		_delay_ms(10);
		keypad_init();
		USART_init();

		uint8_t i=1;
		uint8_t d=1;
		uint8_t ef=1;
		char z = 'B';
		key = -1;

		uint8_t levels[10]= {0,10,20,30,50,90,130,170,210,254};


		uint8_t wzor1[24] = {0,1,2,3,24,5,24,7,8,9,10,11,12,13,24,15,16,17,18,19,20,21,22,23};
		uint8_t wzor2[24] = {0,1,2,3,24,5,24,7,8,9,10,11,12,13,24,15,16,17,18,19,20,21,22,23};
		uint8_t wzor3[24] = {0,1,2,3,24,5,24,7,8,9,10,11,12,13,24,15,16,17,18,19,20,21,22,23};
		uint8_t wzor4[24] = {0,1,2,3,24,5,24,7,8,9,10,11,12,13,24,15,16,17,18,19,20,24,22,23};
		uint8_t wzor5[24] = {7,11,6,14,0,12,1,24,14,13,24,2,10,24,5,24,24,24,24,24,24,24,24,24};
		uint8_t wzor6[24] = {24,1,2,3,24,5,24,7,8,9,10,11,24,13,24,15,16,24,18,19,20,21,22,24};


		/********tutaj nalezy wstawiæ wzor po³¹czeñ w przewodzie tak jak powy¿ej
		nalezy takze uzupe³niæ wpis we wskazanym nizej miejscu!!!!!!********/
		// mala instrukcja
		/*wpis np. wzor5[24]= {0,.....} oznacza ze PIN pierwszy pierwszego  wtyku ma byæ zwarty z PINem pierwszym na drugim wtyku*/
		/*analogicznie np. wzor4[24]= {x,x,5,.....} oznacza ze PIN trzeci pierwszego  wtyku ma byæ zwarty z PINem szostym na drugim wtyku*/
		/*wpis np. wzor5[24]= {x,x,24,.....} oznacza ze PIN trzeci pierwszego  wtyku nie jest zwarty z ¿adnym PINem drugiego wtyku*/
		
		uint8_t *p =0;

	 //koniec inicjalizacji zapalenie leda


   // miernik baterii
  TCCR1A = 0x00; //normal mode
  TCCR1B = 0b00000011; // preskaler na 64 - sprawdza stan baterii co 1 sekundê

  SETBIT (TIMSK,TOIE1); // aktywne przerwanie overflow 


 	sei();     // przerwania



while (1) 
{
start: 
LCD_WriteByte(0x01,0);   // clear display
ADC_init ();

					i=1;
					d=1;
					key = -1;

					LCD_Goto(2,0);
					LCD_WriteText("TEST PRZEWODU");					
					LCD_Goto(2,1);
					LCD_WriteText("USTAWIENIA/INFO.");
					LCD_Goto(3,3);
					LCD_WriteText("((RADIONIKA))");

						while (key != 5 && key !=6) // petla licznika w menu glownym
						{
							switch (key)
							{
							case 2:
							i-=1;
							key =-1;
							break;
							case 8:
							i+=1;
							key=-1;
							break;
							}
						switch(i)	//wyœwietlanie glownego menu
						{
						case 1:	
						LCD_Goto(0,3);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);
						LCD_Goto(0,1);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);			
						LCD_Goto(0,0);
						LCD_WriteText(">>");
						break;
						case 2:
						LCD_Goto(0,0);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);
						LCD_Goto(0,2);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);
						LCD_Goto(0,1);
						LCD_WriteText(">>");
						break;
						}
						if (i==3) i=1; 		//zapewnienie menu kolowego
						else if (i==0) i=2;
						
						}

	switch (i)
		{
		kable:

			case 1: //tutaj zostaje wybrana pozycja pierwsza w menu glownym
			{
				i=d;
				key=-1;
				LCD_WriteByte(0x01,0);//clr display
				
				LCD_Goto(0,3);
				LCD_WriteText("<<4     5 OK     6>>");
			
			
				while (key != 5)
						{
							switch (key) // petla licznika w menu test_przewodu
							{
								case 4:
								i-=1;
								key =-1;
								break;

								case 6:
								i+=1;
								key=-1;
								break;

								case 100: // wyjscie do glownego programu brzydkie ale proste
								key=-1;
								goto start;
								break;
							 }
						switch(i) /*******nalezy tutaj rownie¿ dodaæ kolejny "case" na wzór poni¿szych i zwiêkszyc limity w i*********/
						{
						case 1:	
						LCD_Goto(0,0);
						LCD_WriteText(" PORT A:    PORT B: ");
						LCD_Goto(0,1);
						LCD_WriteText("  DB25   -  DB25    ");
						LCD_Goto(0,2);
						LCD_WriteText("  MESKI     MESKI  ");
						z='B';
						p=wzor1;
						break;
						
						case 2:
						LCD_Goto(0,0);
						LCD_WriteText(" PORT A:   KONCOWKA:");
						LCD_Goto(0,1);
						LCD_WriteText("  DB25   -  DB25    ");
						LCD_Goto(0,2);
						LCD_WriteText("  MESKI     MESKI  ");
						z='E';
						p=wzor2;
						break;
												
						case 3:
						LCD_Goto(0,0);
						LCD_WriteText(" PORT A:    PORT B: ");
						LCD_Goto(0,1);
						LCD_WriteText("  DB25   -  DB25    ");
						LCD_Goto(0,2);
						LCD_WriteText("  MESKI     ZENSKI ");
						z='B';
						p=wzor3;
						break;

						case 4:
						LCD_Goto(0,0);
						LCD_WriteText(" PORT A:   KONCOWKA:");
						LCD_Goto(0,1);
						LCD_WriteText("  DB25   -  DB25    ");
						LCD_Goto(0,2);
						LCD_WriteText("  MESKI     ZENSKI  ");
						z='E';
						p=wzor4;
						break;
						
						case 5:
						LCD_Goto(0,0);
						LCD_WriteText(" PORT A:    PORT B: ");
						LCD_Goto(0,1);
						LCD_WriteText("  DSUB   -  DB15    ");
						LCD_Goto(0,2);
						LCD_WriteText("  ZENSKI    MESKI  ");
						z='B';
						p=wzor5;
						break;

						case 6:
						LCD_Goto(0,0);
						LCD_WriteText(" PORT A:    PORT B: ");
						LCD_Goto(0,1);
						LCD_WriteText("DB25_1634- DB25_1634");
						LCD_Goto(0,2);
						LCD_WriteText("  MESKI     MESKI ");
						z='B';
						p=wzor6;
						break;

						case 7:
						LCD_Goto(0,0);
						LCD_WriteText(" PORT A:   KONCOWKA:");
						LCD_Goto(0,1);
						LCD_WriteText("DB25_1634- DB25_1634");
						LCD_Goto(0,2);
						LCD_WriteText("  MESKI     MESKI ");
						z='E';
						p=wzor6;
						break;

						case 8:
						LCD_Goto(0,0);
						LCD_WriteText(" PORT A:    PORT B: ");
						LCD_Goto(0,1);
						LCD_WriteText("DB25_1634- DB25_1634");
						LCD_Goto(0,2);
						LCD_WriteText("  MESKI     ZENSKI  ");
						z='B';
						p=wzor6;
						break;
						
						case 9:
						LCD_Goto(0,0);
						LCD_WriteText(" PORT A:   KONCOWKA:");
						LCD_Goto(0,1);
						LCD_WriteText("DB25_1634- DB25_1634");
						LCD_Goto(0,2);
						LCD_WriteText("  MESKI     ZENSKI ");
						z='E';
						p=wzor6;
						break;

						}
						d=i;
						if (i==10) i=1;
						else if (i==0) i=9;
						}
			LCD_WriteByte(0x01,0);   // clear display
			LCD_Goto(5,0);
			LCD_WriteText("TESTUJE...");
			wire_test_init();
			testing (z,p);
			LCD_Goto(0,3);
			LCD_WriteText("<<POKAZ     DRUKUJ>>");
			key = -1;
			while  (key!=4 && key!=6 && key!=100) {LCD_Goto(0,0);}
				switch (key)
				{

			case 4:
				{
			
				if(stan==1)
				{
					ef=5; //flaga informuje ze kabel dzia³a
					
				}
				else if (stan ==0)
				{
				LCD_WriteByte(0x01,0);   // clear display
				LCD_Goto(2,0);
				LCD_WriteText("KABEL NIE DZIALA");
				LCD_Goto(3,1);
				LCD_WriteText("BLEDY W PINACH: ");
				_delay_ms(1500);
				LCD_port_state(z);
				}
				LCD_WriteByte(0x01,0);   // clear display
				if(ef==5){
				LCD_Goto(3,1);
				LCD_WriteText("KABEL SPRAWNY"); //tutaj uzyta flaga ef
				ef=1;							//resetwanie flagi
				}			
				LCD_Goto(0,3); 
				LCD_WriteText("*-WYJDZ");
				LCD_Goto(12,3); 
				LCD_WriteText("#-TESTUJ");
				while (key !=100 && key !=101 ){LCD_Goto(0,0);};
					switch(key){
						case 100:break;
						case 101:{
							goto kable;
							break;
						}

					}
				
				break;
				}
			case 6:
				{
				
				if(stan==1)
				{
				USART_Transmit_char(0x0a);
				USART_Transmit_char(0x0d);
				USART_Transmit_line("KABEL SPRAWNY");
				}
				else if (stan ==0)
				{
				USART_Transmit_char(0x0a);
				USART_Transmit_char(0x0d);
				USART_Transmit_line("PRZEWOD NIE DZIALA");
				USART_port_state(z);
				}
				LCD_WriteByte(0x01,0);   // clear display
				LCD_Goto(4,1);
				LCD_WriteText("WYSLANO DANE");
				LCD_Goto(0,3); 
				LCD_WriteText("*-WYJDZ");
				LCD_Goto(12,3); 
				LCD_WriteText("#-TESTUJ");
				while (key !=100 && key !=101 ){LCD_Goto(0,0);};
					switch(key){
						case 100:break;
						case 101:{
						goto kable;
						break;
						}
				}
				break;
				}
			case 100: // wyjscie do g³ownego menu - brzydkie ale program prosty wiec nie namiesza
				{
				goto start;
				}
				break;
				}
			break;
			}
			
			case 2: //pozycja druga w menu glownym
			{
			i = 1;
			key=-1;
			LCD_WriteByte(0x01,0);   // clear display
					LCD_Goto(2,0);
					LCD_WriteText("JASNOSC");
					LCD_Goto(2,1);
					LCD_WriteText("KONTRAST");
					LCD_Goto(2,2);
					LCD_WriteText("TEST BATERII");
					LCD_Goto(2,3);
					LCD_WriteText("INFO/NAWIGACJA");

					while (key != 5 && key !=6)
						{
						
							switch (key) // petla licznika w menu ustawienia
							{
								case 2:
								i-=1;
								key =-1;
								break;

								case 8:
								i+=1;
								key=-1;
								break;

								case 100: // wyjscie do glownego programu brzydkie ale proste
								key=-1;
								goto start;
								break;

								case 4: // wyjscie do glownego programu brzydkie ale proste
								key=-1;
								goto start;
								break;
							}
						
					switch(i) 
						{
						case 1:	
						LCD_Goto(0,3);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);
						LCD_Goto(0,1);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);			
						LCD_Goto(0,0);
						LCD_WriteText(">>");
						break;
						case 2:
						LCD_Goto(0,0);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);
						LCD_Goto(0,2);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);
						LCD_Goto(0,1);
						LCD_WriteText(">>");
						break;
						case 3:
						LCD_Goto(0,1);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);
						LCD_Goto(0,3);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);
						LCD_Goto(0,2);
						LCD_WriteText(">>");
						break;
						case 4:
						LCD_Goto(0,2);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);
						LCD_Goto(0,0);
						LCD_WriteByte(0x20,1);
						LCD_WriteByte(0x20,1);
						LCD_Goto(0,3);
						LCD_WriteText(">>");
						break;
						}
						if (i==5) i=1;
						else if (i==0) i=4;
						}
				switch (i)
				{
				case 1:
				{
				i=4;
				key=-1;
				LCD_WriteByte(0x01,0);   // clear display
				LCD_Goto(1,1);
				LCD_WriteText("POZIOM JASNOSCI= ");
				LCD_Goto(0,3);
				LCD_WriteText("<<4     5 OK     6>>");
				while (key != 5)
				{
						
						LCD_Goto(0,0);

					switch (key)
					{
						case 4:
						i-=1;
						key =-1;
						break;
						
						case 6:
						i+=1;
						key=-1;
						break;
						
						case 100:
						key=-1;
						goto start;
						break;
					}
					if(i==255)i=0;
					else if (i==10) i=9;
					LCD_Goto(17,1);
					LCDdec(i);
					OCR0=levels[i];
				}
				break;
				}			
				case 2:
				{
				i=5;
				key=-1;
				LCD_WriteByte(0x01,0);   // clear display
				LCD_Goto(1,1);
				LCD_WriteText("POZIOM KONTRASTU=");
				LCD_Goto(0,3);
				LCD_WriteText("<<4     5 OK     6>>");
				
					while (key != 5)
					{
						
						LCD_Goto(0,0);

						switch (key)
						{
						case 4:
						i-=1;
						key =-1;
						break;
						
						case 6:
						i+=1;
						key=-1;
						break;
						
						case 100:
						key=-1;
						goto start;
						break;
						}
						if(i==255)i=0;
						else if (i==10) i=9;
						LCD_Goto(18,1);
					    LCDdec(i);
						OCR2=levels[i];
					}
				break;
				}	
				case 3:
				{
				LCD_WriteByte(0x01,0);   // clear display
				LCD_Goto(0,0);
				battery_test ();
				break;
				}
				case 4:
				{
				LCD_WriteByte(0x01,0); // clear display
				LCD_Goto(0,0);
				LCD_WriteText(" 2-GORA     8-DOL");
				LCD_Goto(0,1);
				LCD_WriteText("4-ZMNIEJSZ 6-ZWIEKSZ");
				LCD_Goto(0,2);
				LCD_WriteText("*-WYJSCIE     5-OK ");
				key=-1;
				while (key !=100 && key !=5 && key!=4){LCD_Goto(0,0);};
				break;
				}
				
					
			}
			break;
			}
		
		}
			
	}

			

						
	return 0;


}


ISR (TIMER1_OVF_vect) 
{
wynik = 0;


for(uint8_t z=0;z<3;z++){
SETBIT(ADMUX,MUX1);
SETBIT(ADMUX,MUX0);  // wejscie ADC3 pinu BAT 
CLRBIT(ADMUX,MUX2);
_delay_ms(1);
SETBIT(ADCSRA,ADEN); // w³¹czenie ADC
SETBIT(ADCSRA,ADSC); // start kowersji
_delay_ms(1);
CLRBIT(ADCSRA,ADEN); // wy³¹czenie ADC
_delay_ms(1);
value = ADCH ;
wynik = wynik +value;

}
wynik = wynik/3;

if (wynik < 127){ 
SETBIT(DDRB,LEDB);CPLBIT(PORTB,LEDB); // mruganie diody oznacza niski poziom baterii
}
else {
SETBIT(DDRB,LEDB);SETBIT(PORTB,LEDB);
}


}



