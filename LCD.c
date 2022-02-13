
#include<avr/io.h>
#include"util.h"
#include<util/delay.h>
#include<string.h>
#include <stdlib.h>
#include"LCD.h"
#include "dec.h"
#include "port_definitions.h"

/*
-------------------------------------------------------------------------------
LCD_WriteByte
~~~~~~~~~~~~~

This procedure sends one byte to the LCD unit

Parameters:

  int8_t LCDByte [i] - Byte to send
  int8_t IsData  [i] - This should be 0 if byte contains control instruction
                                   or 1 if byte contains data

-------------------------------------------------------------------------------
*/

void LCD_WriteByte(int8_t LCDByte, int8_t IsData)
{
   /* setting apropriate RS signal depending on whether we send instructions or data */
   if (IsData) SETBIT(LCD_PORT,LCD_RS);
   else CLRBIT(LCD_PORT,LCD_RS);
   /* writing more significant 4bit piece on LCD ports */
   if  (LCDByte & 0x80) SETBIT(LCD_PORT,LCD_DB7);
   else CLRBIT(LCD_PORT,LCD_DB7);
   if  (LCDByte & 0x40) SETBIT(LCD_PORT,LCD_DB6);
   else CLRBIT(LCD_PORT,LCD_DB6);
   if  (LCDByte & 0x20) SETBIT(LCD_PORT,LCD_DB5);
   else CLRBIT(LCD_PORT,LCD_DB5);
   if  (LCDByte & 0x10) SETBIT(LCD_PORT,LCD_DB4);
   else CLRBIT(LCD_PORT,LCD_DB4);
   /* data enable strobe */
   SETBIT(LCD_PORT,LCD_E);
   _delay_ms(1);
   CLRBIT(LCD_PORT,LCD_E);
   _delay_ms(1);
   /* writing less significant 4bit piece on LCD ports */
   if  (LCDByte & 0x08) SETBIT(LCD_PORT,LCD_DB7);
   else CLRBIT(LCD_PORT,LCD_DB7);
   if  (LCDByte & 0x04) SETBIT(LCD_PORT,LCD_DB6);
   else CLRBIT(LCD_PORT,LCD_DB6);
   if  (LCDByte & 0x02) SETBIT(LCD_PORT,LCD_DB5);
   else CLRBIT(LCD_PORT,LCD_DB5);
   if  (LCDByte & 0x01) SETBIT(LCD_PORT,LCD_DB4);
   else CLRBIT(LCD_PORT,LCD_DB4);
   /* data enable strobe */
   SETBIT(LCD_PORT,LCD_E);
   _delay_ms(1);
   CLRBIT(LCD_PORT,LCD_E);
   _delay_ms(1);

} /* LCD_WriteByte */
/*******************************************/
void PWM_init(void)
{
DDRB = (1<<PWM1) | (1<<PWM2);	/*oba piny pwm jako wyjscia*/
TCCR0 = 0b01100001;  			/* PWM z korekcja fazy kasuje przy up-counting i preskaler na 64 co daje 123,5 Hz*/
								/*wype³nienie ok 50 %  dla 255= 100% */							
TCCR2 = 0b01100001;  			/*to samo co wy¿ej tylko dla Timera 2 */

OCR0 =50;
OCR2 =90;

}
/***********************************************/
/*
-------------------------------------------------------------------------------
LCD_WriteText
~~~~~~~~~~~~~

This procedure sends a string of text to the LCD unit

Parameters:

  char text [i] - a null terminated string containing text to be displayed

-------------------------------------------------------------------------------
*/
void LCD_WriteText(char text[])
{
   int8_t i=0;
   while (text[i] != 0)
   {
     LCD_WriteByte(text[i],1);
     i++;

   }
} /* LCD_WriteText */


/*
-------------------------------------------------------------------------------
LCD_Goto
~~~~~~~~

This procedure sets the cursor in desired position

Parametry:

  int8_t column  [i] - column (first one = 0)
  int8_t line    [i] - line (first one = 0)

-------------------------------------------------------------------------------
*/
void LCD_Goto(int8_t column, int8_t line)
{
if (line < 2){
  LCD_WriteByte(((line * 64) + column) | 0x80,0);
}
else if (line == 2)
{
LCD_WriteByte((20 + column) | 0x80,0);
}
else if (line == 3)
{
LCD_WriteByte((84 + column) | 0x80,0);
}
}  /* LCD_Goto */


