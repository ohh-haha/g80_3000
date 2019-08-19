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

// 扫描方法 置对应行为输出低电平 置对应列为输入 Pull resistance 扫描列是否为低电平

/* 引脚     对应寄存器  连接的Multiplex   模式     默认值
 * 2号引脚  PD1        Col S1           Output  default 0
 * 3号引脚  PD0        Col S2           Output  default 0
 * 4号引脚  Pd4        Col S3           Output  default 0
 * 5号引脚  Pd5        Col S4           Output  default 0
 * 6号引脚  Pd6        Col SIn          Input   default 1
 *
 * 8号引脚  Pb4        Row S1           Output  default 0
 * 9号引脚  Pb5        Row S2           Output  default 0
 * 10号引脚 Pb6        Row S3           Output  default 0
 * 14号引脚 Pb2        Row S4           Output  default 0
 * 15号引脚 Pb1        Row SIn          Output  default 0
 *
 * 初始状态  DDRD  |= (1 < 1) | (1 < 0) | (1 < 4) | (1 < 5);
 *          PORTD |= (1 < 6);
 *
 *          DDRB  |= (1 < 4) | (1 < 5) | (1 < 6) | (1 < 2) | (1 < 1);
 * */
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

static void select_rows(uint8_t row);

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
    // D2,D3,D4 with output mode,D6 with input mode;
    DDRD = (DDRD & (~0b10000000)) | 0b00010011;
    // default D2,D3,D4 Output low level,D6 pull resistance;
    PORTD = (PORTD | 0b10000000) & (~0b0010011);
    // D5 with output mode;
    DDRC |= 0b01000000;
    //default D5 Output low level;
    PORTC = PORTC & (~0b01000000);

    // D8,D9,D10,D14,D15 with output mode;
    DDRB = (DDRB & (~0b01111010)) | 0b01111010;
    // D8,D9,D10,D14,D15 default output low level;
    PORTB &= (~0b01111010);
}

void select_cols(uint8_t current_col) {
    /* 引脚     对应寄存器  连接的Multiplex   模式     默认值
 * 2号引脚  PD1        Col S1           Output  default 0
 * 3号引脚  PD0        Col S2           Output  default 0
 * 4号引脚  Pd4        Col S3           Output  default 0
 * 5号引脚  Pd5        Col S4           Output  default 0
     */
    switch (current_col) {
        case 0:
//            PORTD1 = 0;
//            PORTD0 = 0;
//            PORTD4 = 0;
//            PORTD5 = 0;
            PORTD = PORTD & (~0b00010011);
            PORTC = PORTC & (~0b01000000);
            break;
        case 1:
            // 0b 0000  0001
//            PORTD1 = 1;
//            PORTD0 = 0;
//            PORTD4 = 0;
//            PORTD5 = 0;
            PORTD = (PORTD & (~0b00010011)) | 0b00000010;
            PORTC = PORTC & (~0b01000000);
            break;
        case 2:
//            PORTD1 = 0;
//            PORTD0 = 1;
//            PORTD4 = 0;
//            PORTD5 = 0;
            PORTD = (PORTD & (~0b00010011)) | 0b00000001;
            PORTC = PORTC & (~0b01000000);
            break;
        case 3:
//            PORTD1 = 1;
//            PORTD0 = 1;
//            PORTD4 = 0;
//            PORTD5 = 0;
            PORTD = (PORTD & (~0b00010011)) | 0b00000011;
            PORTC = PORTC & (~0b01000000);
            break;
        case 4:
//            PORTD1 = 0;
//            PORTD0 = 0;
//            PORTD4 = 1;
//            PORTD5 = 0;
            PORTD = (PORTD & (~0b00010011)) | 0b00010000;
            PORTC = PORTC & (~0b01000000);
            break;
        case 5:
//            PORTD1 = 1;
//            PORTD0 = 0;
//            PORTD4 = 1;
//            PORTD5 = 0;
            PORTD = (PORTD & (~0b00010011)) | 0b00010010;
            PORTC = PORTC & (~0b01000000);
            break;
        case 6:
//            PORTD1 = 0;
//            PORTD0 = 1;
//            PORTD4 = 1;
//            PORTD5 = 0;
            PORTD = (PORTD & (~0b00010011)) | 0b00010001;
            PORTC = PORTC & (~0b01000000);
            break;
        case 7:
//            PORTD1 = 1;
//            PORTD0 = 1;
//            PORTD4 = 1;
//            PORTD5 = 0;
            PORTD = (PORTD & (~0b00010011)) | 0b00010011;
            PORTC = PORTC & (~0b01000000);
            break;
        case 8:
//            PORTD1 = 0;
//            PORTD0 = 0;
//            PORTD4 = 0;
//            PORTD5 = 1;
            PORTD = PORTD & (~0b00010011);
            PORTC = (PORTC & (~0b01000000)) | 0b01000000;
            break;
        case 9:
//            PORTD1 = 1;
//            PORTD0 = 0;
//            PORTD4 = 0;
//            PORTD5 = 1;
            PORTD = (PORTD & (~0b00010011)) | 0b00000010;
            PORTC = (PORTC & (~0b01000000)) | 0b01000000;
            break;
        case 10:
//            PORTD1 = 0;
//            PORTD0 = 1;
//            PORTD4 = 0;
//            PORTD5 = 1;
            PORTD = (PORTD & (~0b00010011)) | 0b00000001;
            PORTC = (PORTC & (~0b01000000)) | 0b01000000;
            break;
        case 11:
//            PORTD1 = 1;
//            PORTD0 = 1;
//            PORTD4 = 0;
//            PORTD5 = 1;
            PORTD = (PORTD & (~0b00010011)) | 0b00000011;
            PORTC = (PORTC & (~0b01000000)) | 0b01000000;
            break;
        case 12:
//            PORTD1 = 0;
//            PORTD0 = 0;
//            PORTD4 = 1;
//            PORTD5 = 1;
            PORTD = (PORTD & (~0b00010011)) | 0b00010000;
            PORTC = (PORTC & (~0b01000000)) | 0b01000000;
            break;
        case 13:
//            PORTD1 = 1;
//            PORTD0 = 0;
//            PORTD4 = 1;
//            PORTD5 = 1;
            PORTD = (PORTD & (~0b00010011)) | 0b00010010;
            PORTC = (PORTC & (~0b01000000)) | 0b01000000;
            break;
        case 14:
//            PORTD1 = 0;
//            PORTD0 = 1;
//            PORTD4 = 1;
//            PORTD5 = 1;
            PORTD = (PORTD & (~0b00010011)) | 0b00010001;
            PORTC = (PORTC & (~0b01000000)) | 0b01000000;
            break;
        case 15:
//            PORTD1 = 1;
//            PORTD0 = 1;
//            PORTD4 = 1;
//            PORTD5 = 1;
            PORTD = (PORTD & (~0b00010011)) | 0b00010011;
            PORTC = (PORTC & (~0b01000000)) | 0b01000000;
            break;
        default:
            break;
    }
}

