#ifndef ADC_H_INC
#define ADC_H_INC
/*
    adc.h: declarations relating to the uC's ADC module

    Stuart Wallace <stuartw@atom.net>, September 2018.
*/

#include <stdint.h>
#include <avr/io.h>


// ADC_REF_t - enumeration of possible voltage reference sources
//
typedef enum ADC_REF
{
    ADC_REF_INTERNAL    = 0,        // Use the internal VREF peripheral as a voltage reference
    ADC_REF_VDD         = 1         // Use VDD as a voltage reference
} ADC_REF_t;


// ADC_PRESCALE_DIV_t - enumeration of possible values of the ADC clock prescaler
//
typedef enum ADC_PRESCALE_DIV
{
    ADC_PRESCALE_DIV2   = ADC_PRESC_DIV2_gc,        // ADC clock = PCLK/2
    ADC_PRESCALE_DIV4   = ADC_PRESC_DIV4_gc,        // ADC clock = PCLK/4
    ADC_PRESCALE_DIV8   = ADC_PRESC_DIV8_gc,        // ADC clock = PCLK/8
    ADC_PRESCALE_DIV16  = ADC_PRESC_DIV16_gc,       // ADC clock = PCLK/16
    ADC_PRESCALE_DIV32  = ADC_PRESC_DIV32_gc,       // ADC clock = PCLK/32
    ADC_PRESCALE_DIV64  = ADC_PRESC_DIV64_gc,       // ADC clock = PCLK/64
    ADC_PRESCALE_DIV128 = ADC_PRESC_DIV128_gc,      // ADC clock = PCLK/128
    ADC_PRESCALE_DIV256 = ADC_PRESC_DIV256_gc       // ADC clock = PCLK/256
} ADC_PRESCALE_DIV_t;


typedef enum ADC_INITDELAY
{
    ADC_INITDELAY_0     = ADC_INITDLY_DLY0_gc,      // Init delay = 0 ADC CLK cycles
    ADC_INITDELAY_16    = ADC_INITDLY_DLY16_gc,     // Init delay = 16 ADC CLK cycles
    ADC_INITDELAY_32    = ADC_INITDLY_DLY32_gc,     // Init delay = 32 ADC CLK cycles
    ADC_INITDELAY_64    = ADC_INITDLY_DLY64_gc,     // Init delay = 64 ADC CLK cycles
    ADC_INITDELAY_128   = ADC_INITDLY_DLY128_gc,    // Init delay = 128 ADC CLK cycles
    ADC_INITDELAY_256   = ADC_INITDLY_DLY256_gc     // Init delay = 256 ADC CLK cycles
} ADC_INITDELAY_t;


void adc_set_vref(const ADC_REF_t ref, const uint8_t reduce_sample_cap);
void adc_set_prescaler(const ADC_PRESCALE_DIV_t div);
void adc_set_initdelay(const ADC_INITDELAY_t delay);
void adc_enable(const uint8_t enable);

#endif
