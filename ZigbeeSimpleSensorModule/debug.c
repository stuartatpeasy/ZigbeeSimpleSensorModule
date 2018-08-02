/*
    debug.c - various debugging functions

    Stuart Wallace <stuartw@atom.net>, August 2018.
*/

#include "debug.h"

#ifdef _DEBUG

#include <stdarg.h>
#include <stdio.h>


static char debug_buf[DEBUG_BUF_LEN];


// debug_init() - initialise debugging via USART0.
//
void debug_init()
{
    // Configure UART as debug channel
    usart0_configure_io(USART_PINSET_DEFAULT);
    usart0_set_baud_rate(DEBUG_BAUD_RATE);
    usart0_enable(USART_ENABLE_RX | USART_ENABLE_TX);
    usart0_puts_p(PSTR("\n\nDebug mode\n"));
    usart0_flush_tx();
}


// debug_printf() - a variant of printf() that writes to the debug channel (usually USART0).  Note
// that the length of the temporary buffer used by this function is specified by DEBUG_BUF_LEN, and
// is likely to be quite small.
//
void debug_printf(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    vsnprintf(debug_buf, DEBUG_BUF_LEN, fmt, ap);
    debug_putstr(debug_buf);

    va_end(ap);
}

#endif
