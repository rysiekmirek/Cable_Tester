
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <stdint.h>
#include <avr/io.h>
#include "util.h"
#include <util/delay.h>
#include "port_definitions.h"
#include "LCD.h"
#include "dec.h"
#include "globals.h"
     

void ADC_init (void)
{
	DDRF = 0x00; 			// ustawienie portu F jako wejscie
	PORTF= 0x00;

	CLRBIT(ADMUX,MUX4);
	CLRBIT(ADMUX,MUX3);

	CLRBIT(ADMUX,REFS1);	//ustawienie napiecia referencyjnego na Vcc
	SETBIT(ADMUX,REFS0);

	SETBIT(ADMUX,ADLAR); 	// wyrównanie wyniku konwersji do lewej - wystarczy jako wynik zczytaæ 8. bitowy ADCH

	SETBIT(ADCSRA,ADPS2);	//czestotliwoœæ próbkowania ustawiona na F_CPU/16 = 252kHz
	CLRBIT(ADCSRA,ADPS0);
	CLRBIT(ADCSRA,ADPS1);
	SETBIT(ADCSRA,ADIE); 	//w³aczone przerwanie po skonczeniu konwersji ADC interrupt

	CLRBIT(ADMUX,MUX0);
	CLRBIT(ADMUX,MUX1);		//wejscie ADC4 pinu overload
	SETBIT(ADMUX,MUX2);
	_delay_us(500);
	SETBIT(ADCSRA,ADEN); 	// w³¹czenie ADC
	SETBIT(ADCSRA,ADSC); 	// start kowersji
	_delay_ms(1);
	CLRBIT(ADCSRA,ADEN); 	// wy³¹czenie ADC - w zmiennej value znajduje siê wynik odczytu pinu overload
	
	if (value > 50 )
	{
		while (1)
		{
		LCD_Goto(0,0);
		LCD_WriteText("NAPIECIE NA WTYKU!");
		LCD_Goto(0,1);
		LCD_WriteText("ODLACZ TESTOWANE");
		LCD_Goto(0,2);
		LCD_WriteText("PRZEWODY I SPRAWDZ");
		LCD_Goto(0,3);
		LCD_WriteText("ICH STAN -> RESET");
		}
	}

}


/********************************************************************************/

void wire_test_init(void)
{

DDRA = 0xFF;
DDRC = 0xFF;
PORTA = 0x00;
PORTC = 0x00;

SETBIT(PORTC,RSTA);//resetowanie macierzy
SETBIT(PORTC,RSTB);
SETBIT(PORTC,RSTC);

_delay_us(10);

CLRBIT(PORTC,RSTA);//resetowanie macierzy
CLRBIT(PORTC,RSTB);
CLRBIT(PORTC,RSTC);
}


/*************************************************************************************************/

uint8_t get_result( uint8_t wire_number, char port )
{
CLRBIT(ADMUX,MUX2);

		if ((port == 'A') && (wire_number<16)) 
		{
		CLRBIT(ADMUX,MUX0);//wybor wejscia ADC0
		CLRBIT(ADMUX,MUX1);
		}
	else if ((port == 'A') && (wire_number>15))
		{
							
		CLRBIT(ADMUX,MUX1); //wejscie ADC1
		SETBIT(ADMUX,MUX0);
		}
	else if ((port == 'B') && (wire_number < 8 ))
		{
		CLRBIT(ADMUX,MUX1); // wejscie ADC1
		SETBIT(ADMUX,MUX0);
		}
	else if ((port == 'B') && (wire_number > 7))
		{
		CLRBIT(ADMUX,MUX0); // wejscie ADC2
		SETBIT(ADMUX,MUX1);
		}
	_delay_us(500);
	SETBIT(ADCSRA,ADEN); 			// w³¹czenie ADC
	SETBIT(ADCSRA,ADSC); 			// start kowersji, ktora konczy sie generacj¹ przerwania ADC_vect
	_delay_us(500);
	CLRBIT(ADCSRA,ADEN); 			// wy³¹czenie ADC

return value;	
}


ISR(ADC_vect)                       	// przerwanie z przetwornika ADC
{
  value = ADCH ;                    		// czytaj wartoœæ z przetwornika ADC
}


