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


// TODO: fix ADC channels in comments

int main(){
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    CAN0->CAN_IDR = 0xffffffff; //Disable interrupts for debugging

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
    encoder_calibrate();
    solenoid_init();

    delay_ms(1000);
    printf("MIN: %d, MID: %d, MAX: %d\r\n", ENCODER_MIN, ENCODER_MID, ENCODER_MAX);


    CAN_MESSAGE msg;
    uint32_t last_time = time_now();
    while (1){

        uint32_t now_time = time_now();
        float t = (float) totalSeconds(now_time - last_time);

        // printf("Message recieved");
        if (!can_receive(&msg, 0)){
            if (msg.id == CAN_ID_JOYSTICK){
                int32_t joystick_x = (int32_t) (msg.data[0]) - 100;
                int32_t pad_x = (int32_t) (msg.data[1]) - 100;
            if (t >= PERIOD) {       // 0.01 s = 10 ms
                motor_set_duty_cycle_and_dir(pad_x);
                last_time = now_time;
            }
            servo_set_duty_cycle(joystick_x);
            // printf("pad_x: %d\r\n", pad_x);
            }

            // printf("Message received with ID: %u \r\n", msg.id);  
            if (msg.id == CAN_ID_JOYSTICK_BUTTON){
                // printf("Button pressed\r\n");
                solenoid_trigger();
            }
        }
        if (ir_detect_crossing()){
            printf("(Animal) crossing detected!\r\n");
            CAN_MESSAGE crossingMsg = {
                .id = CAN_ID_IR,
                .data_length = 0,
                ""
            };
            can_send(&crossingMsg, 0);
        }
    }
    return 0;
}