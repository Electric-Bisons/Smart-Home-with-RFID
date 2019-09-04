/*
 * ds18b20_driver.c
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
#include "ds18b20_driver.h"
#include "lcd_driver.h"

volatile uint8_t U8CCP;

/*  store temperature in a vector for better LCD print  */
uint8_t U8TempVector[3];
/*  store the temperature in a temporary register  */
uint16_t U16TempRead = 0;
/*  store the temperature in a global register  */
uint16_t U16GlobalTemp = 0;

void writeBus(uint8_t U8Reg)
{
   /*  store the I-bit status in a temporary register  */
   uint8_t tempU8SREG = SREG;
   /*  stop any interrupt  */
   cli();
   /*  write on the sensor bit by bit  */
   for(uint8_t U8Index = 0 ; U8Index < 8 ; U8Index++)
   {
      TEMPERATURE_PORTB_2 = 0;
      DS18b20_WRITE;
      _delay_us(1);
      /*  send bits in LSB order  */
      if((U8Reg >> U8Index) & 1)
      {
         DS18b20_READ;
      }
      _delay_us(60);
      DS18b20_READ;
   }
   /*  restore the SREG  */
   SREG = tempU8SREG;
}

uint8_t readBus(void)
{
   /*  store the I-bit status in a temporary register  */
   uint8_t tempU8SREG = SREG;
   /*  stop any interrupt  */
   cli();
   /*  store the temperature register temporary  */
   uint8_t tempU8Reg = 0;
   /*  read the sensor bit by bit  */
   for(uint8_t U8Index = 0 ; U8Index < 8 ; U8Index++)
   {
      TEMPERATURE_PORTB_2 = 0;
      DS18b20_WRITE;
      _delay_us(1);
      DS18b20_READ;
      _delay_us(14);
      /*  sample the pin  */
      if(DS18b20_PIN_READ)
      {
         tempU8Reg |= (1 << U8Index);
      }
      else
      {
         tempU8Reg |= (0 << U8Index);
      }
      _delay_us(45);
   }
   /*  restore the SREG  */
   SREG = tempU8SREG;
   /*  return the register read  */
   return tempU8Reg;
}

void ResetTemperatureSensor(void)
{
   TEMPERATURE_PORTB_2 = 0;
   DS18b20_WRITE;
   _delay_us(480);
   DS18b20_READ;
   _delay_us(60);
   /*  wait for the pin read to be 1  */
   while(!DS18b20_PIN_READ);
   _delay_us(420);
}

void storeTemp(uint8_t tempU8TempVector[])
{
   /*  put the number in a vector digit by digit  */
   tempU8TempVector[2] = U16TempRead % 10 + '0';
   U16TempRead /= 10;
   tempU8TempVector[1] = U16TempRead % 10 + '0';
   U16TempRead /= 10;
   tempU8TempVector[0] = U16TempRead % 10 + '0';
   U16TempRead /= 10;
}

void Temperature(void)
{
   /*  LOW and HIGH register for the temperature  */
   uint8_t U8TempLB = 0;
   uint8_t U8TempHB = 0;
   /*  check sensor and start a conversion  */
   ResetTemperatureSensor();
   writeBus(DS18b20_COM_SKIP_ROM);
   writeBus(DS18b20_FUN_CONVERT_T);
   /*  wait for the conversion to be done  */
   while(!readBus());

   /*  check for sensor and read SP  */
   ResetTemperatureSensor();
   writeBus(DS18b20_COM_SKIP_ROM);
   writeBus(DS18b20_FUN_READ_SP);
   /*  read the two temperature bytes and the stop the read  */
   U8TempLB = readBus();
   U8TempHB = readBus();
   ResetTemperatureSensor();

   /*  store the two bytes in 2 bytes and convert to Celsius (3 digits)  */
   U16TempRead = ((U8TempHB * 256) + U8TempLB) * 10 / 16;
   U16GlobalTemp = U16TempRead;
}

void LCDTemperature(void)
{
   /*  store the result in a vector  */
   storeTemp(U8TempVector);
   /*  show on LCD the temperature  */
   U8CCP = 0xC0;
   writeWordOnLCD("T = ");
   writeLCD(U8TempVector[0], U8CCP);
   writeLCD(U8TempVector[1], U8CCP);
   writeWordOnLCD(".");
   writeLCD(U8TempVector[2], U8CCP);
   /*  ASCII code for degrees  */
   writeLCD(223, U8CCP);
   writeWordOnLCD("C");
}
