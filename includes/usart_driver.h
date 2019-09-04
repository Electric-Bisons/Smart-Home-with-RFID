/*
 * usart_driver.h
 *
 *  Created on: Aug 5, 2019
 *      Author: uic14909
 */

#ifndef INCLUDES_USART_DRIVER_H_
#define INCLUDES_USART_DRIVER_H_

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#endif /* INCLUDES_USART_DRIVER_H_ */

/*  define BAUD rate  */
#define BAUD 9600
#define baud ((16000000 / BAUD / 16) - 1)

/*  usart initialization  */
void usart_init(void);
/*  usart receive function  */
char usart_receive(void);
/*  usart transmit function  */
void usart_transmit(char data);
/*  button usart function  */
uint32_t usartButton(void);
