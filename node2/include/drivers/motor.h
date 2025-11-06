#ifndef MOTOR_H
#define MOTOR_H
#define MOTOR_DIR_PIN (1 << 23) //PC23

#include "sam3x8e.h"
#include "pwm.h"


void motor_init();
void motor_set_direction(uint8_t dir);
void motor_set_duty_cycle_and_dir(int32_t touch_x);


#endif