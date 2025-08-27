/*
 * MAX7219.c
 *
 *  Created on: Aug 23, 2025
 *      Author: thekh
 */

#include "MAX7219.h"

void MAX7219_init(MAX7219* ld)
{
	// pull the chip select to idle high
	HAL_GPIO_WritePin((ld->CS_line)->prt, (ld->CS_line)->pin, 1);
	ld->spiData[0] = 0;
	ld->spiData[1] = 0;
	// clear the buffer
	for (int i = 0; i < 8; i++)
		ld->status[i] = 0x00;

	// no display test
	set_displayTest(ld, 0);
	// no decode
	set_decode(ld, 0x00);
	// minimum brightness
	set_intensity(ld, 0x00);
	// maximum scan-limit
	set_scanLimit(ld, 0x07);
	// enter shutdown mode on startup
	shutdown(ld, 0);
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

	for (uint8_t i = 0; i < 8; i++)
	{
		ld->status[i] = 0x00;
		if (ld->autoRefresh)
			SPI_write(ld, i + 1, 0x00);
	}
};


void set_led(MAX7219* ld, uint8_t row, uint8_t col, uint8_t state)
{
	if (col > 7 || row > 7)
		return;

	// bit mask to select the right column
	uint8_t mask = 0x80 >> col;

	if (state)
	{
		// make the led at the correct row and column be 1
		ld->status[row] |= mask;
	} else
	{
		// make the led at the correct row and column be 0
		ld->status[row] &= ~mask;
	}

	if (ld->autoRefresh)
		SPI_write(ld, row + 1, ld->status[row]);
};


void set_row(MAX7219* ld, uint8_t row, uint8_t state)
{
	if (row > 7) return;

	if (state)
	{
		ld->status[row] = 0xFF;
	} else
	{
		ld->status[row] = 0x00;
	}

	if (ld->autoRefresh)
		SPI_write(ld, row + 1, ld->status[row]);
};


void set_col(MAX7219* ld, uint8_t col, uint8_t state)
{
	if (col > 7) return;
	// bit mask to select the right column
	uint8_t mask = 0x80 >> col;

	for (int i = 0; i < 8; i++)
	{
		if (state)
		{
			// make the led at the correct row and column be 1
			ld->status[i] |= mask;
		} else
		{
			// make the led at the correct row and column be 0
			ld->status[i] &= ~mask;
		}

		if (ld->autoRefresh)
			SPI_write(ld, i + 1, ld->status[i]);
	}
};


void SPI_write(MAX7219* ld, uint8_t opcode, uint8_t data)
{
	// opcode first, data second
	ld->spiData[0] = opcode;
	ld->spiData[1] = data;

	// pull CS low
	HAL_GPIO_WritePin((ld->CS_line)->prt, (ld->CS_line)->pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(ld->spiHandle, ld->spiData, 2, 100);
	// latches data
	HAL_GPIO_WritePin((ld->CS_line)->prt, (ld->CS_line)->pin, GPIO_PIN_SET);

};


void refresh(MAX7219* ld)
{
	// flush the buffer all at once
	for (int i = 0; i < 8; i++)
		SPI_write(ld, i + 1, ld->status[i]);
}




