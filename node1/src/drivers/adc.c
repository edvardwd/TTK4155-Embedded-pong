#include "drivers/adc.h"
/////////////////////////////////////////////

void adc_init(){
    // Init clock
    DDRD |= (1 << PD5); // Sets pin PD5 as  output
    TCCR1A |= (1 << COM1A0); // Toggle OC1A (Output compare match A)
    TCCR1B |= (1 << WGM12) | (1 << CS10); // Sets timer to CTC-modus | prescaler = 1
    OCR1A = 0; // Comparevalue 

    DDRB &= ~(1<<DDB0);
    PORTB &= ~(1<<PORTB0);
}


volatile uint8_t adc_read(){
    return *ADC;
}
