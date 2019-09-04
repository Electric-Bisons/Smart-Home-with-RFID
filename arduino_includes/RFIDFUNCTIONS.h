
#ifndef RFIDFUNCTIONS_H_
#define RFIDFUNCTIONS_H_
#include "MFRC522.h"
unsigned char status;
unsigned char str[MAX_LEN];

void get_UID()
{
	//Find cards, return card type - Don't remove this sub
	status = MFRC522_Request(PICC_REQIDL, str);
	//Anti-collision, return card serial number 4 bytes
	status = MFRC522_Anticoll(str);
	memcpy(serNum, str, 5);
	//check if there is a new card detected
	if (status == MI_OK)
	{
		// take serNum and format it and return read
		SetFormat();
		//serialString("UID:");
		for (int i =0;i<8;i++)
		{
			serialChar(read[i]);
			
		}
		serialString("\n\r");		
	}
	// Command card into hibernation
	MFRC522_Halt();
	_delay_ms(1000);
}


#endif /* RFIDFUNCTIONS_H_ */