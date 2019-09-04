/*
 * l298n_driver.h
 *
 *  Created on: Jul 29, 2019
 *      Author: uic14909
 */

#ifndef INCLUDES_L298N_DRIVER_H_
#define INCLUDES_L298N_DRIVER_H_

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#endif /* INCLUDES_L298N_DRIVER_H_ */

/*  IN2 is connected to GND  */
/*  define IN1 pin  */
#define IN1_ON PORTA |= (1 << PA4)
#define IN1_OFF PORTA &= ~(1 << PA4)

void motor_control_init(void);
void motor_control_stop(void);
void motor_control_start(void);
