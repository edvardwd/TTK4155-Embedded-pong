#include "drivers/adc.h"
#include <stdio.h>
/////////////////////////////////////////////

uint8_t test_adc(void) {
    uint8_t errors = 0;
    for (uint8_t ch = 0; ch < 4; ch++) {
        uint8_t val = adc_read(ch);
        printf("Channel %u = %3u\n", ch, val);
        if (val > 255) errors++;
    }
    return (errors > 0);
}
