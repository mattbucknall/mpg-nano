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

#ifndef _APP_IO_H_
#define _APP_IO_H_

#include <avr/io.h>

// Port B pin assignments
#define APP_IO_B_AXIS_Z         0
#define APP_IO_B_AXIS_4         1
#define APP_IO_B_NANO_LED       5

#define APP_IO_B_PORT_INIT      ((1 << 7) | (1 << 6) | (1 << 4) | (1 << 3) | (1 << 2) | \
                                (1 << APP_IO_B_AXIS_Z) | (1 << APP_IO_B_AXIS_4))

#define APP_IO_B_DDRB_INIT      (1 << APP_IO_B_NANO_LED)


// Port C pin assignments
#define APP_IO_C_ENC_AP         0
#define APP_IO_C_ENC_AN         1
#define APP_IO_C_ENC_BP         2
#define APP_IO_C_ENC_BN         3
#define APP_IO_C_AXIS_X         4
#define APP_IO_C_AXIS_Y         5


#define APP_IO_C_PORT_INIT      ((1 << APP_IO_C_AXIS_X) | (1 << APP_IO_C_AXIS_Y))

#define APP_IO_C_DDRC_INIT      0


// Port D pin assignments
#define APP_IO_D_RXD            0
#define APP_IO_D_TXD            1
#define APP_IO_D_X1             2
#define APP_IO_D_X10            3
#define APP_IO_D_X100           4
#define APP_IO_D_MPG_LED        5
#define APP_IO_D_ESTOP          6
#define APP_IO_D_RAPID          7

#define APP_IO_D_PORT_INIT      ((1 << APP_IO_D_TXD) | (1 << APP_IO_D_X1) | (1 << APP_IO_D_X10) | \
                                (1 << APP_IO_D_X100) | (1 << APP_IO_D_ESTOP) | (1 << APP_IO_D_RAPID))

#define APP_IO_D_DDRD_INIT      (1 << APP_IO_D_MPG_LED)


/**
 * Sets GPIO pin initial states, pull-ups and directions.
 */
void app_io_init(void);

#endif // _APP_IO_H_
