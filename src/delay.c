/* simple, uncalibrated delay function */

#include <stdint.h>
#include "delay.h"

void delay_ms(unsigned int i)
{
	uint32_t j = i * 100;
	while(j--) { }
}
