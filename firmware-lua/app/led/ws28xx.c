/*
 * ws28xx.c
 *
 *  Created on: Jan 30, 2015
 *      Author: sid
 */

#include "c_types.h"
#include "osapi.h"
#include "espmissingincludes.h"
#include "driver/uart.h"

#include "led/led.h"
#include "ws28xx.h"

led_t *ws28xx_led = NULL;
unsigned int leds_total;
unsigned int current_led;

// UartDev is defined and initialized in rom code.
extern UartDevice UartDev;

/*uart_tx_one_char(uint8 uart, uint8 TxChar)
{
    while (true)
    {
      uint32 fifo_cnt = READ_PERI_REG(UART_STATUS(uart)) & (UART_TXFIFO_CNT<<UART_TXFIFO_CNT_S);
      if ((fifo_cnt >> UART_TXFIFO_CNT_S & UART_TXFIFO_CNT) < 126) {
        break;
      }
    }

    WRITE_PERI_REG(UART_FIFO(uart) , TxChar);
    return OK;
}
*/

LOCAL void inline ICACHE_RAM_ATTR fill_fifo(void)
{
	if(WS28XX_FIFO_CNT >= 126 && !current_led)
	{
		//we were too slow
		ws28xx_led = NULL;

		//disable tx_interrupt
		CLEAR_PERI_REG_MASK(UART_INT_ENA(UART1), UART_TXFIFO_EMPTY_INT_ENA);

		return;
	}

	while(WS28XX_FIFO_CNT < 119)	//126-7 (more)
	{
		if(ws28xx_led[current_led].type == LED_WS2801)
		{									//FIXME
			// full green
			uart_tx_one_char(1, 0x12);	//111
			uart_tx_one_char(1, 0x12);	//111
			uart_tx_one_char(1, 0x52);	//110
			uart_tx_one_char(1, 0x5b);	//000
			uart_tx_one_char(1, 0x5b);	//000
			uart_tx_one_char(1, 0x5b);	//000
			uart_tx_one_char(1, 0x5b);	//000
			uart_tx_one_char(1, 0x5b);	//000
		}
		else if(ws28xx_led[current_led].type == LED_WS281X)
		{
			// full green
			uart_tx_one_char(1, 0x12);	//111
			uart_tx_one_char(1, 0x12);	//111
			uart_tx_one_char(1, 0x52);	//110
			uart_tx_one_char(1, 0x5b);	//000
			uart_tx_one_char(1, 0x5b);	//000
			uart_tx_one_char(1, 0x5b);	//000
			uart_tx_one_char(1, 0x5b);	//000
			uart_tx_one_char(1, 0x5b);	//000

		}

		if(++current_led >= leds_total)
		{
			//we are done
			ws28xx_led = NULL;

			//disable tx_interrupt
			CLEAR_PERI_REG_MASK(UART_INT_ENA(UART1), UART_TXFIFO_EMPTY_INT_ENA);
			return;
		}
	}


}

LOCAL void ICACHE_RAM_ATTR intr_handler(void *para)
{
	    /* uart0 and uart1 intr combine together, when interrupt occur, see reg 0x3ff20020, bit2, bit0 represents
	     * uart1 and uart0 respectively
	     */

	if(UART_TXFIFO_EMPTY_INT_ST == (READ_PERI_REG(UART_INT_ST(UART1)) & UART_TXFIFO_EMPTY_INT_ST))
	{

//		fill_fifo();

		//clr int
//		WRITE_PERI_REG(UART_INT_CLR(UART1), UART_TXFIFO_EMPTY_INT_CLR);
	}

	if(READ_PERI_REG(UART_INT_ST(UART0)))	//any UART0 stuff
	{
		uart0_rx_intr_handler(&(UartDev.rcv_buff));
	}
}

void ws28xx_init(uint8_t type)
{
	PIN_FUNC_SELECT(WS28XX_MUX, WS28XX_FUNC);

	if(type == LED_WS281X)
		gpio_output_set(1<<WS28XX_GPIO, 0, 1<<WS28XX_GPIO, 0);
	else if(type == LED_WS2801)
		gpio_output_set(0, 1<<WS28XX_GPIO, 1<<WS28XX_GPIO, 0);
	else
		return;

	//todo: give the device 25ms to sattle i.e. delay timer


	ETS_UART_INTR_DISABLE();

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_U1TXD_BK);

	uart_div_modify(1, UART_CLK_FREQ / (2400000));

	//7n1
	WRITE_PERI_REG(UART_CONF0(UART1), (ONE_STOP_BIT << UART_STOP_BIT_NUM_S) | (SEVEN_BITS << UART_BIT_NUM_S));

	//clear rx and tx fifo,not ready
	SET_PERI_REG_MASK(UART_CONF0(UART1), UART_RXFIFO_RST | UART_TXFIFO_RST);
	CLEAR_PERI_REG_MASK(UART_CONF0(UART1), UART_RXFIFO_RST | UART_TXFIFO_RST);

	//set tx fifo trigger
//	WRITE_PERI_REG(UART_CONF1(UART1), 54 << UART_TXFIFO_EMPTY_THRHD_S); //gives us 200uS to reakt
//fixme seems so activate fifo int??
	//clear all interrupt
	WRITE_PERI_REG(UART_INT_CLR(UART1), 0xffff);

	//overload uart int handler
	ETS_UART_INTR_ATTACH(intr_handler,  NULL);

	ETS_UART_INTR_ENABLE();
}

void ws28xx_deinit(void)
{
	gpio_output_set(0, 0, 0, 1<<WS28XX_GPIO);
	CLEAR_PERI_REG_MASK(UART_INT_ENA(UART1), UART_TXFIFO_EMPTY_INT_ENA);
}

void ws28xx_transmitt(led_t *leds, unsigned int total)
{
	if(ws28xx_led != NULL)
	{
		//not quite done here
		return;
	}

	ws28xx_led = leds;
	leds_total = total;
	current_led = 0;

	//enable tx_interrupt
//	SET_PERI_REG_MASK(UART_INT_ENA(1), UART_TXFIFO_EMPTY_INT_ENA);

}
