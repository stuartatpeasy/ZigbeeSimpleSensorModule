#ifndef RTC_H_INC
#define RTC_H_INC
/*
    rtc.h - declarations for helper functions relating to the uC's real-time counter

    Stuart Wallace <stuartw@atom.net>, September 2018.
*/

#include <avr/io.h>


// RTCPrescaler_t - enumeration of possible values of the RTC clock prescaler.
//
typedef enum RTCPrescaler
{
    RTCPrescalerDiv_1       = RTC_PRESCALER_DIV1_gc,        // RTC prescaler = 1 (no division)
    RTCPrescalerDiv_2       = RTC_PRESCALER_DIV2_gc,        // RTC prescaler = 2
    RTCPrescalerDiv_4       = RTC_PRESCALER_DIV4_gc,        // RTC prescaler = 4
    RTCPrescalerDiv_8       = RTC_PRESCALER_DIV8_gc,        // RTC prescaler = 8
    RTCPrescalerDiv_16      = RTC_PRESCALER_DIV16_gc,       // RTC prescaler = 16
    RTCPrescalerDiv_32      = RTC_PRESCALER_DIV32_gc,       // RTC prescaler = 32
    RTCPrescalerDiv_64      = RTC_PRESCALER_DIV64_gc,       // RTC prescaler = 64
    RTCPrescalerDiv_128     = RTC_PRESCALER_DIV128_gc,      // RTC prescaler = 128
    RTCPrescalerDiv_256     = RTC_PRESCALER_DIV256_gc,      // RTC prescaler = 256
    RTCPrescalerDiv_512     = RTC_PRESCALER_DIV512_gc,      // RTC prescaler = 512
    RTCPrescalerDiv_1024    = RTC_PRESCALER_DIV1024_gc,     // RTC prescaler = 1024
    RTCPrescalerDiv_2048    = RTC_PRESCALER_DIV2048_gc,     // RTC prescaler = 2048
    RTCPrescalerDiv_4096    = RTC_PRESCALER_DIV4096_gc,     // RTC prescaler = 4096
    RTCPrescalerDiv_8192    = RTC_PRESCALER_DIV8192_gc,     // RTC prescaler = 8192
    RTCPrescalerDiv_16384   = RTC_PRESCALER_DIV16384_gc,    // RTC prescaler = 16384
    RTCPrescalerDiv_32768   = RTC_PRESCALER_DIV32768_gc     // RTC prescaler = 32768
} RTCPrescaler_t;


// RTCPITPeriod_t - enumeration of possible cycle-counts between RTC periodic interrupt timer
// interrupts.
//
typedef enum RTCPITPeriod
{
    RTCPITPeriod_Off        = RTC_PERIOD_OFF_gc,            // RTC PIT disabled
    RTCPITPeriod_4          = RTC_PERIOD_CYC4_gc,           // RTC PIT period = 4 cycles
    RTCPITPeriod_8          = RTC_PERIOD_CYC8_gc,           // RTC PIT period = 8 cycles
    RTCPITPeriod_16         = RTC_PERIOD_CYC16_gc,          // RTC PIT period = 16 cycles
    RTCPITPeriod_32         = RTC_PERIOD_CYC32_gc,          // RTC PIT period = 32 cycles
    RTCPITPeriod_64         = RTC_PERIOD_CYC64_gc,          // RTC PIT period = 64 cycles
    RTCPITPeriod_128        = RTC_PERIOD_CYC128_gc,         // RTC PIT period = 128 cycles
    RTCPITPeriod_256        = RTC_PERIOD_CYC256_gc,         // RTC PIT period = 256 cycles
    RTCPITPeriod_512        = RTC_PERIOD_CYC512_gc,         // RTC PIT period = 512 cycles
    RTCPITPeriod_1024       = RTC_PERIOD_CYC1024_gc,        // RTC PIT period = 1024 cycles
    RTCPITPeriod_2048       = RTC_PERIOD_CYC2048_gc,        // RTC PIT period = 2048 cycles
    RTCPITPeriod_4096       = RTC_PERIOD_CYC4096_gc,        // RTC PIT period = 4096 cycles
    RTCPITPeriod_8192       = RTC_PERIOD_CYC8192_gc,        // RTC PIT period = 8192 cycles
    RTCPITPeriod_16384      = RTC_PERIOD_CYC16384_gc,       // RTC PIT period = 16384 cycles
    RTCPITPeriod_32768      = RTC_PERIOD_CYC32768_gc        // RTC PIT period = 32768 cycles
} RTCPITPeriod_t;


// RTCClkSel_t - enumeration of possible values of the RTC clock-select register.
//
typedef enum RTCClkSel
{
    RTCClkInt32K    = RTC_CLKSEL_INT32K_gc,     // 32kHz from internal ultra-low power oscillator
    RTCClkInt1K     = RTC_CLKSEL_INT1K_gc,      // 1kHz from internal ultra-low power oscillator
    RTCClkOsc32K    = RTC_CLKSEL_TOSC32K_gc,    // 32kHz from external crystal oscillator
    RTCClkExt       = RTC_CLKSEL_EXTCLK_gc      // External clock via the TOSC1 pin
} RTCClkSel_t;


void rtc_set_clock(const RTCClkSel_t clock);
void rtc_set_prescaler(const RTCPrescaler_t prescaler);
void rtc_enable(const uint8_t enable);
void rtc_enable_pit(const uint8_t enable);
void rtc_enable_pit_irq(const uint8_t enable);
void rtc_set_pit_period(const RTCPITPeriod_t period);

#endif