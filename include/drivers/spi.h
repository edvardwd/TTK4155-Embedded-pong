#ifndef SPI_H
#define SPI_H
///////////////////////////////////////////////////////////////////
#include "avr/io.h"


#define DDR_SPI DDRB

#define DATA_N_C PB1
#define RND_SS PB2
#define DISP_SS_PIN PB3
#define IO_SS_PIN PB4
#define MOSI_PIN PB5
#define MISO_PIN PB6
#define SCK_PIN PB7


void spi_master_init();

void spi_master_transmit_byte(char data);
void spi_master_transmit_bytes(char* data, uint16_t length);

char spi_master_read_byte();
void spi_master_read_bytes(char* read_buffer, uint16_t length);

void spi_master_select_slave(uint8_t slave_id);


///////////////////////////////////////////////////////////////////
#endif