#include "drivers/spi.h"


void spi_master_init() {
    // Set MOSI and SCK output, rest input
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK) | (0b1111 << 1);
    
    // Enable SPI, Master, set clock rate fck / 16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

}

void spi_master_transmit(char data) {
    // Start transmission
    SPDR = data;

    while (!(SPSR & (1 << SPIF))); // Wait until it completes
}

