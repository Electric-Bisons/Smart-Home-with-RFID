#include <avr/io.h>


#define F_CPU 16000000
#define BAUD 9600
#define BRC ((F_CPU/16/BAUD)-1)
#include <util/delay.h>

void serialInit(void) 
{
	UBRR0 = BRC; //baud rate
	UCSR0B = (1 << TXEN0);
}

void serialChar(uint8_t c)  // afisare caracter 
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void serialString(char *str)    // afisez string pe serial
{
	while (*str)
	{
		serialChar(*str++);
	}
}

int main(void) 
{
	serialInit();
	serialString("Hello");
	_delay_ms(50);
	serialString("World");
	_delay_ms(50);
	serialChar('a');
	while(1) 
	{
		
		
	}
}

