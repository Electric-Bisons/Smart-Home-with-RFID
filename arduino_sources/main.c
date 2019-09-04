#ifndef F_CPU
#define F_CPU 16000000UL //16 Mhz clock speeed
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/INIT.h"
#include "src/RFIDFUNCTIONS.h"

int main()
{
	//initialize serial,rfid,spi
	init();
	while(1)
	{
		//if there are any cards get the UID card number
		get_UID();
	}
	return 0;
}
