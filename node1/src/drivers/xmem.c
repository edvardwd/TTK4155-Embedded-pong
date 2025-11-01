#include "drivers/xmem.h"
/////////////////////////////////////////////

void xmem_init() {
    MCUCR |= (1 << SRE); // Enable XMEM
    SFIOR |= (1 << XMM2); // Mask bits
}


void xmem_write(uint8_t data, uint16_t address) {
    EXT_MEM[address] = data;
}


uint8_t xmem_read(uint16_t address) {
    return EXT_MEM[address];
}

void SRAM_test() {
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;

    printf("Starting SRAM test..\n");
    uint16_t seed = rand();

    // Write phase: immediately check that the corrct value was stored
    srand(seed);
    for (uint16_t i = 0; i < SRAM_SIZE; i++) {
        uint8_t some_value = rand();
        xmem_write(some_value, i);

        uint8_t retrieved_value = xmem_read(i);
        // uint8_t retrieved_value = ext_ram[i];
        if (retrieved_value != some_value) {
            write_errors++;
            // printf("Write phase error: EXT_MEM[%4d] = %02X (should be %02X)\n", i, retrieved_value, some_value);
        }
    }

    // Retrieval phase: Check that no value swere changed during or after the write phase
    srand(seed);
    for (uint16_t i = 0; i < SRAM_SIZE; i++) {
        uint8_t some_value = rand();
        uint8_t retrieved_value = xmem_read(i);

        if (retrieved_value != some_value) {
            retrieval_errors++;
            // printf("Retrieval phase error: EXT_MEM[%4d] = %02X (should be %02X)\n", i, retrieved_value, some_value);
        }
    }

    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}