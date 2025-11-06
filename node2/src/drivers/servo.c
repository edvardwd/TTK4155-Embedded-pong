#include "drivers/servo.h"

void servo_set_duty_cycle(int32_t joystick_x){
    // joystick_x should be within [-100, 100]
    uint32_t duty_cycle;
    duty_cycle = (uint32_t) (MID_DUTY_CYCLE + (5 * joystick_x));
    duty_cycle = min(max(MIN_DUTY_CYCLE, duty_cycle), MAX_DUTY_CYCLE); 

    pwm_update_duty_cycle(duty_cycle, PWM_CH_SERVO);
}