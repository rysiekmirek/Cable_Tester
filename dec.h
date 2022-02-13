
//deklaracje


void keypad_init(void);
int keypad (void);
void battery_test (void);
void USART_init(void);
void USART_Transmit_char( unsigned char data );
void USART_Transmit_line( char text[] ) ;
unsigned char USART_Receive( void );
void USART_port_state(char port_name);
void USART_Transmit_number(int number);
