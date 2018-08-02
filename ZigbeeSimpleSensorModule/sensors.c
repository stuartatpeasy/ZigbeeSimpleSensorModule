/*
    sensors.h - definitions relating to the various sensors attached to the system

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include "sensors.h"
#include "gpio.h"
#include "platform.h"


void sensor_init()
{
    gpio_make_output(PIN_SENSOR_nENABLE);
}


void sensor_activate(const uint8_t activate)
{
    if(activate)
        gpio_clear(PIN_SENSOR_nENABLE);
    else
        gpio_set(PIN_SENSOR_nENABLE);
}
