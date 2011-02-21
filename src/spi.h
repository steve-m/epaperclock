#ifndef SPI_H
#define SPI_H

#define SCLK	BIT5
#define SDI	BIT7
#define SDO	BIT6
#define CS	BIT4
#define RESET	BIT3

void spi_init(void);
void spi_write(uint16_t c);
void spi_write_32(uint32_t c);

#endif /* SPI_H */

