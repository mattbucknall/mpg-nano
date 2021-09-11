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
#include <avr/io.h>

#include "app-io.h"


static volatile int16_t m_delta;


/**
 * Pin Change Interrupt 1 ISR. Updates delta count every time encoder inputs change state.
 */
ISR(PCINT1_vect) {
    static uint8_t prev_bits;

    // capture encoder input states
    uint8_t bits = PINC;

    // decode encoder inputs and adjust delta accordingly
    if ( (bits & (1 << APP_IO_C_ENC_AP)) && !(prev_bits & (1 << APP_IO_C_ENC_AP)) ) {
        if ( bits & (1 << APP_IO_C_ENC_BP) ) {
            m_delta--;
        } else {
            m_delta++;
        }
    }

    // save encoder input states for next iteration
    prev_bits = bits;
}


void app_encoder_reset(void) {
    // clear delta counter
    cli();
    m_delta = 0;
    sei();
}

int16_t app_encoder_delta(void) {
    int16_t delta;

    // capture and clear delta counter
    cli();
    delta = m_delta;
    m_delta = 0;
    sei();

    return delta;
}


void app_encoder_init(void) {
    // unmask encoder inputs for PCIE1
    PCMSK1 |= (1 << APP_IO_C_ENC_BP) | (1 << APP_IO_C_ENC_AP);

    // enable PCIE1
    PCICR |= (1 << PCIE1);
}
