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

/*
 *	MAX7219 struct for easy handling
 */

typedef struct
{
	GPIO_TypeDef* prt;
	uint16_t pin;
} GPIO_t;

typedef struct
{

	SPI_HandleTypeDef* spiHandle;
	GPIO_t* CS_line;
	uint16_t spiData[8];
	uint32_t size;

} MAX7219;

/*
 * init function
 * param:
 * 		ld: pointer to struct
 * 		spi: spi struct from stm32
 *
 */

uint32_t MAX7219_init(MAX7219* ld, SPI_HandleTypeDef* spi);

/*
 * helper function
 * param:
 * 		ld: pointer to struct
 * 		spi: spi struct from stm32
 *
 */

void SPI_write(MAX7219* ld, SPI_HandleTypeDef* spi);

/*
 * shutdown function
 * param:
 * 		ld: pointer to struct
 * 		mode: 1 for normal
 * 			  0 for shutdown
 */

void shutdown(MAX7219* ld);

void set_decode(MAX7219* ld, uint8_t mde);

void set_intensity(MAX7219* ld, uint8_t intensity);

void set_displayTest(MAX7219* ld, uint8_t mde);

void set_scanLimit(MAX7219* ld, uint8_t lim);





























#endif /* INC_MAX7219_H_ */
