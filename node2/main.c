#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "drivers/uart.h"
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
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    CAN0->CAN_IDR = 0xffffffff; //Disable interrupts for debugging

    // Uncomment after including uart above
    uart_init(F_CPU, 9600);
    printf("Node 2 starting...\n\r");

    // Config CAN_BR
    // can_init((CanInit){.brp = 6, .propag = 2, .phase1 = 11, .phase2 = 7, .sjw = 1, .smp = 0}, 1); // 500kbit/s according to Chat
    
    //can_message_t msg;
    //can_create_message(&msg, 0x01, "Hello");
    
    // Send meldingen
    //can_send_message(&msg);
    
    //uart_transmit_string("Message sent: 'Hello'\n");
    
    // Bit-timing matching MCP2515 (≈ 250 kbit/s)
    // can_init((CanInit){.brp = 6, .phase1 = 12, .phase2 = 4, .propag = 4, .sjw=3, .smp=0}, 0);
    
    //can_init((CanInit){.brp = 20, .phase1 = 7, .phase2 = 4, .propag = 1, .sjw=2, .smp=0}, 0); //THIS now matches node1
    uint32_t can_br =
        (20  << 16) |   // BRP
        (1  << 8)  |   // PROPAG
        (7 << 4)  |   // PHASE1
        (4 << 0)  |   // PHASE2./s
        (2  << 12) |   // SJW
        (0  << 24);    // SMP

    can_controller_init(can_br, 1, 1);

    //if(!can_init_def_tx_rx_mb(can_br)){
    //printf("CAN initialized (Normal mode)\n");}

    CAN_MESSAGE msg;
    while (1){
        if (!can_receive(&msg, 0)){
          if (msg.id == 0x43){
            int8_t x = (int8_t) (msg.data[0]) - 100;
            int8_t y = (int8_t) (msg.data[1]) - 100;
            printf("X: %d, Y: %d\r\n", x, y);
          }
        }
    }
    return 0;
}