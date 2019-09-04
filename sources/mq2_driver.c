/*
 * mq2_driver.c
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
#include <math.h>
#include "mq2_driver.h"
#include "adc_driver.h"
#include "lcd_driver.h"

/*  ppm, RS/R0 and slope of LPG (this is the initial point)  */
double LPG_GAS_VECTOR[3] = {200, 1.6, -0.43};
double D64PPMvalue = 0;
double tempD64RSValue = 0;
/*  make this 1.6 to make sure it won't activate on the initialization  */
double tempD64RS_RO_value = 1.6;

void RS_RO_function(void)
{
   /*  the RS is in series with RL and we get a voltage divider  */
   tempD64RSValue = RL_VALUE * (1023.0 / ADC - 1);
   /*  make the division between the RS value and the default RO value  */
   tempD64RS_RO_value = tempD64RSValue / RO_VALUE;
}

void PPM_function(void)
{
   /*  x2 = 10 ^ ((log(y2) - log(y1) + m * log(x1)) / m)  */
   D64PPMvalue = pow(10, ((log10(tempD64RS_RO_value) - log10(LPG_GAS_VECTOR[1])\
                 + (LPG_GAS_VECTOR[2] * log10(LPG_GAS_VECTOR[0])))\
                 / LPG_GAS_VECTOR[2]));
}

void PPM_display(uint8_t tempU8PPMvector[])
{
   /*  get the PPM digits  */
   tempU8PPMvector[3] = (uint16_t)D64PPMvalue % 10 + '0';
   D64PPMvalue /= 10;
   tempU8PPMvector[2] = (uint16_t)D64PPMvalue % 10 + '0';
   D64PPMvalue /= 10;
   tempU8PPMvector[1] = (uint16_t)D64PPMvalue % 10 + '0';
   D64PPMvalue /= 10;
   tempU8PPMvector[0] = (uint16_t)D64PPMvalue % 10 + '0';
   D64PPMvalue /= 10;
}
