/*
 * led13bit.c

 *
 *  Created on: Dec 21, 2014
 *      Author: sid
 */
#include "c_types.h"
#include "osapi.h"

#include "led/apa102.h"

////public

void led13b_init(int amountLeds)
{
	apa102_init();

	apa102_start();
	uint32_t i;
	apa102_t apa = { { 0, 0, 0, 0 } };
	for (i = 0; i < amountLeds; i++)
		apa102_setLed(apa);

	apa102_stop(amountLeds);
}

void led13b_start(void)
{
	apa102_start();
}

void led13b_set32(uint32_t red, uint32_t grn, uint32_t blu)
{
	apa102_t apa102;

	uint16_t r16 = red / 541200;
	uint16_t g16 = grn / 541200;
	uint16_t b16 = blu / 541200;
	//	os_printf("apa r=%d, g=%d, b=%d\n", r16, g16, b16);

	uint16_t pre;
	uint16_t tmp;

	//find smallest global prescaler
	pre = r16 / 256;
	tmp = g16 / 256;
	if (tmp < pre)
		pre = tmp;
	tmp = b16 / 256;
	if (tmp < pre)
		pre = tmp;

	if (pre == 0)
		pre = 1;

	//adjust prescaler to fit brightest color
	while ((r16 / pre > 255 || g16 / pre > 255 || b16 / pre > 255) && pre < 31)
	{
		pre++;
	}
	if (pre)
	{
		r16 /= pre;
		g16 /= pre;
		b16 /= pre;
	}

	//	os_printf("pre=%d, r=%d, g=%d, b=%d\n", pre, r16, g16, b16);

	apa102.global = pre;
	apa102.red = r16;
	apa102.blu = b16;
	apa102.grn = g16;

	apa102_setLed(apa102);

}

void led13b_stop(int amountLed)
{
	apa102_stop(amountLed);
}
