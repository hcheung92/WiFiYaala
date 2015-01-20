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
struct __attribute__((packed))
{
	uint8_t pcaWhiteBehav[PCA_LEDS];
	uint8_t apa102Init:1;
	uint8_t pca9685Init:1;
	uint8_t ws281xInit:1;
	uint8_t ws2801Init:1;
} state;

os_event_t    procTaskQueue[1];

void inline led_refresh(void)
{
	uint32_t apastate = 0, pcaCh = 0;
	uint32_t loop;

	for(loop = 0; loop < leds && apastate == 0; loop++)
	{
		apastate += led[loop].steps;
	}

	if(apastate == 0)
		return;				//we are steady. no update required

	apastate = 0;

	for(loop=0; loop<leds; loop++)
	{
		if(led[loop].steps)
		{
			led[loop].color.red += led[loop].colorStep.red;
			led[loop].color.grn += led[loop].colorStep.grn;
			led[loop].color.blu += led[loop].colorStep.blu;
			led[loop].steps--;
		}

		uint32_t red = led[loop].color.red;
		uint32_t grn = led[loop].color.grn;
		uint32_t blu = led[loop].color.blu;
		uint32_t white = 0;

		if(led[loop].type == LED_PCA9685)
		{
			if(state.pcaWhiteBehav[pcaCh] != WHITE_NA && state.pcaWhiteBehav[pcaCh] != WHITE_EXTRA)
			{
				white = min(red, min(grn, blu));

				if(state.pcaWhiteBehav[pcaCh] == WHITE_ADJUST)
				{
					white = min(0x2AAAA000, white);
					red -= white;
					grn -= white;
					blu -= white;
					white*= 3;
				}
			}
		}

		/* Gamma correction. Gamma = 2.0 */
		red = ((uint32_t)(((uint16_t)(red>>15)) * ((uint16_t)(red>>15)))) >> led[loop].dim;
		grn = ((uint32_t)(((uint16_t)(grn>>15)) * ((uint16_t)(grn>>15)))) >> led[loop].dim;
		blu = ((uint32_t)(((uint16_t)(blu>>15)) * ((uint16_t)(blu>>15)))) >> led[loop].dim;

		switch(led[loop].type)
		{
		case LED_PCA9685:
			pca9685_set32(pcaCh, red, grn, blu);
			if(pcaCh < PCA_LEDS && (state.pcaWhiteBehav[pcaCh] == WHITE_ADD || state.pcaWhiteBehav[pcaCh] == WHITE_ADJUST))
				pca9685_setWhite32(pcaCh, ((uint32_t)(((uint16_t)(white>>15)) * ((uint16_t)(white>>15)))) >> led[loop].dim);
			else if(pcaCh >= PCA_LEDS && pcaCh < (PCA_LEDS<<1) && state.pcaWhiteBehav[pcaCh - PCA_LEDS] == WHITE_EXTRA)
				pca9685_setWhite32(pcaCh - PCA_LEDS, red);
			pcaCh++;
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
		apa102_stop(leds);
}

static void ICACHE_FLASH_ATTR procTask(os_event_t *events)
{
	if(leds)
		led_refresh();

}

LOCAL void ICACHE_FLASH_ATTR tim1_intr_handler(void)
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
    
	if(state.apa102Init)
	{
		//todo apa102_deinit();
		state.apa102Init = 0;
	}
	if(state.pca9685Init)
	{
		pca9685_deinit();
		state.pca9685Init = 0;
	}
	if(state.ws2801Init)
	{
		//TODO
		state.ws2801Init = 0;
	}
	if(state.ws281xInit)
	{
		//todo
		state.ws281xInit = 0;
	}

}

int led_init(int newLeds)
{
	if(leds || led != NULL)
		led_deinit();

	led = os_malloc(sizeof(led_t) * newLeds);
	if(led == NULL)
		return 0;
	leds = newLeds;

	state.apa102Init = 0;
	state.pca9685Init = 0;
	state.ws281xInit = 0;
	state.ws2801Init = 0;

	os_memset(led, 0, sizeof(led_t)*leds);

	system_os_task(procTask, USER_TASK_PRIO_1, procTaskQueue, 1);

	ETS_FRC_TIMER1_INTR_ATTACH(tim1_intr_handler, NULL);
	TM1_EDGE_INT_ENABLE();
	ETS_FRC1_INTR_ENABLE();

	RTC_REG_WRITE(FRC1_CTRL_ADDRESS, DIVDED_BY_16 | FRC1_ENABLE_TIMER | TM_EDGE_INT);
	RTC_REG_WRITE(FRC1_LOAD_ADDRESS, US_TO_RTC_TIMER_TICKS(LED_INTERVALL_MS*1000));

	return leds;
}

int led_setType(uint8_t type, unsigned int from, unsigned int to)
{
	if(to >= leds || from >= to)
		return 0;

	type &= 0x0F;

	unsigned int i;
	for(i=from; i<=to; i++)
	{
		led[i].type = type;
	}

	if(type == LED_PCA9685 && state.pca9685Init == 0)
	{
		pca9685_init();
		state.pca9685Init = 1;
	}
	else if(type == LED_APA102 && state.apa102Init == 0)
	{
		apa102_init(leds);
		state.apa102Init = 1;
	}
	else if(type == LED_WS2801 && state.ws2801Init == 0 && state.ws281xInit == 0)
	{
		//TODO INIT
		state.ws2801Init = 1;
	}
	else if(type == LED_WS281X && state.ws281xInit == 0 && state.ws2801Init == 0)
	{
		//TODO INIT
		state.ws281xInit = 1;
	}

	return to-from+1;
}

void led_setWhiteBehaviour(uint8_t ch0, uint8_t ch1, uint8_t ch2, uint8_t ch3)
{
	state.pcaWhiteBehav[0] = ch0;
	state.pcaWhiteBehav[1] = ch1;
	state.pcaWhiteBehav[2] = ch2;
	state.pcaWhiteBehav[3] = ch3;
}

int led_set(uint32_t channel, rgb8_t ledValue, uint32_t ms)
{
	if(channel >= leds)
		return 0;

	sint32_t targetRed = ledValue.red<<23 | 0x00400000;
	sint32_t targetGrn = ledValue.grn<<23 | 0x00400000;
	sint32_t targetBlu = ledValue.blu<<23 | 0x00400000;

	led[channel].steps =  ms / LED_INTERVALL_MS;
	if(led[channel].steps < 2)				//set immediately
	{
		led[channel].color.red = targetRed;
		led[channel].colorStep.red = 0;
		led[channel].color.grn = targetGrn;
		led[channel].colorStep.grn = 0;
		led[channel].color.blu = targetBlu;
		led[channel].colorStep.blu = 0;
		led[channel].steps = 1;
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
	if(channel >= leds)
		return 0;

	led[channel].dim = value & 0x0F;
	return 1;
}