/*
-------------------------------------------------------------------------------
LCD_ClrLine
~~~~~~~~~~~

this procedure clears one line on the LCD unit

Parameters:

  int8_t line  [i] - line to be cleared (first one = 0)

-------------------------------------------------------------------------------
*/
void LCD_ClrLine(int8_t line)
{
   int8_t i;
   LCD_Goto(0,line);    // set cursor at the beginning of line
   for (i=0;i<20;i++)   // for entire line
    {
      LCD_WriteByte(0x20,1);   // send ' ' (space) character
    }
} /* LCD_ClrLine */

/*
-------------------------------------------------------------------------------
LCD_Init
~~~~~~~~

This procedure initializes the LCD unit

Parameters: none

At this moment this procedure sets the LCD unit in 4bit modus only

-------------------------------------------------------------------------------
*/
void LCD_Init(void)
{

   /* setting the proper direction of AVR I/O ports (see #define section
      in LCD.h header file) */
	  
   LCD_DDR |= (1 << LCD_DB7);
   LCD_DDR |= (1 << LCD_DB6);
   LCD_DDR |= (1 << LCD_DB5);
   LCD_DDR |= (1 << LCD_DB4);
   LCD_DDR |= (1 << LCD_RS);
   LCD_DDR |= (1 << LCD_E);

   /* instruction initializing the LCD display unit: */
   
   PWM_init();
   
   LCD_WriteByte(0x33,0);
   LCD_WriteByte(0x32,0);
   LCD_WriteByte(0x28,0);   // set interface length (4bit)
   LCD_WriteByte(0x0c,0);   // enable display
   LCD_WriteByte(0x01,0);   // clear display
   _delay_ms(10);
   LCD_WriteByte(0x06,0);   // set cursor move direction
   _delay_ms(10);
} /* LCDInit */



/*
-------------------------------------------------------------------------------
LCD_BarGraphInit
~~~~~~~~~~~~~~~~

This procedure initializes a horizontal bar graph. It defines a set of characters
in LCD's CGRAM at locations: 0x00..0x03

Parameters: none

-------------------------------------------------------------------------------
*/

void LCD_BarGraphInit(void)
{
  int8_t i;
  LCD_WriteByte(0x40,0);
  for (i=0;i<8;i++)         // 0x00
  {
    LCD_WriteByte(0x10,1);
  }
   for (i=0;i<8;i++)        // 0x01
  {
    LCD_WriteByte(0x18,1);
  }
   for (i=0;i<8;i++)        // 0x02
  {
    LCD_WriteByte(0x1c,1);
  }
   for (i=0;i<8;i++)        // 0x03
  {
    LCD_WriteByte(0x1e,1);
  }
} /* LCD_BarGraphInit */


/*
-------------------------------------------------------------------------------
LCD_BarGraph
~~~~~~~~~~~~

This procedure displays a horizontal bar graph on the LCD unit in desired line.
The graph is 10 characters wide. Value is displayed at the end of line witch
'%' character.
Before using this function application must call LCD_BarGraphInit.


Parameters:

  int8_t line  [i] - desired bar graph line (first one = 0)
  int8_t l_value [i] - value to be displayed (0-100)

-------------------------------------------------------------------------------
*/
void LCD_BarGraph(int8_t line, int8_t l_value)
{
  int8_t WholeBars,Rest,tmp,i;
  char text[5] = {' ',' ',' ',' ',0};

  /* error message if value out of bounds */
  if (l_value > 100)
  {
     LCD_Goto(0,line);
     LCD_WriteText("Bar Value Error");
  }
  /* display all "full" characters */
  LCD_Goto(0,line);
  WholeBars = (int8_t)(l_value / 7);  /* 2 pixel = 2% */
  if (WholeBars > 0)
  for (i=0;i<WholeBars;i++)
  {
    LCD_WriteByte(0xff,1);
  }
  
  /* display optional character partially filled  */
  Rest = (int8_t)((l_value % 7)/4);
  if (Rest > 0)
     LCD_WriteByte(Rest,1);   /* number of filled pixels is also a character id */

  /* calculate the number of unused charaters in line */
  tmp = 14 - WholeBars;
  if (Rest > 0) tmp--;

  /* clear  the unused characters in line */
  if (tmp > 0)
    for (i=0;i<tmp;i++)
      LCD_WriteByte(' ',1);

  /* clear the value display region */
  LCD_Goto(14,line);
  LCD_WriteText(text);
  /* display the value with '%' character */
  itoa(l_value,text,10);
  LCD_Goto(19-strlen(text),line);
  LCD_WriteText(text);
  LCD_Goto(19,line);
  LCD_WriteByte('%',1);
  
} /* LCD_BarGraph */

/***************************************************************************************************/
void LCDdec(int val)
{
	char bufor[7];
	itoa(val, bufor, 10);
	LCD_WriteText(bufor);
}

