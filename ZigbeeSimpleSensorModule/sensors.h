#ifndef SENSORS_H_INC
#define SENSORS_H_INC
/*
    sensors.h - declarations relating to the various sensors attached to the system

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include <stdint.h>


void sensor_init();
void sensor_activate(const uint8_t activate);
void sensor_read();

#endif
