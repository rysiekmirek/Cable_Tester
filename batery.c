
#include <avr/io.h>
#include <stdlib.h>
#include "dec.h"
#include "globals.h"
#include "util.h"
#include "LCD.h"
#include <util/delay.h>
#include "port_definitions.h"
#include "testing.h"

/*testowanie stanu baterii*/

void battery_test (void)
{
int y=0;
int wynik = 0;

for(y=0;y<5;y++)
{
ADC_init ();
key=-1;
SETBIT(ADMUX,MUX1);
SETBIT(ADMUX,MUX0);  /* wejscie ADC3 pinu BAT */
CLRBIT(ADMUX,MUX2);

_delay_ms(1);
SETBIT(ADCSRA,ADEN); // w³¹czenie ADC
SETBIT(ADCSRA,ADSC); // start kowersji
_delay_ms(1);
CLRBIT(ADCSRA,ADEN); // wy³¹czenie ADC - w zmiennej value znajduje siê wynik odczytu pinu overload
_delay_ms(2);
wynik = wynik + value;
}
wynik = ((wynik/5));
LCD_Goto(8,3);
//LCDdec(wynik);
if (wynik > 127){
LCD_Goto(0,2);
LCD_WriteText("      =WYSOKI=  ");
}
else if (wynik > 126){
LCD_Goto(0,2);
LCD_WriteText("      =SREDNI=  ");
}
else if (wynik <= 126){
LCD_Goto(0,2);
LCD_WriteText("      =NISKI=  ");
}

LCD_Goto(0,0);
LCD_WriteText(" POZIOM NALADOWANIA ");

LCD_Goto(0,1);
LCD_WriteText("     AKUMULATORA   ");


while(key!= 100 && key!=5 && key!=4){LCD_Goto(0,0);};
}


