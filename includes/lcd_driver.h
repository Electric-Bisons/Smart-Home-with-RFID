/*
 * lcd_driver.h
 *
 *  Created on: Jul 23, 2019
 *      Author: uic14909
 */

#ifndef INCLUDES_LCD_DRIVER_H_
#define INCLUDES_LCD_DRIVER_H_

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#endif /* INCLUDES_LCD_DRIVER_H_ */

void initLCD(void);
void LCDloading(void);
void Eon(void);
void Eoff(void);
void writeOn(void);
void writeOff(void);
void clearDisplay(void);
void writeLCD(uint8_t U8Text, uint8_t U8Position);
void writeWordOnLCD(char* tempWord);
