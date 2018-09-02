/*
    ZigbeeSimpleSensorModule.c: entry point for the firmware which drives the Zigbee Simple Sensor
    Module.

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include "platform.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include "lib/adc.h"
#include "lib/clk.h"
#include "lib/debug.h"
#include "lib/gpio.h"
#include "lib/spi.h"
#include "lib/usart.h"
#include "lib/vref.h"
#include "sensors.h"
#include "xbee/xbee.h"


void handle_periodic_irq()
{
    static uint8_t counter = 0;

    gpio_set(PIN_LED);
    ++counter;

    sensor_read();

    if(!(counter & 0x07))
    {
        xbee_set_power_state(XBEE_WAKE);        // Signal the XBee module to awaken
        xbee_wait_power_state(XBEE_WAKE);       // Wait for the XBee module to wake up

        spi0_port_activate(1);                  // Activate SPI port pins
        spi0_enable(1);                         // Enable SPI interface

	    // Transmit data
        spi0_slave_select(1);
        // ... transmit all the datas ...

        spi0_slave_select(0);                   // De-select the SPI slave
        xbee_set_power_state(XBEE_SLEEP);       // Ask the XBee module to go to sleep
    }

    spi0_enable(0);                             // Disable SPI peripheral
    spi0_port_activate(0);                      // Deactivate the port

    gpio_clear(PIN_LED);
}


// ISR for interrupts from the periodic interrupt timer (PIT).  These interrupts are received every
// 8 seconds.
//
ISR(RTC_PIT_vect)
{
    handle_periodic_irq();
	RTC_PITINTFLAGS = 1;	                    // Clear periodic interrupt flag
}


// main() - entry point.
//
int main(void)
{
    cli();

    pclk_set_divisor_val(2);                        // Set peripheral clock = main clock / 2
    pclk_enable();                                  // Enable peripheral clock

    // Configure, activate and enable the SPI port
    spi0_configure_master(SPI_PINSET_ALTERNATE, SPI_CLK_DIV4);
    spi0_port_activate(1);
    spi0_enable(1);

    gpio_make_output(PIN_LED);                      // Make the LED control pin an output
    gpio_clear(PIN_LED);                            // Switch off the LED

    debug_init();                                   // Init debugging - this is a NOP in release mode

    //
    // Configure internal voltage reference and ADC
    //
    vref_set(VREF_ADC0, VREF_2V5);      		    // Select 2.5V internal reference for ADCs

    adc_set_vref(ADC_REF_INTERNAL, 1);              // Set ADC ref voltage and reduce sample cap
    adc_set_prescaler(ADC_PRESCALE_DIV_64);         // Set ADC clock = main clock / 64
    adc_set_initdelay(ADC_INITDELAY_64);            // Set ADC startup delay to 64 ADC clocks

    adc_configure_input(PIN_AIN_VBATT);             // }
    adc_configure_input(PIN_AIN_TEMP);              // } Configure analogue inputs as ADC input pins
    adc_configure_input(PIN_AIN_LIGHT);             // }

	//
	// Configure RTC and periodic interrupt timer (PIT)
	//

	// Select the 1kHz output from the ultra-low-power internal 32kHz oscillator as the source clock
	RTC_CLKSEL = RTC_CLKSEL_INT1K_gc;

	// Set the number of RTC clock cycles between periodic interrupts to 8192, giving 8.192 seconds
	// between PIT interrupts; also set the PIT enable flag.
	RTC_PITCTRLA = RTC_PERIOD_CYC8192_gc | RTC_PITEN_bm;

	RTC_PITINTCTRL = RTC_PI_bm;                     // Enable PIT interrupts

    sensor_init();                                  // Initialise sensors
    xbee_init();                                    // Initialise the XBee module interface
    xbee_configure();                               // Set initial configuration in the XBee module
    xbee_set_power_state(XBEE_SLEEP);               // Put the XBee module to sleep

    debug_flush();

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);            // Set the lowest-power sleep mode
	sei();                                          // Enable interrupts

    while(1)
		sleep_mode();                               // Await PIT wake-up interrupts
}
