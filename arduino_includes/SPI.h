
#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>
#define SPI_CONFIG_AS_MASTER 	1

void spi_masterinit();
uint8_t spi_mastertransmit( uint8_t data);
#endif /* SPI_H_ */