/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "drivers/can_interrupt.h"

// #include "../uart_and_printf/printf-stdarg.h"


#define DEBUG_INTERRUPT 0


/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */

volatile CAN_MESSAGE rx_message;
volatile uint8_t RX_MESSAGE_READY = 0;

void CAN0_Handler(){
    char can_sr = CAN0->CAN_SR; 
    
    if(can_sr & CAN_SR_MB1){
        can_receive(&rx_message, 1);  // Read immediately!
        RX_MESSAGE_READY = 1;
    }
    NVIC_ClearPendingIRQ(ID_CAN0);
}

void can_interrupt_process(CAN_MESSAGE* msg){
    if (!RX_MESSAGE_READY){
        msg->id = CAN_ID_NOP;
        msg->data_length = 0;
        return;
    }
    
    *msg = rx_message;  // Copy the saved message
    RX_MESSAGE_READY = 0;
}


// void CAN0_Handler()
// {
// 	if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
// 	char can_sr = CAN0->CAN_SR; 
	
// 	//RX interrupt
// 	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
// 	{
// 		CAN_MESSAGE message;
// 		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
// 		{
// 			can_receive(&message, 1);

// 		}
// 		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
// 		{
// 			can_receive(&message, 2);
// 		}
// 		else
// 		{
// 			printf("CAN0 message arrived in non-used mailbox\n\r");
// 		}
// 	}
	
// 	if(can_sr & CAN_SR_MB0)
// 	{
// 		if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
// 	//Disable interrupt
// 		CAN0->CAN_IDR = CAN_IER_MB0;

// 	}

// 	if(can_sr & CAN_SR_ERRP)
// 	{
// 		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

// 	}
// 	if(can_sr & CAN_SR_TOVF)
// 	{
// 		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

// 	}
	
// 	NVIC_ClearPendingIRQ(ID_CAN0);
// 	//sei();*/
// }
