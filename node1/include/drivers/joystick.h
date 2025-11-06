#ifndef JOYSTICK_H
#define JOYSTICK_H
/////////////////////////////////////////////

#include "stdint.h"
#include "drivers/adc.h"
#include "avr/io.h"
#include "avr/delay.h"
#include "drivers/can.h"
/////////////////////////////////////////////

#define X_CENTER 162
#define X_MIN 69
#define X_MAX 251

#define Y_CENTER 162
#define Y_MIN 67
#define Y_MAX 252
#define DEADZONE 50

#define JOYSTICK_BUTTON PE0
/////////////////////////////////////////////

typedef struct {
    int8_t joystick_x;
    int8_t joystick_y;
    int8_t slider_x;
    int8_t slider_y;
} pos_t;

typedef enum {
    UP = 1,
    DOWN = -1,
    LEFT = -1,
    RIGHT = 1,
    NEUTRAL = 0
} joystick_dir_enum;

typedef struct {
    joystick_dir_enum horizontal_dir;
    joystick_dir_enum vertical_dir;
} joystick_dir_t;

pos_t get_pos();
int8_t map_to_percent(uint8_t raw, uint8_t min, uint8_t center, uint8_t max);
int8_t clip_value(int8_t val, int8_t min, int8_t max);
joystick_dir_t joystick_get_dir();
void joystick_print_dir(joystick_dir_t dir);

/////////////////////////////////////////////
#endif