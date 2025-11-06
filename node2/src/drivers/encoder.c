#include "drivers/encoder.h"

void encoder_init(){
  PMC->PMC_PCER1 |= PMC_PCER1_PID33;  //  Enable timer counter clock for TC channel 6
  PMC->PMC_PCER0 |= PMC_PCER0_PID13; // PIOC = ID13

  PIOC->PIO_PDR |= (PIO_PDR_P25 | PIO_PDR_P26);
  PIOC->PIO_ABSR |= (PIO_ABSR_P25 | PIO_ABSR_P26);

  TC2->TC_BMR = TC_BMR_QDEN 
              | TC_BMR_POSEN 
              | TC_BMR_EDGPHA
              | TC_BMR_TC0XC0S_TIOA1;
  
  TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0;
  TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

volatile int32_t encoder_get_motor_position(){
  return (volatile int32_t) TC2->TC_CHANNEL[0].TC_CV;
}

void encoder_reset(){
  TC2->TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG;
}
