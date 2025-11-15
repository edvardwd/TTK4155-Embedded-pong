/*
 * can_controller.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include <stdint.h>
#include "can_controller.h"


typedef enum {
	CAN_ID_NOP =  0x00,
	CAN_ID_CALIBRATE = 0x0a,
	CAN_ID_IR = 0x2a,
	CAN_ID_JOYSTICK_BUTTON = 0x31,
	CAN_ID_JOYSTICK = 0x43,
	CAN_ID_SOLENOID = 0x45,
	CAN_ID_ERROR = 0x49
} can_id_t;


typedef struct can_message_t
{
	uint16_t id;
	char data_length;
	char data[8];
} CAN_MESSAGE;

uint8_t can_init_def_tx_rx_mb(uint32_t can_br);
uint8_t can_init(uint32_t can_br, uint8_t num_tx_mb, uint8_t num_rx_mb);

uint8_t can_send(CAN_MESSAGE* can_msg, uint8_t mailbox_id);
uint8_t can_receive(CAN_MESSAGE* can_msg, uint8_t mailbox_id);
void can_print_message(CAN_MESSAGE *msg);

void can_send_id(can_id_t id);


#endif /* CAN_CONTROLLER_H_ */