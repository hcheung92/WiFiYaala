/*
 * pwm12bit.c
 *
 *  Created on: Nov 15, 2014
 *      Author: sid
 */

#include "c_types.h"
#include "osapi.h"

#include "led/pca9685.h"
#include "driver/i2c_master.h"

#include "espmissingincludes.h"

bool read(uint8_t addr, uint8_t *buf, uint8_t length)
{
	i2c_master_start();
	i2c_master_writeByte(PCA9685_ADDR | I2C_WRITE);
	if (i2c_master_getAck())
	{
//		os_printf("addr not ack when tx write cmd \n");
		i2c_master_stop();
		return false;
	}

	i2c_master_writeByte(addr);
	if (i2c_master_getAck())
	{
//		os_printf("data not ack when tx write byte1 \n");
		i2c_master_stop();
		return false;
	}

	i2c_master_start();
	i2c_master_writeByte(PCA9685_ADDR | I2C_READ);
	if (i2c_master_getAck())
	{
//		os_printf("addr not ack when tx read cmd \n");
		i2c_master_stop();
		return false;
	}

	uint8_t i = 0;
	for(i=0; i<length; i++)
	{
		buf[i] = i2c_master_readByte();
		i2c_master_setAck(0);//(i == (length - 1)) ? 1 : 0);
	}

	i2c_master_stop();
	return true;
}

bool write(uint8_t addr, uint8_t *buf, uint8_t length)
{
	i2c_master_start();
	i2c_master_writeByte(PCA9685_ADDR | I2C_WRITE);
	if (i2c_master_getAck())
	{
//		os_printf("addr not ack when tx write cmd \n");
		i2c_master_stop();
		return false;
	}

	i2c_master_writeByte(addr);
	if (i2c_master_getAck())
	{
//		os_printf("data not ack when tx write byte1 \n");
		i2c_master_stop();
		return false;
	}

	uint8_t i;
	for(i=0; i<length; i++)
	{
		i2c_master_writeByte(buf[i]);
		if (i2c_master_getAck())
		{
//			os_printf("data not ack when tx write byte %d\n", i);
			i2c_master_stop();
			return false;
		}
	}
	i2c_master_stop();
	return true;
}



void pca9685_init(void)
{
	i2c_master_gpio_init();

	uint8_t reg = PCA9685_SLEEP;
	if(!write(PCA9685_MODE1, &reg, 1))
	{
		os_printf("pwm1 failed\n");
	}

//	reg = PCA9685_SLEEP | PCA9685_EXTCLK;			// turn on extclk (sticky)
//	if(!write(PCA9685_MODE1, &reg, 1))
//	{
//		os_printf("pwm2 faild\n");
//	}

	reg = 3;
	if(!write(PCA9685_PRESCALE, &reg, 1))
	{
		os_printf("prescaler faild\n");
	}

	reg = PCA9685_AI | PCA9685_ALLCALL;			// auto  increment, disable sleep mode
	if(!write(PCA9685_MODE1, &reg, 1))
	{
		os_printf("pwm3 failed\n");
	}

	//read(PCA9685_PRESCALE, &reg, 1);
	//os_printf("1: %d\n", reg);


}


bool pca9685_setPWM(uint8_t num, uint16_t on, uint16_t off)
{
	uint8_t buf[4];
	buf[0] = on;
	buf[1] = on >> 8;
	buf[2] = off;
	buf[3] = off>>8;
	return write(PCA9685_LED0_ON_L + 4*num, buf, 4);
}

bool pca9685_setOff(uint8_t num)
{
	uint8_t buf[2];
	buf[0] = 0;
	buf[1] = PCA9685_PWMOFF;
	return write(num*4 + PCA9685_LED0_OFF_L, buf, 2);
}
