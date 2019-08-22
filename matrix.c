/* ftfunjth<imallrightgg@gmail.com>
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

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <quantum/quantum.h>
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

static void init_register(void);

static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col);

static void select_cols(uint8_t col);

static void select_rows(uint8_t rows);

static void unselect_rows(void);

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

void init_register(void) {
    DDRB |= 0x7a;
    PORTB |= 0x7a;

    DDRC |= 0x40;
    PORTC |= 0x40;

    DDRD |= 0x93;
    PORTD = (PORTD & 0x6c) | 0x80;

    DDRE |= 0x40;
    PORTE |= 0x40;

    DDRF = (DDRF & 0x7f) | 0x40;
    PORTF |= 0xf0;
}

void matrix_init(void) {
    init_register();
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    matrix_init_quantum();
}


void select_cols(uint8_t current_col) {
/*  Using a CH4067 Multiplex.
 *  SIN        S0       S1       S2       S3
 *  A0(PF7)    A1(PF6)  D2(PD1)  D3(PD0)  D4(PD4)
 *  Col0       0        0        0        0
 *  Col1       1        0        0        0
 *  Col2       0        1        0        0
 *  Col3       1        1        0        0
 *  Col4       0        0        0        0
 *  Col5       0        0        0        0
 *  Col6       0        0        0        0
 *  Col7       0        0        0        0
 *  Col8       0        0        0        0
 *  Col9       0        0        0        0
 *  Col10      0        0        0        0
 *  Col11      0        0        0        0
 *  Col12      0        0        0        0
 *  Col13      0        0        0        0
 *  Col14      0        0        0        0
 *  Col15      0        0        0        0
 *  Col16             A3(PF4)
 *  Col17             A2(PF5)
 *  We don't select echo one, otherwise ,Just using bit operator for shortcut.
 *  */
    if (current_col < 16) {
        PORTD = ((PORTD & 0xec) |
                 (((current_col & 0x08) << 1) | ((current_col & 0x04) >> 2) | ((current_col & 0x02) << 0)));
        PORTF = ((PORTF & 0xbf) | ((current_col & 0x01) << 6));
    } else {
        PORTF &= ~(0x01 << (4 + (current_col & 0x01)));
    }
}

void select_rows(uint8_t current_row) {
/*
 *   Index  Com
 *   Row0   D5(PC6)
 *   Row1   D6(PD7)
 *   Row2   D7(PE6)
 *   Row3   D8(PB4)
 *   Row4   D9(PB5)
 *   Row5   D10(PB6)
 *   Row6   D14(PB3)
 *   Row7   D15(PB1)
 *
 *   PORTB Init Status ?1111?1?
 *   PORTC Init Status ?1??????
 *   PORTD Init Status 1???????
 *   PORTE Init Status ?1??????
*/
    switch (current_row) {
        case 0:
            PORTC &= 0xbf;
            break;
        case 1:
            PORTD &= 0x7f;
            break;
        case 2:
            PORTE &= 0xbf;
            break;
        case 3:
            PORTB &= 0xef;
            break;
        case 4:
            PORTB &= 0xdf;
            break;
        case 5:
            PORTB &= 0xbf;
            break;
        case 6:
            PORTB &= 0xf7;
            break;
        case 7:
            PORTB &= 0xfd;
            break;
        default:
            break;
    }
}

void unselect_rows(void) {
    PORTB = (PORTB | 0x7a);
    PORTC = (PORTC | 0x40);
    PORTD = (PORTD | 0x80);
    PORTE = (PORTE | 0x40);
}

uint8_t matrix_scan(void) {
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        select_rows(current_row);
        wait_us(15);
        bool matrix_changed = read_rows_on_col(matrix_debouncing, current_row);
        if (matrix_changed) {
            debouncing = true;
            debouncing_time = timer_read();
        }
        unselect_rows();
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
        select_cols(i);
        wait_us(15);
        matrix_row_t current_value = current_matrix[select_row];

        if (i > 0x0f) {
            if ((PINF & (1 << (5 - (i & 0x01)))) == 0) {
                current_matrix[select_row] |= ((uint32_t) 1 << i);
            } else {
                current_matrix[select_row] &= ~((uint32_t) 1 << i);
            }
        } else {
            if ((PIND & (1 << 2)) == 0) {
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