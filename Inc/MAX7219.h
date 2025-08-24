/*
 * MAX7219.h
 *
 *  Created on: Aug 23, 2025
 *      Author: thekh
 */

#ifndef INC_MAX7219_H_
#define INC_MAX7219_H_

#include "stm32f4xx_hal.h"

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
	uint8_t spiData[2];
	// buffer
	uint8_t status[8];
	// auto use refresh()
	uint8_t autoRefresh;

} MAX7219;

/*
 * initialise the MAX7219
 * param:
 * 		ld: pointer to struct
 * 		spi: spi struct from stm32
 *
 */

uint32_t MAX7219_init(MAX7219* ld, SPI_HandleTypeDef* spi);

/*
 * helper function to communicate with MAX7219
 * param:
 * 		ld: pointer to struct
 * 		spi: spi struct from stm32
 *
 */

void SPI_write(MAX7219* ld, SPI_HandleTypeDef* spi);

/*
 * shutdown MAX7219
 * param:
 * 		ld: pointer to struct
 * 		mode: 0x01 for normal
 * 			  0x00 for shutdown
 */

void shutdown(MAX7219* ld, uint8_t mode);

/*
 * set the decode mode
 * param:
 * 		ld: pointer to struct
 * 		mode: 0x00 for no decode
 * 			  0x01 for code B for 0 only
 * 			  0x0F for code B for 3-0 only
 * 			  0xFF for code B for all
 *
 * note: read datasheet if you want to know what code B is
 */


void set_decode(MAX7219* ld, uint8_t mode);

/*
 * Set the brightness of the display
 * param:
 * 		ld: pointer to struct
 * 		intensity: 0x00 - 0x0F
 */

void set_intensity(MAX7219* ld, uint8_t intensity);

/*
 * Set the number of digits (or rows) to be displayed
 * param:
 * 		ld: pointer to struct
 * 		limit: 0x00 - 0x07
 */

void set_scanLimit(MAX7219* ld, uint8_t lim);

/*
* turn all leds on
* param:
* 		ld: pointer to struct
* 		mode: 0 for normal
* 			  1 for all leds on
*/

void set_displayTest(MAX7219* ld, uint8_t mode);

/*
* clear the buffer
* param:
* 		ld: pointer to struct
* 		state: 0 for no
* 			   1 for yes
*/

void clear(MAX7219* ld);

/*
* set a single led status
* param:
* 		ld: pointer to struct
* 		row: row of the led
* 		col: column of the led
* 		state: 0 for off
* 			   1 for on
*/

void set_led(MAX7219* ld, uint8_t row, uint8_t col, uint8_t state);

/*
* set row to be on or off
* param:
* 		ld: pointer to struct
* 		row: row of the led
* 		state: 0 for off
* 			   1 for on
*/

void set_row(MAX7219* ld, uint8_t row, uint8_t state);

/*
* set column to be on or off
* param:
* 		ld: pointer to struct
* 		col: column of the led
* 		state: 0 for off
* 			   1 for on
*/

void set_col(MAX7219* ld, uint8_t col, uint8_t state);

/*
* refresh the status[8] buffer
* param:
* 		ld: pointer to struct
*/

void refresh(MAX7219* ld);


#endif /* INC_MAX7219_H_ */
