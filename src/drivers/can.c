#include "drivers/can.h"

volatile uint8_t CAN_INTERRUPT_FLAG = 0;

void can_init(){
    mcp2515_init();

    // Set loopback mode
    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);
    _delay_ms(1);

    // Verify loopback mode
    uint8_t value = mcp2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_LOOPBACK) {
        printf("MCP2515 failed to enter LOOPBACK mode! (CANSTAT=0x%02X)\n", value);
        return;
    }

    uart_transmit_string("MCP2515 initialized successfully (Loopback mode active)\n");
    _delay_ms(50);

    GICR |= (1 << INT1);  // Enable INT1
    sei();                // Enable global interrupts
}



void can_send_message(can_message_t* msg, uint8_t transmit_buffer_n){
    // Calculate buffer address offset
    uint8_t offset = 0x10 * transmit_buffer_n;
    
    // // Send ID
    // mcp2515_write(MCP_TXB0SIDH + offset, (uint8_t) msg->id & 0x7f8); // 8 first bits (of the 11 valid)
    // mcp2515_write(MCP_TXB0SIDL + offset, (uint8_t) msg->id & (0x7 << 5)); // 3 next (shifted to MSB)

     // Send ID (corrected bit shifting)
    mcp2515_write(MCP_TXB0SIDH + offset, (uint8_t)(msg->id >> 3)); // Upper 8 bits
    mcp2515_write(MCP_TXB0SIDL + offset, (uint8_t)((msg->id & 0x07) << 5)); // Lower 3 bits
    
    // Send data length
    uint8_t length = msg->data_length & 0xf; // Only allowed to send 8 bytes
    mcp2515_write(MCP_TXB0DLC + offset, length); 

    // Write the data bytes
    for (uint8_t i = 0; i < length; i++) {
        if (i >= 8) break;
        mcp2515_write(MCP_TXB0 + offset + i, msg->data[i]);
    }
    printf("Filling transmit buffer %u \n", transmit_buffer_n);
    mcp2515_request_to_send(transmit_buffer_n);
}

void can_create_message(can_message_t* message_buf, uint16_t id, char* message){
    message_buf->id = id;
    message_buf->data_length = strlen(message);
    // message_buf->data[message_buf->data_length] = '\0'; // Terminate string
    memcpy(message_buf->data, message, message_buf->data_length + 1); 
}

void can_print_message(can_message_t *msg){
    printf("ID: %u\tlen: %u\t%s\n", msg->id, msg->data_length, msg->data);
}

void can_read_message(can_message_t *msg, uint8_t rx_buffer_n){
    printf("Trying to read buffer RX%u\n", rx_buffer_n);
    spi_master_select_slave(RND_SS);
    spi_master_transmit_byte(MCP_READ_RX0 + 0x4 * rx_buffer_n); // initiate read of RX buffer n
    
    // Fill id field
    uint8_t sidh = spi_master_read_byte();
    uint8_t sidl = spi_master_read_byte();
    
    msg->id = ((uint16_t) sidh << 3) | (sidl >> 5);

    // dummy (extened id)
    spi_master_read_byte();
    spi_master_read_byte(); 

    // Fill length field
    msg->data_length = spi_master_read_byte() & 0xf;

    // Fill data_field
    for (uint8_t i = 0; i < msg->data_length; i++){
        msg->data[i] = spi_master_read_byte();
    }
    msg->data[msg->data_length] = '\0';
    spi_master_deselect_slave(RND_SS); // Flag is automatically cleared when using MCP_READ_RXn
    uart_transmit_string("Received message:\n");
    can_print_message(msg);
}


ISR(INT1_vect){
    CAN_INTERRUPT_FLAG = 1;
}

void can_process_interrupt(){
    if (!CAN_INTERRUPT_FLAG) return;

    uart_transmit_string("ISR called!\n");
    uint8_t canintf = mcp2515_read(MCP_CANINTF);
    //printf("CANINTF: %x\n", canintf);

    if (canintf & MCP_RX0IF) {
        // Message received in RXB0
        can_message_t msg;
        can_read_message(&msg, 0);
        //printf("Message in RX%u:\n", 0);
        uart_transmit_string("Received..\n");
        can_print_message(&msg);
        mcp2515_bit_modify(MCP_CANINTF, (MCP_RX0IF), 0);
    }
    if (canintf & MCP_RX1IF) {
        // Message received in RXB1
        can_message_t msg;
        //printf("Message in RX%u:\n", 1);
        can_read_message(&msg, 1);
        can_print_message(&msg);
        mcp2515_bit_modify(MCP_CANINTF, (MCP_RX1IF), 0);
    }

    else {
        //printf("Undefined interrupt!\n");
        // Clear all interrupt flags just in case
        mcp2515_bit_modify(MCP_CANINTF, 0xff, 0x00);
    }
    CAN_INTERRUPT_FLAG = 0;

}

