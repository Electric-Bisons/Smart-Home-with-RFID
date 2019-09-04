/*
 * hc_sr501_driver.h
 *
 *  Created on: Jul 25, 2019
 *      Author: uic14909
 */

#ifndef INCLUDES_HC_SR501_DRIVER_H_
#define INCLUDES_HC_SR501_DRIVER_H_

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#endif /* INCLUDES_HC_SR501_DRIVER_H_ */

/*  define the PIN input  */
#define SensorTrigger   ((PINA & (1 << PA6)) >> 6)

void hc_sr501_init(void);
void PersonDetection(void);
