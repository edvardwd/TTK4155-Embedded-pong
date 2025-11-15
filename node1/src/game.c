#include "game.h"
/////////////////////////////////////////////

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
void game_over_message(uint8_t score){
    oled_clear_disp();
    char buf[32];
    sprintf(buf, "Final score: %u", score);

    unsigned char* lines[4] = {
        "Game over!",
        buf,
        "",
        "Click for menu"
    };
    
    for (uint8_t i = 0; i < 4; i++){
        oled_print(i*8, 0, lines[i]);
    }
    while (!joystick_get_button_pressed()); // wait for click
    // while (1) _delay_ms(50);
}

void game_play(){
    can_message_t msg;
    uint8_t score = 0;

    game_intro_message();
    _delay_ms(500); // 3s to read

    oled_clear_disp();
    oled_print(0, 0, "Calibrating..");

    can_send_id(CAN_ID_CALIBRATE);
    uint8_t calibrated = 0;


    while (!calibrated){
        can_process_interrupt(&msg);
        //_delay_ms(50);
        if (msg.id == CAN_ID_CALIBRATE) calibrated = 1;
    }

    oled_clear_disp();
    oled_print(0, 0, "Game underway");
    oled_print(8, 0, "Lives remaining: 3");

    can_create_message(&msg, CAN_ID_NOP, "");
    uint8_t alive = 1;

    while(alive){
        _delay_ms(10);
        can_process_interrupt(&msg);
        // Send states to node 2
        can_send_x_pos();
        if (joystick_get_button_pressed()){
            printf("Button pressed\r\n");
            can_send_id(CAN_ID_JOYSTICK_BUTTON);
        }

    
        // can_read_message(&msg, 0);
        uint16_t id = msg.id;
        if (id == CAN_ID_NOP) continue;
        if (id == CAN_ID_ERROR) can_print_message(&msg);
        else if (id == CAN_ID_IR){
            uint8_t lives_remaining = msg.data[0];
            score = msg.data[1];
            char buf[32];
            sprintf(buf, "Lives remaining: %u", lives_remaining);
            oled_clear_line(8);
            oled_print(8, 0, buf);
            oled_print(8 * 2, 0, "To continue:");
            oled_print(8 * 3, 0, "Remove ball");
            oled_print(8* 4, 0, "And press button");
            while(!joystick_get_button_pressed());
            can_send_id(CAN_ID_JOYSTICK_BUTTON);
            for (uint8_t i = 2; i <= 4; i++)oled_clear_line(8*i);
            if(lives_remaining == 0){ alive = 0; printf("Alive = 0\r\n");}
        }             
        else {
            printf("This should in theory be impossible.\r\n");
            return;
        }
    }
    game_over_message(score);
}