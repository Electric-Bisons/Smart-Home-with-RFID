/*
 * usart_driver.c
 *
 *  Created on: Aug 5, 2019
 *      Author: uic14909
 */

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "usart_driver.h"

/*  index used for shifting  */
uint8_t U8ButtonIndex = 0;
/*  mask for the byte received on usart  */
uint8_t U8Mask = 0;
/*  the 4 bytes mask used for the button  */
uint32_t U32USARTBM = 0;

void usart_init(void)
{
   /*  set baud rate  */
   UBRRH = baud >> 8;
   UBRRL = baud;
   /*  enable RX, TX and interrupts  */
   UCSRB = (1 << RXEN) | (1 << RXCIE) | (1 << TXEN) | (1 << TXCIE);
   /*  set 8-bit data  */
   UCSRC = (1 << URSEL) | ( 1<< UCSZ1) | (1 << UCSZ0);
}

char usart_receive(void)
{
   /*  wait for data to be received  */
   while(!(UCSRA & (1 << RXC)));
   /*  get data received  */
   return UDR;
}

void usart_transmit(char data)
{
   /*  wait data buffer empty  */
   while(!(UCSRA & (1 << UDRE)));
   /*  send data  */
   UDR = data;
}

uint32_t usartButton(void)
{
   /*  reset index if shift is done  */
   if(U8ButtonIndex == 0)
   {
      U8ButtonIndex = 32;
   }
   /*  store the byte  */
   U8Mask = usart_receive();
   /*  reset and then store the mask  */
   U32USARTBM = 0;
   U32USARTBM = U8Mask;
   /*  shift the mask to the correct byte (MSB first)  */
   U32USARTBM <<= (U8ButtonIndex - 8);
   /*  move to the next byte  */
   U8ButtonIndex -= 8;
   /*  return the button mask  */
   return U32USARTBM;
}
