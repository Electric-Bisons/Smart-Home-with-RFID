
#ifndef F_CPU
#define F_CPU 16000000UL //16 Mhz clock speeed
#endif
#include "MFRC522.h"
#include "SPI.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
//Initializes MFRC522 CHIP
void MFRC522_Init()
{
	MFRC522_Reset();
	// TAuto=1 timer starts automatically at the end of the transmission in all communication modes at all speeds
	Write_MFRC522(TModeReg, 0x8D);

	Write_MFRC522(TPrescalerReg, 0x3E);
	//Reload timer 0x3E8 =1000 => 25 ms before timeout
	Write_MFRC522(TReloadRegL,30);

	Write_MFRC522(TReloadRegH,0);
	// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
	Write_MFRC522(TxAutoReg, 0x40);
	// Def 0x3F.Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363
	Write_MFRC522(ModeReg, 0x3D);
	AntennaOn();
}

void AntennaOn()
{
	//antenna on enable by turning on TX1 TX2 pins
	unsigned char tmp;
	tmp = Read_MFRC522(TxControlReg);
	if(!(tmp & 0x03))
	{
		SetBitMask(TxControlReg, 0x03);
	}
}

void AntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}
// reset MFRC522 sensor
void MFRC522_Reset()
{
	Write_MFRC522(CommandReg,PCD_RESETPHASE);
}
// writes a byte to the specified  register in the MFRC522 chip

void Write_MFRC522(unsigned char addr, unsigned char val )
{	//enable chip select (low)
	PORTB &= ~(1<<2);
	//MSB = 0 write -address - LSB =0
	spi_mastertransmit((addr<<1) &0x7E);
	spi_mastertransmit(val);
	//disable chip select
	PORTB |= (1<<2);
}

unsigned char  Read_MFRC522(unsigned char addr)
{
	unsigned char  val;
	//enable chip select /select slave
	PORTB &= ~(1<<2);
	//MSB = 1 for reading
	spi_mastertransmit(((addr<<1)&0x7E)|0x80);
	//read the value back
	val = spi_mastertransmit(0x00);
	//disable chip select / release slave
	PORTB |= (1<<2);
	return val;
}

void SetBitMask(unsigned char reg, unsigned char mask)
{
	unsigned char tmp;
	tmp = Read_MFRC522(reg);
	// set bit mask
	Write_MFRC522(reg, tmp | mask);
}

void ClearBitMask(unsigned char reg, unsigned char mask)
{
	unsigned char tmp;
	tmp = Read_MFRC522(reg);
	// clear bit mask
	Write_MFRC522(reg, tmp & (~mask));
}

