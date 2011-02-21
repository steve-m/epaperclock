/* Motorola F3/MOTOFONE e-paper display */
/* the low-level stuff */
#ifndef SEGMENTS_H
#define SEGMENTS_H

/* The SPI transfers to the display controller are 32 bits long
 * and have the following encoding:
 *
 * AAAAAAAA DDDDDDDD DDDDDDDD DDDDDDDD
 * Address  Data2    Data1    Data0
 *
 * There are 8 addresses pointing to the actual display content, see below.
 *
 * We are storing the bit->segment mapping in one byte per segment,
 * looking like this:
 *
 * 765 43210	Bit number
 * AAA BBBBB
 * ||| |||||---- first five bits encode the number of the bit belonging to the
 *  |            segment on that address (out of 24 bits available per address)
 *  |----------- three bits encode the address the segment is mapped to.
 */
				/* First 3 bits:	Address: */
#define ADDR_A0		0x00	/* 000			0xa0 */
#define ADDR_A2		0x20	/* 001			0xa2 */
#define ADDR_A4		0x40	/* 010			0xa4 */
#define ADDR_A6		0x60	/* 011			0xa6 */
#define ADDR_E0		0x80	/* 100			0xe0 */
#define ADDR_E2		0xa0	/* 101			0xe2 */
#define ADDR_E4		0xc0	/* 110			0xe4 */
#define ADDR_E6		0xe0	/* 111			0xe6 */

/* The 6 digits in the first text row have 14 segments:
 *
 *	|--------A--------|		|--------A--------|
 *	|\       |       /|		|\       |       /|
 *	| \      |      / |		| \      |      / |
 *	|  \     |     /  |		|  \     |     /  |
 *	F   G    H    J   B		F   G    G    G   B
 *	|    \   |   /    |		|    \   |   /    |
 *	|     \  |  /     |		|     \  |  /     |
 *	|      \ | /      |		|      \ | /      |
 *	|       \|/       |		|       \|/       |
 *	|---P----|----K---|		|---H----|----H---|
 *	|       /|\       |		|       /|\       |
 *	|      / | \      |		|      / | \      |
 *	|     /  |  \     |		|     /  |  \     |
 *	|    /   |   \    |		|    /   |   \    |
 *	E   N    M    L   C		E   G    G    G   C
 *	|  /     |     \  |		|  /     |     \  |
 *	| /      |      \ |		| /      |      \ |
 *	|/       |       \|		|/       |       \|
 *	|--------D--------|		|--------D--------|
 *	first row			second row
 *
 *
 * The 6 digits in the second row only have 8 segments, since GHJNML are
 * connected to the same segment output, and P+K are joined together as well.
 */

/*
 * We use this array to store the font data, which is encoded in 16 bits each.
 * Bit 0-13 are used for the segments A-P of the 14-segment digits, and bit
 * 14 and 15 are used for compatibility with the 8-segment digits, indicating
 * the combined G and H segments (only if possible).
 */


