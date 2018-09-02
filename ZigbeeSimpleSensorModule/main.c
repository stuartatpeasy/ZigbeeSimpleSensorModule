/*
    ZigbeeSimpleSensorModule.c: entry point for the firmware which drives the Zigbee Simple Sensor
    Module.

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include "platform.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lib/adc.h"
#include "lib/clk.h"
#include "lib/debug.h"
#include "lib/gpio.h"
#include "lib/spi.h"
#include "lib/usart.h"
#include "lib/vref.h"
#include "sensors.h"
#include "xbee/xbee.h"


typedef struct APIFrame
{
	uint8_t header;
	uint16_t len;
	uint8_t data;
} __attribute__((packed)) APIFrame_t;

//
// Port pin definitions
//
#define PORTA_AIN_LIGHT			(1 << 3)			// Analogue input for light sensor
#define PORTA_AIN_TEMP			(1 << 2)			// Analogue input for temperature sensor
#define PORTA_AIN_VBATT			(1 << 1)			// Analogue input for battery voltage sensor
#define PORTA_0_UNUSED			(1 << 0)

#define ADC_VBATT				ADC_CHANNEL_1       // Battery voltage input
#define ADC_TEMP				ADC_CHANNEL_2       // Thermistor input
#define ADC_LIGHT				ADC_CHANNEL_3       // Light sensor input


void handle_periodic_irq()
{
	uint16_t raw_vbatt, raw_temp, raw_light;

    gpio_set(PIN_LED);
    xbee_set_power_state(XBEE_WAKE);            // Wake XBee module
	sensor_activate(1);                         // Enable analogue sensors

    vref_enable(VREF_ADC0, 1);      			// Enable ADC voltage reference
    adc_enable(1);                              // Enable ADC module

	_delay_us(50);								// Wait for the sensors to stabilise

    raw_vbatt = adc_convert_channel(ADC_VBATT); // Measure battery voltage
    raw_temp = adc_convert_channel(ADC_TEMP);   // Measure temperature
    raw_light = adc_convert_channel(ADC_LIGHT); // Measure light level

    debug_putstr_p("vbatt=");
    debug_puthex_word(raw_vbatt);
    debug_putstr_p(" temp=");
    debug_puthex_word(raw_temp);
    debug_putstr_p(" light=");
    debug_puthex_word(raw_light);
    debug_putchar('\n');

    adc_enable(0);                              // Disable ADC
    vref_enable(VREF_ADC0, 0);          		// Disable voltage reference

	sensor_activate(0);                         // Disable analogue sensors

    xbee_wait_power_state(XBEE_WAKE);           // Wait for the XBee module to wake up

    spi0_port_activate(1);                      // Activate SPI port pins
    spi0_enable(1);                             // Enable SPI interface

	// Transmit data
    spi0_slave_select(1);
    // ... transmit all the datas ...

    spi0_slave_select(0);                       // De-select the SPI slave
    spi0_enable(0);                             // Disable SPI peripheral
    spi0_port_activate(0);                      // Deactivate the port

    xbee_set_power_state(XBEE_SLEEP);           // Ask the XBee module to go to sleep
    gpio_clear(PIN_LED);
}


ISR(RTC_PIT_vect)
{
    handle_periodic_irq();
	RTC_PITINTFLAGS = 1;	                    // Clear periodic interrupt flag
}


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
    sensor_init();                                  // Initialise sensors

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

    xbee_init();                                    // Initialise the XBee module interface
    xbee_configure();                               // Set initial configuration in the XBee module
    xbee_set_power_state(XBEE_SLEEP);               // Put the XBee module to sleep

    debug_flush();

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);            // Set the lowest-power sleep mode
	sei();                                          // Enable interrupts

    while(1)
		sleep_mode();                               // Await PIT wake-up interrupts
}
