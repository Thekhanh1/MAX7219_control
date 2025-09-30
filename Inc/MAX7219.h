/*
 * MAX7219.h
 *
 *  Created on: Aug 23, 2025
 *      Author: thekh
 */

#ifndef INC_MAX7219_H_
#define INC_MAX7219_H_

//change to your MCU
#include "stm32f4xx_hal.h"

// macro to decluter the code
#define VALIDATE_PARAMS(ld, addr) \
    do { \
        if (ld == NULL) return; \
        if (addr < 0 || addr >= ld->maxDevices) return; \
    } while(0)


// register address

#define MAX7219_REG_NO_OP           0x00
#define MAX7219_REG_DIGIT_0         0x01
#define MAX7219_REG_DIGIT_1         0x02
#define MAX7219_REG_DIGIT_2         0x03
#define MAX7219_REG_DIGIT_3         0x04
#define MAX7219_REG_DIGIT_4         0x05
#define MAX7219_REG_DIGIT_5         0x06
#define MAX7219_REG_DIGIT_6         0x07
#define MAX7219_REG_DIGIT_7         0x08
#define MAX7219_REG_DECODE          0x09
#define MAX7219_REG_INTENSITY       0x0A
#define MAX7219_REG_SCANLIMIT       0x0B
#define MAX7219_REG_SHUTDOWN        0x0C
#define MAX7219_REG_DISPLAYTEST     0x0F




// MAX7219 struct for easy handling

typedef struct
{

	GPIO_TypeDef* prt;
	uint16_t pin;

} GPIO_t;

typedef struct
{

	SPI_HandleTypeDef* spiHandle;
	// chip select line
	GPIO_t* CS_line;
	// data array
	uint8_t spiData[16];
	// buffer
	uint8_t status[64];
	// auto use refresh() is enabled by default
	uint8_t autoRefresh;
	// maximum number of devices
	uint8_t maxDevices;

} MAX7219;

/*
 * initialise the MAX7219
 * param:
 * 		ld: pointer to struct
 * 		hspi1: SPI struct from stm32 HAL library
 *		CS_pin: create a GPIO_t struct and pass it in
 *		num_devices: how many devices in the chain
 *
 */

void MAX7219_init(MAX7219* ld, int num_devices, SPI_HandleTypeDef* hspi1, GPIO_t* CS_pin);

/*
 * helper function to communicate with MAX7219
 * param:
 * 		ld: pointer to struct
 *		addr: address of the IC (0-7) (0-7)
 */

void MAX7219_transmit(MAX7219* ld, int addr, uint8_t opcode, uint8_t data);

/*
 * shutdown MAX7219
 * param:
 * 		ld: pointer to struct
 * 		addr: address of the IC (0-7)
 * 		mode: 0x01 for normal
 * 			  0x00 for shutdown
 */

void shutdown(MAX7219* ld, int addr, uint8_t mode);

/*
 * set the decode mode
 * param:
 * 		ld: pointer to struct
 * 		addr: address of the IC (0-7)
 * 		mode: 0x00 for no decode
 * 			  0x01 for code B for 0 only
 * 			  0x0F for code B for 3-0 only
 * 			  0xFF for code B for all
 *
 * note: read datasheet if you want to know what code B is
 */


void set_decode(MAX7219* ld, int addr, int mode);

/*
 * Set the brightness of the display
 * param:
 * 		ld: pointer to struct
 * 		addr: address of the IC (0-7)
 * 		intensity: 0x00 - 0x0F
 */

void set_intensity(MAX7219* ld, int addr, int intensity);

/*
 * Set the number of digits (or rows) to be displayed
 * param:
 * 		ld: pointer to struct
 * 		addr: address of the IC (0-7)
 * 		limit: 0x00 - 0x07
 */

void set_scanLimit(MAX7219* ld, int addr, int lim);

/*
* turn all leds on
* param:
* 		ld: pointer to struct
* 		addr: address of the IC (0-7)
* 		mode: 0 for normal
* 			  1 for all leds on
*/

void set_displayTest(MAX7219* ld, int addr, uint8_t mode);

/*
* clear the buffer
* param:
* 		ld: pointer to struct
* 		addr: address of the IC (0-7)
*/

void clear(MAX7219* ld, int addr);

/*
* set a single led status
* param:
* 		ld: pointer to struct
* 		addr: address of the IC (0-7)
* 		row: row of the led
* 		col: column of the led
* 		state: 0 for off
* 			   1 for on
*/

void set_led(MAX7219* ld, int addr, int row, int col, uint8_t state);

/*
* set row to be a pattern
* param:
* 		ld: pointer to struct
* 		addr: address of the IC (0-7)
* 		row: row of the led
* 		state: 8 bit numer
*/

void set_row(MAX7219* ld, int addr, int row, uint8_t state);

/*
* set column to be a pattern
* param:
* 		ld: pointer to struct
* 		addr: address of the IC (0-7)
* 		col: column of the led
* 		state: 8 bit number
*/

void set_col(MAX7219* ld, int addr, int col, uint8_t state);

/*
* refresh the status[8] buffer
* param:
* 		ld: pointer to struct
* 		addr: address of the IC (0-7)
*/

void refresh(MAX7219* ld, int addr);


#endif /* INC_MAX7219_H_ */
