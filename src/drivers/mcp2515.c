#include "drivers/mcp2515.h"



uint8_t mcp2515_read(uint8_t address) {
    spi_master_select_slave(RND_SS);

    spi_master_transmit_byte(MCP_READ);
    spi_master_transmit_byte(address);
    uint8_t result = spi_master_read_byte();

    spi_master_deselect_slave(RND_SS);
    
    return result;
}

void mcp2515_write(uint8_t address, uint8_t data){
    spi_master_select_slave(RND_SS);
    spi_master_transmit_byte(MCP_WRITE);
    spi_master_transmit_byte(address);
    spi_master_transmit_byte(data);
    spi_master_deselect_slave(RND_SS);
}
void mcp2515_init(){
    uint8_t value ;
    spi_master_init () ; // Initialize SPI
    mcp2515_reset () ; // Send reset - command

    // Self - test
    uint8_t value = mcp2515_read(MCP_CANSTAT);
    if (( value & MODE_MASK ) != MODE_CONFIG ) {
        printf("MCP2515 is NOT in configuration mode after reset!\n");
        return;
    }
    // More initialization
    mcp2515_write(MCP_CNF1, 0x40); //SJW = 2 * TQ, BRP = 0
    mcp2515_write(MCP_CNF2, 0x80 | (7 << 3) | 1); // BTLMODE=1, SAM=0 (bus line is sampled once at sample point), PHSEG1=7, PRSEG=1
    mcp2515_write(MCP_CNF3, 0x4); // PHSEG2=4

    // Accept all messages (bypass filters)
    mcp2515_write(MCP_RXB0CTRL, 0x60); // RXM1:RMX0 = 11
    mcp2515_write(MCP_RXB1CTRL, 0x60);
    
    // Enable receive interrupt for RX0 and RX1
    mcp2515_write(MCP_CANINTE, 0x03);

    // Set PD1 as input pin for ATMEGA (to send interrupts)
    DDRD &= ~(1 << PD0);

    _delay_ms(1);
}

void mcp2515_request_to_send();

void mcp2515_reset(){
    spi_master_select_slave(RND_SS);
    spi_master_transmit_byte(MCP_RESET);
    spi_master_deselect_slave(RND_SS);
    _delay_ms(2);
}

void mcp2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data){
    spi_master_select_slave(RND_SS);
    spi_master_transmit_byte(MCP_BITMOD);
    spi_master_transmit_byte(mask);
    spi_master_transmit_byte(data);
    spi_master_deselect_slave(RND_SS);
}

void mcp_2515_read_status();