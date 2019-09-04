/*
 * adc_driver.c
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
#include "adc_driver.h"

void ADCinput(void)
{
   /*  set AVCC as reference and ADC0  */
   ADMUX = (1 << REFS0);
   /*  enable ADC and start the first conversion  */
   ADCSRA |= (1 << ADEN) | (1 << ADSC);
   /*  enable interrupt and set prescaler at 64  */
   ADCSRA |= (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1);
}