/* ASCII font, starting at 0x20 */
const uint16_t row1_font[] = {	/* ZYPNMLKJHGFEDCBA	digit:	*/
#if 0
	0x0000,			/* 0000000000000000	space	*/
	0x0030,			/* 0000000000110000	!	*/
	0x0082,			/* 0000000010000010	"	*/
	0x2a8e,			/* 0010101010001110	#	*/
	0xaaad,			/* 1010101010101101	$	*/
	0x1124,			/* 0001000100100100	%	*/
	0x2559,			/* 0010010101011001	&	*/
	0x0080,			/* 0000000010000000	'	*/
	0x0039,			/* 0000000000111001	(	*/
	0x000f,			/* 0000000000001111	)	*/
	0x7fc0,			/* 1111111111000000	*	*/
	0xaa80,			/* 1010101010000000	+	*/
	0x1000,			/* 0001000000000000	,	*/
	0xa200,			/* 1010001000000000	-	*/
	0x0400,			/* 0000010000000000	.	*/
	0x1100,			/* 0001000100000000	/	*/
#endif
	0x003f,			/* 0000000000111111	0	*/
	0x0006,			/* 0000000000000110	1	*/
	0xa21b,			/* 1010001000011011	2	*/
	0xa20f,			/* 1010001000001111	3	*/
	0xa226,			/* 1010001000100110	4	*/
	0xa22d,			/* 1010001000101101	5	*/
	0xa23d,			/* 1010001000111101	6	*/
	0x0007,			/* 0000000000000111	7	*/
	0xa23f,			/* 1010001000111111	8	*/
	0xa22f,			/* 1010001000101111	9	*/
#if 0
	0x0880,			/* 0000100010000000	:	*/
	0x1080,			/* 0001000010000000	;	*/
	0x5000,			/* 0000010100000000	<	*/
	0xa208,			/* 1010001000001000	=	*/
	0x1040,			/* 0001000001000000	>	*/
	0x0a23,			/* 0000101000100011	?	*/
	0x82bb,			/* 1000001010111011	@	*/
	0xa237,			/* 1010001000110111	A	*/
	0x0a8f,			/* 0000101010001111	B	*/
	0x0039,			/* 0000000000111001	C	*/
	0x088f,			/* 0000100010001111	D	*/
	0xa239,			/* 1010001000111001	E	*/
	0xa231,			/* 1010001000110001	F	*/
	0x823d,			/* 1000001000111101	G	*/
	0xa236,			/* 1010001000110110	H	*/
	0x0889,			/* 0000100010001001	I	*/
	0x001f,			/* 0000000000011111	J	*/
	0x2530,			/* 0010010100110000	K	*/
	0x0038,			/* 0000000000111000	L	*/
	0x0176,			/* 0000000101110110	M	*/
	0x0476,			/* 0000010001110110	N	*/
	0x003f,			/* 0000000000111111	O	*/
	0xa233,			/* 1010001000110011	P	*/
	0x043f,			/* 0000010000111111	Q	*/
	0xa633,			/* 1010011000110011	R	*/
	0xa24d,			/* 1010001001001101	S	*/
	0x0881,			/* 0000100010000001	T	*/
	0x003e,			/* 0000000000111110	U	*/
	0x1130,			/* 0001000100110000	V	*/
	0x1436,			/* 0001010000110110	W	*/
	0x1540,			/* 0001010101000000	X	*/
	0x0940,			/* 0000100101000000	Y	*/
	0x1109,			/* 0001000100001001	Z	*/
	0x0039,			/* 0000000000111001	[	*/
	0x0440,			/* 0000010001000000	\	*/
	0x000f,			/* 0000000000001111	]	*/
	0x0023,			/* 0000000000100011	^	*/
	0x0008,			/* 0000000000001000	_	*/
#endif
};

/*
 * Text Row 1
 */

/* Character 1 */
#define R1C1_SEG_A	(ADDR_A6 +  3)
#define R1C1_SEG_B	(ADDR_A4 + 21)
#define R1C1_SEG_C	(ADDR_A4 +  8)
#define R1C1_SEG_D	(ADDR_A4 + 16)
#define R1C1_SEG_E	(ADDR_A4 + 17)
#define R1C1_SEG_F	(ADDR_A4 + 22)
#define R1C1_SEG_G	(ADDR_A4 + 23)
#define R1C1_SEG_H	(ADDR_A6 +  2)
#define R1C1_SEG_J	(ADDR_A6 +  1)
#define R1C1_SEG_K	(ADDR_A4 + 11)
#define R1C1_SEG_L	(ADDR_A4 +  7)
#define R1C1_SEG_M	(ADDR_A4 +  6)
#define R1C1_SEG_N	(ADDR_A4 + 13)
#define R1C1_SEG_P	(ADDR_A4 + 15)

/* Character 2 */
#define R1C2_SEG_A	(ADDR_A6 +  8)
#define R1C2_SEG_B	(ADDR_A2 +  5)
#define R1C2_SEG_C	(ADDR_A2 +  7)
#define R1C2_SEG_D	(ADDR_A2 + 19)
#define R1C2_SEG_E	(ADDR_A6 + 20)
#define R1C2_SEG_F	(ADDR_A6 + 12)
#define R1C2_SEG_G	(ADDR_A4 +  2)
#define R1C2_SEG_H	(ADDR_A4 +  0)
#define R1C2_SEG_J	(ADDR_A0 + 17)
#define R1C2_SEG_K	(ADDR_A0 + 21)
#define R1C2_SEG_L	(ADDR_A2 +  1)
#define R1C2_SEG_M	(ADDR_A2 + 18)
#define R1C2_SEG_N	(ADDR_A2 + 22)
#define R1C2_SEG_P	(ADDR_A2 + 21)

