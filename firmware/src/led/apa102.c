/*
 * apa102.c
 *
 *  Created on: Nov 30, 2014
 *      Author: sid
 */

#include "c_types.h"
#include "osapi.h"

#include "driver/spi_master.h"
#include "led/apa102.h"

void apa102_init(void)
{
	spi_master_init(HSPI);
}

void apa102_start(void)
{
	spi_master_16bit_write(HSPI, 0x00, 0x00);
	spi_master_16bit_write(HSPI, 0x00, 0x00);
}

void apa102_setLed(apa102_t apa)
{
	apa.global |= 0xE0;
	spi_master_16bit_write(HSPI, apa.raw[0], apa.raw[1]);
	spi_master_16bit_write(HSPI, apa.raw[2], apa.raw[3]);
}

void apa102_stop(int amountLeds)
{
	int i = 0;
	for(i=0; i<(amountLeds>>5)+1;i++)
		spi_master_16bit_write(HSPI, 0xFF, 0xFF);

}
