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

/**
 * Selector switch polling and MPG LED control
 */
#ifndef _APP_SWITCH_H_
#define _APP_SWITCH_H_

#include <stdbool.h>


/**
 * Enumeration of axis select switch positions.
 */
typedef enum {
    APP_SWITCH_AXIS_OFF,
    APP_SWITCH_AXIS_X,
    APP_SWITCH_AXIS_Y,
    APP_SWITCH_AXIS_Z,
    APP_SWITCH_AXIS_4
} app_switch_axis_t;


/**
 * Enumeration of step select switch positions.
 *
 * Uses:
 *   TIMER1
 */
typedef enum {
    APP_SWITCH_STEP_X1,
    APP_SWITCH_STEP_X10,
    APP_SWITCH_STEP_X100,
    APP_SWITCH_STEP_X1000 // <-- selected if MPG's side button is held down
} app_switch_step_t;


/**
 * Must be called once with interrupts globally disabled, before main loop begins.
 */
void app_switch_init(void);


/**
 * @return  True if e-stop switch is pressed.
 */
bool app_switch_e_stop(void);


/**
 * @return  Axis select switch position.
 */
app_switch_axis_t app_switch_axis(void);


/**
 * @return  Step select switch position.
 */
app_switch_step_t app_switch_step(void);

#endif // _APP_SWITCH_H_
