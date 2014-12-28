/*
 * pwm12bit.h
 *
 *  Created on: Nov 15, 2014
 *      Author: sid
 */

#ifndef PWM12BIT_H_
#define PWM12BIT_H_


#define PCA9685_ADDR		0x80

#define PCA9685_SUBADR1 	0x02
#define PCA9685_SUBADR2 	0x03
#define PCA9685_SUBADR3 	0x04

#define PCA9685_MODE1 		0x00
#define 	PCA9685_RESTART		(1<<7)
#define 	PCA9685_EXTCLK		(1<<6)
#define 	PCA9685_AI		(1<<5)
#define 	PCA9685_SLEEP		(1<<4)
#define 	PCA9685_SUB1		(1<<3)
#define 	PCA9685_SUB2		(1<<2)
#define 	PCA9685_SUB3		(1<<1)
#define 	PCA9685_ALLCALL		(1<<0)
#define PCA9685_MODE2 		0x01
#define PCA9685_PRESCALE 	0xFE

#define PCA9685_LED0_ON_L	0x06
#define PCA9685_LED0_ON_H	0x07
#define PCA9685_LED0_OFF_L	0x08
#define PCA9685_LED0_OFF_H	0x09
#define 	PCA9685_PWMOFF		0x10

#define PCA9685_ALLLED_ON_L	0xFA
#define PCA9685_ALLLED_ON_H	0xFB
#define PCA9685_ALLLED_OFF_L	0xFC
#define PCA9685_ALLLED_OFF_H	0xFD

void pca9685_init(void);
bool pca9685_setPWM(uint8_t num, uint16_t on, uint16_t off);
bool pca9685_setOff(uint8_t num);


#endif /* PWM12BIT_H_ */
