#include "drivers/mcp2515.h"
#include "drivers/spi.h"


// SPI INSTRUCTION SET FOR MCP2515 CAN controller
#define MCP_RESET 0x11000000
#define MCP_READ 0x11
#define MCP_READ_RX_BUFFER(n, m) (0x10010000 | ((n) << 2) | ((m) << 1))
#define MCP_WRITE 0x10
#define MCP_LOAD_TX_BUFFER(a, b, c) (0x01000000 | ((a) << 2) | ((b) << 1) | m)
#define MCP_RTS(n) (0x10000000 | ((n) << 2) | ((n) << 1) | n)
#define MCP_READ_STATUS

uint8_t mcp2515_read(uint8_t address) {
    spi_master_select_slave(RND_SS);

    spi_master_read_byte_with_trigger(MCP_READ);
    spi_master_transmit_byte(address);
    uint8_t result = spi_master_read_byte();

    spi_master_deselct_slave(RND_SS);
    
    return 0;
}