void select_rows(uint8_t current_row) {
//    PORTB = (((current_row) & (~(0x08))) | ((PORTB)));

    switch (current_row) {
        case 0:
            // PDB4 = 0;
            // PDB5 = 0;
            // PDB6 = 0;
            // PDB3 = 0;
            // 0b 1000 1011
            PORTB = PORTB & (~0b01111000);
            break;
        case 1:
            // PDB4 =1;
            // PDB5 =0;
            // PDB6 =0;
            // PDB3 =0;
            PORTB = (PORTB & (~0b01111000)) | 0b00010000;
            break;
        case 2:
            // PDB4 = 0;
            // PDB5 = 1;
            // PDB6 = 0;
            // PDB2 = 0;
            PORTB = (PORTB & (~0b01111000)) | 0b00100000;
            break;
        case 3:
            // PDB4 = 1;
            // PDB5 = 1;
            // PDB6 = 0;
            // PDB2 = 0;
            PORTB = (PORTB & (~0b01111000)) | 0b00110000;
            break;
        case 4:
            // PDB4 = 0;
            // PDB5 = 0;
            // PDB6 = 1;
            // PDB2 = 0;
            PORTB = (PORTB & (~0b01111000)) | 0b01000000;
            break;
        case 5:
            // PDB4 = 1;
            // PDB5 = 0;
            // PDB6 = 1;
            // PDB2 = 0;
            PORTB = (PORTB & (~0b01111000)) | 0b01010000;
            break;
        case 6:
            // PDB4 = 0;
            // PDB5 = 1;
            // PDB6 = 1;
            // PDB2 = 0;
            PORTB = (PORTB & (~0b01111000)) | 0b01100000;
            break;
        case 7:
            // PDB4 = 1;
            // PDB5 = 1;
            // PDB6 = 1;
            // PDB2 = 0;
            PORTB = (PORTB & (~0b01111000)) | 0b01110000;
            break;
        default:
            break;
    }
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
        if ((PIND & (1 << 7)) == 0) {
            uprintf("row %u,col %u press\n", select_row, i);
            current_matrix[select_row] |= ((uint32_t) 1 << i);
        } else {
            current_matrix[select_row] &= ~((uint32_t) 1 << i);
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