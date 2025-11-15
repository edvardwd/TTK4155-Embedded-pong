/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_
/////////////////////////////////////////////

#include <stdio.h>
#include "sam.h"
#include "drivers/can_controller.h"
/////////////////////////////////////////////

extern volatile CAN_MESSAGE rx_message;
extern volatile uint8_t RX_MESSAGE_READY;

void CAN0_Handler();
void can_interrupt_process(CAN_MESSAGE* msg);

/////////////////////////////////////////////
#endif /* CAN_INTERRUPT_H_ */