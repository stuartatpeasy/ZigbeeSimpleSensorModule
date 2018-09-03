#ifndef XBEE_H_INC
#define XBEE_H_INC
/*
    xbee.h: declarations relating to XBee interface helper functions

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include <stdint.h>
#include "atcommands.h"
#include "xbeeapi.h"


#define XBEE_RX_ONLY_RETRIES    (10)    // In receive-only mode: # times to wait for a delimiter
#define XBEE_NRESET_ASSERT_MS   (10)    // Length of time to assert XBee's nRESET pin, in ms
#define XBEE_NRESET_WAIT_MS     (50)    // Time to wait after negating XBee's nRESET pin, in ms


// Global transmit and receive packet buffers
XBeePacketBuf_t xbee_rx, xbee_tx;


// XBeePowerState_t - enumeration representing available XBee power states
typedef enum XBeePowerState
{
    XBeePowerStateSleep,
    XBeePowerStateWake
} XBeePowerState_t;


// XBeeTxnStatus_t - return type used by xbee_spi_transaction(), xbee_send_at_command(), etc.
typedef uint8_t XBeeTxnStatus_t;

// Flags used in the return value from xbee_spi_transaction(), xbee_send_at_command(), etc.
//
#define XBEE_TX_SUCCESS                 (0x01)      // Packet was transmitted successfully
#define XBEE_RX_SUCCESS                 (0x02)      // Packet received successfully during transmit
#define XBEE_TX_BAD_FRAME_SIZE          (0x04)      // Transmit frame too long, or zero-length
#define XBEE_RX_NO_DATA                 (0x08)      // Expected to receive a packet, but didn't
#define XBEE_RX_FRAME_TOO_LONG          (0x10)      // RX packet discarded - too long for buffer
#define XBEE_RX_BAD_CHECKSUM            (0x20)      // RX packet discarded - bad checksum
#define XBEE_RX_WRONG_FRAME             (0x40)      // RX packet contained unexpected frame
#define XBEE_TXRX_TIMEOUT               (0x80)      // Timeout occurred during command TX/RX


// xbee_attn() - macro expanding to a GPIO-reading function call which returns non-zero if the
// XBee device is requesting attention on the SPI port.
//
#define xbee_attn()     !gpio_read(PIN_XBEE_SPI_nATTN)

void xbee_init();
void xbee_reset();
void xbee_set_power_state(const XBeePowerState_t state);
void xbee_wait_power_state(const XBeePowerState_t state);
XBeeTxnStatus_t xbee_spi_transaction();
void xbee_configure();

#ifdef _DEBUG
void xbee_dump_packet();
#else
#define xbee_dump_packet()
#endif

#endif
