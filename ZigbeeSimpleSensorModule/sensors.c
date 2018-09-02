/*
    sensors.h - definitions relating to the various sensors attached to the system

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include "sensors.h"
#include "lib/gpio.h"
#include "platform.h"


// sensor_init() - initialise sensor system by configuring the SENSOR_nENABLE pin as an output, and
// negating the value on the pin via a call to sensor_activate().
//
void sensor_init()
{
    gpio_make_output(PIN_SENSOR_nENABLE);
    sensor_activate(0);
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
