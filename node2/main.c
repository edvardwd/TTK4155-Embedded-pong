#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "drivers/uart.h"
#include "drivers/can.h"

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

    // Uncomment after including uart above
    uart_init(F_CPU, 9600);
    printf("Hello World\n\r");

    // Turn on PIOB clock
    PMC->PMC_PCER0 = (1u << ID_PIOB);

    // Let PIOB control PB13
    PIOB->PIO_PER = (1u << 13);

    // Configure PB13 as output
    PIOB->PIO_OER = (1u << 13);

    // Set PB13 high
    PIOB->PIO_SODR = (1u << 13); 

    // Config CAN_BR
    //    can_init((CanInit){.brp = F_CPU/2000000-1, .phase1 = 5, .phase2 = 1, .propag = 6}, 0);
    can_init((CanInit){.brp = 6, .propag = 2, .phase1 = 11, .phase2 = 7, .sjw = 1, .smp = 0}, 1); // 500kbit/s according to Chat



    while (1)
    {
        /* code */
    }
    return 0;
}