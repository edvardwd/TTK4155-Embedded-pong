#ifndef JOYSTICK_H
#define JOYSTICK_H
/////////////////////////
#include "stdint.h"
#define X_CENTER 162
#define X_MIN 69
#define X_MAX 251

#define Y_CENTER 162
#define Y_MIN 67
#define Y_MAX 252



typedef struct {
    int8_t x;
    int8_t y;
} pos_t;

// typedef enum {
//     NEUTRAL,
//     UP,
//     DOWN,
//     LEFT,
//     RIGHT
// } joystick_dir_t;


void update_pos(pos_t* joystick, pos_t* slider);
int8_t map_to_percent(uint8_t raw, uint8_t min, uint8_t center, uint8_t max);
int8_t clip_value(int8_t val, int8_t min, int8_t max);
// joystick_dir_t joystick_get_dir();

#endif