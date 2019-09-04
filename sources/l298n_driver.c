/*
 * l298n_driver.c
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
#include "l298n_driver.h"

/*  DutyCycle register  */
volatile int IDutyCycle = 0;

void motor_control_init(void)
{
   /*  make pwm2 pin as output just for safe  */
   DDRD |= (1 << PD7);
   /*  make IN1 pin as output  */
   DDRA |= (1 << PA4);
   /*  stop the motor  */
   motor_control_stop();
}

void motor_control_stop(void)
{
   /*  turn off IN1 pin and PWM pin  */
   IN1_OFF;
   IDutyCycle = 0;
}

void motor_control_start(void)
{
   /*  turn on IN1  */
   IN1_ON;
}
