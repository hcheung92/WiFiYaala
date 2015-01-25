/*
 * main.c
 *
 *  Created on: Jan 6, 2015
 *      Author: sid
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "flexport.h"

#define IN		B,2
#define DATA 		B,1
#define CLK		B,0

#define SELECT		B,4
#define SELECTpcint	PCINT4



ISR(PCINT0_vect)
{
	wdt_enable(WDTO_15MS);			//reset
	while(1);
}

int main(void)
{
	INPUT(IN);
	CLR(IN);

	INPUT(SELECT);
	CLR(SELECT);

	wdt_reset();
	/* Clear WDRF in MCUSR */
	MCUSR = 0x00;
	/* Write logical one to WDCE and WDE */
	WDTCR |= (1<<WDCE) | (1<<WDE);
	/* Turn off WDT */
	WDTCR = 0x00;

	OUTPUT(DATA);
	OUTPUT(CLK);
	CLR(DATA);
	CLR(CLK);

	GIMSK = 1<<PCIE;
	PCMSK = 1<<SELECTpcint;
	GIFR = 1<<PCIF;			//clear pin change interrupt
	sei();

	if(GET(SELECT))
	{
	//ws281x inverter
	asm volatile (	"ldi r16,3			; reg for PB0,PB1 clk,data high"			"\n\t"
			"ldi r17,0			; reg for PB0,PB1 clk,data low"				"\n\t"

			"1:"											"\n\t"
			"sbic 22,2			; skip next line if PB2 is clr (1noskip 2else)"		"\n\t"
			"rjmp 1b			; jump to start	(2)"					"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"

			"out 24,r16			; set, half wave delayed (1)" 				"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"

			"sbic 22,2			; skip next line if PD2 is clr (1noskip 2else)"		"\n\t"
			"out 24,r17			; clr - faster than cbi, reduces jitter (1)"		"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"

			"sbic 22,2			; skip next line if PD2 is clr (1noskip 2else)"		"\n\t"
			"out 24,r17			; clr - alway for vaild signal (1)"			"\n\t"

			"rjmp 1b			; jump to start	(2)"					"\n\t"


			::);
	}
	else
	{
	//ws281X to ws2801
	asm volatile (	"ldi r16,2			; reg for PB0 data hi"					"\n\t"

			"1:"											"\n\t"
			"sbic 22,2			; skip next line if PB2 is clr (1noskip 2else)"		"\n\t"
			"rjmp 1b			; jump to start	(2)"					"\n\t"

			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"cbi 24,0			; clr PB0 (2 not1!)"					"\n\t"
			"cbi 24,1			; clr PB1 (2 not1!)"					"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"

			"sbis 22,2			; skip next line if PD2 is set (1noskip 2else)"		"\n\t"
			//"sbi 24,1			; set PB1 (data) (2 not1!)"				"\n\t"
			"out 24,r16			; set PB1 - faster than sbi, reduces jitter (1)"	"\n\t"

			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"
			"nop				; (1)"							"\n\t"

			"sbic 22,2			; skip next line if PD2 still clr (1no skip 2else)"	"\n\t"
			"sbi 24,0			; set PB0 (clk) (2 not1!)"				"\n\t"

			//optional: reducing upper nops by 1 (still works) give us a little more time to lower CLK here: "nop" + "out 22,r17" (requires: ldr r17,0 in advance)

			"rjmp 1b			; jump to start (2)"					"\n\t"
			::);
	}

	return 0;
}
