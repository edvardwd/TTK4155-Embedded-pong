#ifndef ENCODER_H
#define ENCODER_H

#include "sam3x8e.h"
#include "motor.h"
#include "time.h"

#define ENCODER_TOL 0x43
#define STEADY_MOTOR_DUTY_CYCLE 13

extern int32_t ENCODER_MAX;
extern int32_t ENCODER_MID;
extern int32_t ENCODER_MIN;


void encoder_init();
volatile int32_t encoder_get_motor_position();
void encoder_wait_for_still();
void encoder_reset();
void encoder_calibrate();
void delay_ms(uint32_t ms);

#endif