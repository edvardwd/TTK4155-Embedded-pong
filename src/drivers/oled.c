#include "drivers/oled.h"
#include "fonts.h"



void oled_write_cmd(char cmd){
    spi_master_select_slave(DISP_SS_PIN); // Select oled
    PORTB &= ~(1 << DATA_N_C); // Trigger command mode
    spi_master_transmit_byte(cmd); // Transmit the command
    PORTB |= (1 << DISP_SS_PIN); // Turn off after transmit
}


void oled_write_byte(char c){
    spi_master_select_slave(DISP_SS_PIN);
    PORTB |= (1 << DATA_N_C); // Trigger data mode
    spi_master_transmit_byte(c);
    PORTB |= (1 << DISP_SS_PIN); // Turn off after transmit
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
}

void oled_init_minimal(){
    oled_write_cmd(0xa1);
    oled_write_cmd(0xc8);
    oled_write_cmd(0xaf);
}

void oled_goto_line(uint8_t line){
    // int page_num = 0xB0 | (line / 7); //Pages range from row n -> n+7.
    int page_num = 0xB0 | (line & 0b111);
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


void oled_reset();
void oled_home();

void oled_clear_line(uint8_t line);
void oled_pos(uint8_t row, uint8_t col);


void oled_print_char(char c){
    // Choose default font
    // TODO: make configurable
    const unsigned char* SELECTED_FONT = font5;
    uint8_t FONT_WIDTH = 5;

    uint8_t line = 0;
    uint8_t col = 0;

    if (c < 32 || c > 126) c = '?'; // Ensure printable ASCII

    const unsigned char* printable = SELECTED_FONT[c - 32];
    for (uint8_t i = 0; i < FONT_WIDTH; i++) {
        oled_goto_line(line);
        oled_goto_column(col);

        char byte = pgm_read_byte(&printable[i]);
        oled_write_byte(byte);
        line++;
        col++;
    }
    // oled_write_byte(' '); // Add spacing between chars
}


void oled_print(const char* msg){
    while (*msg) {
        oled_print_char(*msg++);
    }
}
void oled_set_brightness(uint8_t level);