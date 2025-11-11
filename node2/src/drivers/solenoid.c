#include "drivers/solenoid.h"
#include "drivers/encoder.h"

void solenoid_init(){
    PIOA->PIO_PER |= SOLENOID_PIN; // IO
    PIOA->PIO_OER |= SOLENOID_PIN; // Output
}


void solenoid_trigger(){
    PIOA->PIO_SODR |= SOLENOID_PIN;
    delay_ms(50);
    // PIOA->PIO_SODR &= ~(SOLENOID_PIN);
    PIOA->PIO_CODR |= SOLENOID_PIN; 
}