#include "drivers/oled.h"
/////////////////////////////////////////////



// static struct{
//     const unsigned char* font;
//     uint8_t width;
// } SELECTED_FONT = {
//     .font = font5, // Normal sized font by default
//     .width = 5
// };


// void oled_set_font(oled_font_t font){
// switch (font){
//     case small:
//         SELECTED_FONT.font = font4;
//         SELECTED_FONT.width = 4;
//         break;

//     case normal:
//         SELECTED_FONT.font = font5;
//         SELECTED_FONT.width = 5;
//         break;
//     case large:
//         SELECTED_FONT.font = font8;
//         SELECTED_FONT.width = 8;
//         break;
//     default:
//         SELECTED_FONT.font = font5;
//         SELECTED_FONT.width = 5;
// }
// }

void oled_write_cmd(uint8_t cmd) {
    PORTB &= ~(1 << DATA_N_C);           // Set command-mode first (D/C = 0)
    spi_master_select_slave(DISP_SS_PIN); // CS low
    _delay_us(1);
    spi_master_transmit_byte(cmd);        // Send command
    _delay_us(1);
    PORTB |= (1 << DISP_SS_PIN);          // CS high (deselect)
}

void oled_write_byte(uint8_t c){
    PORTB |= (1 << DATA_N_C); // Trigger data mode
    spi_master_select_slave(DISP_SS_PIN);
    _delay_us(1);
    spi_master_transmit_byte(c);
    _delay_us(1);
    PORTB |= (1 << DISP_SS_PIN); // Deselect after transmit
}


void oled_init(){
    oled_write_cmd(0xae); // Display off
    oled_write_cmd(0xa1); // Degment remap
    oled_write_cmd(0xda); // Common pads hardware: alternative
    oled_write_cmd(0x12);
    oled_write_cmd(0xc8); // Common output scan direction: com63~com0
    oled_write_cmd(0xa8); // Multiplex ratio mode: 63
    oled_write_cmd(0x3f);
    oled_write_cmd(0xd5); // Display divide ratio/osc. freq. mode
    oled_write_cmd(0x80);
    oled_write_cmd(0x81); // Contrast control
    oled_write_cmd(0xff);
    oled_write_cmd(0xd9); // Set pre-charge period
    oled_write_cmd(0xf1);
    oled_write_cmd(0x20); // Set Memory Addressing Mode
    oled_write_cmd(0x02);
    oled_write_cmd(0xdb); // VCOM deselect level mode
    oled_write_cmd(0x30);
    oled_write_cmd(0xad); // Master configuration
    oled_write_cmd(0x00);
    oled_write_cmd(0xa4); // Out follows RAM content
    oled_write_cmd(0xa6); // Set normal display
    oled_write_cmd(0xaf); // Display on

    // Set screen all black initially
    for (uint8_t i = 0; i < 64; i += 8){
        oled_clear_line(i);
    }
}

void oled_init_minimal(){
    oled_write_cmd(0xae); // Display off
    oled_write_cmd(0xA1);
    oled_write_cmd(0xC8);
    oled_write_cmd(0x20);   // set memory addressing mode
    oled_write_cmd(0x02);   // page addressing mode
    oled_write_cmd(0xAF);   //Display on

    // Set screen all black initially
    for (uint8_t i = 0; i < 64; i += 8){
        oled_clear_line(i);
    }
}

void oled_goto_line(uint8_t line){
    uint8_t page_num = 0xB0 | (line >> 3);
    oled_write_cmd(page_num);
}

void oled_goto_column(uint8_t column){
    uint8_t lower_nibble = 0b00001111 & column; //Yields 0000 D3 D2 D1 D0
    uint8_t upper_nibble = 0b00010000 | ((column >> 4) & 0b00001111); //Yields 0001 D7 D6 D5 D4

    oled_write_cmd(lower_nibble);
    oled_write_cmd(upper_nibble);
    /*
    TO DEFINE PTR STARTER LOCATION:
    Call goto_line - sets correct pager
    Call goto_column
        Lower then upper.
    */
}

void oled_pos(uint8_t row, uint8_t col){
    oled_goto_line(row);
    oled_goto_column(col);
}

void oled_clear_line(uint8_t line){ //Assume func wants to clear a page.
    oled_pos(line, 0);
    for (uint8_t i = 0; i < N_COLS; i++)
    {
        oled_write_byte(0); //Sets all pixels in column black
        //Column pointer auto-increments +1 per write in page-adr-mode. 
    }
}


void oled_fill_line(uint8_t line){ //Assume func wants to fill a page.
    oled_pos(line, 0);
    for (uint8_t i = 0; i < N_COLS; i++) {
        oled_write_byte(127); //Sets all pixels in column on
        //Column pointer auto-increments +1 per write in page-adr-mode. 
    }
}

void oled_clear_column(uint8_t line, uint8_t col){
    oled_pos(line, col);
    oled_write_byte(0); 
}

void oled_clear_disp(void){
    for (uint8_t i = 0; i < 64; i += 8){
        oled_clear_line(i);
    }
}



void oled_reset() {
    oled_write_cmd(0xAE);
    oled_init_minimal();
}


void oled_home();



void oled_print_char(uint8_t line, uint8_t col, char c){
    // Choose default font
    // TODO: make configurable

    if (c < 32 || c > 126) c = '?'; // Ensure printable ASCII

    //const unsigned char* printable = SELECTED_FONT[c - 32];
    oled_pos(line, col);
    for (uint8_t i = 0; i < FONT_WIDTH; i++) {
        char byte = pgm_read_byte(&SELECTED_FONT[c-32][i]);
        oled_write_byte(byte);
    }
}


void oled_print(uint8_t line, uint8_t col, const char* msg){
    while (*msg) {
        oled_print_char(line, col, *msg++);
        col += FONT_WIDTH + 1; // Font width spacing between letters
    }
}
void oled_set_brightness(uint8_t level); 

void oled_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    // Draws a line on the display from (x0, y0) to (x1, y1)
    if (x0 == x1){
        // Vertical line
        for (int8_t y = y0; y <= y1; y++){
            oled_pos(y, x0);
            oled_write_byte(127);
        }
        return;
    }
    if (y0 == y1){
        // Horizontal line
        for (int8_t x = x0; x <= x1; x++){
            oled_pos(y0, x);
            oled_write_byte(127);
        }
        return;
    }
    // Find a funtcion on the form y = ax + b:
    int8_t a = (y1 - y0) / (x1 - x0);
    int8_t b = y0 - (a * x0);
    // Draw line
    for (int8_t x = x0; x <= x1; x++){
        int8_t y = a * x + b;
        oled_pos(y, x);
        oled_write_byte(127);
    }

}



void oled_circle(uint8_t x0, uint8_t y0, uint8_t r){
    // Draws a circle with radius r and (x0, y0) in the center
    for (float theta = 0; theta < 2*M_PI; theta += 0.1 ){
        int8_t x = (int8_t) (x0 + r * cos(theta));
        int8_t y  = (int8_t) (y0 + r * sin(theta));
        oled_pos(y, x);
        oled_write_byte(127);
    }
    return;
}