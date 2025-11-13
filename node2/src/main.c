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


void game_loop(){
    // uint32_t time = time_now();
    // uint8_t sent = 0;
    // uint32_t new_time = time_now();
    printf("inside game\n\r");
    delay_ms(5);
    
    CAN_MESSAGE msg;
    uint32_t last_time = time_now();
    
    int32_t joystick_x = 0;
    int32_t pad_x = 0;
    can_send_id(CAN_ID_CALIBRATE);
    
    printf("Message sent!\n\r");
    printf("RX0: %d  RX1: %d  RX2: %d\r\n",
       can_receive(&msg, 0),
       can_receive(&msg, 1),
       can_receive(&msg, 2));
    while (1) {
        printf("Loop started\n\r");
        uint32_t now_time = time_now(); // TODO: revisit
        float t = (float) totalSeconds(now_time - last_time);
        printf("after time\n\r");
        if (!can_receive(&msg, 0)){
            printf("ID is: %u", msg.id);
            if (msg.id == CAN_ID_JOYSTICK){
                joystick_x = (int32_t) (msg.data[0]) - 100;
                pad_x = (int32_t) (msg.data[1]) - 100;
            
                servo_set_duty_cycle(joystick_x);
                // printf("pad_x: %d\r\n", pad_x);
            }
            
              
            if (msg.id == CAN_ID_JOYSTICK_BUTTON){
                printf("Button pressed\r\n");
                solenoid_trigger();
            }
        }

        if (ir_detect_crossing()){
            printf("(Animal) crossing detected!\r\n");
            can_send_id(CAN_ID_IR);
            // return;
        }

        if (t >= PERIOD) {       // 0.01 s = 10 ms
            motor_set_duty_cycle_and_dir(pad_x);
            last_time = now_time;
        }
    }
}



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
    // encoder_calibrate();
    solenoid_init();
    // while(1){
    //     can_send_id(CAN_ID_CALIBRATE);
    // }
    // can_send_id(CAN_ID_CALIBRATE);
    //    while (1){
    //     uint32_t new_time = time_now();
    //     }

    // uint32_t prev_time = time_now();
    // while (1){
    //     uint32_t new_time = time_now();

    //     if (1){
    //         can_send_id(CAN_ID_NOP);
    //         printf("Message sent!\r\n");
    //     }
    //     prev_time = new_time;
    // }

    // delay_ms(1000);
    // printf("MIN: %d, MID: %d, MAX: %d\r\n", ENCODER_MIN, ENCODER_MID, ENCODER_MAX);


    CAN_MESSAGE msg = {
        .id = CAN_ID_NOP,
        .data_length = 0,
        .data = {0xff}
    };

    //delay_ms(25);
    while (1){    
        if(!can_receive(&msg, 0)){
            if (msg.id == CAN_ID_CALIBRATE){
            printf("inside if \n\r");
            encoder_calibrate();
            
            
            //msg.id = CAN_ID_NOP;
            printf("Entering game \n\r");
            game_loop();
            }

        }
    }
    return 0;
}