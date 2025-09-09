#ifndef XMEM_H
#define XMEM_H

////////////////////////////////////////////////////////
#include "stdint.h"

#define SRAM_BASE_ADRESS 0x1800
#define ADC_BASE_ADRESS 0x1000
#define EXT_MEM ((volatile uint8_t* ) SRAM_BASE_ADRESS)




void xmem_init();
void xmem_write(uint8_t data, uint16_t address);
uint8_t xmem_read(uint16_t address);



////////////////////////////////////////////////////////
#endif