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
#include QMK_KEYBOARD_H

enum custom_keycodes {
    ADD_AUTHOR = SAFE_RANGE,
};

bool process_record_user(uint16_t key_code, keyrecord_t *record) {
    switch (key_code) {
        case ADD_AUTHOR:
            if (record->event.pressed) {
                SEND_STRING("/**\n");
                SEND_STRING("* @description: 我只是一个兴趣使然的超人\n");
                SEND_STRING("* @author ftfunjth\n");
                SEND_STRING("*/\n");
            }
            break;
    }

    return true;
}


// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _MA 0
#define _FN 1
// 7 8 7 8 4 7 7 7 6 8 7 7 8 6 2 2
const uint16_t PROGMEM
keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_MA] =
LAYOUT(
        KC_KP_MINUS, KC_RIGHT, KC_DOWN, KC_LEFT, KC_RALT, KC_SPACE, KC_SLASH, KC_N, KC_D, KC_B, KC_RGUI, \
 KC_KP_3, KC_KP_2, KC_KP_1, KC_KP_ENTER, KC_NONUS_BSLASH, KC_K, KC_SCOLON, KC_L, KC_J, KC_A, KC_F, KC_S, \
 KC_KP_6, KC_KP_5, KC_KP_4, MO(
_FN),                              KC_BSPACE,         KC_RBRACKET,  KC_LBRACKET,  KC_F7,     KC_Y,       KC_TAB,     KC_F3,    KC_T,                        KC_CAPSLOCK,              KC_LSHIFT, \
            KC_KP_9,       KC_KP_8,      KC_KP_7,     KC_KP_PLUS,     KC_LOCKING_SCROLL,                       KC_I,         KC_P,         KC_O,      KC_U,       KC_Q,       KC_E,     KC_R,                        KC_W,                                \
            KC_KP_ASTERISK,                                                                                                                                                                                                                               \
            KC_PGUP,       KC_INSERT,    KC_DELETE,   KC_HOME,                              KC_F9,             KC_EQUAL,     KC_MINUS,     KC_F8,     KC_6,       KC_GRAVE,   KC_F2,    KC_5,                        KC_F1,       KC_LCTRL,               \
            KC_PGDOWN,     KC_F12,       KC_F11,      KC_END,         KC_PSCREEN,           KC_F10,            KC_8,         KC_0,         KC_9,      KC_7,       KC_1,       KC_3,     KC_4,                        KC_2,                                \
                           KC_KP_SLASH,  KC_NUMLOCK,  KC_PAUSE,                             KC_ENTER,          KC_COMMA,                   KC_DOT,    KC_M,       KC_Z,       KC_C,     KC_V,                        KC_X,        KC_RCTRL,    KC_RSHIFT, \
            KC_KP_DOT,     KC_KP_0,                   KC_UP,          KC_LALT,              KC_F5,             KC_F6,        KC_QUOTE,                KC_H,       KC_ESCAPE,  KC_F4,    KC_G,    KC_LGUI                                                  \
            ),
[_FN] =
LAYOUT(
        KC_KP_MINUS, KC_RIGHT, KC_DOWN, KC_LEFT, KC_RALT, KC_SPACE, KC_SLASH, KC_N, KC_D, KC_B, KC_RGUI, \
 KC_KP_3, KC_KP_2, KC_KP_1, KC_KP_ENTER, KC_NONUS_BSLASH, KC_K, KC_SCOLON, KC_L, KC_J, KC_A, KC_F, KC_S, \
 KC_KP_6, KC_KP_5, KC_KP_4, MO(
_MA),                              ADD_AUTHOR,         KC_RBRACKET,  KC_LBRACKET,  KC_F7,     KC_Y,       KC_TAB,     KC_F3,    KC_T,                        KC_CAPSLOCK,              KC_LSHIFT, \
            KC_KP_9,       KC_KP_8,      KC_KP_7,     KC_KP_PLUS,     KC_LOCKING_SCROLL,                       KC_I,         KC_P,         KC_O,      KC_U,       KC_Q,       KC_E,     KC_R,                        KC_W,                                \
            KC_KP_ASTERISK,                                                                                                                                                                                                                               \
            KC_PGUP,       KC_INSERT,    KC_DELETE,   KC_HOME,                              KC_F9,             KC_EQUAL,     KC_MINUS,     KC_F8,     KC_6,       KC_GRAVE,   KC_F2,    KC_5,                        KC_F1,       KC_LCTRL,               \
            KC_PGDOWN,     KC_F12,       KC_F11,      KC_END,         KC_PSCREEN,           KC_F10,            KC_8,         KC_0,         KC_9,      KC_7,       KC_1,       KC_3,     KC_4,                        KC_2,                                \
                           KC_KP_SLASH,  KC_NUMLOCK,  KC_PAUSE,                             KC_ENTER,          KC_COMMA,                   KC_DOT,    KC_M,       KC_Z,       KC_C,     KC_V,                        KC_X,        KC_RCTRL,    KC_RSHIFT, \
            KC_KP_DOT,     KC_KP_0,                   KC_UP,          KC_LALT,              KC_F5,             KC_F6,        KC_QUOTE,                KC_H,       KC_ESCAPE,  KC_F4,    KC_G,    KC_LGUI                                                  \
            ),
};

void led_set_user(uint8_t usb_led) {
}


void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}
