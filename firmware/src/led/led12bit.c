/*
 * led12bit.c
 *
 *  Created on: Nov 16, 2014
 *      Author: sid
 */

#include "c_types.h"
#include "espmissingincludes.h"

#include "led/led.h"
#include "led/led12bit.h"
#include "led/pca9685.h"



//config
ledCtl_t led12b[LEDS] = {
		{{{0,0,0}}, {{LED0_RED_OFFSET, LED0_GRN_OFFSET, LED0_BLU_OFFSET}}, {{LED0_RED_CH, LED0_GRN_CH, LED0_BLU_CH}}},		//LED0
		{{{0,0,0}}, {{LED1_RED_OFFSET, LED1_GRN_OFFSET, LED1_BLU_OFFSET}}, {{LED1_RED_CH, LED1_GRN_CH, LED1_BLU_CH}}},		//LED1
		{{{0,0,0}}, {{LED2_RED_OFFSET, LED2_GRN_OFFSET, LED2_BLU_OFFSET}}, {{LED2_RED_CH, LED2_GRN_CH, LED2_BLU_CH}}},		//LED2
};

void ledUpdate(ledCtl_t *l)
{
	uint8_t i;
	for(i=0; i<3; i++)
	{
		if(! l->color.raw[i])
		{
			if(!pca9685_setOff(l->channel.raw[i]))
			{
				os_printf("setOff %d failed\n", l->channel.raw[i]);
			}
		}
		else
		{
			if( l->color.raw[i] > LED12B_PWM_MASK)
				 l->color.raw[i] = LED12B_PWM_MASK;
			if(!pca9685_setPWM(l->channel.raw[i], l->offset.raw[i], ( l->color.raw[i] + l->offset.raw[i]) & LED12B_PWM_MASK))
			{
				os_printf("setPWM %d,%d,%d failed\n", l->channel.raw[i], l->offset.raw[i], ( l->color.raw[i] + l->offset.raw[i]) & LED12B_PWM_MASK);
			}
		}
	}
}



////public

void led12b_init(void)
{
	pca9685_init();

	uint8_t i=0;
	for(i=0; i<16; i++)
	{
		if(!pca9685_setOff(i))
		{
			os_printf("set off %d failed\n", i);
		}
	}
}

void led12b_set32(uint32_t num, uint32_t red, uint32_t grn, uint32_t blu)
{
	led12b[num].color.red = red>>20;
	led12b[num].color.grn = grn>>20;
	led12b[num].color.blu = blu>>20;
	ledUpdate(&led12b[num]);
}