/* Character 3 */
#define R1C3_SEG_A	(ADDR_A6 +  0)
#define R1C3_SEG_B	(ADDR_A0 + 12)
#define R1C3_SEG_C	(ADDR_A0 + 10)
#define R1C3_SEG_D	(ADDR_A0 + 15)
#define R1C3_SEG_E	(ADDR_A2 + 16)
#define R1C3_SEG_F	(ADDR_A2 +  9)
#define R1C3_SEG_G	(ADDR_A2 + 12)
#define R1C3_SEG_H	(ADDR_A0 + 11)
#define R1C3_SEG_J	(ADDR_A0 + 14)
#define R1C3_SEG_K	(ADDR_A2 +  4)
#define R1C3_SEG_L	(ADDR_A2 +  3)
#define R1C3_SEG_M	(ADDR_A2 +  0)
#define R1C3_SEG_N	(ADDR_A2 +  8)
#define R1C3_SEG_P	(ADDR_A2 + 10)

/* Character 4 */
#define R1C4_SEG_A	(ADDR_E6 +  7)
#define R1C4_SEG_B	(ADDR_E4 + 17)
#define R1C4_SEG_C	(ADDR_E4 + 18)
#define R1C4_SEG_D	(ADDR_E6 +  2)
#define R1C4_SEG_E	(ADDR_E0 +  2)
#define R1C4_SEG_F	(ADDR_A0 +  3)
#define R1C4_SEG_G	(ADDR_E6 + 11)
#define R1C4_SEG_H	(ADDR_E6 +  6)
#define R1C4_SEG_J	(ADDR_E6 +  5)
#define R1C4_SEG_K	(ADDR_E4 + 23)
#define R1C4_SEG_L	(ADDR_E4 + 19)
#define R1C4_SEG_M	(ADDR_E6 +  3)
#define R1C4_SEG_N	(ADDR_E6 + 18)
#define R1C4_SEG_P	(ADDR_E0 +  3)

/* Character 5 */
#define R1C5_SEG_A	(ADDR_E0 + 13)
#define R1C5_SEG_B	(ADDR_E2 + 21)
#define R1C5_SEG_C	(ADDR_E2 + 22)
#define R1C5_SEG_D	(ADDR_E4 +  0)
#define R1C5_SEG_E	(ADDR_E4 +  8)
#define R1C5_SEG_F	(ADDR_E4 +  4)
#define R1C5_SEG_G	(ADDR_E0 +  1)
#define R1C5_SEG_H	(ADDR_E0 +  9)
#define R1C5_SEG_J	(ADDR_E0 +  5)
#define R1C5_SEG_K	(ADDR_E6 + 20)
#define R1C5_SEG_L	(ADDR_E6 + 16)
#define R1C5_SEG_M	(ADDR_E4 + 20)
#define R1C5_SEG_N	(ADDR_E4 + 16)
#define R1C5_SEG_P	(ADDR_E6 + 12)

/* Character 6 */
#define R1C6_SEG_A	(ADDR_E0 + 18)
#define R1C6_SEG_B	(ADDR_E0 + 19)
#define R1C6_SEG_C	(ADDR_E2 + 16)
#define R1C6_SEG_D	(ADDR_E2 + 17)
#define R1C6_SEG_E	(ADDR_E2 + 12)
#define R1C6_SEG_F	(ADDR_E2 +  0)
#define R1C6_SEG_G	(ADDR_E0 + 22)
#define R1C6_SEG_H	(ADDR_E0 + 20)
#define R1C6_SEG_J	(ADDR_E0 + 23)
#define R1C6_SEG_K	(ADDR_E2 +  3)
#define R1C6_SEG_L	(ADDR_E2 +  8)
#define R1C6_SEG_M	(ADDR_E2 +  6)
#define R1C6_SEG_N	(ADDR_E2 + 10)
#define R1C6_SEG_P	(ADDR_E2 +  2)

