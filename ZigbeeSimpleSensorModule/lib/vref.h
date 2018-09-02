#ifndef VREF_H_INC
#define VREF_H_INC
/*
    vref.h: declarations relating to the uC's voltage reference module

    Stuart Wallace <stuartw@atom.net>, September 2018.
*/

#include <stdint.h>


// VREF_PERIPHERAL_t - enumeration specifying the peripherals connected to the VREF module.
//
typedef enum VREF_PERIPHERAL
{
    VREF_ADC0,
    VREF_DAC0
} VREF_PERIPHERAL_t;


// VREF_VOLTAGE_t - enumeration of the various voltages supported by the VREF peripeheral.
//
typedef enum VREF_VOLTAGE
{
    VREF_0V55   = 0,
    VREF_1V1    = 1,
    VREF_2V5    = 2,
    VREF_4V3    = 3,
    VREF_1V5    = 4
} VREF_VOLTAGE_t;


void vref_set(const VREF_PERIPHERAL_t peripheral, const VREF_VOLTAGE_t voltage);
void vref_enable(const VREF_PERIPHERAL_t peripheral, const uint8_t enable);

#endif
