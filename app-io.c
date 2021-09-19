/*
 * MPG-Nano - Firmware and UCCNC plugin for Arduino Nano based serial-over-USB
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

#include "app-io.h"


void app_io_init(void) {
    PORTB = APP_IO_B_PORT_INIT;
    DDRB = APP_IO_B_DDRB_INIT;

    PORTC = APP_IO_C_PORT_INIT;
    DDRC = APP_IO_C_DDRC_INIT;

    PORTD = APP_IO_D_PORT_INIT;
    DDRD = APP_IO_D_DDRD_INIT;
}
