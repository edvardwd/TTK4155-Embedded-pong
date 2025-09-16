#include "joystick.h"
#include "adc.h"
#include "avr/io.h"
#include "util/delay.h"


// void pos_calibrate() {
//     uint8_t x_center = adc_read(0);
//     uint8_t y_center = adc_read(1);
// }


pos_t pos_read() {
    // Sample values
    ADC[0] = 0; // sets WR high and samples values
    // while(!(PORTB & (1 << BUSY))) printf("BUSY\n"); // wait until conversion is done
    _delay_us(750);



    pos_t pos;
    volatile uint8_t raw_x = adc_read(0);
    volatile uint8_t raw_y = adc_read(0);
    volatile uint8_t slider1 = adc_read(0);
    volatile uint8_t slider2 = adc_read(0);
    
    pos.x = raw_x;
    pos.y = raw_y;

    // if (pos.x > 100) pos.x = 100;500 
    // if (pos.x < -100) pos.x = -100;
    // if (pos.y > 100) pos.y = 100;
    // if (pos.y < -100) pos.y = -100;
    
    return pos;
}


/*typedef enum {
    NEUTRAL,
    UP,
    DOWN,
    LEFT,
    RIGHT
} joystick_dir_t;

joystick_dir_t joystick_get_dir() {
    pos_t pos = pos_read();
    int8_t threshold = 20;

    if 
}*/