#ifndef SPI_H
#define SPI_H
///////////////////////////////////////////////////////////////////
#include "avr/io.h"



#define IO_SS_PIN PB4
#define DISP_SS_PIN PB3

#define MOSI_PIN PB5
#define DD_MOSI DDB5

#define MISO_PIN PB6
#define DD_MISO DDB6

#define SCK_PIN PB7
#define DD_SCK DDB7

#define DDR_SPI DDRB

void spi_master_init();
void spi_master_transmit();

void spi_slave_init();
char spi_slave_receive();


///////////////////////////////////////////////////////////////////
#endif