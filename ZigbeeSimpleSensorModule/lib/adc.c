/*
    adc.c: definitions relating to the uC's ADC module

    Stuart Wallace <stuartw@atom.net>, September 2018.
*/

#include "adc.h"


// adc_set_vref() - specify the source of the voltage reference for the ADC module.  Also specify
// whether the internal sampling capacitor value should be reduced (this is recommended if the
// reference voltage is above 1V).
//
void adc_set_vref(const ADC_REF_t ref, const uint8_t reduce_sample_cap)
{
    uint8_t regval = (ADC0_CTRLC & ~ADC_REFSEL_gm) | (ref << ADC_REFSEL_gp);

    if(reduce_sample_cap)
        regval |= ADC_SAMPCAP_bm;
    else
        regval &= ~ADC_SAMPCAP_bm;

    ADC0_CTRLC = regval;
}


// adc_set_prescaler() - set the ADC clock prescaler division ratio.
//
void adc_set_prescaler(const ADC_PRESCALE_DIV_t div)
{
    ADC0_CTRLC = (ADC0_CTRLC & ~ADC_PRESC_gm) | (div << ADC_PRESC_gp);
}


// adc_set_initdelay() - set the delay, expressed as a number of ADC clock cycles, following ADC
// startup (or reference change) before the first conversion can occur.
//
void adc_set_initdelay(const ADC_INITDELAY_t delay)
{
    ADC0_CTRLD = (ADC0_CTRLD & ~ADC_INITDLY_gm) | (delay << ADC_INITDLY_gp);
}


// adc_enable() - enable (if <enable> is non-zero) or disable (if <enable> equals zero) the ADC.
//
void adc_enable(const uint8_t enable)
{
    const uint8_t mask = ADC_ENABLE_bm << ADC_ENABLE_bp;

    if(enable)
        ADC0_CTRLA |= mask;
    else
        ADC0_CTRLA &= ~mask;
}
