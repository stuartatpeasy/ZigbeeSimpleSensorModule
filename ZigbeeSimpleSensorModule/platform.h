#ifndef PLATFORM_H_INC
#define PLATFORM_H_INC
/*
    platform.h: declarations relevant to both main.c and other modules

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#define F_CPU       16000000UL

//
// Pin definitions
//

// Port A
#define PIN_LED                     GPIOA(7)    // [O] Indicator LED
#define PIN_BUTTON                  GPIOA(6)    // [I] Pushbutton
// (unused)                         GPIOA(5)
#define PIN_SENSOR_nENABLE          GPIOA(4)    // [O] Enable signal for sensors
#define PIN_SPI_nSS_DEFAULT         GPIOA(4)    // [O] SPI slave select (default pin-set)
#define PIN_AIN_LIGHT               GPIOA(3)    // [I] Light sensor analogue input
#define PIN_SPI_SCK_DEFAULT         GPIOA(3)    // [O] SPI clock (default pin-set)
#define PIN_AIN_TEMP                GPIOA(2)    // [I] Temperature sensor analogue input
#define PIN_SPI_MISO_DEFAULT        GPIOA(2)    // [I] SPI MISO (default pin-set)
#define PIN_USART_RXD_ALT           GPIOA(2)    // [I] USART RXD (alternate pin-set)
#define PIN_AIN_VBATT               GPIOA(1)    // [I] Battery voltage sensor analogue input
#define PIN_SPI_MOSI_DEFAULT        GPIOA(1)    // [O] SPI MOSI (default pin-set)
#define PIN_USART_TXD_ALT           GPIOA(1)    // [O] USART TXD (alternate pin-set)
// (unused - nRESET/UPDI)           GPIOA(0)

// Port B
#define PIN_XBEE_SPI_nATTN          GPIOB(5)    // [I] XBee SPI nATTN (attention request) signal
#define PIN_XBEE_NRESET             GPIOB(4)    // [O] XBee nRESET
#define PIN_USART_RXD_DEFAULT       GPIOB(3)    // [I] USART RXD (default pin-set)
#define PIN_USART_TXD_DEFAULT       GPIOB(2)    // [O] USART TXD (default pin-set)
#define PIN_XBEE_ON_nSLEEP          GPIOB(1)    // [I] XBee ON/nSLEEP (awake/asleep) indicator
#define PIN_XBEE_SLEEP_RQ           GPIOB(0)    // [O] XBee sleep request

// Port C
#define PIN_SPI_nSS_ALT             GPIOC(3)    // [O] SPI slave select (alternate pin-set)
#define PIN_SPI_MOSI_ALT            GPIOC(2)    // [O] SPI MOSI (alternate pin-set)
#define PIN_SPI_MISO_ALT            GPIOC(1)    // [I] SPI MISO (alternate pin-set)
#define PIN_SPI_SCK_ALT             GPIOC(0)    // [O] SPI clock (alternate pin-set)

#endif
