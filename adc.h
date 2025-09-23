#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define ADC_BASE_ADDR 0x1000
#define ADC ((volatile uint8_t*) ADC_BASE_ADDR)
#define BUSY PB0


void adc_init();

uint8_t adc_read();

#endif