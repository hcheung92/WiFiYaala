/*
 * ws28xx.h
 *
 *  Created on: Jan 30, 2015
 *      Author: sid
 */

#ifndef WS28XX_H_
#define WS28XX_H_

//#define WS28XX_MUX PERIPHS_IO_MUX_MTDO_U
//#define WS28XX_GPIO 15
//#define WS28XX_FUNC FUNC_GPIO15

#define WS28XX_FIFO_CNT		((READ_PERI_REG(UART_STATUS(UART1)) & (UART_TXFIFO_CNT<<UART_TXFIFO_CNT_S)) >> UART_TXFIFO_CNT_S & UART_TXFIFO_CNT)

void ws28xx_init(void);
void ws28xx_deinit(void);
void ws28xx_transmitt(led_t *leds, unsigned int total);

void ws28xx_stopPlayout(void);

#endif /* WS28XX_H_ */
