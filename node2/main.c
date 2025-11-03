#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "drivers/uart.h"
#include "drivers/can.h"
#include "drivers/can_controller.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"


int main(){
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    SystemInit();

    // Uncomment after including uart above
    uart_init(F_CPU, 9600);
    printf("Node 2 starting...\n\r");

    // Config CAN_BR
    can_init((CanInit){.brp = 6, .propag = 2, .phase1 = 11, .phase2 = 7, .sjw = 1, .smp = 0}, 1); // 500kbit/s according to Chat
    
    //can_message_t msg;
    //can_create_message(&msg, 0x01, "Hello");
    
    // Send meldingen
    //can_send_message(&msg);
    
    //uart_transmit_string("Message sent: 'Hello'\n");
    
    // Bit-timing matching MCP2515 (≈ 500 kbit/s)
    // can_init((CanInit){.brp = 6, .phase1 = 12, .phase2 = 4, .propag = 4,
    // .sjw=3, .smp=0}, 0);
    uint32_t can_br =
        (6  << 16) |   // BRP
        (4  << 0)  |   // PROPAG
        (12 << 4)  |   // PHASE1
        (4 << 8)  |   // PHASE2
        (3  << 12) |   // SJW
        (0  << 14);    // SMP

    

    //if(!can_init_def_tx_rx_mb(can_br)){
    //printf("CAN initialized (Normal mode)\n");}

    CAN_MESSAGE msg;
    while (1){
        if (can_receive(&msg, 1) == 0) {
            printf("Received ID:%d, len:%d, data:", msg.id, msg.data_length);
            for (uint8_t i = 0; i < msg.data_length; i++)
                printf(" %c", msg.data[i]);
            printf("\n");
        }
    }

    return 0;
}