/*********************************************************************************************/

void adress_do (uint8_t wire_number,char port, char s_r_pin,uint8_t akcja) 
{
/*np adress_do(5,A,s,1) numeracja przewodów od 0 do 23 czyli ³¹cznie 24 przewody (jeden nie testowany - PIN23)

akcja : 0-roz³¹cza crosspoint
        1-³¹czy ¿¹dany crosspoint */
uint8_t piny[16] = {0,1,2,3,4,5,8,9,10,11,12,13,6,7,14,15}; //w macierzy kolejne adresy to nie kolejne piny

if ((port == 'A') && (wire_number < 16) && (wire_number >= 0)) /*dla macierzy 1*/
{

wire_number = piny[wire_number];

	if ((wire_number % 2) == 1) SETBIT(PORTA,AX0);
		else CLRBIT(PORTA,AX0);

	if ( ( (wire_number % 4) == 2 ) || ( (wire_number % 4) == 3 ) ) SETBIT(PORTA,AX1);
		else CLRBIT(PORTA,AX1);

	if ((wire_number > 3 && wire_number < 8) || (wire_number > 11 && wire_number < 16)) SETBIT(PORTA,AX2);
		else CLRBIT(PORTA,AX2);

	if (wire_number > 7 && wire_number < 16) SETBIT(PORTA,AX3);
		else CLRBIT(PORTA,AX3);

	if (s_r_pin == 's')	CLRBIT(PORTA,AY0); /*³¹czy  podany przewód z napieciem testujacym*/
	else SETBIT(PORTA,AY0); /*³¹czy  podany przewód z odpowiednim wejsciem ADC*/

	if (akcja == 0) CLRBIT(PORTA,DATA);			//ustawaia DATA na 1
	else SETBIT(PORTA,DATA);					//zeruje DATA
	
	SETBIT(PORTC,CSA); /*zatrzaœniêcie ustawionego wy¿ej adresu na odpowiedniej macierzy*/
	_delay_us(10);		/*opóŸnienie potrzebne na ustalenie siê adresów, Chip Select i STROBE*/
	SETBIT(PORTA,STBA);
	_delay_us(10);
	CLRBIT(PORTA,STBA);
	_delay_us(10);
	CLRBIT(PORTC,CSA);
}

	else if ((port == 'A') && (wire_number > 15) && (wire_number < 24)) /* dla pierwszej po³owy macierzy 2 */

{
	wire_number -= 16 ;
wire_number = piny[wire_number];

	if ((wire_number % 2) == 1) SETBIT(PORTA,AX0);
		else CLRBIT(PORTA,AX0);

	if ( ( (wire_number % 4) == 2 ) || ( (wire_number % 4) == 3 ) ) SETBIT(PORTA,AX1);
		else CLRBIT(PORTA,AX1);

	if ((wire_number > 3 && wire_number < 8) || (wire_number > 11 && wire_number < 16)) SETBIT(PORTA,AX2);
		else CLRBIT(PORTA,AX2);

	if (wire_number > 7 && wire_number < 16) SETBIT(PORTA,AX3);
		else CLRBIT(PORTA,AX3);

	if (s_r_pin == 's')	CLRBIT(PORTA,AY0); 
	else SETBIT(PORTA,AY0); 

	if (akcja == 0) CLRBIT(PORTA,DATA);
	else SETBIT(PORTA,DATA);
	

	
	SETBIT(PORTC,CSB);
	_delay_us(10);
	SETBIT(PORTA,STBB);
	_delay_us(10);
	CLRBIT(PORTA,STBB);
	_delay_us(10);
	CLRBIT(PORTC,CSB);

}


   else if ((port == 'B') && (wire_number >= 0) && (wire_number < 8)) /* dla drugiej po³owy macierzy 2 */

{
	wire_number += 8 ;
wire_number = piny[wire_number];

	if ((wire_number % 2) == 1) SETBIT(PORTA,AX0);
		else CLRBIT(PORTA,AX0);

	if ( ( (wire_number % 4) == 2 ) || ( (wire_number % 4) == 3 ) ) SETBIT(PORTA,AX1);
		else CLRBIT(PORTA,AX1);

	if ((wire_number > 3 && wire_number < 8) || (wire_number > 11 && wire_number < 16)) SETBIT(PORTA,AX2);
		else CLRBIT(PORTA,AX2);

	if (wire_number > 7 && wire_number < 16) SETBIT(PORTA,AX3);
		else CLRBIT(PORTA,AX3);

	if (s_r_pin == 's')	CLRBIT(PORTA,AY0); 
	else SETBIT(PORTA,AY0); 

	if (akcja == 0) CLRBIT(PORTA,DATA);
	else SETBIT(PORTA,DATA);
	

	SETBIT(PORTC,CSB);
	_delay_us(10);
	SETBIT(PORTA,STBB);
	_delay_us(10);
	CLRBIT(PORTA,STBB);
	_delay_us(10);
	CLRBIT(PORTC,CSB);

}

else if ((port == 'B') && (wire_number > 7 ) && (wire_number < 24)) /* dla macierzy  3 */

{
	wire_number -= 8 ;
wire_number = piny[wire_number];

	if ((wire_number % 2) == 1) SETBIT(PORTA,AX0);
		else CLRBIT(PORTA,AX0);

	if ( ( (wire_number % 4) == 2 ) || ( (wire_number % 4) == 3 ) ) SETBIT(PORTA,AX1);
		else CLRBIT(PORTA,AX1);

	if ((wire_number > 3 && wire_number < 8) || (wire_number > 11 && wire_number < 16)) SETBIT(PORTA,AX2);
		else CLRBIT(PORTA,AX2);

	if (wire_number > 7 && wire_number < 16) SETBIT(PORTA,AX3);
		else CLRBIT(PORTA,AX3);

	if (s_r_pin == 's')	CLRBIT(PORTA,AY0); 
	else SETBIT(PORTA,AY0); 

	if (akcja == 0) CLRBIT(PORTA,DATA);
	else SETBIT(PORTA,DATA);
	

	SETBIT(PORTC,CSC);
	_delay_us(10);
	SETBIT(PORTC,STBC);
	_delay_us(10);
	CLRBIT(PORTC,STBC);
	_delay_us(10);
	CLRBIT(PORTC,CSC);

	}
}

