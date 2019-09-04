/*
 * tsop4836_driver.h
 *
 *  Created on: Jul 30, 2019
 *      Author: uic14909
 */

#ifndef INCLUDES_TSOP4836_DRIVER_H_
#define INCLUDES_TSOP4836_DRIVER_H_

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#endif /* INCLUDES_TSOP4836_DRIVER_H_ */

/*  define the buttons values  */
#define CH_MINUS       0xFFA25D
#define CH             0xFF629D
#define CH_PLUS        0xFFE21D
#define BACKWARD       0xFF22DD
#define FORWARD        0xFF02FD
#define PLAY_PAUSE     0xFFC23D
#define VOL_MINUS      0xFFE01F
#define VOL_PLUS       0xFFA857
#define EQ             0xFF906F
#define FOLDER_MINUS   0xFF9867
#define FOLDER_PLUS    0xFFB04F
#define NUMBER_0       0xFF6897
#define NUMBER_1       0xFF30CF
#define NUMBER_2       0xFF18E7
#define NUMBER_3       0xFF7A85
#define NUMBER_4       0xFF10EF
#define NUMBER_5       0xFF38C7
#define NUMBER_6       0xFF5AA5
#define NUMBER_7       0xFF42BD
#define NUMBER_8       0xFF4AB5
#define NUMBER_9       0xFF52AD

void first_bit(void);
void button_sequence(void);
void last_bit(void);

/*  to stop recording errors, make an OVF interrupt and set the bit flag to 0  */
