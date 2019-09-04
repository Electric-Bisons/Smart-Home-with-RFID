/*
 * mq2_driver.h
 *
 *  Created on: Jul 23, 2019
 *      Author: uic14909
 */

#ifndef INCLUDES_MQ2_DRIVER_H_
#define INCLUDES_MQ2_DRIVER_H_

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#endif /* INCLUDES_MQ2_DRIVER_H_ */

/*  the RL value set from the potentiometer  */
#define RL_VALUE 10
/*  the RS/RO default value  */
#define RS_RO_VALUE_H2 9.8
/*  the RO value after a few tests  */
#define RO_VALUE 40

void RS_RO_function(void);
void PPM_function(void);
void PPM_display(uint8_t tempU8PPMvector[]);
