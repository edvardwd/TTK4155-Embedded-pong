#ifndef GAME_H
#define GAME_H

#include "drivers/oled.h"
#include "drivers/oled_menu.h"
#include "drivers/can.h"

void game_intro_message();
void game_over_message();
void game_play();


#endif