/*
 * Text Row 2
 */

/* Character 1 */
#define R2C1_SEG_A	(ADDR_A4 + 20)
#define R2C1_SEG_B	(ADDR_A6 +  4)
#define R2C1_SEG_C	(ADDR_A4 +  1)
#define R2C1_SEG_D	(ADDR_A6 + 16)
#define R2C1_SEG_E	(ADDR_A4 +  5)
#define R2C1_SEG_F	(ADDR_A4 +  4)
#define R2C1_SEG_G	(ADDR_A4 + 12)
#define R2C1_SEG_H	(ADDR_A4 + 14)

/* Character 2 */
#define R2C2_SEG_A	(ADDR_A0 + 13)
#define R2C2_SEG_B	(ADDR_A2 + 13)
#define R2C2_SEG_C	(ADDR_A2 + 17)
#define R2C2_SEG_D	(ADDR_A2 + 15)
#define R2C2_SEG_E	(ADDR_A0 +  1)
#define R2C2_SEG_F	(ADDR_A2 + 23)
#define R2C2_SEG_G	(ADDR_A2 + 11)
#define R2C2_SEG_H	(ADDR_A0 +  5)

/* Character 3 */
#define R2C3_SEG_A	(ADDR_A0 + 16)
#define R2C3_SEG_B	(ADDR_A0 +  7)
#define R2C3_SEG_C	(ADDR_A0 +  4)
#define R2C3_SEG_D	(ADDR_A0 +  6)
#define R2C3_SEG_E	(ADDR_A0 + 23)
#define R2C3_SEG_F	(ADDR_A2 +  2)
#define R2C3_SEG_G	(ADDR_A0 + 19)
#define R2C3_SEG_H	(ADDR_A0 + 18)

/* Character 4 */
#define R2C4_SEG_A	(ADDR_E6 + 14)
#define R2C4_SEG_B	(ADDR_E4 + 22)
#define R2C4_SEG_C	(ADDR_E4 + 21)
#define R2C4_SEG_D	(ADDR_E6 + 10)
#define R2C4_SEG_E	(ADDR_E6 + 15)
#define R2C4_SEG_F	(ADDR_E6 + 17)
#define R2C4_SEG_G	(ADDR_E6 +  1)
#define R2C4_SEG_H	(ADDR_E6 + 13)

/* Character 5 */
#define R2C5_SEG_A	(ADDR_E4 +  2)
#define R2C5_SEG_B	(ADDR_E2 + 23)
#define R2C5_SEG_C	(ADDR_E4 +  1)
#define R2C5_SEG_D	(ADDR_E6 +  4)
#define R2C5_SEG_E	(ADDR_E4 + 12)
#define R2C5_SEG_F	(ADDR_E4 + 14)
#define R2C5_SEG_G	(ADDR_E6 +  0)
#define R2C5_SEG_H	(ADDR_E6 +  8)

/* Character 6 */
#define R2C6_SEG_A	(ADDR_E2 + 13)
#define R2C6_SEG_B	(ADDR_E2 + 15)
#define R2C6_SEG_C	(ADDR_E2 + 11)
#define R2C6_SEG_D	(ADDR_E2 +  1)
#define R2C6_SEG_E	(ADDR_E2 + 18)
#define R2C6_SEG_F	(ADDR_E0 + 21)
#define R2C6_SEG_G	(ADDR_E2 +  9)
#define R2C6_SEG_H	(ADDR_E2 +  7)

/* Symbols / Icons */

#define BACKGROUND_SEG	(ADDR_A0 +  0)

/*
 * Battery/Network status indicators
 */

#define NETW_ANTENNA_ICON	(ADDR_A6 + 15)
#define NETW_LEVEL_1		(ADDR_A6 + 14)
#define NETW_LEVEL_2		(ADDR_A6 + 17)
#define NETW_LEVEL_3		(ADDR_A6 + 19)
#define NETW_LEVEL_4		(ADDR_A6 + 18)

