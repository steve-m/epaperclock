/*
 * Motorola F3/MOTOFONE e-paper display / Solomon SSD1621 driver
 *
 * (C) 2011 by Steve Markgraf <steve@steve-m.de>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "spi.h"
#include "display.h"
#include "delay.h"
#include "segments.h"

uint32_t display_buffer[8] = { 0xa0000000, 0xa2000000, 0xa4000000, 0xa6000000,
			       0xe0000000, 0xe2000000, 0xe4000000, 0xe6000000 };

void display_set_seg(uint8_t segment, uint8_t on)
{
	uint8_t bit = (segment & 0x1f);
	uint8_t i = ((segment >> 5) & 0x07);
	uint32_t mask = (uint32_t)1 << bit;

	if (on)
		display_buffer[i] |= mask;
	else
		display_buffer[i] &= ~mask;
}

void display_clear(uint8_t on)
{
	uint8_t i;

	for (i = 0; i < 8; i++) {
		if (on)
			display_buffer[i] |= 0x00ffffff;
		else
			display_buffer[i] &= 0xff000000;
	}
}

void display_update(uint8_t refresh)
{
	uint8_t i;

	spi_write_32(0x821b0000);
	/* if we write 0x84fe6600, refreshing the display takes very long,
	 * so this may be the "Built in programmable frequency divider" */
	spi_write_32(0x841e6600);

	/* 0x86[0,2,4,8] seem to regulate the time needed to set the
	 * display content as well */
	spi_write_32(0x86200000);
	spi_write_32(0x16000000);
	delay_ms(20);

	for (i = 0; i < 8; i++)
		spi_write_32(display_buffer[i]);

	spi_write_32(refresh ? 0x80800000 : 0x80400000);
}

/* TODO: add a inverted parameter (for black letters on white background */
void display_putc(uint8_t line, uint8_t pos, unsigned char c)
{
	uint8_t i, segment;
	uint8_t diff = 0;
	uint16_t font = row1_font[c - 32];
	uint16_t mask = 1;

	for (i = 0; i < 16; i++) {
		if (!line) {		/* first line */
			if (i >= 14)
				break;
			segment = segments_row1[i + (14 * pos)];
		} else {		/* second line */
			if ((i >= 6) && (i < 14)) {
				diff++;
				mask <<= 1;
				continue;
			}
			segment = segments_row2[i + (8 * pos) - diff];
		}

		display_set_seg(segment, (font & mask) ? 0 : 1);
		mask <<= 1;
	}
}

void display_puts(const char *str, uint8_t line)
{
	int i;
	for (i = 0; *str != 0x00; i++)
		display_putc(line, i, *str++);
}

/* print a positive integer (up to 999999) right-aligned in given line */
void display_putint(uint32_t i, uint8_t line, uint8_t space)
{
	uint32_t div = 10;
	uint32_t n;
	uint8_t pos = 5 - space;

	do {
		n = (i % div);
		i -= n;
		n *= 10;
		display_putc(line, pos--, '0' + (n / div));
		div *= 10;
	} while (i);

	/* hack for the clock: fill the unused segments to display e.g. 00:09 */
//	while (pos)
//		display_putc(line, pos--, '0');

}

void display_init(uint8_t update)
{
	P1OUT |= BIT3;
	delay_ms(10);
	P1OUT &= ~BIT3;
	delay_ms(10);
	P1OUT |= BIT3;

	spi_write_32(0xf8000000);
	spi_write_32(0xb8000000);
	delay_ms(20);
	spi_write_32(0x95030000);
	spi_write_32(0xd4db0000);

	display_set_seg(ROW1_COLON, 0);
}

