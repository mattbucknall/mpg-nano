/*
 * MPG-Nano - Firmware and UCCNC macro for Arduino Nano based serial-over-USB
 * interface for modified 4-axis Chinese MPG pendant.
 *
 * https://github.com/mattbucknall/mpg-nano
 *
 * Copyright (c) 2021 Matthew T. Bucknall
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISIN
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "app-encoder.h"
#include "app-io.h"
#include "app-serial.h"
#include "app-switch.h"


/**
 * Execution entry point and main loop.
 *
 * @note Never returns.
 */
int main(void) {
    static uint16_t heart_beat;

    // enable watchdog timer
    wdt_enable(WDTO_250MS);

    // start with all peripherals disabled (modules will enable any peripherals they use)
    power_all_disable();

    // initialise modules
    app_io_init();
    app_encoder_init();
    app_switch_init();
    app_serial_init();

    // enable interrupts globally
    sei();

    // enter main loop
    for (;;) {
        // toggle Nano LED every time heart_beat counter rolls over
        if ( heart_beat++ == 0 ) {
            PINB = (1 << APP_IO_B_NANO_LED);
        }

        // reset watchdog
        wdt_reset();

        // handle serial comms
        app_serial_loop();
        app_encoder_loop();
    }
}
