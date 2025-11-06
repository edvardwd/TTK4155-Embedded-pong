#ifndef ENCODER_H
#define ENCODER_H

#include "sam3x8e.h"

void encoder_init();
volatile int32_t encoder_get_motor_position();
void encoder_reset();

#endif