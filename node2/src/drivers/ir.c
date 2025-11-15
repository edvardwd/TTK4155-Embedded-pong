#include "drivers/ir.h"
/////////////////////////////////////////////

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


uint8_t ir_detect_crossing(){
    // Returns 1 if IR detects a crossing, else 0
    static uint8_t detected_counter = 0;
    uint16_t ir_val = ir_read();
    detected_counter = (ir_val >= IR_ADC_THRESHOLD) ? 0 : ((detected_counter == 255) ? 6 : detected_counter + 1);
    return detected_counter == 5; // Check if the IR has detected 5 signals in a row
}