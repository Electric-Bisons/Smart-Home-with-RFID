/*
 * timers_driver.c
 *
 *  Created on: Jul 29, 2019
 *      Author: uic14909
 */

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "timers_driver.h"

void Int0Interrupt(void)
{
   /*  make PD2 as input  */
   DDRD &= ~(1 << PD2);
   /*  activate interrupt on falling edge  */
   MCUCR = (1 << ISC01) | (1 << ISC00);
   /*  activate INT0 interrupt on PD2  */
   GICR = (1 << INT0);
}

void Timer0(void)
{
   /*  prescaler 1024  */
   TCCR0 |= (1 << CS02) | (1 << CS00);
   TCNT0 = 0;
   /*  set OVF interrupt  */
   TIMSK |= (1 << TOIE0);
}

void fastPWM2(void)
{
   /*  make PWM2's PORTD7 as output  */
   DDRD |= (1 << PD7);
   /*  set fastPWM mode, non-inverting mode  */
   TCCR2 |= (1 << WGM21) | (1 << WGM20) | (1 << COM21);
   /*  set prescaler at 64  */
   TCCR2 |= (1 << CS22);
   OCR2 = 0;
   /*  set OVF interrupt  */
   TIMSK |= (1 << TOIE2);
}
