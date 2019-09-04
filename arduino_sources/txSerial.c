#ifndef F_CPU
#define F_CPU 16000000UL //16 Mhz clock speeed
#endif

#include <util/delay.h>
#include <avr/io.h>
void serial_init()
{
	UCSR0A=0x00;
	UCSR0B=0x18;
	UCSR0C=0x86;
	UBRR0H=0x00;
	UBRR0L=0x19;
}

void serial_send(unsigned char dat)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = dat;
}
unsigned char serial_read()
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

int main(void)
{
	
	UDR0 ='8';
	
}