#ifndef CAN_H
#define CAN_H
/////////////////////////////////////////////

#include "drivers/mcp2515.h"
#include "avr/interrupt.h"
#include "string.h"
#include "drivers/uart.h"
#include "drivers/joystick.h"
#include <stdio.h>
#include <stdint.h>
/////////////////////////////////////////////

#define CAN_ID_NOP 0x00
#define CAN_ID_CALIBRATE 0x0a
#define CAN_ID_IR 0x2a
#define CAN_ID_JOYSTICK_BUTTON 0x31
#define CAN_ID_JOYSTICK 0x43
#define CAN_ID_SOLENOID 0x45
#define CAN_ID_ERROR 0x49

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
void can_process_interrupt(can_message_t* msg_buf);
void can_send_x_pos();
void can_send_id(uint16_t id);

/////////////////////////////////////////////
#endif
