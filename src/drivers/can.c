#include "drivers/can.h"



void can_init(){
    mcp2515_init();
    // Set loopback mode
    mcp2515_bit_modify(MCP_CANCTRL, 0xE0, MODE_LOOPBACK);
    _delay_ms(1);

    // Verify loopback mode
    uint8_t value = mcp2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_LOOPBACK) {
        printf("MCP2515 failed to enter LOOPBACK mode! (CANSTAT=0x%02X)\n", value);
        return;
    }

    printf("MCP2515 initialized successfully (Loopback mode active)\n");

    return 0;
}



void can_send_message(can_message* msg){
    // Send ID
    mcp2515_write(MCP_TXB0SIDH, (uint8_t) msg->id & 0x7f8); // 8 first bits (of the 11 valid)
    mcp2515_write(MCP_TXB0SIDL, (uint8_t) msg->id & 0x7); // 3 next
    
    // Send data length
    uint8_t length = msg->data_length & 0xf; // Only allowed to send 8 bytes
    mcp2515_write(MCP_TXB0DLC, length); 

    // Write the data bytes
    for (uint8_t i = 0; i < length; i++) {
        if (i >= 8) break;
        mcp2515_write(MCP_TXB0 + i, msg->data[i]);
    }

    for (uint8_t i = 0;  i < msg->data_length; i++){

    }
}

can_message* can_create_message(uint16_t id, char* message){
    can_message res;
    res.id = id;
    res.data_length = strlen(message);
    res.data[res.data_length] = message; // Not sure if this works
}