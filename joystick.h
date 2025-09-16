#ifndef JOYSTICK_H
#define JOYSTICK_H
/////////////////////////
#include "stdint.h"
#define x_center 128
#define y_center 128


typedef struct {
    uint8_t x;
    uint8_t y;
} pos_t;

// static int8_t x_center = 128; 
// static int8_t y_center = 128;

// void pos_calibrate();


pos_t pos_read();

#endif