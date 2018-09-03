#ifndef DEBUG_H_INC
#define DEBUG_H_INC
/*
    debug.h - various debugging macros and declarations

    Stuart Wallace <stuartw@atom.net>, August 2018.
*/

#include "usart.h"
#include <avr/pgmspace.h>


#ifdef _DEBUG

#define DEBUG_BAUD_RATE         (230400)
#define DEBUG_BUF_LEN           (64)

#define debug_putstr(str)       usart0_puts(str)
#define debug_putstr_p(str)     usart0_puts_p(PSTR(str))
#define debug_putchar(ch)       usart0_tx(ch)
#define debug_puthex_byte(b)    usart0_puthex_byte(b)
#define debug_puthex_word(w)    usart0_puthex_word(w)
#define debug_flush()           usart0_flush_tx()

void debug_init();
void debug_printf(char *fmt, ...);

#else

#define debug_putstr_p(str)
#define debug_putchar(ch)
#define debug_puthex_byte(b)
#define debug_puthex_word(w)
#define debug_flush()

#define debug_init()
#define debug_printf(fmt, ...)

#endif  // _DEBUG

#endif
