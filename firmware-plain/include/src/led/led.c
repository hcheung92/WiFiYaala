/*
 * led.c
 *
 *  Created on: Dec 21, 2014
 *      Author: sid
 */
#include "os_type.h"
#include "user_interface.h"
#include "espmissingincludes.h"

#include "led/led.h"
#include "led/led12bit.h"
#include "led/led13bit.h"

led_t led[LED_MAX];

os_event_t    procTaskQueue[1];
//int i=0;

void inline led_refresh(void)
{
	uint32_t loop;

	led13b_start();

	for(loop=0; loop<LED_MAX; loop++)
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
			led12b_set32(led[loop].misc.channel, red, grn, blu);
			break;
		case LED_APA102:
			led13b_set32(red, grn, blu);
			break;
		default:
			os_printf("LED%u: unknown hardware\n", (unsigned int)loop);
		}


	}

	led13b_stop(LED_MAX);
}

static void ICACHE_FLASH_ATTR procTask(os_event_t *events)
{
//	if(i>1)
//		os_printf("!");
//	i=0;
	led_refresh();

}

LOCAL void tim1_intr_handler(void)
{
	RTC_CLR_REG_MASK(FRC1_INT_ADDRESS, FRC1_INT_CLR_MASK);
	RTC_REG_WRITE(FRC1_LOAD_ADDRESS, US_TO_RTC_TIMER_TICKS(LED_INTERVALL_MS*1000));
	system_os_post(USER_TASK_PRIO_1, 0, '0');
//	i++;
}


//public

void led_init(void)
{
	led12b_init();
	led13b_init(LED_MAX);

	//can be automated (dynamic)
	led[0].type = LED_PCA9685;
	led[0].dim = 0;
	led[0].misc.channel = 0;
	led[1].type = LED_PCA9685;
	led[1].dim = 0;
	led[1].misc.channel = 1;
	led[2].type = LED_PCA9685;
	led[2].dim = 0;
	led[2].misc.channel = 2;
	led[3].type = LED_APA102;
	led[3].dim = 0;


	system_os_task(procTask, USER_TASK_PRIO_1, procTaskQueue, 1);

	ETS_FRC_TIMER1_INTR_ATTACH(tim1_intr_handler, NULL);
	TM1_EDGE_INT_ENABLE();
	ETS_FRC1_INTR_ENABLE();

	RTC_REG_WRITE(FRC1_CTRL_ADDRESS, DIVDED_BY_16 | FRC1_ENABLE_TIMER | TM_EDGE_INT);
	RTC_REG_WRITE(FRC1_LOAD_ADDRESS, US_TO_RTC_TIMER_TICKS(LED_INTERVALL_MS*1000));

}

void led_set(uint32_t channel, rgb8_t ledValue, uint32_t ms)
{
	if(channel >= LED_MAX)
		return;

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


}

void led_setDim(uint32_t channel, uint8_t value)
{
	if(channel >= LED_MAX)
		return;

	led[channel].dim = value & 0x0F;
}

