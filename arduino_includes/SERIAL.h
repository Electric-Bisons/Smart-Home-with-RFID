
#ifndef SERIAL_H_
#define SERIAL_H_
#define BAUD 9600
#define BRC ((F_CPU/16/BAUD)-1)

void serialInit(void)
{	
	//baud rate
	UBRR0H = BRC >>8;
	UBRR0L = BRC; 
	UCSR0B = (1 << TXEN0) |(1<<RXEN0);
	UCSR0C = (1<<UCSZ00) |(1<<UCSZ01);	
}
	// afisare caracter
void serialChar(char c)  
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}
	// afisez string pe serial
void serialString(char *str)    
{
	uint8_t i=0;
	while (str[i]!='\0')
	{
		serialChar(str[i]);
		i++;	
	}
}
#endif /* SERIAL_H_ */