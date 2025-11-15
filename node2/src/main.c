#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "drivers/uart.h"
#include "drivers/can_controller.h"
#include "drivers/pwm.h"
#include "drivers/servo.h"
#include "drivers/ir.h"
#include "drivers/encoder.h"
#include "drivers/motor.h"
#include "drivers/solenoid.h"
#include "drivers/can_interrupt.h"
#include "game.h"
/////////////////////////////////////////////


void init_all(){
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    // CAN0->CAN_IDR = 0xffffffff; //Disable interrupts for debugging

    // Uncomment after including uart above
    uart_init(F_CPU, 9600);
    printf("Node 2 starting...\n\r");

    uint32_t can_br =
    (20  << 16) |   // BRP
    (1  << 8)  |   // PROPAG>
    (7 << 4)  |   // PHASE1
    (4 << 0)  |   // PHASE2./s
    (2  << 12) |   // SJW
    (0  << 24);    // SMP

    can_init(can_br, 1, 1);
    pwm_init();
    ir_init();
    encoder_init();

    motor_init();
    solenoid_init();
}

int main(){
    init_all();

    CAN_MESSAGE msg = {
        .id = CAN_ID_NOP,
        .data_length = 0,
        .data = {}
    };

    while (1){    
        can_interrupt_process(&msg);
        if (msg.id == CAN_ID_CALIBRATE){
            game_loop();
        }
    }
    return 0;
}