/*
    spi.c - definitions related to SPI helper functions

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include "spi.h"
#include "gpio.h"
#include "platform.h"


static GPIOPin_t SPI_nSS;


// spi_init() - configure the SPI peripheral by selecting a pin-set and a clock divider, calling
// spi0_configure_master() (which has additional side-effects - see the comments above that
// function), then activating and enabling the SPI port.
//
void spi0_init(const SPI_PINSET_t pinset, const SPI_CLK_DIV_t div)
{
    spi0_configure_master(pinset, div);
    spi0_port_activate(1);
    spi0_enable(1);
}


// spi0_configure_master() - configure the SPI0 peripheral as a master, and set its clock divider
// according to the value in <div>.  This function unconditionally sets certain options in the SPI
// peripheral: byte-order is hard-wired to MSB-first, 2x-clock mode is disabled, buffered mode is
// enabled, and nSS is ignored.
//
void spi0_configure_master(const SPI_PINSET_t pinset, const SPI_CLK_DIV_t div)
{
    if(pinset == SPI_PINSET_DEFAULT)
    {
        PORTMUX_CTRLB = PORTMUX_SPI0_DEFAULT_gc;
        SPI_nSS = PIN_SPI_nSS_DEFAULT;
    }
    else
    {
        PORTMUX_CTRLB = PORTMUX_SPI0_ALTERNATE_gc;
        SPI_nSS = PIN_SPI_nSS_ALT;
    }

    // Select SPI master mode and set prescaler.  Also: accept the default data order (MSB first),
    // and disable 2x-clock.  Do this without affecting the "enabled" state of the peripheral.
    SPI0_CTRLA = (SPI0_CTRLA & SPI_ENABLE_bm) | SPI_MASTER_bm | div;

    // Enable buffered mode, and instruct the peripheral to ignore the value at the nSS pin.
    SPI0_CTRLB = SPI_BUFEN_bm | SPI_SSD_bm;
}


void spi0_port_activate(const uint8_t activate)
{
    GPIOPin_t mosi, miso, sck;

    if(GPIOPIN_EQUAL(SPI_nSS, PIN_SPI_nSS_DEFAULT))
    {
        // Default pin-set
        mosi = PIN_SPI_MOSI_DEFAULT;
        miso = PIN_SPI_MISO_DEFAULT;
        sck = PIN_SPI_SCK_DEFAULT;
    }
    else
    {
        // Alternate pin-set
        mosi = PIN_SPI_MOSI_ALT;
        miso = PIN_SPI_MISO_ALT;
        sck = PIN_SPI_SCK_ALT;
    }

    // Set "resting" logic levels on the SPI port output pins
    gpio_set(SPI_nSS);
    gpio_clear(mosi);
    gpio_clear(sck);


    gpio_make_input(miso);          // MISO is always an input

    if(activate)
    {
        // Set pin directions for an active port: MOSI, SCK, nSS = outputs
        gpio_make_output(mosi);
        gpio_make_output(sck);
        gpio_make_output(SPI_nSS);
    }
    else
    {
        // Set pin directions for an inactive (sleeping) port: MOSI, SCK, nSS = inputs
        gpio_make_input(mosi);
        gpio_make_input(sck);
        gpio_make_input(SPI_nSS);
    }
}


// spi0_enable() - enable or disable the SPI0 peripheral.  If <enable> is non-zero, the peripheral
// is enabled; otherwise it is disabled.
//
void spi0_enable(const uint8_t enable)
{
    if(enable)
        SPI0_CTRLA |= SPI_ENABLE_bm;
    else
        SPI0_CTRLA &= ~SPI_ENABLE_bm;
}


// spi0_slave_select() - if <select> is non-zero, assert (i.e. set to logic 0) the SPI nSS line;
// otherwise negate (i.e. set to logic 1) the SPI nSS line.
//
void spi0_slave_select(const uint8_t select)
{
    if(select)
        gpio_clear(SPI_nSS);
    else
        gpio_set(SPI_nSS);
}
