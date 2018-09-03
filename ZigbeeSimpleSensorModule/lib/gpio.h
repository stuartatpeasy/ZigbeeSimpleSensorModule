#ifndef GPIO_H_INC
#define GPIO_H_INC
/*
    gpio.h - declarations for helper functions relating to GPIO ports

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include <stdint.h>
#include <avr/io.h>


// GPIOPort_t - enumeration of GPIO ports.  This list is specific to the smaller ATTiny models.
typedef enum GPIOPort
{
    GPIOPortA = 0,
    GPIOPortB,
    GPIOPortC
} GPIOPort_t;


// GPIOPin_t - struct representing a pin+port combination.
//
typedef struct GPIOPin
{
    GPIOPort_t port:5;
    unsigned int pin:3;
} GPIOPin_t;


// GPIOAction_t - list of allowable GPIO "actions".  These will be used, together with a port
// identifier, by gpio_do_action() when it is calculating a register address.
//
typedef enum GPIOAction
{
    GPIOAction_DIRSET = 0,
    GPIOAction_DIRCLR = 1,
    GPIOAction_OUTSET = 2,
    GPIOAction_OUTCLR = 3,
    GPIOAction_READ = 4,
    GPIOAction_DIRGET = 5,
    GPIOAction_PINCTRL = 6,
    GPIOAction_end
} GPIOAction_t;


// GPIOSense_t - list of possible pin input/sense configurations.
//
typedef enum GPIOSense
{
    GPIOSense_INTDISABLE    = PORT_ISC_INTDISABLE_gc,
    GPIOSense_BOTHEDGES     = PORT_ISC_BOTHEDGES_gc,
    GPIOSense_RISING        = PORT_ISC_RISING_gc,
    GPIOSense_FALLING       = PORT_ISC_FALLING_gc,
    GPIOSense_INPUTDISABLE  = PORT_ISC_INPUT_DISABLE_gc,
    GPIOSense_LEVEL         = PORT_ISC_LEVEL_gc
} GPIOSense_t;


#define GPIO_PIN_MAX            (7)
#define GPIO_PORT_MAX           (GPIOPortC)        // Highest port on ATTiny{4|8}16

// Macro enabling the construction of a GPIOPin_t struct from a port specifier and a pin number
#define GPIOPIN(port_, pin_)    ((GPIOPin_t) { .port = port_, \
                                               .pin = ((uint8_t) pin_) & GPIO_PIN_MAX})

// Further macros enabling the construction of a GPIOPin_t struct from a port name and a pin number
#define GPIOA(pin_)             GPIOPIN(GPIOPortA, pin_)
#define GPIOB(pin_)             GPIOPIN(GPIOPortB, pin_)
#define GPIOC(pin_)             GPIOPIN(GPIOPortC, pin_)

// Macro to test whether two GPIOPort_t objects refer to the same port and pin
#define GPIOPIN_EQUAL(a, b)     (((a).port == (b).port) && ((a).pin == (b).pin))

void gpio_action_write(const GPIOPin_t pin, const GPIOAction_t action);
uint8_t gpio_action_read(const GPIOPin_t pin, const GPIOAction_t action);

#define gpio_make_output(pin)   gpio_action_write(pin, GPIOAction_DIRSET)
#define gpio_make_input(pin)    gpio_action_write(pin, GPIOAction_DIRCLR)
#define gpio_set(pin)           gpio_action_write(pin, GPIOAction_OUTSET)
#define gpio_clear(pin)         gpio_action_write(pin, GPIOAction_OUTCLR)
#define gpio_read(pin)          gpio_action_read(pin, GPIOAction_READ)
#define gpio_get_dir(pin)       gpio_action_read(pin, GPIOAction_DIRGET)

void gpio_wait_high(const GPIOPin_t pin);
void gpio_wait_low(const GPIOPin_t pin);
void gpio_set_sense(const GPIOPin_t pin, const GPIOSense_t sense);

#endif
