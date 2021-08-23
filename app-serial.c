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
#include <avr/power.h>

#include <stdbool.h>

#include "app-encoder.h"
#include "app-serial.h"
#include "app-switch.h"


// size of transmit buffer, in bytes
#define APP_SERIAL_TX_BUFFER_SIZE       16

// USART UCSRxB receive configuration
#define APP_SERIAL_UCSRXB_RECEIVE       ((1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0))

// USART UCSRxB transmit configuration
#define APP_SERIAL_UCSRXB_TRANSMIT      (APP_SERIAL_UCSRXB_RECEIVE | (1 << UDRIE0))


// enumeration of serial protocol states
typedef enum {
    APP_SERIAL_STATE_READY,
    APP_SERIAL_STATE_HAVE_RESET_REQ,
    APP_SERIAL_STATE_HAVE_STATUS_REQ,
    APP_SERIAL_STATE_RESPONDING
} app_serial_state_t;


static volatile int8_t m_state;
static volatile char m_tx_buffer[APP_SERIAL_TX_BUFFER_SIZE];
static volatile uint8_t m_tx_index;
static volatile uint8_t m_tx_count;


/**
 * USART0 receive complete ISR. Interprets received command characters and modifies module state accordingly.
 */
ISR(USART_RX_vect) {
    uint8_t flags;
    char cmd;

    // get USART0 status flags and received command
    flags = UCSR0A;
    cmd = (char) UDR0;

    // do nothing if not ready for a new command or if a frame error occurred
    if ( m_state != APP_SERIAL_STATE_READY || flags & (1 << FE0) ) {
        return;
    }

    // interpret received command
    switch(cmd) {
    case 'R': // reset request
        m_state = APP_SERIAL_STATE_HAVE_RESET_REQ;
        break;

    case 'S': // status request
        m_state = APP_SERIAL_STATE_HAVE_STATUS_REQ;
        break;

    default:
        // ignore invalid command characters
        break;
    }
}


/**
 * USART0 data register empty ISR. Transmits any pending characters in transmit buffer, then sets module state to
 * 'ready'.
 */
ISR(USART_UDRE_vect) {
    if ( m_tx_index < m_tx_count ) {
        UDR0 = m_tx_buffer[m_tx_index++];
    } else {
        UCSR0B = APP_SERIAL_UCSRXB_RECEIVE;
        m_state = APP_SERIAL_STATE_READY;
    }
}


/**
 * Sets module state to 'responding' and initiates response transmission.
 *
 * @param n_chars       Number of characters in transmit buffer to send.
 */
static void send_response(uint8_t n_chars) {
    // enter 'responding' state
    m_state = APP_SERIAL_STATE_RESPONDING;

    // set buffer index and character count
    m_tx_index = 0;
    m_tx_count = n_chars;

    // begin transmission
    UCSR0B = APP_SERIAL_UCSRXB_TRANSMIT;
}


static void nibble_to_hex(volatile char* buffer, uint8_t nibble) {
    buffer[0] = (char) ((nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10));
}


static void uint8_to_hex(volatile char* buffer, uint8_t value) {
    nibble_to_hex(buffer, value >> 4);
    nibble_to_hex(buffer + 1, value & 0xF);
}


static void uint16_to_hex(volatile char* buffer, uint16_t value) {
    uint8_to_hex(buffer, value >> 8);
    uint8_to_hex(buffer + 2, value & 0xFF);
}


void app_serial_loop(void) {
    uint16_t enc_delta;
    uint8_t switch_bits;

    // act on current state
    switch(m_state) {
    case APP_SERIAL_STATE_HAVE_RESET_REQ:
        // reset encoder counter
        app_encoder_reset();

        // prepare reset response
        m_tx_buffer[0] = '[';
        m_tx_buffer[1] = 'R';
        m_tx_buffer[2] = ']';
        m_tx_buffer[3] = '\r';
        m_tx_buffer[4] = '\n';

        // start transmission
        send_response(5);
        break;

    case APP_SERIAL_STATE_HAVE_STATUS_REQ:
        // get encoder delta
        enc_delta = app_encoder_delta();

        // encode switch states
        switch_bits = (uint8_t) app_switch_axis();
        switch_bits |= ((uint8_t) app_switch_step()) << 3;
        switch_bits |= app_switch_e_stop() ? (1 << 5) : 0;

        // prepare status response
        m_tx_buffer[0] = '[';
        m_tx_buffer[1] = 'S';

        uint16_to_hex(&m_tx_buffer[2], enc_delta);
        uint8_to_hex(&m_tx_buffer[6], switch_bits);

        m_tx_buffer[8] = ']';
        m_tx_buffer[9] = '\r';
        m_tx_buffer[10] = '\n';

        // start transmission
        send_response(11);
        break;

    default:
        // do nothing
        break;
    }
}


void app_serial_init(void) {
    // enable USART0
    power_usart0_enable();

    // configure USART0 (38400 8n1)
    UBRR0 = 25;
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    UCSR0B = APP_SERIAL_UCSRXB_RECEIVE;
}
