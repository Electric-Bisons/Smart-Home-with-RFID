/*
 * main.c
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
#include "ds18b20_driver.h"
#include "hc_sr501_driver.h"
#include "l298n_driver.h"
#include "lcd_driver.h"
#include "mq2_driver.h"
#include "smart_home_driver.h"
#include "timers_driver.h"
#include "tsop4836_driver.h"
#include "usart_driver.h"

/*  0/1 - unlocked/locked  */
extern uint8_t U8PassFlag;
/*  AC flag  */
extern uint8_t U8ACFlag;
/*  IR Remote Control bit function flag  */
extern volatile uint8_t U8BitFlag;
/*  LCD location register  */
extern volatile uint8_t U8CCP;
/*  unlocked house function register  */
extern int IFunctionFlag;
/*  DutyCycle register  */
extern volatile int IDutyCycle;
/*  global temperature register  */
extern uint16_t U16GlobalTemp;
/*  Remote Control pressed button mask  */
extern volatile uint32_t U32Button;
/*  temporary Remote Control mask  */
extern volatile uint32_t U32ButtonMask;
/*  gas value  */
double tempD64RS_RO_value;

/*  0/1 - stop/start ADC conversion  */
volatile uint8_t U8ADCflag = 0;
/*  rfid mask  */
volatile uint8_t U8RFIDMask = 0;
/*  register to count the 5 seconds temperature actualization  */
volatile uint16_t U16TimerCount = 0;

/* Connections:
 * LCD: PORTC <7:0> - data and PORTB <1:0> - RS and E
 * DS18B20: PORTB <2> - 1-wire-bus
 * MQ2: PORTA <0> - AO
 * HC-SR501: PORTA <6:5> - DO and buzzer output
 * L298N: PORTD <7> - PWM on ENA, PORTA <4> - IN1 and GND - IN2
 * TSOP4836 - PORTD <2> - OUT
 * LED ON/OFF - PORTA <7> - LED
 * LED RED/GREEEN - PORTA<2:3>
 * */

/*  after every button press the U32Button needs to be reseted  */

int main(void)
{
   /*  initialize the sensors and functions  */
   sei();
   initLCD();
   LCDloading();
   lockHouse();
   Timer0();
   Int0Interrupt();
   hc_sr501_init();
   motor_control_init();
   fastPWM2();
   ADCinput();
   usart_init();
   Temperature();

   /*  RFID leds red/green  */
   DDRA |= (1 << PA2) | (1 << PA3);
   PORTA |= (1 << PA2) | (1 << PA3);

   while(1)
   {
      /*  make a new ADC conversion to check for gas  */
      U8ADCflag = 1;
      /*  reset the LCD position every cycle  */
      U8CCP = 0x80;
      /*  !!!check for high temperature and gas leak!!! works every cycle  */
      if((U16GlobalTemp >= 300) || (tempD64RS_RO_value < 1.6))
      {
         if(tempD64RS_RO_value < 1.6)
         {
            _delay_ms(500);
            clearDisplay();
            U8CCP = 0x80;
            writeWordOnLCD("WARNING         GAS LEAK");
            /*  turn on the buzzer  */
            PORTA |= (1 << PA5);
         }
         motor_control_start();
         IDutyCycle = 100;
      }
      /*  stop the AC automatically only if AC menu is not on and AC is not turned on by user  */
      if((U16GlobalTemp < 300) && (tempD64RS_RO_value > 1.6) && (IFunctionFlag != 2) && (U8ACFlag == 0))
      {
         motor_control_stop();
         /*  turn off buzzer  */
         PORTA &= ~(1 << PA5);
      }
      /*  lock the house  */
      if(U32Button == FOLDER_MINUS)
      {
         U32Button = 0;
         lockHouse();
      }
      /*  unlock house only if house is already locked  */
      if((U32Button == FOLDER_PLUS) && (U8PassFlag == 0))
      {
         U32Button = 0;
         passwordUnlock();
      }
      /*  functions while house is locked  */
      /*if(U8PassFlag == 0)
      {
           if a person is detected
         PersonDetection();
      }*/
      /*  functions while house is unlocked  */
      if(U8PassFlag == 1)
      {
         /*  detect if any there is a function change  */
         changeFunction();
         /*  get inside an if for the current function number  */
         if(IFunctionFlag == 0)
         {
            /*  welcome menu  */
         }
         if(IFunctionFlag == 1)
         {
            /*  temperature menu  */
         }
         if(IFunctionFlag == 2)
         {
            /*  control the AC  */
            function_AC();
         }
         if(IFunctionFlag == 3)
         {
            /*  turn on/off the light  */
            function_LED();
         }
      }
   }
}

ISR(USART_TXC_vect)
{
   //interrupt just to make the code work :)
}

ISR(USART_RXC_vect)
{
   /*  reset the mask  */
   U8RFIDMask = usart_receive();
   /*  store the rfid flag from usart in a register  */
   if( (U8PassFlag == 0) && (U8RFIDMask == 0xFF) )
   {
      unlockHouse();
   }
   if( (U8PassFlag == 0) && (U8RFIDMask == 0x0F) )
   {
      writeWordOnLCD("ACCES DENIED");
      /*  turn on red LED  */
      PORTA &= ~(1 << PA2);
      _delay_ms(1000);
      /*  turn off red LED  */
      PORTA |= (1 << PA2);
      lockHouse();
   }
   /*  reset the mask  */
   U8RFIDMask = 0;
}

ISR(TIMER0_OVF_vect)
{
   /*  increment the time register  */
   U16TimerCount++;
   /*  every 5 seconds  */
   if(U16TimerCount > 312)
   {
      /*  make a new temperature check  */
      Temperature();
      if((U8PassFlag == 1) && (IFunctionFlag == 1))
      {
         LCDTemperature();
      }
      /*  reset the register  */
      U16TimerCount = 0;
   }
   /*  in case of OVF of the timer reset the Remote Control function  */
   U8BitFlag = 0;
}

/*  get through the steps of the Remote Control Read  */
ISR(INT0_vect)
{
   if(U8BitFlag == 0)
   {
      first_bit();
   }
   if(U8BitFlag == 1)
   {
      button_sequence();
   }
   if(U8BitFlag == 2)
   {
      /*  store the button mask in a global variable  */
      U32Button = U32ButtonMask;
      last_bit();
   }
}

ISR(TIMER2_OVF_vect)
{
   /*  convert the DutyCycle value as a value for the PWM  */
   OCR2 = IDutyCycle * 255 / 100;
}

ISR(ADC_vect)
{
   /*  if ADCflag store a new gas value  */
   if(U8ADCflag)
   {
      RS_RO_function();
      /*  set flag to 0  */
      U8ADCflag = 0;
   }
   /*  start a new conversion  */
   ADCSRA |= (1 << ADSC);
}
