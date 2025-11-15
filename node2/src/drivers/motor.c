#include "drivers/motor.h"
/////////////////////////////////////////////

void motor_init(){
    PIOC->PIO_PER |= MOTOR_DIR_PIN; //Set P23 as I/O
    PIOC->PIO_OER |= MOTOR_DIR_PIN; //Set P23 output;
}

void motor_set_direction(motor_dir_t dir){
    if (dir) PIOC->PIO_SODR |= MOTOR_DIR_PIN; //P23 = Forward (1)
    else PIOC->PIO_CODR |= MOTOR_DIR_PIN; //P23 = Reverse (0)
}


void motor_set_duty_cycle_and_dir(int32_t pad_x){
    int32_t target_pos = min(-ENCODER_MIN, ENCODER_MAX) * pad_x / 100;
    // printf("TARGET: %d \r\n", target_pos);
    int32_t curr_pos = encoder_get_motor_position();
    int32_t error = target_pos - curr_pos;
    int32_t u = motor_pid(error);
    motor_dir_t dir = curr_pos < target_pos ? REVERSE : FORWARD;
    
    
    motor_set_direction(dir);
    uint32_t duty_cycle = min(u, MAX_DUTY_CYCLE_MOTOR);
    pwm_update_duty_cycle(duty_cycle, PWM_CH_MOTOR);
}

int32_t motor_pid(int32_t error){
    static int32_t ERROR = 0;
    ERROR += error;

    ERROR = (ERROR < -5000) ? -5000 : (ERROR > 5000) ? 5000 : ERROR; // Anti windup
    
    int32_t u = (int32_t) (K_P * abs(error) + PERIOD * K_I  * abs(ERROR));
    return u;
}

void motor_go_to_pos(int32_t pos){
    while(abs(encoder_get_motor_position() - pos) > ENCODER_TOL){
        motor_dir_t dir = encoder_get_motor_position() < pos ? REVERSE : FORWARD;
        
        motor_set_direction(dir);
        pwm_update_duty_cycle(STEADY_MOTOR_DUTY_CYCLE, PWM_CH_MOTOR);

    }
    pwm_update_duty_cycle(0, PWM_CH_MOTOR); // Pos reached
}

void motor_move(int32_t pad_x){
    int32_t encoder_pos = min(-ENCODER_MIN, ENCODER_MAX) * pad_x / 100;
    motor_go_to_pos(encoder_pos);
}