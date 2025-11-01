#include "drivers/xmem.h"
#include <stdio.h>
#include <stdlib.h>
/////////////////////////////////////////////

#define MAX_ALLOWED_ERRORS 20
/////////////////////////////////////////////

uint8_t test_xmem(void) {
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;

    printf("Starting SRAM test...\n");

    uint16_t seed = rand();

    // --- Write phase ---
    srand(seed);
    for (uint16_t i = 0; i < SRAM_SIZE; i++) {
        uint8_t some_value = rand();
        xmem_write(some_value, i);

        uint8_t retrieved_value = xmem_read(i);
        if (retrieved_value != some_value) {
            write_errors++;
        }
    }

    // --- Retrieval phase ---
    srand(seed);
    for (uint16_t i = 0; i < SRAM_SIZE; i++) {
        uint8_t some_value = rand();
        uint8_t retrieved_value = xmem_read(i);
        if (retrieved_value != some_value) {
            retrieval_errors++;
        }
    }

    printf("SRAM test completed:\n"
           "  write errors: %u\n"
           "  retrieval errors: %u\n",
           write_errors, retrieval_errors);

    uint32_t total_errors = write_errors + retrieval_errors;

    if (total_errors <= MAX_ALLOWED_ERRORS) {
        printf("SRAM test PASSED (total errors = %lu ≤ %d)\n",
               (unsigned long)total_errors, MAX_ALLOWED_ERRORS);
        return 0;
    } else {
        printf("SRAM test FAILED (too many errors)\n");
        return 1;
    }
}
