/*
 * led13bit.h
 *
 *  Created on: Dec 21, 2014
 *      Author: sid
 */

#ifndef LED13BIT_H_
#define LED13BIT_H_

void led13b_init(int amountLeds);
void led13b_start(void);
void led13b_set32(uint32_t red, uint32_t grn, uint32_t blu);
void led13b_stop(int amountLed);



#endif /* LED13BIT_H_ */
