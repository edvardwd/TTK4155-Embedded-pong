#ifndef CAN_H
#define CAN_H

#include "drivers/mcp2515.h"
#include "avr/interrupt.h"
#include "string.h"
#include "drivers/uart.h"

typedef struct{
    uint16_t id;
    uint8_t data_length;
    uint8_t data[8];
} can_message_t;

extern volatile uint8_t CAN_INTERRUPT_FLAG;

void can_init();

void can_send_message(can_message_t* msg, uint8_t transmit_buffer_n);

void can_create_message(can_message_t* message_buf, uint16_t id, char* message);
void can_print_message(can_message_t* msg);

void can_read_message(can_message_t *msg, uint8_t rx_buffer_n);
void can_process_interrupt();

#endif