/******************************************************************************************/

uint8_t testing ( char mode, uint8_t *cable)


{
/*
A- test wszystkich kabli - tryb bez zapêtlarki
E - test wszystkich kabli - tryb z uzyciem zapêtlarki
*/
stan =0;
uint8_t i,j= 0;
//uint8_t f=0;
//port_state_B[24][24];	/* stan portów 0-rozwarty,1-zwarty */
//port_state_A[24][24];	/*wyniki w macierzy port state : napiêcie testujace na wierszu a zczytane w kolumnach*/

for(i=0;i<24;i++)
{
port_state[i][24]=0;
}

if (mode == 'B')

{
	for (i=0;i<24;i++)
	
	{
	LCD_BarGraphInit();
	LCD_BarGraph(1, (i+2)*4);
	SETBIT(DDRB,LEDA);CPLBIT(PORTB,LEDA);

	adress_do(i,'A','s',1); /*podanie napiêcia testujacego na przewód "i" w porcie A*/
	_delay_us(100); 

			for (j=0;j<24;j++) //zczytanie wartosci portu B i zapis jego stanu do port_state
			{
			adress_do(j,'B','r',1);
			_delay_us(10);
			if (get_result(j,'B')>220)
			port_state[i][j] = 1;
			else port_state[i][j] = 0;
			adress_do(j,'B','r',0);
			}

	adress_do(i,'A','s',0); //od³¹czenie napiêcia testuj¹cego od pinu i 
	_delay_us(100);
	}
		SETBIT(DDRB,LEDA);CLRBIT(PORTB,LEDA);
		
		for(j=0;j<24;j++)
		{
			if (port_state[j][cable[j]] == 1 ) port_state[j][cable[j]] =5;
			else port_state[j][cable[j]] = 7;
		//	if (cable[j] == 24 && port_state[j][cable[j]] == 7) port_state[j][cable[j]] = 0;
		}
	for(i=0;i<24;i++) //druga pêtla sprawdza który pin jest zwarty z ktorym
	{		

		for(j=0;j<24;j++)
		{
			if (port_state[i][j]==1 ) port_state[i][j] = 6;
			
			if (port_state[i][j] < 5 || port_state[i][j] >7) port_state[i][j] = 0;
		}
		for(j=0;j<24;j++)
		{
		if(port_state[i][j] == 5 || port_state[i][j] ==0) flag[i]=1;
			else
				{
				flag[i]=0;
				break;
				}
		}
	
					
}

		for (j=0;j<24;j++)
		{
		if (flag[j] == 1)
			stan=1;
		else if (flag[j] ==0)
			{
			stan =0;
			break;
			}
		}
}





else if (mode == 'E')

	{

	for (i= 0;i<24;i++)		
	
	{
	LCD_BarGraphInit();
	LCD_BarGraph(1,(i+2)*4);
	SETBIT(DDRB,LEDA);CPLBIT(PORTB,LEDA);
		
	adress_do(i,'A','s',1); //podanie napiêcia testujacego na przewód "i" w porcie A
	_delay_us(100);
		
		for (j=0;j<24;j++)  //zczytanie wartoœci napiecia na wszystkich pinach po podaniu napiêcia na przewód i
		{
			adress_do(j,'A','r',1);
		_delay_us(10);
		if (get_result(j,'A')>254)
			port_state[i][j] = 1;
		else if (get_result(j,'A')>230)		// zwarcie 2. rodzaju napiêcie na pinie = ok. 3,6 V po 2. diodach
			port_state[i][j] = 2;
		else if (get_result(j,'A')>215)		// zwarcie 3. rodzaju napiêcie na pinie = ok. 2,9 V po 3. diodach
			port_state[i][j] = 3;
		else if (get_result(j,'A')>200)		// zwarcie 4. rodzaju napiêcie na pinie = ok. 2,2 V po 4. diodach
			port_state[i][j] = 4;
		else port_state[i][j] = 0;			// traktowane jako rozwarcie napiêcie na pinie mniejsze ni¿ 2,2 V 
		 		
			adress_do(j,'A','r',0); 		// od³aczenie napiêcia testuj¹cego z pinu i	
	_delay_us(100);
		}
	adress_do(i,'A','s',0);
	}
	SETBIT(DDRB,LEDA);CLRBIT(PORTB,LEDA);	


		/*5 - przewod dziala
		  6 - przewod zwarty z tym pinem
		  7 - rozwarcie tego pinu */
		
		
		for(j=0;j<24;j++) //pierwsza pêtla sprawdza czy dany pin podaje napiêcie na zapetlarke czyli czy nie jest czasem przerwany/rozwarty
		{
		port_state[j][j]=0;

		if ( (port_state[j][(cable[j]+1)%24]>0) || (port_state[j][(cable[j]+2)%24]>0) || (port_state[j][(cable[j]+3)%24]>0) || (port_state[j][(cable[j]+4)%24]>0))
			port_state[j][cable[j]]=5;
		else port_state[j][cable[j]]=7;
		if (port_state[j][cable[j]] == 7 && cable[j] == 24) port_state[j][cable[j]] = 0;

		}

		for(i=0;i<24;i++) //druga pêtla sprawdza który pin jest zwarty z ktorym
		{		

			for(j=0;j<24;j++)
				{
					if (port_state[i][j]==1 ) port_state[i][j] = 6;
					
					if (port_state[i][j] < 5 || port_state[i][j] >7) port_state[i][j] = 0;

				}

		for(j=0;j<24;j++)
		{
		if(port_state[i][j] == 5 || port_state[i][j] ==0) flag[i]=1;
			else
				{
				flag[i]=0;
				break;
				}
		}
	
	}

		for (j=0;j<24;j++)
			{
			if (flag[j] == 1)
				stan=1;
			else if (flag[j] ==0)
				{
				stan =0;
				break;
				}
			}
	
	}
return 0;
}

