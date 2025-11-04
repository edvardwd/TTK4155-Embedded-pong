#ifndef PWM_H
#define PWM_H

#include "sam3x8e.h"

#define MIN_DUTY_CYCLE 1000 // 1 ms = 1000 µs
#define MID_DUTY_CYCLE 1500 // 1.5 ms = 1500 µs
#define MAX_DUTY_CYCLE 2000 // 2 ms = 2000 µs

void pwm_init();

void pwm_update_duty_cycle(uint32_t duty_cycle);

#endif