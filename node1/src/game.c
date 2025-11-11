#include "game.h"


void game_intro_message(){
    oled_clear_disp();
    unsigned char* lines[7] = {
        "Welcome to Pong!",
        "Keep the ball up.",
        "",
        "Moving:",
        "Side: pad_x",
        "Angle: joystick_x",
        "Trigger: joystick_btn"
    };

    for (uint8_t i = 0; i < 7; i++){
        oled_print(i*8, 0, lines[i]);
    }
}
void game_over_message();

void game_play(){
    game_intro_message();
    _delay_ms(3000); // 3s to read

    while(1){
        if (CAN_INTERRUPT_FLAG){
            
        }
    }
}