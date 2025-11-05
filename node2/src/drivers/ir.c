#include "drivers/ir.h"



volatile uint16_t GAME_SCORE = 0;

void ir_init(){
    // Enable clock for ADC
    PMC->PMC_PCER1 |= PMC_PCER1_PID37;

    // Disable PIO on IR pin
    PIOA->PIO_PDR = IR_PIN;
    PIOA->PIO_ABSR |= IR_PIN;

    ADC->ADC_MR = ADC_MR_PRESCAL(7) // ADC_CLK = MCLK / ((PRESCAL + 1)*2) = 84 Mhz / 16 = 5.25 Mhz
                | ADC_MR_STARTUP_SUT8
                | ADC_MR_SETTLING_AST17
                | ADC_MR_TRACKTIM(3);

    ADC->ADC_CHER = ADC_CHER_CH0;
}


uint16_t ir_read(){
    ADC->ADC_CR = ADC_CR_START;
    while (!(ADC->ADC_ISR & ADC_ISR_EOC0));
    return ADC->ADC_CDR[0] & 0xfff; // 12 bits ADC
}


uint8_t ir_update_score(){
    // Returns 1 if score is updated, else 0
    
    static uint16_t prev_ir_val = 1800; // approx value for no IR block
    uint16_t ir_val = ir_read();
    
    uint8_t ret_val = 0;

    if (ir_val < IR_ADC_THRESHOLD && prev_ir_val >= IR_ADC_THRESHOLD){
        GAME_SCORE++;
        ret_val = 1;
    }
    prev_ir_val = ir_val;
    
    return ret_val;
}