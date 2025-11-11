#include "drivers/joystick.h"
/////////////////////////////////////////////


int8_t map_to_percent(uint8_t raw, uint8_t min, uint8_t center, uint8_t max) {
    int8_t percent;
    if (raw <= center) {
        percent = -100 + (int16_t)(raw - min) * 100 / (center - min);
    } else {
        percent = (int16_t)(raw - center) * 100 / (max - center);
    }
    return clip_value(percent, -100, 100);
}


int8_t clip_value(int8_t val, int8_t min, int8_t max){
    if (val < min) return min;
    if (val > max) return max;
    return val;
}



pos_t get_pos() {
    pos_t pos;

    // Sample values
    *ADC = 0; // sets WR high and samples values
    while(!(PINB & (1 << BUSY_PIN))); // wait until conversion is done

    volatile uint8_t raw_joystick_x = adc_read();
    volatile uint8_t raw_joystick_y = adc_read();
    volatile uint8_t raw_pad_y = adc_read();
    volatile uint8_t raw_pad_x = adc_read();
    
    pos.joystick_x = map_to_percent(raw_joystick_x, X_MIN, X_CENTER, X_MAX);
    pos.joystick_y = map_to_percent(raw_joystick_y, Y_MIN, Y_CENTER, Y_MAX);
    pos.pad_x = clip_value(((int16_t) raw_pad_x*100 >> 7) - 100, -100, 100); 
    pos.pad_y = clip_value(((int16_t) raw_pad_y*100 >> 7) - 100, -100, 100);
    return pos;
}

joystick_dir_t joystick_get_dir() {
    pos_t pos = get_pos();
    joystick_dir_t dir = {NEUTRAL, NEUTRAL}; //Neutral

    int8_t x = pos.joystick_x; //reads x value
    int8_t y = pos.joystick_y; //reads y value

    if (y > DEADZONE)       dir.vertical_dir = UP;
    else if (y < -DEADZONE) dir.vertical_dir = DOWN;

    if (x > DEADZONE)       dir.horizontal_dir = RIGHT;
    else if (x < -DEADZONE) dir.horizontal_dir = LEFT;

    return dir;
}


void joystick_print_dir(joystick_dir_t dir) {
    uint8_t printed = 0;
    
    if (dir.vertical_dir == UP) {
        printf("UP ");
        printed = 1;
    }
    if (dir.vertical_dir == DOWN) {
        printf("DOWN ");
        printed = 1;
    }
    if (dir.horizontal_dir == LEFT) {
        printf("LEFT ");
        printed = 1;
    }
    if (dir.horizontal_dir == RIGHT) {
        printf("RIGHT ");
        printed = 1;
    }
    if (!printed) {
        printf("CENTER");
    }

    printf("\n");
}


uint8_t joystick_get_button_pressed(){
    static uint8_t prev_state = 0;
    uint8_t state = !(PINE & (1 << JOYSTICK_BUTTON));
    uint8_t btn_pressed = state && (prev_state != state);
    prev_state = state;
    return btn_pressed;
}