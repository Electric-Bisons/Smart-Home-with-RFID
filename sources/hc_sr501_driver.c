/*
 * hc_sr501_driver.c
 *
 *  Created on: Jul 25, 2019
 *      Author: uic14909
 */

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "hc_sr501_driver.h"
#include "lcd_driver.h"

extern volatile uint8_t U8CCP;

uint8_t U8ProximityFlag = 0;

/*  Suggestions:
 *  the sensitivity is going to be set with pots (suggested 5s delay and any distance)
 *  the L mode is going to be used so it stays active while the person is in range of the sensor
 *  */

void hc_sr501_init(void)
{
   /*  set the PORTC4 as input for the digital output of the sensor  */
   DDRA &= ~(1 << PA6);
   /*  set PORTC3 as output for the buzzer  */
   DDRA |= (1 << PA5);
   PORTA &= ~(1 << PA5);
}

void PersonDetection(void)
{
   /*  if person was detected  */
   if(SensorTrigger == 1)
   {
      if(U8ProximityFlag)
      {
         clearDisplay();
         U8CCP = 0x80;
         writeWordOnLCD("WARNING         PERSON DETECTED!");
         /*  deactivate proximity flag  */
         U8ProximityFlag = 0;
      }
   }
   else
   {
      /*  activate proximity flag  */
      U8ProximityFlag = 1;
   }
}
