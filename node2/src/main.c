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

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

// TODO: fix ADC channels in comments

#define CAN_ID_IR 0x2a
#define CAN_ID_JOYSTICK_BUTTON 0x31
#define CAN_ID_JOYSTICK 0x43
#define CAN_ID_SOLENOID 0x45


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
      printf("ADC: %u\r\n", ir_read());
      // printf("Message recieved");
      if (!can_receive(&msg, 0)){
        if (msg.id == CAN_ID_JOYSTICK){
            int32_t joystick_x = (int32_t) (msg.data[0]) - 100;
            int32_t slider_x = (int32_t) (msg.data[1]) - 100;
            if (t >= PERIOD) {       // 0.01 s = 10 ms
              motor_set_duty_cycle_and_dir(slider_x);
              last_time = now_time;
            }
            servo_set_duty_cycle(joystick_x);
            // printf("Slider_x: %d\r\n", slider_x);
          }
          
        if (msg.id == CAN_ID_JOYSTICK_BUTTON){
          solenoid_trigger();
        }
        }
  }
  return 0;
}