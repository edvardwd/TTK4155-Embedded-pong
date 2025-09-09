#include "uart.h"
#include "xmem.h"
#include "util/delay.h"

void test_receive_transmit() {
    while (1) {
        // Receive from computer and send back
        unsigned char data_from_computer = uart_receive();
        uart_transmit(data_from_computer);
    }
}

void test_stdio() {
    char input[50];
    printf("Write something:\n");
    scanf("%49s", input);
    printf("You wrote: %s\n", input);
}

void test_latch() {
    uint8_t val = 0;
    while (1) {
        printf("Setting PA0 to %d\n", val);
        // Count 0 -> 7
        xmem_write(val, PA0);
        _delay_ms(3000);
        
        val++;
        val %= 8;
    }
}


int main() {
    uart_init(UBRR);
    xmem_init();
    test_latch();
    // test_receive_transmit();
    // test_stdio();
    return 0;
}