/******************************************************************************************/

void LCD_port_state(char port_name)
{
uint8_t j = 0;
uint8_t n =9;
uint8_t o =11;
uint8_t i;		
if (port_name == 'B')
{
	LCD_WriteByte(0x01,0);   // clear display
	
	for (i=0;i<24;i++)
		{
		if(flag[i]==0)
		{
			LCD_Goto(0,0);
			LCD_WriteText("PIN:");
				if(i<9)
				{
				LCD_Goto(6,0);
				LCD_WriteText("A");
				}
				else
				{
				LCD_Goto(7,0);
				LCD_WriteText("A");
				}
		LCD_Goto(5,0);

		if(i<22) LCDdec(i+1);
		else LCDdec(i+2);
		
			for (j=0;j<24;j++)
			{
				if (port_state[i][j] == 6)
				{
			
				LCD_Goto(0,1);
				LCD_WriteText("ZWARTY Z:");
				LCD_Goto(n,1); 
					if(j<22) LCDdec(j+1);
					else LCDdec(j+2);
					if (j<10)
					{
					LCD_Goto(n+1,1); 
					LCD_WriteText("B");
					n+=2;
					}
					else
					{
					LCD_Goto(n+2,1); 
					LCD_WriteText("B");
					n+=3;
					}
				}
				else if (port_state[i][j] == 7)
				{
				LCD_ClrLine(2);
				LCD_Goto(0,2);
				LCD_WriteText("ROZWARTY Z:");
					LCD_Goto(o,2); 
					if(j<22) LCDdec(j+1);
					else LCDdec(j+2);
				LCD_Goto(o+2,2); 
				LCD_WriteText("B");
				if(j<10) o+=2;
				else o+=3; 
				}
			}
			
			n=9;
			o=11;
			LCD_Goto(0,3); 
			LCD_WriteText("<<4              6>>");
			key = -1;
			while (key !=4 && key !=6 && key !=100 ){LCD_Goto(0,0);};
			if (key ==4) 
				{
					if(i==1)i=-1;
					else 
					{
					while (flag[i-1] !=0) {i--;}
					i-=2;
					}
				}
			else if (key ==6)LCD_Goto(0,0);
			else if (key == 100) i=24;
			LCD_WriteByte(0x01,0);   // clear display
		}
	}
}
	

else if (port_name == 'E')
{
	LCD_WriteByte(0x01,0);   // clear display

		
	for (i=0;i<24;i++)
		{
		if(flag[i]==0)
		{
			LCD_Goto(0,0);
			LCD_WriteText("PIN:");
				if(i<9)
				{
				LCD_Goto(6,0);
				LCD_WriteText("A");
				}
				else
				{
				LCD_Goto(7,0);
				LCD_WriteText("A");
				}
		LCD_Goto(5,0);

		if(i<22) LCDdec(i+1);
		else LCDdec(i+2);
		
			for (j=0;j<24;j++)
			{
				if (port_state[i][j] == 6)
				{
			
				LCD_Goto(0,1);
				LCD_WriteText("ZWARTY Z:");
				LCD_Goto(n,1); 
					if(j<22) LCDdec(j+1);
					else LCDdec(j+2);
					if (j<10)
					{
					LCD_Goto(n+1,1); 
					LCD_WriteText("E");
					n+=2;
					}
					else
					{
					LCD_Goto(n+2,1); 
					LCD_WriteText("E");
					n+=3;
					}
				}
				else if (port_state[i][j] == 7)
				{
				LCD_ClrLine(2);
				LCD_Goto(0,2);
				LCD_WriteText("ROZWARTY Z:");
					LCD_Goto(o,2); 
					if(j<22) LCDdec(j+1);
					else LCDdec(j+2);
				LCD_Goto(o+2,2); 
				LCD_WriteText("E");
				if(j<10) o+=2;
				else o+=3; 
				}
			}
			
			n=9;
			o=11;
			LCD_Goto(0,3); 
			LCD_WriteText("<<4              6>>");
			key = -1;
			while (key !=4 && key !=6 && key !=100 ){LCD_Goto(0,0);};
			if (key ==4) 
				{
					if(i==1)i=-1;
					else 
					{
					while (flag[i-1] !=0) {i--;}
					i-=2;
					}
				}
			else if (key ==6)LCD_Goto(0,0);
			else if (key == 100) i=24;
			LCD_WriteByte(0x01,0);   // clear display

		}
	}	
  }
}




