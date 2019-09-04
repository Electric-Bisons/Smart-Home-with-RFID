/*
 * tsop4836_driver.c
 *
 *  Created on: Jul 30, 2019
 *      Author: uic14909
 */

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "tsop4836_driver.h"
#include "timers_driver.h"

/*  IR remote control function flag
 *  0 - first bit
 *  1 - shift mask
 *  2 - skip last falling edge  */
volatile uint8_t U8BitFlag = 0;
/*  bit Index for the bits transmitted (default 33 for avoiding errors)  */
volatile uint8_t U8BitIndex = 33;
/*  the button mask  */
volatile uint32_t U32ButtonMask = 0;

/*  first bit transmitted is skipped  */
void first_bit(void)
{
   /*  reset button mask, get to button sequence and make a bit index larger than 32  */
   U8BitFlag = 1;
   U32ButtonMask = 0;
   U8BitIndex = 33;
}

/*  get the bit seq  */
void button_sequence(void)
{
   /*  start the TCNT0 count after the first bit was transmitted */
   if(U8BitIndex > 32)
   {
      TCNT0 = 0;
      U8BitIndex = 0;
   }
   /*  shift the mask by one to the left  */
   else
   {
      U32ButtonMask <<= 1;
      /*  if the time is greater than aprox 1.2-1.3 ms than it is a 1  */
      if(TCNT0 > 20)
      {
         U32ButtonMask |= 1;
      }
      /*  increment the bit Index  */
      U8BitIndex++;
      /*  if the shift is complete than stop the shifting  */
      if(U8BitIndex > 32)
      {
         U8BitFlag = 2;
      }
      /*  reset the TCNT0 for a new recording  */
      TCNT0 = 0;
   }
   /*  in case of error reset the function  */
   if(U8BitIndex < 32 && TCNT0 > 40)
   {
      U8BitFlag = 0;
   }
}

/*  skip the last falling edge  */
void last_bit(void)
{
   U8BitFlag = 0;
}
