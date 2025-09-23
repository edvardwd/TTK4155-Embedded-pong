#include "joystick.h"
#include "adc.h"
#include "avr/io.h"
#include "avr/delay.h"


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
    while(!(PINB & (1 << BUSY))); // wait until conversion is done

    volatile uint8_t raw_joystick_x = adc_read();
    volatile uint8_t raw_joystick_y = adc_read();
    volatile uint8_t raw_slider_y = adc_read();
    volatile uint8_t raw_slider_x = adc_read();
    
    joystick->x = map_to_percent(raw_joystick_x, X_MIN, X_CENTER, X_MAX);
    joystick->y = map_to_percent(raw_joystick_y, Y_MIN, Y_CENTER, Y_MAX);
    slider->x = clip_value(((int16_t) raw_slider_x*100 >> 7) - 100, -100, 100); 
    slider->y = clip_value(((int16_t) raw_slider_y*100 >> 7) - 100, -100, 100);
    
}



