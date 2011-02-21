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

#include <io.h>
#include <signal.h>
#include "spi.h"
#include "delay.h"
#include "display.h"

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

	/* to save space, since we do it in the interrupt anyway */
	P1DIR |= DISPLAY_SUPPLY;
	P1OUT |= DISPLAY_SUPPLY;

#if 0
	display_puts("MSP430", 0);
	display_puts("123456", 1);
#endif

	display_init(1);
	delay_ms(100);
	display_update(1);


	/* enable falling edge interrupt for BUSY1 of the display, which
	 * becomes low when the display content has been updated */
	P1IE |= DISPLAY_BUSY1;
	P1IES |= DISPLAY_BUSY1;

	/* Timer A: input divider /8, ACLK clock source, UP mode */
	TACTL = ID_3 | TASSEL_1 | MC_3;

	/* enable the TACCR0 compare interrupt */
	TACCTL0 = CCIE;

	/* load TACCR0, starts timer. (32768Hz / 8) * 15 = 61440, so the
	 * timer interrupt fires every 15 seconds */
	TACCR0 = 61440 - 1;

	/* enable global interrupts and enter LPM3 */
	_BIS_SR(LPM3_bits + GIE);

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
	display_putint(total_hours, 1, 0);
	display_init(1);
	display_update(1);
}

interrupt(TIMERA0_VECTOR) timer_a0_isr(void)
{
	if (j == 3) {
		P1DIR = DISPLAY_32K | DISPLAY_SUPPLY | DISPLAY_RESET | SCLK | SDO | CS;
		P1OUT |= DISPLAY_SUPPLY;
		P1SEL |= DISPLAY_32K;
		
		handle_clock();
		j = 0;
	} else {
		j++;
	}
}

/* our DISPLAY_BUSY isr */
interrupt(PORT1_VECTOR) port1_isr(void)
{
	/* turn off display clock */
	P1SEL &= ~(DISPLAY_32K);

	/* switch display supply to input, since pullup/down resistors are
	 * disabled, this is high-Z */
	P1DIR &= ~(DISPLAY_SUPPLY | SCLK | SDO | CS);
	P1OUT &= ~(DISPLAY_SUPPLY | DISPLAY_RESET | DISPLAY_32K);

	/* reset the interrupt flag (needed!) */
	P1IFG = 0;
}

