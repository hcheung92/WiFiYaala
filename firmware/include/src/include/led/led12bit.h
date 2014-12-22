/*
 * led12bit.h
 *
 *  Created on: Nov 16, 2014
 *      Author: sid
 */

#ifndef LED12BIT_H_
#define LED12BIT_H_

#define LEDS			3

#define LED0_RED_CH		2
#define LED0_GRN_CH		1
#define LED0_BLU_CH		0
#define LED1_RED_CH		5
#define LED1_GRN_CH		4
#define LED1_BLU_CH		3
#define LED2_RED_CH		8
#define LED2_GRN_CH		7
#define LED2_BLU_CH		6

#define OFFSET_ZERO		0
#define OFFSET_FRST		1365
#define OFFSET_SECD		2730
#define LED12B_PWM_MASK		0x0FFF

#define LED0_RED_OFFSET		OFFSET_ZERO
#define LED0_GRN_OFFSET		OFFSET_FRST
#define LED0_BLU_OFFSET		OFFSET_SECD
#define LED1_RED_OFFSET		OFFSET_FRST
#define LED1_GRN_OFFSET		OFFSET_SECD
#define LED1_BLU_OFFSET		OFFSET_ZERO
#define LED2_RED_OFFSET		OFFSET_SECD
#define LED2_GRN_OFFSET		OFFSET_ZERO
#define LED2_BLU_OFFSET		OFFSET_FRST


typedef struct __attribute__((packed))
{
	rgb16_t color;
	rgb16_t offset;
	rgb8_t channel;
} ledCtl_t;


void led12b_init(void);
void led12b_set32(uint32_t num, uint32_t red, uint32_t grn, uint32_t blu);


#endif /* LED12BIT_H_ */
