
#ifndef INIT_H_
#define INIT_H_
#include "SPI.h"
#include "MFRC522.h"
#include "SERIAL.h"

void init()
{
	// initialize uart for serial PORT
	serialInit();
	spi_masterinit();
	_delay_ms(50);
	serialString("SPI has been initialized\n\r");
	//ss active
	PORTB &= ~(1<<2);
	//initialize the sensor
	MFRC522_Init();
	_delay_ms(50);
	serialString("RFID sensor has been initialized\n\r");
}
#endif /* INIT_H_ */