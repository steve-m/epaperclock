/* Motorola F3/MOTOFONE e-paper display / Solomon SSD1621 driver */

#ifndef DISPLAY_H
#define DISPLAY_H

void display_set_seg(uint8_t segment, uint8_t on);
void display_clear(uint8_t on);
void display_update(uint8_t refresh);
void display_putc(uint8_t line, uint8_t pos, unsigned char c);
void display_puts(const char *str, uint8_t line);
void display_putint(uint32_t i, uint8_t line, uint8_t space);
void display_init(uint8_t refresh);

#endif /* DISPLAY_H */
