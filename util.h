//Utility definitions

#define SETBIT(ADRESS, BIT) (ADRESS  |=  (1<<BIT)) //sets bit# BIT in ADDRESS
#define CLRBIT(ADRESS, BIT) (ADRESS  &= ~(1<<BIT)) //clears a bit
#define CHKBIT(ADRESS, BIT) (ADRESS  &   (1<<BIT)) //checks a bit. returns 0 if it is 0. returns something if not
#define CPLBIT(ADRESS, BIT) (ADRESS  ^=  (1<<BIT)) //sets complimentary bit. 0->1, 1->0

#define GETBIT(ADRESS, BIT) ((ADRESS  &   (1<<BIT))>>BIT) //extanded version of CHKBIT - returns 0 if it is 0 and 1 if it is 1

#define F_CPU 4032000UL
