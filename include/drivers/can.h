#ifndef CAN_H
#define CAN_H

#include "drivers/mcp2515.h"


typedef struct{
    uint16_t id;
    uint8_t data_length;
    uint8_t data[8];
} can_message;

void can_init();

void can_send_message(can_message* msg);

can_message* can_create_message(uint16_t id, char* message);

#endif
