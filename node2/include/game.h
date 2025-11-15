#ifndef GAME_H
#define GAME_H
/////////////////////////////////////////////

#include "drivers/can_controller.h"
#include "drivers/motor.h"
#include "drivers/solenoid.h"
#include "drivers/ir.h"
/////////////////////////////////////////////

typedef enum {
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER
} game_state_t;

void game_loop();

/////////////////////////////////////////////
#endif