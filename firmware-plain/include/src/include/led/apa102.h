/*
 * apa102.h
 *
 *  Created on: Nov 30, 2014
 *      Author: sid
 */

#ifndef APA102_H_
#define APA102_H_

typedef union __attribute__((packed))
{
	struct
	{
		uint8_t global;
		uint8_t blu;
		uint8_t grn;
		uint8_t red;
	};
	uint8_t raw[4];
} apa102_t;

void apa102_init(void);
void apa102_start(void);
void apa102_setLed(apa102_t apa);
void apa102_stop(int amountLeds);

#endif /* APA102_H_ */


