/*
    xbee.c: definitions relating to XBee interface helper functions

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/

#include "xbee.h"
#include "../lib/debug.h"
#include "../lib/gpio.h"
#include "../lib/spi.h"
#include "../platform.h"
#include <util/delay.h>


XBeeTxnStatus_t xbee_send_at_command(const XBeeATCmd_t command, const uint8_t param_len);
XBeeTxnStatus_t xbee_receive_packet();
XBeeTxnStatus_t xbee_do_at_command(const XBeeATCmd_t command, const uint8_t param_len);


// XBeeCmdState_t - enumeration to express the state machine used for command
// transmission/reception
//
typedef enum XBeeCmdState
{
    XBeeCmdStateIdle,
    XBeeCmdStateHeader,
    XBeeCmdStateLen1,
    XBeeCmdStateLen2,
    XBeeCmdStateFrameType,
    XBeeCmdStateData,
    XBeeCmdStateCksum
} XBeeCmdState_t;


// xbee_init() - configure the sleep/wake and SPI_nATTN lines as inputs/outputs as appropriate, and
// set the XBee module SLEEP_RQ line low (requesting "awake" mode).  Also reset the length fields
// in the command-transmit/-receive buffer objects to indicate that no command is pending
// transmission or processing.
//
void xbee_init()
{
    gpio_make_input(PIN_XBEE_ON_nSLEEP);    // XBee's ON_nSLEEP output pin is an input to the uC
    gpio_make_input(PIN_XBEE_SPI_nATTN);    // XBee's SPI_nATTN output pin is an input to the uC
    gpio_clear(PIN_XBEE_SLEEP_RQ);          // } Wake the XBee by setting the XBEE_SLEEP_RQ pin to
    gpio_make_output(PIN_XBEE_SLEEP_RQ);    // } 0 and making the pin an output.

    xbee_tx.len = 0;                        // Indicates that there is nothing to transmit
    xbee_rx.len = 0;                        // Indicates that no packet has been received
}


// xbee_reset() - reset the XBee.  Do this by asserting the uC output connected to the XBee's
// nRESET input, then making that uC pin an output, then waiting XBEE_NRESET_ASSERT_MS
// milliseconds, then negating the nRESET pin, then waiting XBEE_NRESET_WAIT_MS milliseconds.
// Finally, make the uC output connected to nRESET an input, to reduce power consumption during
// sleep.  The pin must be connected to a pull-up resistor.
//
void xbee_reset()
{
    gpio_clear(PIN_XBEE_NRESET);
    gpio_make_output(PIN_XBEE_NRESET);
    _delay_ms(XBEE_NRESET_ASSERT_MS);
    gpio_set(PIN_XBEE_NRESET);
    _delay_ms(XBEE_NRESET_WAIT_MS);
//    gpio_make_input(PIN_XBEE_NRESET);
}


void xbee_set_power_state(const XBeePowerState_t state)
{
    if(state == XBeePowerStateWake)
        gpio_clear(PIN_XBEE_SLEEP_RQ);
    else if(state == XBeePowerStateSleep)
        gpio_set(PIN_XBEE_SLEEP_RQ);
}


void xbee_wait_power_state(const XBeePowerState_t state)
{
    if(state == XBeePowerStateWake)
        gpio_wait_high(PIN_XBEE_ON_nSLEEP);
    else if(state == XBeePowerStateSleep)
        gpio_wait_low(PIN_XBEE_ON_nSLEEP);
}


// xbee_spi_transaction() - attempt to receive a data frame via the SPI bus, and optionally
// simultaneously transmit a frame.  This function reads data from the buffer object in the global
// variable <xbee_tx>. If <frame_type> != XBeeFrameNone, then the transmit buffer  is assumed to
// contain data for an API frame of type <type> with ID <frame_id>.  In this case, the frame is
// transmitted.  During transmission, the XBee module may send data to us;  in this case, the data
// will be received in full (buffer space permitting) and written to the global <xbee_rx> buffer.
// If the XBee module attempts to send a frame longer than <xbee_rx.len>, the frame will be
// discarded.  If no frame transmission is requested (i.e. the function is called solely in order
// to attempt to receive data) then it will retry attempts to receive a start-of-frame delimiter.
// XBEE_RX_ONLY_RETRIES attempts will be made.  In cases where frame transmission is requested, no
// receive retries will be made.
//
// The function returns a bit-field in a uint8_t.  Zero or more of the following bits will be set:
//      XBEE_TX_SUCCESS         - a frame was successfully transmitted
//      XBEE_TX_FRAME_TOO_LONG  - the supplied frame exceeds the length of the transmit buffer;
//                                neither transmission or reception occur in this case
//      XBEE_TX_FRAME_EMPTY     - a frame of type other than XBEE_FRAME_NONE was specified, but its
//                                length equals zero
//      XBEE_RX_SUCCESS         - a frame was successfully received
//      XBEE_RX_FRAME_TOO_LONG  - a frame longer than the xbee_rx buffer was received and discarded
//      XBEE_RX_BAD_CHECKSUM    - the received packet checksum is invalid; the packet was discarded
//
XBeeTxnStatus_t xbee_spi_transaction()
{
    uint8_t txcount, rxcksum, txcksum, data, ret;
    int8_t retries;
    uint16_t packet_len;
    XBeeCmdState_t rxstate, txstate;

    packet_len = txcount = rxcksum = txcksum = data = ret = 0;

    // If the requested frame type is XBeeFrameNone, no frame will be transmitted but a frame may
    // still be received.  In this case, we enter the main loop with <txstate> equal to
    // XBEE_TX_DONE, which results in a stream of 0x00-value bytes being sent while packet
    // reception is in progress.  If a frame delimiter is not immediately received at the SPI port,
    // the loop will exit.
    if(xbee_tx.frame_type == XBeeFrameNone)
    {
        // We will only try to receive a frame - nothing will be transmitted.
        txstate = XBeeCmdStateIdle;         // No frame to transmit
        retries = XBEE_RX_ONLY_RETRIES;     // Number of times to wait for a frame delimiter
    }
    else
    {
        // We will be transmitting a frame, and possibly also receiving one.
        txstate = XBeeCmdStateHeader;       // Start by transmitting a frame header
        retries = 0;                        // No retries - don't hang around waiting for a frame

        // Size-validate the frame
        if(!xbee_tx.len || (xbee_tx.len >= XBEE_BUF_LEN))
            return XBEE_TX_BAD_FRAME_SIZE;
    }
    rxstate = XBeeCmdStateIdle;

    spi0_slave_select(1);                   // Assert the SPI slave-select output

    do
    {
        // Command-transmit state machine
        switch(txstate)
        {
            case XBeeCmdStateHeader:
                data = XBEE_FRAME_DELIMITER;
                txstate = XBeeCmdStateLen1;
                break;

            case XBeeCmdStateLen1:
                data = (xbee_tx.len + 1) >> 8;
                txstate = XBeeCmdStateLen2;
                break;

            case XBeeCmdStateLen2:
                data = (xbee_tx.len + 1) & 0xff;
                txstate = XBeeCmdStateFrameType;
                break;

            case XBeeCmdStateFrameType:
                data = xbee_tx.frame_type;
                txcksum += data;
                txstate = XBeeCmdStateData;
                break;

            case XBeeCmdStateData:
                data = xbee_tx.raw[txcount++];
                txcksum += data;
                if(txcount == xbee_tx.len)
                    txstate = XBeeCmdStateCksum;
                break;

            case XBeeCmdStateCksum:
                data = 0xff - txcksum;
                txstate = XBeeCmdStateIdle;
                break;

            case XBeeCmdStateIdle:
                data = 0x00;
                break;
        }

        spi0_tx(data);
        spi0_wait_tx();
        data = spi0_read();

        // Command-receive state machine
        switch(rxstate)
        {
            case XBeeCmdStateIdle:
                if(data == XBEE_FRAME_DELIMITER)
                {
                    xbee_rx.len = 0;
                    retries = 0;            // Found a frame - no need for any more retries
                    rxstate = XBeeCmdStateLen1;
                }
                break;

            case XBeeCmdStateHeader:
                // This state is not used in the "receive" state machine, and is unreachable.
                // This case is present in order to suppress a compiler warning.
                break;

            case XBeeCmdStateLen1:
                packet_len = data;
                packet_len <<= 8;
                rxstate = XBeeCmdStateLen2;
                break;

            case XBeeCmdStateLen2:
                packet_len |= data;
                rxstate = XBeeCmdStateFrameType;
                if(packet_len >= XBEE_BUF_LEN)
                    ret |= XBEE_RX_FRAME_TOO_LONG;
                break;

            case XBeeCmdStateFrameType:
                xbee_rx.frame_type = data;
                rxcksum += data;
                --packet_len;
                rxstate = XBeeCmdStateData;
                break;

            case XBeeCmdStateData:
                rxcksum += data;
                if(!--packet_len)
                    rxstate = XBeeCmdStateCksum;
                if(xbee_rx.len < XBEE_BUF_LEN)
                    xbee_rx.raw[xbee_rx.len++] = data;
                break;

            case XBeeCmdStateCksum:
                if((0xff - rxcksum) != data)
                    ret |= XBEE_RX_BAD_CHECKSUM;
                else if(!ret)
                    ret |= XBEE_RX_SUCCESS;
                rxstate = XBeeCmdStateIdle;
                break;
        }
    } while((txstate != XBeeCmdStateIdle) || (rxstate != XBeeCmdStateIdle) || (retries-- > 0));

    spi0_slave_select(0);                   // Negate the SPI slave-select output

    if(xbee_tx.frame_type == XBeeFrameNone)
    {
        // No frame transmission was requested, and the retry counter has expired.  Conclude that
        // we were expecting to receive a packet but didn't; set the appropriate error code.
        if(retries < 0)
            ret |= XBEE_RX_NO_DATA;
    }
    else
    {
        // In this case a frame transmission was requested.  If this point has been reached, the
        // transmission was successful.
        ret |= XBEE_TX_SUCCESS;
    }

    return ret;
}


// xbee_send_at_command() - send the AT command <command> to the XBee module.  Any command
// parameters must be populated into <xbee_tx.at.parameter_value[]> by the caller before calling
// this function.
//
XBeeTxnStatus_t xbee_send_at_command(const XBeeATCmd_t command, const uint8_t param_len)
{
    xbee_tx.frame_type = XBeeFrameATCommand;
    xbee_tx.at.cmd = command;
    xbee_tx.at.frame_id = 0x55;         // FIXME - move constant
    xbee_tx.len = param_len + sizeof(xbee_tx.at.cmd) + sizeof(xbee_tx.at.frame_id);

    return xbee_spi_transaction();
}


// xbee_receive_packet_no_wait() - transmit dummy data in order to receive a packet from the XBee
// device.  Do this without first waiting for the XBee to assert the SPI nATTN (attention) line.
//
XBeeTxnStatus_t xbee_receive_packet_no_wait()
{
    xbee_tx.frame_type = XBeeFrameNone;   // Set up a receive-only SPI transaction
    xbee_tx.len = 0;

    return xbee_spi_transaction();          // Attempt to receive the command response
}


// xbee_receive_packet() - wait for the XBee to assert the SPI nATTN (attention) line, then
// transmit dummy data in order to receive a packet from the XBee.
//
XBeeTxnStatus_t xbee_receive_packet()
{
    while(!xbee_attn())                     // Wait for the device to respond
        ;                                   // FIXME - implement a timeout

    return xbee_receive_packet_no_wait();
}


// xbee_do_at_command() - helper function that sends the AT command specified by <command>, waits
// for and then reads the response packet.  Command parameters must have been placed in the
// <xbee_tx> buffer by the caller.  The length of the parameter data must be passed in <param_len>.
//
XBeeTxnStatus_t xbee_do_at_command(const XBeeATCmd_t command, const uint8_t param_len)
{
    XBeeTxnStatus_t ret;

    ret = xbee_send_at_command(command, param_len);
    if(!(ret & XBEE_TX_SUCCESS))
        return ret;

    ret = xbee_receive_packet();
    if(ret & XBEE_RX_SUCCESS)
    {
        // Ensure that the received frame is an AT command response
        if(xbee_rx.frame_type != XBeeFrameATCommandResponse)
        {
            debug_printf("E: AT%c%c: unexpected response frame type %02x\n",
                            command, command >> 8, xbee_rx.frame_type);
            return XBEE_RX_WRONG_FRAME;
        }
        else if(xbee_rx.at_resp.status != XBeeATCmdOK)
            debug_printf("E: AT%c%c: cmd failed: %02x\n", command, command >> 8,
                            xbee_rx.at_resp.status);
        else
            debug_printf("I: AT%c%c: OK\n", command, command >> 8);
    }
    else
        debug_printf("E: AT%c%c: send failed: %02x\n", command, command >> 8, ret);

    return ret;
}


// xbee_configure() - send initial configuration commands to the XBee module.
//
void xbee_configure()
{
    uint8_t ret;

    while(1)
    {
        debug_putstr_p("Config start: XBee reset\n");
        xbee_reset();                       // Perform a hardware reset

        xbee_receive_packet_no_wait();      // Cause some SPI activity to provoke nATT assertion
        ret = xbee_receive_packet();
        if(!(ret & XBEE_RX_SUCCESS))
            continue;                       // Try again

        // The first received frame should be a modem status frame with its status byte set to
        // 0x00, indicating a hardware reset.
        if(!(ret & XBEE_RX_SUCCESS) || (xbee_rx.frame_type != XBeeFrameModemStatus) ||
           (xbee_rx.ms.status != XBeeModemStatusHardwareReset))
            continue;                       // Try again

        // Send an ATD9 command to configure pin DIO9 as ON/nSLEEP
        xbee_tx.at.parameter_value[0] = XBeePinCfgAlternateFunction;

        ret = xbee_do_at_command(XBeeATCmdATD9, 1);
        if(!(ret & XBEE_RX_SUCCESS) || (xbee_rx.at_resp.status != XBeeATCmdOK))
            continue;                       // Try again

        // Send an ATD8 command to configure pin DIO8 as DTR/SLEEP_RQ
        xbee_tx.at.parameter_value[0] = XBeePinCfgAlternateFunction;
        ret = xbee_do_at_command(XBeeATCmdATD8, 1);
        if(!(ret & XBEE_RX_SUCCESS) || (xbee_rx.at_resp.status != XBeeATCmdOK))
            continue;                       // Try again

        // TODO: set sleep mode (SM) = pin sleep
        xbee_tx.at.parameter_value[0] = XBeeSleepModePinSleep;
        ret = xbee_do_at_command(XBeeATCmdATSM, 1);
        if(!(ret & XBEE_RX_SUCCESS) || (xbee_rx.at_resp.status != XBeeATCmdOK))
            continue;                       // Try again

        break;
    }
}


#if 0
void xbee_dump_packet()
{
    uint8_t i;

    usart0_puts_p(PSTR("Pkt: ("));
    usart0_puthex_byte(xbee_rx.frame_type);
    usart0_tx(')');
    for(i = 0; i < xbee_rx.len; ++i)
    {
        usart0_tx(' ');
        usart0_puthex_byte(xbee_rx.raw[i]);
    }
    usart0_tx('\n');
}
#endif
