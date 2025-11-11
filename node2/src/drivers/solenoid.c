#include "drivers/solenoid.h"
#include "drivers/encoder.h"

void solenoid_init(){
    PIOA->PIO_PER |= SOLENOID_PIN;
    PIOA->PIO_OER |= SOLENOID_PIN;
    PIOA->PIO_SODR |= SOLENOID_PIN; // Initialize to in state
}


void solenoid_trigger(){
    PIOA->PIO_CODR |= SOLENOID_PIN; 
    delay_ms(50);
    PIOA->PIO_SODR |= SOLENOID_PIN;
}