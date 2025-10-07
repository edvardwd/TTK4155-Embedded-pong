#include "drivers/joystick.h"
#include "drivers/adc.h"
#include "avr/io.h"
#include "avr/delay.h"
#include <cstdint>


// void pos_calibrate() {
//     uint8_t x_center = 100;
//     uint8_t y_center = adc_read(1);
// }

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



void update_pos(pos_t* joystick, pos_t* slider) {
    // Sample values
    *ADC = 0; // sets WR high and samples values
    while(!(PINB & (1 << BUSY_PIN))); // wait until conversion is done

    volatile uint8_t raw_joystick_x = adc_read();
    volatile uint8_t raw_joystick_y = adc_read();
    volatile uint8_t raw_slider_y = adc_read();
    volatile uint8_t raw_slider_x = adc_read();
    
    joystick->x = map_to_percent(raw_joystick_x, X_MIN, X_CENTER, X_MAX);
    joystick->y = map_to_percent(raw_joystick_y, Y_MIN, Y_CENTER, Y_MAX);
    slider->x = clip_value(((int16_t) raw_slider_x*100 >> 7) - 100, -100, 100); 
    slider->y = clip_value(((int16_t) raw_slider_y*100 >> 7) - 100, -100, 100);
    
}




joystick_dir_t joystick_get_dir(pos_t* joystick) {
    joystick_dir_t dir = {0, 0, 0, 0}; //Neutral
    update_pos(joystick, NULL);

    int8_t x = joystick->x; //reads x value
    int8_t y = joystick->y; //reads y value

    if (y > DEADZONE)       dir.up = 1;
    else if (y < -DEADZONE) dir.down = 1;

    if (x > DEADZONE)       dir.right = 1;
    else if (x < -DEADZONE) dir.left = 1;

    return dir;
}


void joystick_print_dir(joystick_dir_t dir) {
    uint8_t printed = 0;
    
    if (dir.up) {
        printf("UP ");
        printed = 1;
    }
    if (dir.down) {
        printf("DOWN ");
        printed = 1;
    }
    if (dir.left) {
        printf("LEFT ");
        printed = 1;
    }
    if (dir.right) {
        printf("RIGHT ");
        printed = 1;
    }
    if (!printed) {
        printf("CENTER");
    }

    printf("\n");
}