unsigned char MFRC522_Request(unsigned char reqMode, unsigned char *TagType)
{
	unsigned char status;
	//The received data bits
	unsigned int backBits;
	//TxLastBists = BitFramingReg[2..0]        ???
	Write_MFRC522(BitFramingReg, 0x07);
	TagType[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);
	if ((status != MI_OK) || (backBits != 0x10))
	{
		status = MI_ERR;
	}
	return status;
}
unsigned char MFRC522_ToCard(unsigned char command,
unsigned char *sendData,
unsigned char sendLen,
unsigned char *backData,
unsigned int *backLen)
{
	unsigned char status = MI_ERR;
	unsigned char irqEn = 0x00;
	unsigned char waitIRq = 0x00;
	unsigned char lastBits;
	unsigned char n;
	unsigned int i;

	switch (command)
	{	//Certification cards close
		case PCD_AUTHENT:
		{
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		//Transmit FIFO data
		case PCD_TRANSCEIVE:
		{
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
		break;
	}
	//Interrupt request
	Write_MFRC522(CommIEnReg, irqEn|0x80);
	//Clear all interrupt request bit
	ClearBitMask(CommIrqReg, 0x80);
	//FlushBuffer=1, FIFO Initialization
	SetBitMask(FIFOLevelReg, 0x80);
	//NO action; Cancel the current command???
	Write_MFRC522(CommandReg, PCD_IDLE);

	//Writing data to the FIFO
	for (i=0; i<sendLen; i++)
	{
		Write_MFRC522(FIFODataReg, sendData[i]);
	}
	//Execute the command
	Write_MFRC522(CommandReg, command);
	if (command == PCD_TRANSCEIVE)
	{
		SetBitMask(BitFramingReg, 0x80);                //StartSend=1,transmission of data starts
	}
	//Waiting to receive data to complete
	i = 2000;
	do
	{
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
		n = Read_MFRC522(CommIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&waitIRq));
	//StartSend=0
	ClearBitMask(BitFramingReg, 0x80);
	if (i != 0)
	{	 //BufferOvfl Collerr CRCErr ProtecolErr
		if(!(Read_MFRC522(ErrorReg) & 0x1B))
		{
			status = MI_OK;
			if (n & irqEn & 0x01)
			{
				status = MI_NOTAGERR;
			}
			if (command == PCD_TRANSCEIVE)
			{
				n = Read_MFRC522(FIFOLevelReg);
				lastBits = Read_MFRC522(ControlReg) & 0x07;
				if (lastBits)
				{
					*backLen = (n-1)*8 + lastBits;
				}
				else
				{
					*backLen = n*8;
				}
				if (n == 0)
				{
					n = 1;
				}
				if (n > MAX_LEN)
				{
					n = MAX_LEN;
				}
				//Reading the received data in FIFO
				for (i=0; i<n; i++)
				{
					backData[i] = Read_MFRC522(FIFODataReg);
				}
			}
		}
		else
		{
			status = MI_ERR;
		}
	}
	return status;
}
unsigned char MFRC522_Anticoll(unsigned char *serNum)
{
	unsigned char status;
	unsigned char i;
	unsigned char serNumCheck=0;
	unsigned int unLen;
	//TxLastBists = BitFramingReg[2..0]
	Write_MFRC522(BitFramingReg, 0x00);
	serNum[0] = PICC_ANTICOLL;
	serNum[1] = 0x20;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

	if (status == MI_OK)
	{
		//Check card serial number
		for (i=0; i<4; i++)
		{
			serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i])
		{
			status = MI_ERR;
		}
	}
	return status;
}
void SetFormat(void)
{
	char_send[0] = Separate_hexP10(serNum[0]);
	char_send[1] = Separate_hexP1(serNum[0]);
	char_send[2] = Separate_hexP10(serNum[1]);
	char_send[3] = Separate_hexP1(serNum[1]);
	char_send[4] = Separate_hexP10(serNum[2]);
	char_send[5] = Separate_hexP1(serNum[2]);
	char_send[6] = Separate_hexP10(serNum[3]);
	char_send[7] = Separate_hexP1(serNum[3]);
	read[0]=char_send[0];
	read[1]= char_send[1];
	read[2]= char_send[2];
	read[3]= char_send[3];
	read[4]= char_send[4];
	read[5]= char_send[5];
	read[6]= char_send[6];
	read[7]= char_send[7];
}
unsigned char Separate_hexP10(unsigned char val)
{
	val = val & 0xF0;
	val = val >> 4;
	if (val < 10)
	{
		return val + 48;
	}
	else
	{
		return val + 55;
	}
}
unsigned char Separate_hexP1(unsigned char val)
{
	val = val & 0x0F;
	if (val < 10)
	{
		return val + 48;
	}
	else
	{
		return val + 55;
	}
}
void MFRC522_Halt(void)
{
	unsigned char status;
	unsigned int unLen;
	unsigned char buff[4];
	buff[0] = PICC_HALT;
	buff[1] = 0;
	CalulateCRC(buff, 2, &buff[2]);
	status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
}
void CalulateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
	unsigned char i, n;
	//CRCIrq = 0
	ClearBitMask(DivIrqReg, 0x04);
	//Clear the FIFO pointer
	SetBitMask(FIFOLevelReg, 0x80);
	//Writing data to the FIFO
	for (i=0; i<len; i++)
	{
		Write_MFRC522(FIFODataReg, *(pIndata+i));
	}
	Write_MFRC522(CommandReg, PCD_CALCCRC);
	//Wait CRC calculation is complete
	i = 0xFF;
	do
	{
		n = Read_MFRC522(DivIrqReg);
		i--;
	}
	//CRCIrq = 1
	while ((i!=0) && !(n&0x04));
	//Read CRC calculation result
	pOutData[0] = Read_MFRC522(CRCResultRegL);
	pOutData[1] = Read_MFRC522(CRCResultRegM);
}





