#include "drivers/encoder.h"

int32_t ENCODER_MAX = 0;
int32_t ENCODER_MID = 0;
int32_t ENCODER_MIN = 0;


void encoder_init(){
    PMC->PMC_PCER1 |= PMC_PCER1_PID33;  //  Enable timer counter clock for TC channel 6
    PMC->PMC_PCER0 |= PMC_PCER0_PID13; // PIOC = ID13

    PIOC->PIO_PDR |= (PIO_PDR_P25 | PIO_PDR_P26);
    PIOC->PIO_ABSR |= (PIO_ABSR_P25 | PIO_ABSR_P26);

    TC2->TC_BMR = TC_BMR_QDEN 
                | TC_BMR_POSEN 
                | TC_BMR_EDGPHA
                | TC_BMR_TC0XC0S_TIOA1;
    
    TC2->TC_CHANNEL[PWM_CH_MOTOR].TC_CMR = TC_CMR_TCCLKS_XC0;
    TC2->TC_CHANNEL[PWM_CH_MOTOR].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

volatile int32_t encoder_get_motor_position(){
    return (volatile int32_t) TC2->TC_CHANNEL[PWM_CH_MOTOR].TC_CV;
}

void encoder_reset(){
    TC2->TC_CHANNEL[PWM_CH_MOTOR].TC_CCR |= TC_CCR_SWTRG;
}

void delay_ms(uint32_t ms) {
    time_spinFor(msecs(ms));
}

void encoder_wait_for_still(){
  // Runs until the encoder has measured the same 5 times in a row

    int32_t pos = encoder_get_motor_position();
    uint8_t counter = 0;

    while (counter < 5){
        int32_t new_pos = encoder_get_motor_position();
        counter = (new_pos == pos) ? counter + 1 : 0;
        pos = new_pos;
        delay_ms(10);
    }
}

void encoder_calibrate(){
    printf("Calibratin.... \r\n");
    encoder_reset();
    delay_ms(1000);

    //START GO FORWARD  
    motor_set_direction(FORWARD);
    pwm_update_duty_cycle(STEADY_MOTOR_DUTY_CYCLE, PWM_CH_MOTOR);
    
    encoder_wait_for_still();
    pwm_update_duty_cycle(0, PWM_CH_MOTOR); //Stop motor
    ENCODER_MIN = encoder_get_motor_position();
    printf("Min pos: %ld\r\n", ENCODER_MIN);

    //GO REVERSE
    motor_set_direction(REVERSE);
    pwm_update_duty_cycle(STEADY_MOTOR_DUTY_CYCLE, PWM_CH_MOTOR);
    
    encoder_wait_for_still();
    pwm_update_duty_cycle(0, PWM_CH_MOTOR);
    ENCODER_MAX = encoder_get_motor_position();
    printf("Max pos: %ld\r\n", ENCODER_MAX);
    
    ENCODER_MID = (ENCODER_MIN + ENCODER_MAX) / 2;
    printf("Mid pos: %ld\r\n", ENCODER_MID);
    
    motor_go_to_pos(ENCODER_MID);
    pwm_update_duty_cycle(0, PWM_CH_MOTOR);
    encoder_reset();
    
    ENCODER_MIN = -(ENCODER_MID - ENCODER_MIN);
    ENCODER_MAX = ENCODER_MAX - ENCODER_MID;
    ENCODER_MID = 0;

    printf("Calibration done!\r\n");
}