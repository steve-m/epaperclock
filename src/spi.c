#include <msp430.h>
#include <stdint.h>
#include "spi.h"

void spi_init(void)
{
	P1DIR |= SCLK | SDO | CS | RESET;

	/* configure USI for SPI */
	USICTL0 = USIPE6 | USIPE5 | USIMST | USIOE | USISWRST | USIGE;

	/* set the USI clock source and divider (DIV = SMCLK/1) */
	USICKCTL = USIDIV_0 | USISSEL_2;

	/* set clock phase */
	USICTL1 |= USICKPH;

	/* release the USI from reset */
	USICTL0 &= ~USISWRST;
}

void spi_write(uint16_t c)
{
	USISR = c;
	/* clear the interrupt flag */
	USICTL1 &= ~USIIFG;

	/* set 16-bit mode and number of bits to send, this will start TX */
	USICNT = USI16B | 16;
	
	/* poll the interrupt flag */
	while(!(USICTL1 & USIIFG));
}

void spi_write_32(uint32_t c)
{
	P1OUT &= ~CS;
	spi_write((c >> 16) & 0xffff);
	spi_write(c & 0xffff);
	P1OUT |= CS;
}

