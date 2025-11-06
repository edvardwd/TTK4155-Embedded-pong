#include "drivers/motor.h"

void motor_init(){
  PIOC->PIO_PER |= MOTOR_DIR_PIN; //Set P23 as I/O
  PIOC->PIO_OER |= MOTOR_DIR_PIN; //Set P23 output;
}

void motor_set_direction(uint8_t dir){
  if (dir) PIOC->PIO_SODR |= MOTOR_DIR_PIN; //P23 = Forward (1)
  else PIOC->PIO_CODR |= MOTOR_DIR_PIN; //P23 = Reverse (0)
}


void motor_set_duty_cycle_and_dir(int32_t touch_x){
  uint32_t dir;

}
