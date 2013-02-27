/*
 * MSP430 e-paper clock
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
#include "delay.h"
#include "display.h"
#define NODATA
#include "segments.h"

/* PORT1 defines */
#define DISPLAY_32K	BIT0
#define DISPLAY_SUPPLY	BIT1
#define DISPLAY_BUSY1	BIT2
#define DISPLAY_RESET	BIT3

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;

	/* configure P2.6 and P2.7 as XIN/XOUT function pins */
	P2DIR &= ~BIT6;
	P2DIR |= BIT7;
	P2SEL |= BIT7 | BIT6;

	/* 32kHz external crystal, XCAP = 12.5pF, clear LFXT1 osc fault */
	BCSCTL3 |= XCAP0 | XCAP1;
	BCSCTL3 &= ~(LFXT1S0 | LFXT1S1);

	/* enable ACLK function of P1.0 (32KHz clock output) */
	P1DIR |= BIT0;
	P1SEL |= BIT0;

	spi_init();

	P1DIR = DISPLAY_32K | DISPLAY_SUPPLY | DISPLAY_RESET | SCLK | SDO | CS | BIT7;
	P1OUT |= DISPLAY_SUPPLY | BIT7;
	P1SEL |= DISPLAY_32K;

#if 0
	display_puts("MSP430", 0);
	display_puts("123456", 1);
#endif

	display_init(1);
	delay_ms(100);
	display_clear(1);
	display_update(1);

	/* enable falling edge interrupt for BUSY1 of the display, which
	 * becomes low when the display content has been updated */
//	P1IE |= DISPLAY_BUSY1;
//	P1IES |= DISPLAY_BUSY1;

	/* Timer A: input divider ACLK clock source, UP mode */
	TACTL = TASSEL_1 | MC_1| ID_1;

	/* enable the TACCR0 compare interrupt */
	TACCTL0 = CCIE;

	/* load TACCR0, starts timer. */
	TACCR0 = 0xffff;

	/* enable TA0.CCI1A */
	P1DIR &= ~BIT2;
	P1SEL |= BIT2;

	/* enable the TACCR1 capture interrupt, capture on rising+falling edge */
	TACCTL1 = CCIE | CAP | CM_3 | SCS;		// SCS to synchronize with timerclock

	/* enable global interrupts */
	_BIS_SR(GIE);

	while (1) { };
}

/* global variables, used in the interrupt routines */
volatile unsigned int minute = 0;
volatile unsigned int hour = 0;
volatile unsigned int total_hours = 0;
volatile uint8_t j = 0;

void handle_clock(void)
{
	if (minute == 59) {
		minute = 0;
		if (hour == 23)
			hour = 0;
		else
			hour++;
		total_hours++;
	} else {
		minute++;
	}

	display_clear(1);
	display_putint((hour * 100) + minute, 0, 1);
//	display_putint(total_hours, 1, 0);
//	display_init(1);
	display_update(0);
}

#ifdef __GNUC__
void __attribute__((interrupt (TIMER0_A0_VECTOR))) timer_a0_isr(void)
#else
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
#endif
{
//	handle_clock();
}

volatile uint64_t dcf;
volatile uint8_t dcf_index = 0;

uint8_t calc_parity(uint32_t data)
{
	uint8_t parity = 0, i = 32;

	while (i--)
		parity ^= (data >> i) & 1;

	return parity;
}

uint8_t parse_dcf(void)
{
	uint8_t min, hour, day, day_of_week, month, year;
	uint8_t error = 0, parity;

	display_clear(1);

	/* check for startbit */
	if (!(dcf & (1ULL << 20)))
		error = 1;

	/** timezone **/

	/** minute **/
	min = (dcf >> 21) & 0xff;
	parity = min & 0x80;
	min &= 0x7f;

	if (calc_parity(min) > parity)
		error = 1;
	else
		display_set_seg(NETW_LEVEL_1, 0);

	/* convert BCD to integer */
	min = (((min & 0x70) >> 4) * 10) + (min & 0x0f);

	/** hour **/
	hour = (dcf >> 29) & 0x7f;
	parity = hour & 0x40;
	hour &= 0x3f;

	if (calc_parity(hour) > parity)
		error = 1;
	else
		display_set_seg(NETW_LEVEL_2, 0);

	hour = (((hour & 0x30) >> 4) * 10) + (hour & 0x0f);

	/** day **/
	day = (dcf >> 36) & 0x3f;
	day = (((day & 0x30) >> 4) * 10) + (day & 0x0f);

	day_of_week = (dcf >> 42) & 0x07;

	/** month **/
	month = (dcf >> 45) & 0x1f;
	month = (((month & 0x10) >> 4) * 10) + (month & 0x0f);

	/** year **/
	year = (dcf >> 50) & 0xff;
	year = (((year & 0xf0) >> 4) * 10) + (year & 0x0f);

	parity = (dcf >> 58) & 1;
	if (calc_parity((dcf >> 36) & 0x3fff) > parity)
		error = 1;
	else
		display_set_seg(NETW_LEVEL_3, 0);

	/* check symbol if parity is okay */
	if (!error)
		display_set_seg(ICON_CHECKED, 0);

	display_putint(hour, 0, 4);
	display_putint(min, 0, 2);
	display_putint(0, 0, 0);
	display_putint(0, 0, 1);

	display_putint(day, 1, 4);
	display_putint(month, 1, 2);
	display_putint(year, 1, 0);
	display_update(0);

	return parity;
}

volatile uint16_t gottime = 0;

#ifdef __GNUC__
void __attribute__((interrupt (TIMER0_A1_VECTOR))) timer_a1_isr(void)
#else
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A0_ISR (void)
#endif
{
	uint16_t value;
	/* disable interrupt, clear interrupt flag */
	TACCTL1 &= ~(CCIE | CCIFG);
	value = TACCR1;
	TACCR1 = 0;

	/* rising edge */
	if ((TACCTL1 & CCI)) {
		TAR = 0;
		if ((value > 20000) ) {
			if (dcf_index < 59)
				dcf <<= (59 - dcf_index);

			dcf_index = 0;
			parse_dcf();
			gottime = 1;
		} else {
			display_set_seg(ICON_CLOCK , 0);
			display_update(0);
		}
	} else if (value > 1400) {
		if (value > 2400) {
			dcf |= (1ULL << dcf_index);
			display_set_seg(ROW2_DOWNARROW, 0);
		} else {
			dcf &= ~(1ULL << dcf_index);
			display_set_seg(ROW2_DOWNARROW, 1);
		}

		dcf_index++;
		display_putint(dcf_index, 0, 0);
		if (!gottime)
			display_putint(value, 1, 0);
		else
			display_set_seg(ROW2_SLASHES, 0);

		display_set_seg(ICON_CLOCK , 1);

		display_update(0);
	}

	/* enable interrupt again */
	TACCTL1 |= CCIE;
}

/* our DISPLAY_BUSY isr */
#ifdef __GNUC__
void __attribute__((interrupt (PORT1_VECTOR))) port1_isr(void)
#else
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR (void)
#endif
{
	/* turn off display clock */
//	P1SEL &= ~(DISPLAY_32K);

	/* switch display supply to input, since pullup/down resistors are
	 * disabled, this is high-Z */
//	P1DIR &= ~(DISPLAY_SUPPLY | SCLK | SDO | CS);
//	P1OUT &= ~(DISPLAY_SUPPLY | DISPLAY_RESET | DISPLAY_32K);

	/* reset the interrupt flag (needed!) */
	P1IFG = 0;
}

