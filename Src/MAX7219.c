/*
 * MAX7219.c
 *
 *  Created on: Aug 23, 2025
 *      Author: thekh
 */

#include "MAX7219.h"

void MAX7219_init(MAX7219* ld, int num_devices, SPI_HandleTypeDef* hspi1, GPIO_t* CS_pin)
{
	if (ld == NULL || hspi1 == NULL || CS_pin == NULL) return;
	if (num_devices <= 0 || num_devices > 8) num_devices = 8;
	ld->maxDevices = num_devices;
	//set up the SPI and the chip select
    ld->spiHandle = hspi1;
	ld->CS_line = CS_pin;

	// pull the chip select to idle high
	HAL_GPIO_WritePin((ld->CS_line)->prt, (ld->CS_line)->pin, 1);

	// clear the buffer and data in the SPI array
	for (int i = 0; i < 16; i++)
		ld->spiData[i] = 0x00;
	for (int i = 0; i < 64; i++)
		ld->status[i] = 0x00;

	for (int i = 0; i < num_devices; i++)
	{
		// no display test
		set_displayTest(ld, i, 0);
		// no decode
		set_decode(ld, i, 0x00);
		// minimum brightness
		set_intensity(ld, i, 0x00);
		// maximum scan-limit
		set_scanLimit(ld, i, 0x07);
		// wake up the MAX7219 on init
		shutdown(ld, i, 1);
	}
	// default to always refresh
	ld->autoRefresh = 1;
};


void shutdown(MAX7219* ld, int addr, uint8_t mode)
{
	VALIDATE_PARAMS(ld, addr);
	// normalisation to 0 or 1
	mode = (mode != 0x00);
	MAX7219_transmit(ld, addr, MAX7219_REG_SHUTDOWN, mode);
};


void set_decode(MAX7219* ld, int addr, int mode)
{
	VALIDATE_PARAMS(ld, addr);
	if (mode != 0x00 && mode != 0x01 && mode != 0x0F && mode != 0xFF) return;
	MAX7219_transmit(ld, addr, MAX7219_REG_DECODE, mode);
};


void set_intensity(MAX7219* ld, int addr, int intensity)
{
	VALIDATE_PARAMS(ld, addr);
	if (intensity > 0x0F || intensity < 0x00) return;
	MAX7219_transmit(ld, addr, MAX7219_REG_INTENSITY, intensity);
};


void set_scanLimit(MAX7219* ld, int addr, int lim)
{
	VALIDATE_PARAMS(ld, addr);
	if (lim > 0x07 || lim < 0x00) return;
	MAX7219_transmit(ld, addr, MAX7219_REG_SCANLIMIT, lim);
};


void set_displayTest(MAX7219* ld, int addr, uint8_t mode)
{
	VALIDATE_PARAMS(ld, addr);
	// normalisation to 0 or 1
	mode = (mode != 0x00);
	MAX7219_transmit(ld, addr, MAX7219_REG_DISPLAYTEST, mode);
};


void clear(MAX7219* ld, int addr)
{
	VALIDATE_PARAMS(ld, addr);

	uint8_t offset = 8*addr;

	for (uint8_t i = 0; i < 8; i++)
	{
		ld->status[offset + i] = 0x00;
		if (ld->autoRefresh)
			MAX7219_transmit(ld, addr, i + 1, 0x00);
	}
};


void set_led(MAX7219* ld, int addr, int row, int col, uint8_t state)
{
	VALIDATE_PARAMS(ld, addr);
	if (col > 7 || row > 7 || col < 0 || row < 0) return;

	uint8_t offset = 8*addr;

	// bit mask to select the right column
	uint8_t mask = 0x80 >> col;

	if (state)
	{
		// make the led at the correct row and column be 1
		ld->status[offset + row] |= mask;
	} else
	{
		// make the led at the correct row and column be 0
		ld->status[offset + row] &= ~mask;
	}

	if (ld->autoRefresh)
		MAX7219_transmit(ld, addr, row + 1, ld->status[offset + row]);
};


void set_row(MAX7219* ld, int addr, int row, uint8_t state)
{
	VALIDATE_PARAMS(ld, addr);
	if (row > 7 || row < 0) return;

	uint8_t offset = 8*addr;

	ld->status[offset + row] = state;

	if (ld->autoRefresh)
		MAX7219_transmit(ld, addr, row + 1, ld->status[offset + row]);
};


void set_col(MAX7219* ld, int addr, int col, uint8_t state)
{
	VALIDATE_PARAMS(ld, addr);
	if (col > 7 || col < 0) return;

	uint8_t offset = 8*addr;
	// bit mask to select the right column
	uint8_t mask = 0x80 >> col;

	for (int i = 0; i < 8; i++)
	{
		uint8_t tmp = state >> (7 - i);
		tmp &= 0x01;
		if (tmp)
			// make the led at the column be 1
			ld->status[offset + i] |= mask;
		else
			// make the led at the column be 0
			ld->status[offset + i] &= ~mask;
	}

	if (ld->autoRefresh)
		refresh(ld, addr);
};


void MAX7219_transmit(MAX7219* ld, int addr, uint8_t opcode, uint8_t data)
{
	VALIDATE_PARAMS(ld, addr);

	uint8_t maxData = ld->maxDevices*2;
	// sending data is reversed, the first device will receive its data last and vice versa
	uint8_t reverse_offset = maxData - 2*addr - 2;
	// clear the data so the only device we want to talk to receives data, all the other ones will receive NO_OP instead
	for (int i = 0; i < maxData; i++)
		ld->spiData[i] = 0x00;
	// opcode first, data second
	ld->spiData[reverse_offset] = opcode;
	ld->spiData[reverse_offset + 1] = data;

	// pull CS low
	HAL_GPIO_WritePin((ld->CS_line)->prt, (ld->CS_line)->pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(ld->spiHandle, ld->spiData, maxData, 100);
	// latches data
	HAL_GPIO_WritePin((ld->CS_line)->prt, (ld->CS_line)->pin, GPIO_PIN_SET);

};

void refresh(MAX7219* ld, int addr)
{
	VALIDATE_PARAMS(ld, addr);

	uint8_t offset = 8*addr;
	// flush the buffer of a device all at once
	for (int i = 0; i < 8; i++)
		MAX7219_transmit(ld, addr, i + 1, ld->status[offset + i]);
};
