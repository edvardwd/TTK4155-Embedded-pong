#include "xmem.h"
#include "avr/io.h"


void xmem_init() {
    MCUCR |= (1 << SRE); // Enable XMEM
    SFIOR |= (0b100 << XMM0); // Mask bits
    
}


void xmem_write(uint8_t data, uint16_t address) {
    EXT_MEM[address] = data;
}


uint8_t xmem_read(uint16_t address) {
    return EXT_MEM[address];
}