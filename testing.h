
uint8_t testing ( char mode,uint8_t *cable );
volatile void ADC_init (void);
volatile uint8_t get_result(int wire_number, char port );
void adress_do (int wire_number,char port, char s_r_pin,int akcja);
void LCD_port_state(char port_name);
void wire_test_init(void);

