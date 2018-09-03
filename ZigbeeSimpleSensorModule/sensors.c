/*
    sensors.c - definitions relating to the various sensors attached to the system

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include "sensors.h"
#include "lib/adc.h"
#include "lib/debug.h"
#include "lib/gpio.h"
#include "lib/vref.h"
#include "platform.h"
#include <util/delay.h>


#define ADCVBatt				ADCChannel1         // Battery voltage input
#define ADCTemp				    ADCChannel2         // Thermistor input
#define ADCLight				ADCChannel3         // Light sensor input


// Struct which accumulates sensor values
typedef struct SensorReadings
{
    uint16_t    vbatt;
    uint16_t    light;
    uint16_t    temp;
} SensorReadings_t;

static SensorReadings_t acc;
static const uint8_t avg_len = 8;


// sensor_init() - initialise sensor system by configuring voltage reference and ADC modules,
// making the SENSOR_nENABLE pin an output, and performing an initial read of the sensors.
//
void sensor_init()
{
    // Configure voltage reference module
    vref_set(VRefADC0, VRef2V5);      		        // Select 2.5V internal reference for ADCs

    // Configure ADC module
    adc_set_vref(ADCRefInternal, 1);                // Set ADC ref voltage and reduce sample cap
    adc_set_prescaler(ADCPrescaleDiv64);            // Set ADC clock = main clock / 64
    adc_set_initdelay(ADCInitDelay64);              // Set ADC startup delay to 64 ADC clocks

    adc_configure_input(PIN_AIN_VBATT);             // }
    adc_configure_input(PIN_AIN_TEMP);              // } Configure analogue inputs as ADC input pins
    adc_configure_input(PIN_AIN_LIGHT);             // }

    acc.vbatt = 0;
    acc.light = 0;
    acc.temp = 0;

    gpio_make_output(PIN_SENSOR_nENABLE);
    sensor_read();

    // Multiply the first set of readings by the length of the moving average, so that the global
    // accumulator contains a correctly-scaled value.  This value will always be divided by
    // <avg_len> before being used.
    acc.vbatt *= avg_len;
    acc.light *= avg_len;
    acc.temp *= avg_len;
}


// sensor_activate() - activate (if <activate> is non-zero) or de-activate (if <activate> equals
// zero) sensors by asserting or negating the SENSOR_nENABLE output pin.
//
void sensor_activate(const uint8_t activate)
{
    if(activate)
        gpio_clear(PIN_SENSOR_nENABLE);
    else
        gpio_set(PIN_SENSOR_nENABLE);
}


// sensor_read() - activate sensors, ADC, and the VREF module; read sensors; update the moving
// average values in the global struct <acc>; deactivate sensors, ADC and the VREF module.
//
void sensor_read()
{
	sensor_activate(1);                         // Enable analogue sensors

    vref_enable(VRefADC0, 1);      			// Enable ADC voltage reference
    adc_enable(1);                              // Enable ADC module

	_delay_us(50);								// Wait for the sensors to stabilise

    acc.light -= ((acc.light + (avg_len / 2)) / avg_len);
    acc.light += adc_convert_channel(ADCLight);

    acc.temp -= ((acc.temp + (avg_len / 2)) / avg_len);
    acc.temp += adc_convert_channel(ADCTemp);

    acc.vbatt -= ((acc.vbatt + (avg_len / 2)) / avg_len);
    acc.vbatt += adc_convert_channel(ADCVBatt);

    adc_enable(0);                              // Disable ADC
    vref_enable(VRefADC0, 0);          		// Disable voltage reference

	sensor_activate(0);                         // Disable analogue sensors

    debug_putstr_p("vbatt=");
    debug_puthex_word((acc.vbatt + (avg_len / 2)) / avg_len);
    debug_putstr_p(" temp=");
    debug_puthex_word((acc.temp + (avg_len / 2)) / avg_len);
    debug_putstr_p(" light=");
    debug_puthex_word((acc.light + (avg_len / 2)) / avg_len);
    debug_putchar('\n');
}
