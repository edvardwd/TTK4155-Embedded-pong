#ifndef MOTOR_H
#define MOTOR_H
#define MOTOR_DIR_PIN (1 << 23) //PC23

#include "sam3x8e.h"
#include "pwm.h"
#include "encoder.h"

typedef enum {
  REVERSE,
  FORWARD
}motor_dir_t;

void motor_init();
void motor_set_direction(motor_dir_t dir);
void motor_set_duty_cycle_and_dir(int32_t touch_x);
void motor_go_to_pos(int32_t pos);
void motor_move(int32_t slider_x);

#endif