#define BATTERY_ICON		(ADDR_E4 + 11)
#define BATTERY_LEVEL_1		(ADDR_E0 +  8)
#define BATTERY_LEVEL_2		(ADDR_E0 + 11)
#define BATTERY_LEVEL_3		(ADDR_E0 + 10)
#define BATTERY_LEVEL_4		(ADDR_E0 + 12)

/* Icons at the top */
#define ICON_123		(ADDR_A6 +  6)
#define ICON_ABC		(ADDR_A6 +  9)
#define ICON_PHONE_TOP		(ADDR_A6 +  7)
#define ICON_BEACON		(ADDR_A6 + 13)
#define ICON_HOME		(ADDR_A6 + 10)
#define ICON_MUTE		(ADDR_A6 + 11)
#define ICON_X			(ADDR_E0 + 14)
#define ICON_VOICEMAIL		(ADDR_E0 + 15)
#define ICON_VOICEM_BORDER	(ADDR_E0 + 16)
#define ICON_CLOCK_TOP		(ADDR_E4 +  5)
#define ICON_BELL		(ADDR_E4 +  7)
#define ICON_RING_BELL		(ADDR_E4 + 13)
#define ICON_OKAY		(ADDR_E4 + 10)
#define ICON_LOCK		(ADDR_E4 + 15)

/* Segments which belong to the first text row */
#define ROW1_COLON	(ADDR_A0 +  8)
#define ROW1_LARROW	(ADDR_A4 + 19)	/* arrow left of first row */
#define ROW1_RARROW	(ADDR_E2 +  4)	/* arrow right of first row */
#define ROW1_UPARROW	(ADDR_A6 +  5)	/* arrow above first row */

/* Segments which belong to the second text row */
#define ROW2_DOWNARROW	(ADDR_E6 + 19)
#define ROW2_SLASHES	(ADDR_A2 +  6)	/* the two slashes in row 2 */
#define ICON_CURRENCY	(ADDR_E0 + 17)	/* money icon with border around row2 */
#define ROW2_DOT	(ADDR_E4 +  9)
#define ROW2_BACKGROUND	(ADDR_E2 + 20)	/* background segment of the
					 * last 2 digits */

/* Icons on the bottom */
#define ICON_SMS		(ADDR_A2 + 20)
#define ICON_PHONE_BOTTOM	(ADDR_A2 + 14)
#define ICON_PHONE_LARROW	(ADDR_A0 + 20)	/* left arrow in phone symbol */
#define ICON_PHONE_RARROW	(ADDR_A0 + 22)	/* right arrow in phone symb. */
#define ICON_PHONEBOOK		(ADDR_E2 + 19)
#define ICON_BOOK_ARROW		(ADDR_E2 +  5)	/* arrow in the phonebook */
#define ICON_SETTINGS		(ADDR_E4 +  3)
#define ICON_CLOCK		(ADDR_E4 +  6)
#define ICON_BELL_LARGE		(ADDR_E6 +  9)
#define ICON_CHECKED		(ADDR_A4 +  9)
#define ICON_SPEAKER		(ADDR_A0 +  8)
#define ICON_TEXT_LARGE		(ADDR_A4 + 10)
#define ICON_TRASH		(ADDR_A0 +  2)
#define ICON_LARROW		(ADDR_A4 + 18)	/* arrow left of icon row */
#define ICON_RARROW		(ADDR_E2 + 14)	/* arrow right of icon row */

/* Unused segment outputs, just for completeness */
#define UNUSED_SEG0	(ADDR_A6 + 21)
#define UNUSED_SEG1	(ADDR_A6 + 22)
#define UNUSED_SEG2	(ADDR_A6 + 23)
#define UNUSED_SEG3	(ADDR_E0 +  0)
#define UNUSED_SEG4	(ADDR_E0 +  4)
#define UNUSED_SEG5	(ADDR_E0 +  6)
#define UNUSED_SEG6	(ADDR_E0 +  7)
#define UNUSED_SEG7	(ADDR_E6 + 21)
#define UNUSED_SEG8	(ADDR_E6 + 22)
#define UNUSED_SEG9	(ADDR_E6 + 23)

