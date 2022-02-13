
/*komunikacja po usarcie*/
#include <avr/io.h>
#include "util.h"
#define BAUD 9600     // Baud rate
#include "dec.h"
#include <avr/interrupt.h>
#include "globals.h"
#include "LCD.h"
#include "port_definitions.h"


void USART_init(void)
{

 UCSR1C = 0x06;
 UBRR1L = 0x19; //set baud rate lo
 UBRR1H = 0x00; //set baud rate hi
 UCSR1B = 0x18;
 }

/*****************************************************************************************************/

void USART_Transmit_char( unsigned char data )
{

	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE1)) )
	;
	/* Put data into buffer, sends the data */
UDR1 = data;
}

/**********************************************************************************************/

void USART_Transmit_line( char text[] ) 
{	
	int8_t i=0;
	while (text[i] != 0)
	{
		USART_Transmit_char( text[i] );
		i++;
	}

}

/*************************************************************************************************/

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC1)) )
	;
	/* Get and return received data from buffer */
	return UDR1;
}

/********************************************************************************************/

void USART_port_state(char port_name)
{

int i,j = 0;



if (port_name == 'B')
{
	USART_Transmit_char(0x0a);
	USART_Transmit_char(0x0d);
	USART_Transmit_line("BLEDY W NASTEPUJACYCH PRZEWODACH:");
	USART_Transmit_char(0x0a);
	USART_Transmit_char(0x0d);	

	for (i=0;i<24;i++)
	{
		if(flag[i] == 0)
		{
		USART_Transmit_char(0x0a);
		USART_Transmit_char(0x0d);
		USART_Transmit_line("PIN ");	
		if(i>21)USART_Transmit_number(i+2);
		else USART_Transmit_number(i+1);
		USART_Transmit_line("A: ");


		for (j=0;j<24;j++)
			{
	//	USART_Transmit_number(port_state[i][j]); //debug
				if (port_state[i][j] == 6)
				{
				USART_Transmit_line(" ZWARTY Z-");
			
					if (j>21) USART_Transmit_number(j+2);
					else USART_Transmit_number(j+1);
				USART_Transmit_char('B');				
				}
				else if (port_state[i][j] == 7)
				{
				USART_Transmit_line(" ROZWARTY Z-");
					if (j>21) USART_Transmit_number(j+2);
					else USART_Transmit_number(j+1);
				USART_Transmit_char('B');
				
				}
			}
		}
	}
		
}

else if (port_name == 'E')
{
	USART_Transmit_char(0x0a);
	USART_Transmit_char(0x0d);
	USART_Transmit_line("BLEDY W NASTEPUJACYCH PRZEWODACH:");
	USART_Transmit_char(0x0a);
	USART_Transmit_char(0x0d);

for (i=0;i<24;i++)
	{
		if(flag[i] == 0)
		{
		USART_Transmit_char(0x0a);
		USART_Transmit_char(0x0d);
		USART_Transmit_line("PIN ");
			if(i>21)USART_Transmit_number(i+2);
			else USART_Transmit_number(i+1);
			USART_Transmit_line("A: ");
		
		for (j=0;j<24;j++)
			{
	//	USART_Transmit_number(port_state[i][j]); //debuger
				if (port_state[i][j] == 6)
				{
				USART_Transmit_line(" ZWARTY Z-");
					if (j>21) USART_Transmit_number(j+2);
					else USART_Transmit_number(j+1);
				USART_Transmit_char('E');
				}
				else if (port_state[i][j] == 7)
				{
				USART_Transmit_line(" ROZWARTY Z-");
					if (j>21) USART_Transmit_number(j+2);
					else USART_Transmit_number(j+1);
				USART_Transmit_char('E');
					
				}
			}
		}		
	}

}

}

/******************************************************************************************/

void USART_Transmit_number(int number) // 
{
	if (number < 10)
		USART_Transmit_char(number+0x30);
	else if (number<20)
	{
		USART_Transmit_char(0x31);
		USART_Transmit_char(0x30+(number%10));
	}
	else
	{
		USART_Transmit_char(0x32);
		USART_Transmit_char(0x30 +(number%20));
	}

}
