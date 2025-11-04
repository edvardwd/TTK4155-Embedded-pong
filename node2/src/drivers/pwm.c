#include "drivers/pwm.h"

void pwm_init(){
    // Activate peripheral device clock
    PMC->PMC_PCER0 |= (1 << ID_PIOB);
    PMC->PMC_PCER1 |= (1 << ID_PWM - 32);

    // Deactivate PIO
    PIOB->PIO_PDR |= PIO_PDR_P13;
    PIOB->PIO_ABSR |= PIO_PB13B_PWMH1; // Peripheral B for PB13

    // Set up PWM clock
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);
    // f_PWMCLK = MCK / DIVA = 84 Mhz / 84 = 1 MHz

    // CLKA channel mode, left-aligned (default)
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;


    /*
        CPRD = f_PWMCLK / f_signal
        CDTY = CPRD x duty
    */
    // Set period and duty cycle
    PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_CPRD_CPRD(20000); // 20 ms channel period
    PWM->PWM_CH_NUM[1].PWM_CDTY = PWM_CDTY_CDTY(1500); // 1.5ms channel duty cycle

    // Activate channel
    PWM->PWM_ENA = PWM_ENA_CHID1;
}