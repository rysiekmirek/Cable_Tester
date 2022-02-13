
/*definicje portów dla Atmega 128*/

//port A
#define AX0 PORTA0
#define AX1 PORTA1
#define AX2 PORTA2
#define AX3 PORTA3
#define AY0 PORTA4
#define DATA PORTA5
#define STBA PORTA6
#define STBB PORTA7

//port B
#define SCK PORTB1
#define KEY_COL1 PORTB2
#define KEY_COL2 PORTB3
#define PWM1 PORTB4
#define LEDA PORTB5
#define LEDB PORTB6
#define PWM2 PORTB7


//port C
#define STBC PORTC0
#define RSTA PORTC1
#define RSTB PORTC2
#define RSTC PORTC3
#define CSA PORTC4
#define CSB PORTC5
#define CSC PORTC6

//port D
/*#define LCD_RS PORTD0
#define LCD_E PORTD1*/
#define RXD1 PIND2
#define TXD1 PORTD3/*
#define LCD_DB4 PORTD4
#define LCD_DB5 PORTD5
#define LCD_DB6 PORTD6
#define LCD_DB7 PORTD7*/ /*te makra znajduj¹ sie w LCD.h*/

//port E
#define Serial_RxD PINE0
#define Serial_TxD PORTE1
#define KEY_COL3 PORTE2
#define KEY_ROW1 PINE3
#define KEY_ROW2 PINE4
#define KEY_ROW3 PINE5
#define KEY_ROW4 PINE6
#define KEY_INT PINE7

//port F
#define ADC0 PINF0
#define ADC1 PINF1
#define ADC2 PINF2
#define BAT PINF3
#define OVERLOAD PINF4

