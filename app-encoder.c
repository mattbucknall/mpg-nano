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


static int16_t m_delta;
static uint8_t m_prev_bits;


void app_encoder_reset(void) {
    // clear delta counter
    m_delta = 0;
}


int16_t app_encoder_delta(void) {
    int16_t delta;

    // capture and clear delta counter
    delta = m_delta;
    m_delta = 0;

    return delta;
}


void app_encoder_loop(void) {
    static uint8_t phase;
    static uint8_t hyst;

    // not using PROGMEM because this LUT is small and better off in RAM
    static const int8_t LUT[16] = {
             0,  1, -1,  0,
            -1,  0,  0,  1,
             1,  0,  0, -1,
             0, -1,  1,  0
    };

    // capture encoder input states
    uint8_t bits = PINC;
    int8_t dir;

    // decode direction of quadrature transition
    bits = (bits & (1 << APP_IO_C_ENC_AP)) | ((bits & (1 << APP_IO_C_ENC_BP)) >> 1);
    dir = LUT[bits | m_prev_bits];
    m_prev_bits = bits << 2;

    // update phase
    phase = (phase + dir) & 3;

    // apply hysteresis and update delta
    if ( phase == 0 && !hyst ) {
        if ( dir > 0 ) {
            m_delta++;
        } else if ( dir < 0 ) {
            m_delta--;
        }

        hyst = 1;
    }
    else if ( phase == 2 ) {
        hyst = 0;
    }
}


void app_encoder_init(void) {
    uint8_t bits;

    // capture initial state
    bits = PINC;
    m_prev_bits = (bits & (1 << APP_IO_C_ENC_AP)) | ((bits & (1 << APP_IO_C_ENC_BP)) >> 1);
    m_prev_bits = m_prev_bits << 2;
}
