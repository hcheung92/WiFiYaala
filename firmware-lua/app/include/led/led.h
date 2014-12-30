/*
 * led.h
 *
 *  Created on: Dec 21, 2014
 *      Author: sid
 */

#ifndef LED_H_
#define LED_H_

#define LED_MAX			4
#define LED_INTERVALL_MS	15			//min 2

enum
{
	LED_NONE,
	LED_PCA9685,
	LED_APA102

}LED_TYPE;

typedef union __attribute__((packed))
{
	struct
	{
		uint8_t red;
		uint8_t grn;
		uint8_t blu;
	};
	uint8_t raw[3];
} rgb8_t;

typedef union __attribute__((packed))
{
	struct
	{
		uint16_t red;
		uint16_t grn;
		uint16_t blu;
	};
	uint16_t raw[3];
} rgb16_t;

typedef union __attribute__((packed))
{
	struct
	{
		sint16_t red;
		sint16_t grn;
		sint16_t blu;
	};
	sint16_t raw[3];
} srgb16_t;

typedef union __attribute__((packed))
{
	struct
	{
		uint32_t red;
		uint32_t grn;
		uint32_t blu;
	};
	uint32_t raw[3];
} rgb32_t;

typedef union __attribute__((packed))
{
	struct
	{
		sint32_t red;
		sint32_t grn;
		sint32_t blu;
	};
	sint32_t raw[3];
} srgb32_t;

typedef struct __attribute__((packed))
{
	srgb32_t color;
	srgb32_t colorStep;
	sint32_t steps;
	uint8_t	type:4;
	uint8_t dim:4;
	union __attribute__((packed))
	{
		uint8_t channel;
	} misc;
} led_t;

typedef struct __attribute__((packed))
{
	rgb16_t color;
	rgb16_t offset;
	rgb8_t channel;
} ledCtl_t;


//XXX: 0xffffffff/(80000000/16)=35A
#define US_TO_RTC_TIMER_TICKS(t)          \
    ((t) ?                                   \
     (((t) > 0x35A) ?                   \
      (((t)>>2) * ((APB_CLK_FREQ>>4)/250000) + ((t)&0x3) * ((APB_CLK_FREQ>>4)/1000000))  :    \
      (((t) *(APB_CLK_FREQ>>4)) / 1000000)) :    \
     0)

#define FRC1_ENABLE_TIMER  BIT7

typedef enum {
    DIVDED_BY_1 = 0,
    DIVDED_BY_16 = 4,
    DIVDED_BY_256 = 8,
} TIMER_PREDIVED_MODE;

typedef enum {
    TM_LEVEL_INT = 1,
    TM_EDGE_INT   = 0,
} TIMER_INT_MODE;


void led_deinit(void);
int led_init(int newLeds);
int led_set(uint32_t channel, rgb8_t ledValue, uint32_t ms);
int led_setDim(uint32_t channel, uint8_t value);

int led_setTypeApa102(unsigned int from, unsigned int to);
int led_setTypePca9685(unsigned int ledidx, unsigned int channel);


#endif /* LED_H_ */
