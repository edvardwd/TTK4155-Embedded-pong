#include "drivers/can.h"
#include "drivers/mcp2515.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
/////////////////////////////////////////////

uint8_t test_can(void) {
    printf("Starting CAN loopback test...\n");

    // 1. Set MCP2515 to loopback mode
    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);
    _delay_ms(10);

    // 2. Verify mode actually changed
    uint8_t mode = mcp2515_read(MCP_CANSTAT) & MODE_MASK;
    if (mode != MODE_LOOPBACK) {
        printf("Failed to enter loopback mode! (CANSTAT=0x%02X)\n", mode);
        return 1;
    }

    // 3. Prepare a message
    can_message_t tx;
    can_create_message(&tx, 0x7F, "LoopOK");
    printf("Sending CAN message in loopback mode...\n");
    can_print_message(&tx);

    // 4. Send it
    can_send_message(&tx, 0);   // transmit buffer 0
    _delay_ms(50);

    // 5. Check RX buffer
    can_message_t rx;
    memset(&rx, 0, sizeof(rx));
    can_read_message(&rx, 0);   // read from RX buffer 0

    // 6. Compare results
    uint8_t pass = (rx.id == tx.id) &&
                   (rx.data_length == tx.data_length) &&
                   (memcmp(rx.data, tx.data, rx.data_length) == 0);

    if (pass) {
        printf("CAN loopback OK: ID=0x%03X Data=\"%s\"\n", rx.id, rx.data);
        // Restore normal mode so later tests keep working
        mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
        return 0;   // success
    } else {
        printf("CAN loopback FAIL!\n");
        printf("Sent: ID=0x%03X len=%u data='%s'\n",
               tx.id, tx.data_length, tx.data);
        printf("Recv: ID=0x%03X len=%u data='%s'\n",
               rx.id, rx.data_length, rx.data);
        mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
        return 1;   // fail
    }
}
