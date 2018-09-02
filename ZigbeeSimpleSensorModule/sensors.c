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


#define ADC_VBATT				ADC_CHANNEL_1       // Battery voltage input
#define ADC_TEMP				ADC_CHANNEL_2       // Thermistor input
#define ADC_LIGHT				ADC_CHANNEL_3       // Light sensor input


// Struct which accumulates sensor values
typedef struct SensorReadings
{
    uint16_t    vbatt;
    uint16_t    light;
    uint16_t    temp;
} SensorReadings_t;

static SensorReadings_t acc;
static const uint8_t avg_len = 8;


// sensor_init() - initialise sensor system by configuring the SENSOR_nENABLE pin as an output,
// and performing an initial read of the sensors.
//
void sensor_init()
{
    acc.vbatt = 0;
    acc.light = 0;
    acc.temp = 0;

    gpio_make_output(PIN_SENSOR_nENABLE);
    sensor_read();

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

    vref_enable(VREF_ADC0, 1);      			// Enable ADC voltage reference
    adc_enable(1);                              // Enable ADC module

	_delay_us(50);								// Wait for the sensors to stabilise

    acc.light -= ((acc.light + (avg_len / 2)) / avg_len);
    acc.light += adc_convert_channel(ADC_LIGHT);

    acc.temp -= ((acc.temp + (avg_len / 2)) / avg_len);
    acc.temp += adc_convert_channel(ADC_TEMP);

    acc.vbatt -= ((acc.vbatt + (avg_len / 2)) / avg_len);
    acc.vbatt += adc_convert_channel(ADC_VBATT);

    adc_enable(0);                              // Disable ADC
    vref_enable(VREF_ADC0, 0);          		// Disable voltage reference

	sensor_activate(0);                         // Disable analogue sensors

    debug_putstr_p("vbatt=");
    debug_puthex_word((acc.vbatt + (avg_len / 2)) / avg_len);
    debug_putstr_p(" temp=");
    debug_puthex_word((acc.temp + (avg_len / 2)) / avg_len);
    debug_putstr_p(" light=");
    debug_puthex_word((acc.light + (avg_len / 2)) / avg_len);
    debug_putchar('\n');
}
