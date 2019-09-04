
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "SPI.h"
#if SPI_CONFIG_AS_MASTER

void spi_masterinit()
{
	// MOSI , SCK OUTPUT ,SS
	DDRB |= (1<<3)|(1<<5)|(1<<2);
	//MISO INPUT
	DDRB &= ~(1<<4);
	//prescaler 64 , Master mode  , SPI enable
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

uint8_t  spi_mastertransmit( uint8_t data)
{
	// loading data in buffer
	SPDR = data;
	// wait for the flag SPIF to be 1
	while(!(SPSR & (1<<SPIF)));
	// return data
	return (SPDR);
}
#endif