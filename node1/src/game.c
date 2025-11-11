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
void game_over_message(){
    oled_clear_disp();
    unsigned char* lines[2] = {
        "Game over!",
        "Score: "
    };
    
    for (uint8_t i = 0; i < 2; i++){
        oled_print(i*8, 0, lines[i]);
    }
    while (1) _delay_ms(50);
}

void game_play(){
    game_intro_message();
    _delay_ms(3000); // 3s to read

    oled_clear_disp();
    oled_print(0, 0, "Game underway");

    can_message_t msg;
    can_create_message(&msg, CAN_ID_NOP, "");

    uint8_t alive = 1;

    while(alive){
        _delay_ms(10);

        if (CAN_INTERRUPT_FLAG) can_process_interrupt(&msg);

        // Debug
        // printf("MESSAGE:\r\n");
        // can_print_message(&msg);
        // printf("\r\n");

        // Send states to node 2
        can_send_x_pos();
        if (joystick_get_button_pressed()) can_send_button_pressed();

        
        // Polling
        // can_read_message(&msg, 0);
        uint16_t id = msg.id;
        if (id == CAN_ID_NOP) continue;
        if (id == CAN_ID_ERROR) can_print_message(&msg);
        else if (id == CAN_ID_IR) alive = 0;
        else {
            printf("This should in theory be impossible.\r\n");
            return;
        }
    }

    game_over_message();
}