/*
 * MAX7219.c
 *
 *  Created on: Aug 23, 2025
 *      Author: thekh
 */

#include "MAX7219.h"

uint32_t MAX7219_init(MAX7219* ld, SPI_HandleTypeDef* spi)
{

};

void SPI_write(MAX7219* ld, uint8_t opcode, uint8_t data)
{
	// opcode first, data second
	ld->spiData[0] = opcode;
	ld->spiData[1] = data;

	// pull CS low
	HAL_GPIO_WritePin((ld->CS_line)->prt, (ld->CS_line)->pin, 0);
	HAL_SPI_Transmit(ld->spiHandle, ld->spiData, 2, 100);
	// latches data
	HAL_GPIO_WritePin((ld->CS_line)->prt, (ld->CS_line)->pin, 1);

};

void shutdown(MAX7219* ld, uint8_t mode)
{
	SPI_write(ld, MAX7219_REG_SHUTDOWN, mode);
};

void set_decode(MAX7219* ld, uint8_t mode)
{
	SPI_write(ld, MAX7219_REG_DECODE, mode);
};

void set_intensity(MAX7219* ld, uint8_t intensity)
{
	SPI_write(ld, MAX7219_REG_INTENSITY, intensity);
};

void set_scanLimit(MAX7219* ld, uint8_t lim)
{
	SPI_write(ld, MAX7219_REG_SCANLIMIT, lim);
};

void set_displayTest(MAX7219* ld, uint8_t mode)
{
	SPI_write(ld, MAX7219_REG_DISPLAYTEST, mode);
};

void clear(MAX7219* ld)
{
	for (uint8_t i = 1; i <= 8; i++)
		SPI_write(ld, i, 0x00);
};

void set_led(MAX7219* ld, uint8_t row, uint8_t col, uint8_t state)
{
	if (col > 7 || row > 7)
		return;

	uint8_t mask = 0x80 >> col;

	if(state)
	{
		ld->status[row] |= mask;
	} else
	{
		ld->status[row] &= ~mask;
	}

	SPI_write(ld, row + 1, ld->status[row]);
};

void set_row(MAX7219* ld, uint8_t row, uint8_t state)
{
	
};

void set_col(MAX7219* ld, uint8_t col, uint8_t state)
{

};






