/*
 * smart_home_driver.c
 *
 *  Created on: Jul 31, 2019
 *      Author: uic14909
 */

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "adc_driver.h"
#include "ds18b20_driver.h"
#include "hc_sr501_driver.h"
#include "l298n_driver.h"
#include "lcd_driver.h"
#include "mq2_driver.h"
#include "smart_home_driver.h"
#include "timers_driver.h"
#include "tsop4836_driver.h"
#include "usart_driver.h"

extern volatile uint8_t U8CCP;
extern volatile int IDutyCycle;

/*  0/1 - unlocked/locked  */
uint8_t U8PassFlag = 0;
/*  AC flag  */
uint8_t U8ACFlag = 0;
/*  unlocked house function register  */
int IFunctionFlag = 0;
/*  password check register  */
uint8_t U8PassCheck = 0;
/*  password vector  */
uint32_t U32PassVector[] = {NUMBER_0, NUMBER_1, NUMBER_4, NUMBER_7};
/*  Remote Control pressed button mask  */
volatile uint32_t U32Button = 0;

void lockHouse(void)
{
   /*  turn off buzzer  */
   PORTA &= ~(1 << PA5);
   /*  set bit flag to locked  */
   U8PassFlag = 0;
   /*  reset check register  */
   U8PassCheck = 0;
   /*  reset function menu  */
   IFunctionFlag = 0;
   /*  reset AC flag  */
   U8ACFlag = 0;
   clearDisplay();
   U8CCP = 0x80;
   writeWordOnLCD("HOUSE LOCKED!");
}

void functionMenu(void)
{
   /*  change message by the menu  */
   if(IFunctionFlag == 0)
   {
      clearDisplay();
      U8CCP = 0x80;
      writeWordOnLCD("WELCOME, ANDREI!");
   }
   if(IFunctionFlag == 1)
   {
      clearDisplay();
      U8CCP = 0x80;
      writeWordOnLCD("TEMPERATURE MENU");
   }
   if(IFunctionFlag == 2)
   {
      clearDisplay();
      U8CCP = 0x80;
      writeWordOnLCD("AC MENU");
   }
   if(IFunctionFlag == 3)
   {
      clearDisplay();
      U8CCP = 0x80;
      writeWordOnLCD("LED MENU");
      /*  make PORTA7 as output  */
      DDRA |= (1 << PA7);
      /*  set LED off by default  */
      PORTA |= (1 << PA7);
   }
}

void unlockHouse(void)
{
   /*  turn off buzzer  */
   PORTA &= ~(1 << PA5);
   /*  turn on buzzer and green LED  */
   PORTA |= (1 << PA5);
   PORTA &= ~(1 << PA3);
   _delay_ms(1000);
   /*  turn off buzzer and green LED  */
   PORTA &= ~(1 << PA5);
   PORTA |= (1 << PA3);
   /*  set bit to unlocked  */
   U8PassFlag = 1;
   clearDisplay();
   U8CCP = 0x80;
   writeWordOnLCD("HOUSE UNLOCKED!");
   _delay_ms(5000);
   clearDisplay();
   U8CCP = 0x80;
   writeWordOnLCD("Welcome!");
   _delay_ms(5000);
   functionMenu();
}

void passCheck(uint32_t U32PassVect[], uint8_t U8PassIndex)
{
   /*  wait for a button press  */
   while(U32Button == 0);
   /*  if button was correctly pressed store it in a flag  */
   if(U32Button == U32PassVect[U8PassIndex])
   {
      U8PassCheck |= (1 << U8PassIndex);
   }
   U32Button = 0;
   writeWordOnLCD("*");
}

void passwordUnlock(void)
{
   /*  turn off buzzer  */
   PORTA &= ~(1 << PA5);
   clearDisplay();
   U8CCP = 0x80;
   writeWordOnLCD("PASS:");
   passCheck(U32PassVector, 0);
   passCheck(U32PassVector, 1);
   passCheck(U32PassVector, 2);
   passCheck(U32PassVector, 3);
   /*  if the password flag is correct unlock the house  */
   if(U8PassCheck == 0x0F)
   {
      /*  correct password message  */
      clearDisplay();
      U8CCP = 0x80;
      writeWordOnLCD("Correct         Password!");
      _delay_ms(1000);
      unlockHouse();
   }
   /*  if the password flag is incorrect keep the house locked  */
   else
   {
      /*  wrong password message  */
      clearDisplay();
      U8CCP = 0x80;
      writeWordOnLCD("Wrong           Password!");
      _delay_ms(1000);
      lockHouse();
   }
}

void changeFunction(void)
{
   /*  change through functions  */
   if(U32Button == FORWARD)
   {
      U32Button = 0;
      /*  increment function flag  */
      IFunctionFlag++;
      /*  if flag is greater than 3 then set it to 1  */
      if(IFunctionFlag > 3)
      {
         IFunctionFlag = 1;
      }
      functionMenu();
   }
   if(U32Button == BACKWARD)
   {
      U32Button = 0;
      /*  decrement function flag  */
      IFunctionFlag--;
      /*  if flag is less than 1 then set it to 3  */
      if(IFunctionFlag < 1)
      {
         IFunctionFlag = 3;
      }
      functionMenu();
   }
}

void function_AC(void)
{
   /*  change the AC value  */
   if(U32Button == VOL_PLUS)
   {
      U32Button = 0;
      clearDisplay();
      U8CCP = 0x80;
      /*  increment by 10  */
      IDutyCycle += 10;
      writeWordOnLCD("AC ON");
      motor_control_start();
      if(IDutyCycle < 50)
      {
         IDutyCycle = 50;
      }
      if(IDutyCycle > 100)
      {
         IDutyCycle = 100;
      }
      U8CCP = 0xC0;
      if(IDutyCycle == 100)
      {
         writeWordOnLCD("100%");
      }
      else
      {
         writeLCD(IDutyCycle / 100 + '0', U8CCP);
         writeLCD(IDutyCycle / 10 + '0', U8CCP);
         writeWordOnLCD("0%");
      }
   }
   if(U32Button == VOL_MINUS)
   {
      U32Button = 0;
      /*  decrement by 10  */
      IDutyCycle -= 10;
      if(IDutyCycle >= 50)
      {
         clearDisplay();
         writeWordOnLCD("AC ON");
         U8CCP = 0xC0;
         if(IDutyCycle == 100)
         {
            writeWordOnLCD("100%");
         }
         else
         {
            writeLCD(IDutyCycle / 100 + '0', U8CCP);
            writeLCD(IDutyCycle / 10 + '0', U8CCP);
            writeWordOnLCD("0%");
         }
      }
      if(IDutyCycle < 50)
      {
         motor_control_stop();
         clearDisplay();
         U8CCP = 0x80;
         writeWordOnLCD("AC OFF");
      }
   }
   /*  activate AC flag if AC is on  */
   if(IDutyCycle >= 50)
   {
      U8ACFlag = 1;
   }
   else
   {
      U8ACFlag = 0;
   }
}

void function_LED(void)
{
   /*  change LED state by pressing a button  */
   if(U32Button == EQ)
   {
      U32Button = 0;
      PORTA ^= (1 << PA7);
   }
}
