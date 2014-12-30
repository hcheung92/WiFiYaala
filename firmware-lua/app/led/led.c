/*
 * led.c
 *
 *  Created on: Dec 21, 2014
 *      Author: sid
 */
#include "os_type.h"
#include "user_interface.h"
#include "espmissingincludes.h"
#include "mem.h"

#include "led/led.h"
#include "pca9685.h"
#include "apa102.h"

led_t *led = NULL;
unsigned int leds = 0;

os_event_t    procTaskQueue[1];
//int i=0;

void inline led_refresh(void)
{
	uint32_t apastate =0;
	uint32_t loop;



	for(loop=0; loop<leds; loop++)
	{
		if(led[loop].steps)
		{
			led[loop].color.red += led[loop].colorStep.red;
			led[loop].color.grn += led[loop].colorStep.grn;
			led[loop].color.blu += led[loop].colorStep.blu;
			led[loop].steps--;
		}

		/* Gamma correction. Gamma = 2.0 */
		uint32_t red = ((uint32_t)(((uint16_t)(led[loop].color.red>>15)) * ((uint16_t)(led[loop].color.red>>15)))) >> led[loop].dim;
		uint32_t grn = ((uint32_t)(((uint16_t)(led[loop].color.grn>>15)) * ((uint16_t)(led[loop].color.grn>>15)))) >> led[loop].dim;
		uint32_t blu = ((uint32_t)(((uint16_t)(led[loop].color.blu>>15)) * ((uint16_t)(led[loop].color.blu>>15)))) >> led[loop].dim;

		switch(led[loop].type)
		{
		case LED_PCA9685:
			pca9685_set32(led[loop].misc.channel, red, grn, blu);
			break;
		case LED_APA102:
			if(!apastate)
			{
				apa102_start();
				apastate++;
			}
			apa102_set32(red, grn, blu);
			break;
		//default:
			//os_printf("LED%u: unknown hardware\n", (unsigned int)loop);
		}


	}

	if(apastate)
		apa102_stop(LED_MAX);
}

static void ICACHE_FLASH_ATTR procTask(os_event_t *events)
{
	if(leds)
		led_refresh();

}

LOCAL void tim1_intr_handler(void)
{
	RTC_CLR_REG_MASK(FRC1_INT_ADDRESS, FRC1_INT_CLR_MASK);
	if(leds)
	{
		RTC_REG_WRITE(FRC1_LOAD_ADDRESS, US_TO_RTC_TIMER_TICKS(LED_INTERVALL_MS*1000));
		system_os_post(USER_TASK_PRIO_1, 0, '0');
	}
}


//public

void led_deinit(void)
{
	leds = 0;
	if(led != NULL)
		os_free(led);

}

int led_init(int newLeds)
{
	if(leds || led != NULL)
		led_deinit();

	led = os_malloc(sizeof(led_t) * newLeds);
	if(led == NULL)
		return 0;
	leds = newLeds;

	pca9685_init();
	apa102_init(leds);

	os_memset(led, 0, sizeof(led_t)*leds);

	system_os_task(procTask, USER_TASK_PRIO_1, procTaskQueue, 1);

	ETS_FRC_TIMER1_INTR_ATTACH(tim1_intr_handler, NULL);
	TM1_EDGE_INT_ENABLE();
	ETS_FRC1_INTR_ENABLE();

	RTC_REG_WRITE(FRC1_CTRL_ADDRESS, DIVDED_BY_16 | FRC1_ENABLE_TIMER | TM_EDGE_INT);
	RTC_REG_WRITE(FRC1_LOAD_ADDRESS, US_TO_RTC_TIMER_TICKS(LED_INTERVALL_MS*1000));

	return leds;
}

int led_setTypeApa102(unsigned int from, unsigned int to)
{
	if(to >= leds || from >= leds)
		return 0;

	unsigned int i;
	for(i=from; i<=to; i++)
	{
		led[i].type = LED_APA102;
	}

	return to-from+1;
}

int led_setTypePca9685(unsigned int ledidx, unsigned int channel)
{
	if(ledidx >= leds)
		return 0;

	led[ledidx].type = LED_PCA9685;
	led[ledidx].misc.channel = channel;

	return 1;
}


int led_set(uint32_t channel, rgb8_t ledValue, uint32_t ms)
{
	if(channel >= LED_MAX)
		return 0;

	sint32_t targetRed = ledValue.red<<23 | 0x00400000;
	sint32_t targetGrn = ledValue.grn<<23 | 0x00400000;
	sint32_t targetBlu = ledValue.blu<<23 | 0x00400000;

	led[channel].steps =  ms / LED_INTERVALL_MS;
	if(led[channel].steps < 2)				//set immediately
	{
		led[channel].color.red = targetRed;
		led[channel].color.grn = targetGrn;
		led[channel].color.blu = targetBlu;
		led[channel].steps = 0;
	}
	else
	{
		led[channel].colorStep.red  = ((sint32_t)(targetRed - led[channel].color.red)) / led[channel].steps;
		led[channel].colorStep.grn  = ((sint32_t)(targetGrn - led[channel].color.grn)) / led[channel].steps;
		led[channel].colorStep.blu  = ((sint32_t)(targetBlu - led[channel].color.blu)) / led[channel].steps;

		//os_printf("led%d: stepCol=%d target=%d col=%d\n", (int)channel, (int)led[channel].colorStep.red,(int)targetRed, (int)led[channel].color.red);
	}

	return 1;

}

int led_setDim(uint32_t channel, uint8_t value)
{
	if(channel >= LED_MAX)
		return 0;

	led[channel].dim = value & 0x0F;
	return 1;
}

