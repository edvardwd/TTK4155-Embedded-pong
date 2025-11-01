#include "drivers/spi.h"
/////////////////////////////////////////////

void spi_master_init(){
    // Set MOSI, SCK, IO_SS, DISP_SS, RND_SS and DATA_N_C as output, rest input
    DDR_SPI = (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << IO_SS_PIN) | 
            (1 << DISP_SS_PIN) | (1 << RND_SS) | (1 << DATA_N_C);
    
    PORTB |= (1 << IO_SS_PIN) | (1 << DISP_SS_PIN) | (1 << RND_SS); // Deselect all slaves initially
    // Enable SPI, Master, set clock rate fck / 16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}


void spi_master_transmit_byte(char data){
    SPDR = data; // Start transmission
    while (!(SPSR & (1 << SPIF))); // Wait until it completes
    // volatile uint8_t dummy = SPSR; 
    // dummy = SPDR;
}


void spi_master_transmit_bytes(char* data, uint16_t length){
    // Transmit multiple bytes
    for (uint16_t i = 0; i < length; i++){
        spi_master_transmit_byte(data[i]);
    }
}
char spi_master_read_byte() {
    return spi_master_read_byte_with_trigger(0x00); // Send dummy byte
}

char spi_master_read_byte_with_trigger(uint8_t trigger_byte) {
    SPDR = trigger_byte; // Send trigger byte
    while (!(SPSR & (1 << SPIF))); // Wait for reception complete
    return SPDR;
}

void spi_master_read_bytes(char* read_buffer, uint16_t length){
    // Read multiple bytes
    for (uint16_t i = 0; i < length; i++){
        read_buffer[i] = spi_master_read_byte();
    }
    // read_buffer is now filled
}


void spi_master_select_slave(uint8_t slave_id){
    // Initially, turn of all slaves by driving them high
    PORTB |= (1 << IO_SS_PIN) | (1 << DISP_SS_PIN) | (1 << RND_SS);

    // Turn on selected slave by driving it low
    PORTB &= ~(1 << slave_id);
}

void spi_master_deselect_slave(uint8_t slave_id){
    PORTB |= (1 << slave_id);
}
