#ifndef OLED_H
#define OLED_H
/////////////////////////////////////////////

#include "stdint.h"
#include "drivers/spi.h"
#include "avr/pgmspace.h"
#include "util/delay.h"
#include "fonts.h"
/////////////////////////////////////////////

#define N_COLS 128
#define SELECTED_FONT font5
#define FONT_WIDTH 5
/////////////////////////////////////////////

void oled_init();
void oled_init_minimal();
void oled_reset();
void oled_home();

void oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t column);
void oled_clear_line(uint8_t line);
void oled_clear_column(uint8_t line, uint8_t col);
void oled_fill_line(uint8_t line);
void oled_pos(uint8_t row, uint8_t col);
void oled_write_cmd(uint8_t data);
void oled_write_byte(uint8_t data);

void oled_print(uint8_t line, uint8_t col, const char* msg);
void oled_set_brightness(uint8_t level);
void oled_print_char(uint8_t line, uint8_t col, char c);

void oled_clear_disp(void);

void oled_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void oled_circle(uint8_t x0, uint8_t y0, uint8_t r);



/////////////////////////////////////////////
#endif