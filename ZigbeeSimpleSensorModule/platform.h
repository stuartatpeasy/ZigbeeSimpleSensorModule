#ifndef PLATFORM_H_INC
#define PLATFORM_H_INC
/*
    platform.h: declarations relevant to both main.c and other modules

    Stuart Wallace <stuartw@atom.net>, July 2018.

    Note that these declarations make use of the GPIO*() macros defined in lib/gpio.h.
*/

#include "platform_attinyX16.h"

#define F_CPU       16000000UL

//
// Pin definitions
//

// Port A
#define PIN_LED                     GPIOA(7)    // [O] Indicator LED
#define PIN_BUTTON                  GPIOA(6)    // [I] Pushbutton
// (unused)                         GPIOA(5)
#define PIN_SENSOR_nENABLE          GPIOA(4)    // [O] Enable signal for sensors
#define PIN_AIN_LIGHT               GPIOA(3)    // [I] Light sensor analogue input
#define PIN_AIN_TEMP                GPIOA(2)    // [I] Temperature sensor analogue input
#define PIN_AIN_VBATT               GPIOA(1)    // [I] Battery voltage sensor analogue input
// (unused - nRESET/UPDI)           GPIOA(0)

// Port B
#define PIN_XBEE_SPI_nATTN          GPIOB(5)    // [I] XBee SPI nATTN (attention request) signal
#define PIN_XBEE_NRESET             GPIOB(4)    // [O] XBee nRESET
#define PIN_XBEE_ON_nSLEEP          GPIOB(1)    // [I] XBee ON/nSLEEP (awake/asleep) indicator
#define PIN_XBEE_SLEEP_RQ           GPIOB(0)    // [O] XBee sleep request

#endif

