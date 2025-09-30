#include "drivers/uart.h"

void uart_init(unsigned int ubrr) {
    // Set baud rate
    UBRR0H = (unsigned char) (ubrr  >> 8);
    UBRR0L = (unsigned char) ubrr;

    // Enable transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // Set time format: 8 data bits, 2 stop bits
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

    // Set up stdio
    FILE* uart_stream = fdevopen(uart_putchar, uart_getchar);
    stdout = uart_stream;
    stdin = uart_stream;
}

void uart_transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait until data register empty
    UDR0 = data;
}


unsigned char uart_receive() {
    while (!(UCSR0A & (1 << RXC0))); // Wait until receive complete
    // printf("%c", UDR0);
    return UDR0;
}

int uart_putchar(char c, FILE* stream) {
    if (c == '\n') uart_putchar('\r', stream);
    uart_transmit(c);
    return 0; // EXIT SUCCESS
}

int uart_getchar(FILE* stream) {
    return uart_receive();
}