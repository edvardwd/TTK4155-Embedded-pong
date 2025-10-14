#include "stdint.h"



void mcp2515_init();
uint8_t mcp2515_read(uint8_t address);
void mcp2515_write(char byte);
void mcp2515_request_to_send();
void mcp2515_bit_modify();
void mcp2515_reset();
void mcp_2515_read_status();