/* We store the segments in two arrays, one for each line.
 * This ends up automatically in flash on the MSP430.
 * If somebody wants to port that to a microcontroller with an Harvard-
 * architecture, like the AVR, one should store this in flash (e.g. using the
 * PROGMEM-macro in avr-gcc), otherwise this will bloat the SRAM */

const uint8_t segments_row1[] = {
	R1C1_SEG_A, R1C1_SEG_B, R1C1_SEG_C, R1C1_SEG_D, R1C1_SEG_E,
	R1C1_SEG_F, R1C1_SEG_G, R1C1_SEG_H, R1C1_SEG_J, R1C1_SEG_K,
	R1C1_SEG_L, R1C1_SEG_M, R1C1_SEG_N, R1C1_SEG_P,
	R1C2_SEG_A, R1C2_SEG_B, R1C2_SEG_C, R1C2_SEG_D, R1C2_SEG_E,
	R1C2_SEG_F, R1C2_SEG_G, R1C2_SEG_H, R1C2_SEG_J, R1C2_SEG_K,
	R1C2_SEG_L, R1C2_SEG_M, R1C2_SEG_N, R1C2_SEG_P,
	R1C3_SEG_A, R1C3_SEG_B, R1C3_SEG_C, R1C3_SEG_D, R1C3_SEG_E,
	R1C3_SEG_F, R1C3_SEG_G, R1C3_SEG_H, R1C3_SEG_J, R1C3_SEG_K,
	R1C3_SEG_L, R1C3_SEG_M, R1C3_SEG_N, R1C3_SEG_P,
	R1C4_SEG_A, R1C4_SEG_B, R1C4_SEG_C, R1C4_SEG_D, R1C4_SEG_E,
	R1C4_SEG_F, R1C4_SEG_G, R1C4_SEG_H, R1C4_SEG_J, R1C4_SEG_K,
	R1C4_SEG_L, R1C4_SEG_M, R1C4_SEG_N, R1C4_SEG_P,
	R1C5_SEG_A, R1C5_SEG_B, R1C5_SEG_C, R1C5_SEG_D, R1C5_SEG_E,
	R1C5_SEG_F, R1C5_SEG_G, R1C5_SEG_H, R1C5_SEG_J, R1C5_SEG_K,
	R1C5_SEG_L, R1C5_SEG_M, R1C5_SEG_N, R1C5_SEG_P,
	R1C6_SEG_A, R1C6_SEG_B, R1C6_SEG_C, R1C6_SEG_D, R1C6_SEG_E,
	R1C6_SEG_F, R1C6_SEG_G, R1C6_SEG_H, R1C6_SEG_J, R1C6_SEG_K,
	R1C6_SEG_L, R1C6_SEG_M, R1C6_SEG_N, R1C6_SEG_P,
};

const uint8_t segments_row2[] = {
	R2C1_SEG_A, R2C1_SEG_B, R2C1_SEG_C, R2C1_SEG_D,
	R2C1_SEG_E, R2C1_SEG_F, R2C1_SEG_G, R2C1_SEG_H,
	R2C2_SEG_A, R2C2_SEG_B, R2C2_SEG_C, R2C2_SEG_D,
	R2C2_SEG_E, R2C2_SEG_F, R2C2_SEG_G, R2C2_SEG_H,
	R2C3_SEG_A, R2C3_SEG_B, R2C3_SEG_C, R2C3_SEG_D,
	R2C3_SEG_E, R2C3_SEG_F, R2C3_SEG_G, R2C3_SEG_H,
	R2C4_SEG_A, R2C4_SEG_B, R2C4_SEG_C, R2C4_SEG_D,
	R2C4_SEG_E, R2C4_SEG_F, R2C4_SEG_G, R2C4_SEG_H,
	R2C5_SEG_A, R2C5_SEG_B, R2C5_SEG_C, R2C5_SEG_D,
	R2C5_SEG_E, R2C5_SEG_F, R2C5_SEG_G, R2C5_SEG_H,
	R2C6_SEG_A, R2C6_SEG_B, R2C6_SEG_C, R2C6_SEG_D,
	R2C6_SEG_E, R2C6_SEG_F, R2C6_SEG_G, R2C6_SEG_H,
};

#endif /* SEGMENTS_H */
