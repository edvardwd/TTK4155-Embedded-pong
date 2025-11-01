#include "drivers/uart.h"
#include "tests.h"
#include <stdio.h>

// Optional includes for each driver, guarded by defines
#ifdef USE_XMEM
#include "drivers/xmem.h"
#endif
#ifdef USE_ADC
#include "drivers/adc.h"
#endif
#ifdef USE_CAN
#include "drivers/can.h"
#include "drivers/mcp2515.h"
#endif
#ifdef USE_SPI
#include "drivers/spi.h"
#endif

static uint8_t run(uint8_t (*fn)(void), const char *name) {
    printf("\n=== Running %s ===\n", name);
    uint8_t result = fn();
    printf("\n==========\n");
    if (result == 0) printf("[PASS] %s\n", name);
    else             printf("[FAIL] %s\n", name);
    printf("==========\n\n");
    return result;
}

int main(void) {
    uart_init(UBRR);

#ifdef USE_XMEM
    xmem_init();
#endif
#ifdef USE_ADC
    adc_init();
#endif
#ifdef USE_SPI
    spi_master_init();
#endif
#ifdef USE_CAN
    can_init();
#endif

#ifdef TEST_UART
    run(test_uart, "UART");
#elif defined(TEST_XMEM)
    run(test_xmem, "SRAM");
#elif defined(TEST_ADC)
    run(test_adc, "ADC");
#elif defined(TEST_CAN)
    run(test_can, "CAN");
#else
    printf("No test defined for this build.\n");
#endif

    while (1);
}
