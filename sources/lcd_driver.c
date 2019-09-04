/*
 * lcd_driver.c
 *
 *  Created on: Jul 23, 2019
 *      Author: uic14909
 */

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "lcd_driver.h"

/*  create a global variable for the position of the cursor
 *  Row1 = 0x80...0x8F
 *  Row2 = 0xC0...0xCF  */
volatile uint8_t U8CCP = 0x80;

void initLCD(void)
{
   /*  set PORTC as D7...D0 data pins  */
   DDRC = 0xFF;
   /*  set PB1 and PB0 as RS and E pins and make them 0 just to  be sure  */
   DDRB |= (1 << PB1) | (1 << PB0);
   PORTB &= ~(1 << PB1) & ~(1 << PB0);
   /*  function set as 8 bit, 2 LINES, 5x8  */
   for(int i = 0 ; i < 4 ; i++)
   {
      Eon();
      PORTC = 0x38;
      Eoff();
   }
   /*  deactivate LCD  */
   Eon();
   PORTC = 0x08;
   Eoff();
   /*  clear display  */
   Eon();
   PORTC = 0x01;
   Eoff();
   /*  set cursor  */
   Eon();
   PORTC = 0x06;
   Eoff();
   /*  activate display and cursor  */
   Eon();
   PORTC = 0x0E;
   Eoff();
}

void LCDloading(void)
{
   /*  temporary Index and Vector  */
   uint8_t tempU8Index = 0;
   uint8_t tempU8IndexVector[3];
   clearDisplay();
   U8CCP = 0x80;
   writeWordOnLCD("Welcome to the");
   U8CCP = 0xC0;
   writeWordOnLCD("SMART HOUSE!");
   _delay_ms(5000);
   clearDisplay();
   /*  write on display Loading...0-100%  */
   for(uint8_t U8Index = 0; U8Index < 100 ; U8Index++)
   {
      /*  store the Index  */
      tempU8Index = U8Index;
      /*  store the digits in a vector  */
      tempU8IndexVector[2] = tempU8Index % 10 + '0';
      tempU8Index /= 10;
      tempU8IndexVector[1] = tempU8Index % 10 + '0';
      tempU8Index /= 10;
      tempU8IndexVector[0] = tempU8Index % 10 + '0';
      tempU8Index /= 10;
      U8CCP = 0x80;
      writeWordOnLCD("Loading...");
      writeLCD(tempU8IndexVector[0], U8CCP);
      writeLCD(tempU8IndexVector[1], U8CCP);
      writeLCD(tempU8IndexVector[2], U8CCP);
      writeWordOnLCD("%");
      _delay_ms(10);
   }
   clearDisplay();
}

void Eon(void)
{
   /*  turn E on  */
   PORTB |= (1 << PB0);
}

void Eoff(void)
{
   /*  turn E off after a little delay  */
   _delay_ms(5);
   PORTB &= ~(1 << PB0);
}

void writeOn(void)
{
   /*  turn RS and E on  */
   PORTB |= (1 << PB1) | (1 << PB0);
}

void writeOff(void)
{
   /*  turn RS and E off after a little delay  */
   _delay_ms(1);
   PORTB &= ~(1 << PB1) & ~(1 << PB0);
}

void clearDisplay(void)
{
   Eon();
   PORTC = 0x01;
   Eoff();
}

void writeLCD(uint8_t U8Text, uint8_t U8Position)
{
   /*  set CCP to the current position  */
   U8CCP = U8Position;
   /*  if the position at the end of the row1 move it to row2  */
   if((U8CCP > 0x8F) && (U8CCP < 0xC0))
   {
      U8CCP = U8CCP - 0x8F + 0xC0 - 1;
   }
   /*  set the text and his position  */
   Eon();
   PORTC = U8CCP;
   Eoff();
   writeOn();
   PORTC = U8Text;
   writeOff();
   /*  update the position of the cursor  */
   U8CCP++;
}

void writeWordOnLCD(char* tempWord)
{
   uint8_t tempU8StringIndex = 0;
   /*  read through the char pointer as long it is != of the null character  */
   while(tempWord[tempU8StringIndex] != '\0')
   {
      /*  write the current character at the current position  */
      writeLCD(tempWord[tempU8StringIndex], U8CCP);
      tempU8StringIndex++;
   }
}
