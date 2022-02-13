
/*
  Do prawidlowej pracy konieczne jest prawidlowe zdefiniowanie portu LCD
  i pinow odpowiadajacych za komunikacje z wyswietlaczem.
*/
#define LCD_PORT   PORTD // port ktorego piny polaczone sa z wyswietlaczem

#define LCD_DDR    DDRD  // rejestr kierunku dla portu LCD
#define LCD_E      PD1   // numer pinu na porcie LCD odpowiadajacego funkcji E
#define LCD_RS     PD0   // numer pinu na porcie LCD odpowiadajacego funkcji RS
#define LCD_DB4    PD4   // numer pinu na porcie LCD odpowiadajacego bitowi DB4
#define LCD_DB5    PD5   // numer pinu na porcie LCD odpowiadajacego bitowi DB5
#define LCD_DB6    PD6   // numer pinu na porcie LCD odpowiadajacego bitowi DB6
#define LCD_DB7    PD7   // numer pinu na porcie LCD odpowiadajacego bitowi DB7


/* naglowki procedur: */
void LCD_WriteByte(int8_t LCDByte, int8_t IsData);
void LCD_WriteText(char text[]);
void LCD_Goto(int8_t column, int8_t line);
void LCD_ClrLine(int8_t line);
void LCD_Init(void);
void LCD_BarGraphInit(void);
void LCD_BarGraph(int8_t line, int8_t value);
void PWMs_init(void);
