#include "game.h"
/////////////////////////////////////////////

void game_loop(){
    static uint8_t calibrated = 0;
    if (!calibrated){
        encoder_calibrate();
        calibrated = 1;
    }
    can_send_id(CAN_ID_CALIBRATE);

    game_state_t state = GAME_STATE_PLAYING;

    uint8_t n_lives = 3;
    uint16_t score = 0; // 16 bits should be enough
    uint32_t game_last_start_time = time_now();
     
    CAN_MESSAGE msg = {
        .id = CAN_ID_NOP,
        .data_length = 0,
        .data = {}
    };
    uint32_t last_time = time_now();
    
    int32_t joystick_x = 0;
    int32_t pad_x = 0;

    while (n_lives) {
        // Process joystick and pad in all states
        uint32_t now_time = time_now();
        float t = (float) totalSeconds(now_time - last_time);
        
        can_interrupt_process(&msg);

        if (msg.id == CAN_ID_JOYSTICK){
            joystick_x = (int32_t) (msg.data[0]) - 100;
            pad_x = (int32_t) (msg.data[1]) - 100;
        
            servo_set_duty_cycle(joystick_x);
        }

        switch (state)
        {
        case GAME_STATE_PLAYING:
            if (msg.id == CAN_ID_JOYSTICK_BUTTON) solenoid_trigger();

            if (ir_detect_crossing()){

                // Update state and score
                state = (n_lives) ? GAME_STATE_PAUSED : GAME_STATE_GAME_OVER;
                score = (uint16_t) min(255, score + totalSeconds((uint32_t) time_now() - game_last_start_time));
                
                msg.id = CAN_ID_IR;
                msg.data_length = 2;
                msg.data[0] = --n_lives;
                msg.data[1] = (uint8_t) score & 0xff;
                can_send(&msg, 0);

                // Move to middle and center servo
                motor_go_to_pos(0);
                servo_set_duty_cycle(0);
            }
            break;
            
        case GAME_STATE_PAUSED:
            if (msg.id == CAN_ID_JOYSTICK_BUTTON){
                state = GAME_STATE_PLAYING;
                game_last_start_time = time_now();
            }
                
            break;
        
        case GAME_STATE_GAME_OVER:
            return;

        default:
            break;
        }


        if (t >= PERIOD) {       // 0.01 s = 10 ms
            motor_set_duty_cycle_and_dir(pad_x);
            last_time = now_time;}
    }
}