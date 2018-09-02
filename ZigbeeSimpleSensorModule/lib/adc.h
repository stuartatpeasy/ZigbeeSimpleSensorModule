#ifndef ADC_H_INC
#define ADC_H_INC
/*
    adc.h: declarations relating to the uC's ADC module

    Stuart Wallace <stuartw@atom.net>, September 2018.
*/

#include <stdint.h>
#include <avr/io.h>
#include "gpio.h"


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
    ADC_PRESCALE_DIV_2      = ADC_PRESC_DIV2_gc,        // ADC clock = PCLK/2
    ADC_PRESCALE_DIV_4      = ADC_PRESC_DIV4_gc,        // ADC clock = PCLK/4
    ADC_PRESCALE_DIV_8      = ADC_PRESC_DIV8_gc,        // ADC clock = PCLK/8
    ADC_PRESCALE_DIV_16     = ADC_PRESC_DIV16_gc,       // ADC clock = PCLK/16
    ADC_PRESCALE_DIV_32     = ADC_PRESC_DIV32_gc,       // ADC clock = PCLK/32
    ADC_PRESCALE_DIV_64     = ADC_PRESC_DIV64_gc,       // ADC clock = PCLK/64
    ADC_PRESCALE_DIV_128    = ADC_PRESC_DIV128_gc,      // ADC clock = PCLK/128
    ADC_PRESCALE_DIV_256    = ADC_PRESC_DIV256_gc       // ADC clock = PCLK/256
} ADC_PRESCALE_DIV_t;


// ADC_INITDELAY_t - enumeration of possible values of the initialisation delay.  This delay is
// applied when the ADC starts up, or the reference changes, so that the ADC input may settle
// before the first conversion is performed.
//
typedef enum ADC_INITDELAY
{
    ADC_INITDELAY_0     = ADC_INITDLY_DLY0_gc,          // Init delay = 0 ADC CLK cycles
    ADC_INITDELAY_16    = ADC_INITDLY_DLY16_gc,         // Init delay = 16 ADC CLK cycles
    ADC_INITDELAY_32    = ADC_INITDLY_DLY32_gc,         // Init delay = 32 ADC CLK cycles
    ADC_INITDELAY_64    = ADC_INITDLY_DLY64_gc,         // Init delay = 64 ADC CLK cycles
    ADC_INITDELAY_128   = ADC_INITDLY_DLY128_gc,        // Init delay = 128 ADC CLK cycles
    ADC_INITDELAY_256   = ADC_INITDLY_DLY256_gc         // Init delay = 256 ADC CLK cycles
} ADC_INITDELAY_t;


// ADC_CHANNEL_t - enumeration of possible ADC input channel values.
//
typedef enum ADC_CHANNEL
{
    ADC_CHANNEL_0           = ADC_MUXPOS_AIN0_gc,       // ADC channel 0
    ADC_CHANNEL_1           = ADC_MUXPOS_AIN1_gc,       // ADC channel 1
    ADC_CHANNEL_2           = ADC_MUXPOS_AIN2_gc,       // ADC channel 2
    ADC_CHANNEL_3           = ADC_MUXPOS_AIN3_gc,       // ADC channel 3
    ADC_CHANNEL_4           = ADC_MUXPOS_AIN4_gc,       // ADC channel 4
    ADC_CHANNEL_5           = ADC_MUXPOS_AIN5_gc,       // ADC channel 5
    ADC_CHANNEL_6           = ADC_MUXPOS_AIN6_gc,       // ADC channel 6
    ADC_CHANNEL_7           = ADC_MUXPOS_AIN7_gc,       // ADC channel 7
    ADC_CHANNEL_8           = ADC_MUXPOS_AIN8_gc,       // ADC channel 8
    ADC_CHANNEL_9           = ADC_MUXPOS_AIN9_gc,       // ADC channel 9
    ADC_CHANNEL_10          = ADC_MUXPOS_AIN10_gc,      // ADC channel 10
    ADC_CHANNEL_11          = ADC_MUXPOS_AIN11_gc,      // ADC channel 11
    ADC_CHANNEL_DAC0        = ADC_MUXPOS_DAC0_gc,       // Connect ADC to DAC output
    ADC_CHANNEL_INTREF      = ADC_MUXPOS_INTREF_gc,     // Connect ADC to internal voltage ref
    ADC_CHANNEL_TEMPSENSE   = ADC_MUXPOS_TEMPSENSE_gc,  // Connect ADC to on-chip temp sensor
    ADC_CHANNEL_GND         = ADC_MUXPOS_GND_gc         // Connect ADC to ground
} ADC_CHANNEL_t;


void adc_set_vref(const ADC_REF_t ref, const uint8_t reduce_sample_cap);
void adc_set_prescaler(const ADC_PRESCALE_DIV_t div);
void adc_set_initdelay(const ADC_INITDELAY_t delay);
void adc_enable(const uint8_t enable);
void adc_set_channel(const ADC_CHANNEL_t channel);
uint16_t adc_convert();
uint16_t adc_convert_channel(const ADC_CHANNEL_t channel);
void adc_configure_input(const GPIOPin_t pin);

#endif
