/*
 * smart_home_driver.h
 *
 *  Created on: Jul 31, 2019
 *      Author: uic14909
 */

#ifndef INCLUDES_SMART_HOME_DRIVER_H_
#define INCLUDES_SMART_HOME_DRIVER_H_

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#endif /* INCLUDES_SMART_HOME_DRIVER_H_ */

void lockHouse(void);
void functionMenu(void);
void unlockHouse(void);
void passCheck(uint32_t U32PassVect[], uint8_t U8PassIndex);
void passwordUnlock(void);
void changeFunction(void);
void function_AC(void);
void function_LED(void);
