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
  
  TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0;
  TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

volatile int32_t encoder_get_motor_position(){
  return (volatile int32_t) TC2->TC_CHANNEL[0].TC_CV;
}

void encoder_reset(){
  TC2->TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG;
}

void delay_ms(uint32_t ms) {
    time_spinFor(msecs(ms));
}

void encoder_calibrate(){
  //START GO FORWARD  
  printf("Calibratin.... \r\n");
  encoder_reset();
  delay_ms(1000);
  motor_set_direction(1);
  pwm_update_duty_cycle(13, 0);
  uint32_t encoder_pos = encoder_get_motor_position();
  uint32_t eq_counter = 0;
  while(eq_counter < 20){
    uint32_t new_pos = encoder_get_motor_position();
    if(encoder_pos == new_pos){
      eq_counter ++;
    }
    else{
      encoder_pos = new_pos;
      eq_counter = 0;
    }
    delay_ms(10);
  }
  pwm_update_duty_cycle(0, 0); //Stop motor
  ENCODER_MIN = encoder_get_motor_position();
  printf("Min pos: %ld\r\n", ENCODER_MIN);


  //GO REVERSE
  motor_set_direction(0);
  pwm_update_duty_cycle(13, 0);
  encoder_pos = encoder_get_motor_position();
  eq_counter = 0;
  while(eq_counter < 5){
    uint32_t new_pos = encoder_get_motor_position();
    if(encoder_pos == new_pos){
      eq_counter ++;
    }
    else{
    encoder_pos = new_pos;
    eq_counter = 0;
    }
    delay_ms(10);
  }
  pwm_update_duty_cycle(0, 0);
  ENCODER_MAX = encoder_get_motor_position();
  printf("Max pos: %ld\r\n", ENCODER_MAX);
  
  ENCODER_MID = ENCODER_MIN + (ENCODER_MAX - ENCODER_MIN) / 2;
  printf("Mid pos: %ld\r\n", ENCODER_MID);

  encoder_pos = encoder_get_motor_position();
  while(abs((int32_t)encoder_get_motor_position() - (int32_t)ENCODER_MID) > 20){
    motor_set_direction(1); //Go to middle
    pwm_update_duty_cycle(12, 0);

    delay_ms(10);
  }
  pwm_update_duty_cycle(0, 0); //Middle reached
  encoder_reset();
  
  printf("Calibration done!\r\n");
}