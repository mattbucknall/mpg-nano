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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>

#include "app-io.h"
#include "app-switch.h"


// MPG LED slow flash rate
#define APP_SELECT_SLOW_FLASH           15

// MPG LED fast flash rate
#define APP_SELECT_FAST_FLASH           4


static volatile uint8_t m_e_stop_state;
static volatile uint8_t m_axis;
static volatile uint8_t m_step;


/**
 * TIMER1 overflow ISR. Flashes MPG LED.
 */
ISR(TIMER1_OVF_vect) {
    static uint8_t flash_div;

    uint8_t flash_rate;
    uint8_t portb_bits;
    uint8_t portc_bits;
    uint8_t portd_bits;

    // poll switch positions
    portb_bits = PINB;
    portc_bits = PINC;
    portd_bits = PIND;

    // decode e-stop state
    m_e_stop_state = (portd_bits & (1 << APP_IO_D_ESTOP)) != 0;

    // decode step selection (rotary switch + rapid button)
    if ( portd_bits & (1 << APP_IO_D_RAPID) ) {
        // use slow MPG LED flash rate if in normal mode
        flash_rate = APP_SELECT_SLOW_FLASH;

        // decode step size
        if ( !(portd_bits & (1 << APP_IO_D_X10)) ) {
            m_step = APP_SWITCH_STEP_X10;
        } else if ( !(portd_bits & (1 << APP_IO_D_X100)) ) {
            m_step = APP_SWITCH_STEP_X100;
        } else {
            m_step = APP_SWITCH_STEP_X1;
        }
    } else {
        // use x1000 step size if rapid button is pressed
        m_step = APP_SWITCH_STEP_X1000;

        // use fast MPG LED flash rate if in rapid mode
        flash_rate = APP_SELECT_FAST_FLASH;
    }

    if ( !(portc_bits & (1 << APP_IO_C_AXIS_X)) ) {
        m_axis = APP_SWITCH_AXIS_X;
    } else if ( !(portc_bits & (1 << APP_IO_C_AXIS_Y)) ) {
        m_axis = APP_SWITCH_AXIS_Y;
    } else if ( !(portb_bits & (1 << APP_IO_B_AXIS_Z)) ) {
        m_axis = APP_SWITCH_AXIS_Z;
    } else if ( !(portb_bits & (1 << APP_IO_B_AXIS_4)) ) {
        m_axis = APP_SWITCH_AXIS_4;
    } else {
        m_axis = APP_SWITCH_AXIS_OFF;

        // force MPG LED off if no axis is selected
        flash_rate = 0;
    }

    // flash MPG LED
    flash_div++;

    if (flash_rate == 0 ) {
        // ensure MPG LED is off if toggle rate is zero
        if ( PORTD & (1 << APP_IO_D_MPG_LED) ) {
            PIND = (1 << APP_IO_D_MPG_LED);
        }
    } else if (flash_div >= flash_rate ) {
        // toggle MPG LED state
        PIND = (1 << APP_IO_D_MPG_LED);

        // reset flash division counter
        flash_div = 0;
    }
}


bool app_switch_e_stop(void) {
    return m_e_stop_state;
}


app_switch_axis_t app_switch_axis(void) {
    return (app_switch_axis_t) m_axis;
}


app_switch_step_t app_switch_step(void) {
    return (app_switch_step_t) m_step;
}


void app_switch_init(void) {
    // enable TIMER1
    power_timer1_enable();

    // configure TIMER1 to overflow at ~30.5Hz
    TIFR1 = (1 << TOV1);
    TIMSK1 = (1 << TOIE1);
    TCCR1B = (1 << CS11);
}
