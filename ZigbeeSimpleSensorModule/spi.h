#ifndef SPI_H_INC
#define SPI_H_INC
/*
    spi.h - declarations related to SPI helper functions

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include <avr/io.h>


// Constants used by spi0_configure_master() to determine whether to use the default or alternative
// pin-set for SPI0
typedef enum SPI_PINSET
{
    SPI_PINSET_DEFAULT = 0,
    SPI_PINSET_ALTERNATE = 1
} SPI_PINSET_t;


// Constants used by spi0_configure_master() to set the SPI clock prescaler
typedef enum SPI_CLK_DIV
{
    SPI_CLK_DIV4 = SPI_PRESC_DIV4_gc,
    SPI_CLK_DIV16 = SPI_PRESC_DIV16_gc,
    SPI_CLK_DIV64 = SPI_PRESC_DIV64_gc,
    SPI_CLK_DIV128 = SPI_PRESC_DIV128_gc
} SPI_CLK_DIV_t;


// spi0_flush_tx() - wait for SPI transmission to complete by busy-waiting on the TXCIF bit in the
// SPI INTFLAGS register.  After the bit becomes 1, transmission is complete; at this point a 1 is
// written to the register in order to clear it.
//
#define spi0_flush_tx()                                 \
            do                                          \
            {                                           \
                while(!(SPI0_INTFLAGS & SPI_TXCIF_bm))  \
                    ;                                   \
                SPI0_INTFLAGS |= SPI_TXCIF_bm;          \
            } while(0)


// spi0_wait_tx() - wait for the SPI data register to become available.  This register is available
// once its contents has been shifted into the SPI transmit shift register.  This macro busy-waits
// on the DREIF bit in the SPI INTFLAGS register.  The DREIF bit is set once it is safe to write
// another byte to the SPI DATA register.
//
#define spi0_wait_tx()                                  \
            do                                          \
            {                                           \
                while(!(SPI0_INTFLAGS & SPI_DREIF_bm))  \
                    ;                                   \
            } while(0)


// spi0_tx() - wait for the SPI data register to become available, and then write the value in
// <data> to the register.
//
#define spi0_tx(data)                                   \
            do                                          \
            {                                           \
                while(!(SPI0_INTFLAGS & SPI_DREIF_bm))  \
                    ;                                   \
                SPI0_DATA = data;                       \
            } while(0)


// spi0_read() - macro expanding to a dereferenced pointer to the SPI DATA register.  This provides
// a function-like interface to obtain the last byte shifted into the data register.
//
#define spi0_read()         SPI0_DATA


void spi0_init(const SPI_PINSET_t pinset, const SPI_CLK_DIV_t div);
void spi0_configure_master(const SPI_PINSET_t pinset, const SPI_CLK_DIV_t div);
void spi0_port_activate(const uint8_t activate);
void spi0_enable(const uint8_t enable);
void spi0_slave_select(const uint8_t select);

#endif
