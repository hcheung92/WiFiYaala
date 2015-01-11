WS28XX Converter
================

It converts a UART signal into a WS2811/WS2812/WS2812B or WS2801 signal on a ATTiny

Input Signal
------------

An exact(!) 2400000 (2M4) baud 7N1 (8N1 also works) forming an inverted WS281X signal needs to be fed in to PB2. Either 3V3 or 5V level will work. Each UART byte represents 3 bit for the WS28XX LEDs. 

Within each byte:
0) The start bit plus bit 0 and 1 form the first WS bit. Note, bit 1 must be 1.
1) Bit 2 to 4 build the second bit. Note, bit 2 must be 0 and bit 4 must be 1.
2) Bits 5,6, and the stop bit form the third bit. Note, bit 5 must be 0.
(0 means low, 1 means high)
In case of 8N1 bit 7 must always be 1.

Use an UART + FIFO or DMA in order to ensure a constant byte stream. If not, you have up to 9uS between two bytes.

If at boot time the PB2 is high the output will support WS281X LEDs. If not, WS2801 can be used.

Output Signal
-------------

In case of WS281x connect the DIN line of the first LED to either PB0 or PB1.
Else, use PB0 and PB1 as clock and data lines, respectively. 

Hardware
--------

A ATTinyX5 @ 25Mhz (external crystal) is required.
