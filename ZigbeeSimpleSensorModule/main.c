/*
    ZigbeeSimpleSensorModule.c: entry point for the firmware which drives the Zigbee Simple Sensor
    Module.

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include "platform.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
//#include "atcommands.h"
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

#define ADC_VBATT				ADC_MUXPOS_AIN1_gc	// Battery voltage input
#define ADC_TEMP				ADC_MUXPOS_AIN2_gc	// Thermistor input
#define ADC_LIGHT				ADC_MUXPOS_AIN3_gc	// Light sensor input

uint16_t adc_do_conversion(const ADC_MUXPOS_t channel);


// adc_do_conversion() - perform a conversion on the channel specific by <channel>.  Busy-wait
// until the conversion is complete, and return the result.
//
uint16_t adc_do_conversion(const ADC_MUXPOS_t channel)
{
	ADC0_MUXPOS = channel;						// Set channel multiplexer
	ADC0_COMMAND |= ADC_STCONV_bm;				// Start conversion

	while(ADC0_COMMAND & ADC_STCONV_bm)			// Wait for conversion to complete
		;

	return ADC0_RES;
}


void do_stuff()
{
#if 0
//	uint16_t raw_vbatt, raw_temp, raw_light;

	sensor_activate(1);                         // Enable analogue sensors

    vref_enable(VREF_ADC0, 1);      			// Enable ADC voltage reference
    adc_enable(1);                              // Enable ADC module

	_delay_us(50);								// Wait for the sensors to stabilise

//	raw_vbatt = adc_do_conversion(ADC_VBATT);	// Measure battery voltage
//	raw_temp = adc_do_conversion(ADC_TEMP);		// Measure temperature
//	raw_light = adc_do_conversion(ADC_LIGHT);	// Measure light level

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
#endif

    ////////////////////////////////// temporary code //////////////////////////////
//    uint8_t ret;

    gpio_set(PIN_LED);
    xbee_wait_power_state(XBEE_WAKE);           // Wait for the XBee module to wake up

    spi0_enable(1);                             // Enable SPI interface

    spi0_enable(0);                             // Disable SPI peripheral

    gpio_clear(PIN_LED);
    ////////////////////////////////// temporary code //////////////////////////////
}


ISR(RTC_PIT_vect)
{
    xbee_set_power_state(XBEE_WAKE);            // Wake XBee module

    do_stuff();

    xbee_set_power_state(XBEE_SLEEP);           // Ask the XBee module to go to sleep

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

    debug_init();

	//
	// Configure ADCs (Port A pins 1..3)
	//
	PORTA_PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTA_PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
	PORTA_PIN3CTRL = PORT_ISC_INPUT_DISABLE_gc;

    // Configure clock

    //
    // Configure internal voltage reference and ADC
    //
    vref_set(VREF_ADC0, VREF_2V5);      		    // Select 2.5V internal reference for ADCs

    adc_set_vref(ADC_REF_INTERNAL, 1);              // Set ADC ref voltage and reduce sample cap
    adc_set_prescaler(ADC_PRESCALE_DIV64);          // Set ADC clock = main clock / 64
    adc_set_initdelay(ADC_INITDELAY_64);

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

    // TODO - do any necessary initialisation of the XBee module
    xbee_set_power_state(XBEE_SLEEP);               // Put the XBee module to sleep

    debug_flush();

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);            // Set the lowest-power sleep mode
	sei();                                          // Enable interrupts

    while(1)
		sleep_mode();                               // Await PIT wake-up interrupts
}
