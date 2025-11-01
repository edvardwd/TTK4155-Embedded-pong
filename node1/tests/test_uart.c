#include "drivers/uart.h"
#include <stdio.h>
#include <string.h>
/////////////////////////////////////////////

#define EXPECTED_WORD "ok"
/////////////////////////////////////////////

uint8_t test_uart(void) {
    printf("UART manual test:\r\n");
    printf("Type \"%s\" and press Enter. The board will echo what you send.\r\n", EXPECTED_WORD);

    char buffer[16] = {0};

    // Read a single word (until whitespace or newline)
    scanf("%15s", buffer);
    printf("You wrote: %s\r\n", buffer);

    if (strcmp(buffer, EXPECTED_WORD) == 0) {
        printf("[PASS] UART received \"%s\" as expected.\r\n", EXPECTED_WORD);
        return 0;   // success
    } else {
        printf("[FAIL] Expected \"%s\" but got \"%s\".\r\n", EXPECTED_WORD, buffer);
        return 1;   // failure
    }
}
