/* Copyright 2018 Maarten Dekkers <maartenwut@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
    18 个 针脚  5 + 13

    // 行 Output lower level
    // 列 Input Pull resistance

    PC  0 0 0 0 0 0 0 0
    0 0

    DDRB =  (DDRB & (~0xf0)) | 0xf0;
    DDRC &= ~0xc0;
    DDRD =  (DDRD & (~0xdf)) | 0x17;
    DDRE =  (DDRE & (~0x40)) | 0x40;
    DDRF &= ~0x30;

    PORTB &= (~0xf0);
    PORTC &= ~0xc0;
    PORTD = (PORTD & (~0xdf)) | 0x08;
    PORTE &= ~0x40;
    PORTF = (PORTF & (~0x30)) | 0x30;

    0b 1 1 0 1 1 1 1 1

    RX1   SIN       PD3
    TX0   S0        PD2
    2     S1        PD1
    3     S2        PD0
    4     S3        PD4
    5     Row1      PC6
    6     Row2      PD7
    7     Row3      PE6
    8     Row4      PB4
    9     Row5      PB5
    10    Row6      PB6
    14    Row7      PB7
    15    Row8      PD6
    16    Row9      PC7
    A0
    A1
    A2    Col12     PF5
    A3    Col11     PF4


*/

#include <stdint.h>
#include <stdbool.h>
#include <quantum/quantum.h>
#include <avr/io.h>
#include "wait.h"
#include <print.h>
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "timer.h"

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif

static uint16_t debouncing_time;
static bool debouncing = false;

static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static void init(void);

static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col);

static void select_cols(uint8_t col);

__attribute__ ((weak))
void matrix_init_user(void) {}

__attribute__ ((weak))
void matrix_scan_user(void) {}

__attribute__ ((weak))
void matrix_init_kb(void) {
    matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
    matrix_scan_user();
}

inline
uint8_t matrix_rows(void) {
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void) {
    return MATRIX_COLS;
}

void matrix_init(void) {
    debug_enable = true;
    init();
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    matrix_init_quantum();
}

void init(void) {
    DDRB = (DDRB & (~0xf0)) | 0xf0;
    DDRC &= ~0xc0;
    DDRD = (DDRD & (~0xdf)) | 0x17;
    DDRE = (DDRE & (~0x40)) | 0x40;
    DDRF &= ~0x30;

    PORTB &= (~0xf0);
    PORTC &= ~0xc0;
    PORTD = (PORTD & (~0xdf)) | 0x08;
    PORTE &= ~0x40;
    PORTF = (PORTF & (~0x30)) | 0x30;
}

void select_cols(uint8_t current_col) {
    uint8_t index = current_col > 14 ? current_col - 2 : current_col;
    PORTD = (PORTD & (~0x17)) |
            (((index & 0x04) >> 2) | ((index & 0x02) << 0) | ((index & 0x01) << 2) | ((index & 0x08) << 1));
}

uint8_t matrix_scan(void) {
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        bool matrix_changed = read_rows_on_col(matrix_debouncing, current_row);
        if (matrix_changed) {
            debouncing = true;
            debouncing_time = timer_read();
        }
    }

    if (debouncing && (timer_elapsed(debouncing_time) > DEBOUNCE)) {
        for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
            matrix[i] = matrix_debouncing[i];
        }
        debouncing = false;
    }

    matrix_scan_quantum();
    return 1;
}

bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t select_row) {
    bool matrix_changed = false;
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        if (i < 13 || i > 14) {
            select_cols(i);
            wait_us(12);
        }
        matrix_row_t current_value = current_matrix[select_row];

        if (i > 12 && i < 15) {
            if ((PINF & (1 << ((i & 0x03) + 3))) == 0) {
                uprintf("row %u,col %u press\n", select_row, i);
                current_matrix[select_row] |= ((uint32_t) 1 << i);
            } else {
                current_matrix[select_row] &= ~((uint32_t) 1 << i);
            }
        } else {
            if ((PIND & (1 << 3)) == 0) {
                uprintf("row %u,col %u press\n", select_row, i);
                current_matrix[select_row] |= ((uint32_t) 1 << i);
            } else {
                current_matrix[select_row] &= ~((uint32_t) 1 << i);
            }
        }

        if ((current_value != current_matrix[select_row]) && !(matrix_changed)) {
            matrix_changed = true;
        }
    }
    return matrix_changed;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col) {
    return (matrix[row] & ((matrix_row_t) 1 << col));
}

inline
matrix_row_t matrix_get_row(uint8_t row) {
    return matrix[row];
}

void matrix_print(void) {
    print("\nr/c 0123456789ABCDEFGHIJKLMNOPQRSTUV  \n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row);
        print(": ");
        print_bin_reverse32(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop32(matrix[i]);
    }
    return count;
}