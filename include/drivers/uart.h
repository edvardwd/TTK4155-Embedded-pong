#ifndef UART_H
#define UART_H

////////////////////////////////////////////////////////

#include <avr/io.h>
#include "stdio.h"

#define FOSC 4915200UL // Oscillator frequency
#define BAUD_RATE 9600
#define UBRR (FOSC/16/BAUD_RATE) - 1

void uart_init(unsigned int ubrr);
void uart_transmit(unsigned char data);
unsigned char uart_receive();

// For stdio
int uart_putchar(char c, FILE* stream);
int uart_getchar(FILE* stream);

////////////////////////////////////////////////////////
#endif