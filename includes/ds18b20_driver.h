/*
 * ds18b20_driver.h
 *
 *  Created on: Jul 23, 2019
 *      Author: uic14909
 */

#ifndef INCLUDES_DS18B20_DRIVER_H_
#define INCLUDES_DS18B20_DRIVER_H_

#ifndef __AVR_ATmega32A__
#define __AVR_ATmega32A__
#endif

#endif /* INCLUDES_DS18B20_DRIVER_H_ */

/*  create a structure for selecting specific bits  */
typedef struct
{
   uint8_t bit0:1;
   uint8_t bit1:1;
   uint8_t bit2:1;
   uint8_t bit3:1;
   uint8_t bit4:1;
   uint8_t bit5:1;
   uint8_t bit6:1;
   uint8_t bit7:1;
}U1IOReg;

/*  define temperature sensor commands  */
#define DS18b20_COM_SEARCH_ROM   0xF0
#define DS18b20_COM_READ_ROM     0x33
#define DS18b20_COM_MATCH_ROM    0x55
#define DS18b20_COM_SKIP_ROM     0xCC
#define DS18b20_COM_ALARM_SEARCH 0xEC

/*  define temperature sensor functions  */
#define DS18b20_FUN_CONVERT_T    0x44
#define DS18b20_FUN_WRITE_SP     0x4E
#define DS18b20_FUN_READ_SP      0xBE
#define DS18b20_FUN_COPY_SP      0x48
#define DS18b20_FUN_RECALL_E2    0xB8
#define DS18b20_FUN_READ_PW_SPLY 0xB4

/*  define temperature sensor resolution  */
#define DS18b20_RES_9BITS        0x1F
#define DS18b20_RES_10BITS       0x3F
#define DS18b20_RES_11BITS       0x5F
#define DS18b20_RES_12BITS       0x7F

/*  define temperature sensor status  */
#define DS18b20_TASK_IN_PROGRESS 0
#define DS18b20_TASK_IN_DONE     1

/*  define temperature sensor pin for the 1-wire-bus  */
#define TEMPERATURE_PORTB_2      ((volatile U1IOReg*)_SFR_MEM_ADDR(PORTB))->bit2

/*  define input and output PORTC5  */
#define DS18b20_WRITE            DDRB |= (1 << PB2)
#define DS18b20_READ             DDRB &= ~(1 << PB2)
/*  read = PINx & the PINxn  */
#define DS18b20_PIN_READ         ((PINB & (1 << PB2)) >> 2)

/*  write/read on 1-wire-bus  */
void writeBus(uint8_t U8Reg);
uint8_t readBus(void);

/*  initialize or reset function  */
void ResetTemperatureSensor(void);

/*  store temperature in a vector  */
void storeTemp(uint8_t tempU16TempVector[]);

/*  calculate the temperature  */
void Temperature(void);

/*  convert and write temperature on LCD  */
void LCDTemperature(void);
