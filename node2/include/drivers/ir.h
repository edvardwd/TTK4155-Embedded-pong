#ifndef IR_H
#define IR_H

#include "sam3x8e.h"
#include <stdint.h>

#define IR_PIN PIO_PA2 // ADC CH0, pin A7 on Arduino
#define IR_ADC_THRESHOLD 1200 // Empiric value



void ir_init();
uint16_t ir_read();
uint8_t ir_detect_crossing();

#endif