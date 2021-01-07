/* Copyright 2019-2020 DMQ Design
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

enum layers {
    _NUMPAD,
    _MOD,
    _RGB,
    _VS
};

enum custom_keycodes {
    HELLO_WORLD = SAFE_RANGE,
};

//The below layers are intentionally empty in order to give a good starting point for how to configure multiple layers.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_NUMPAD] = LAYOUT(/* Base */
                KC_7, KC_8, KC_9, TO(_NUMPAD),
                KC_4, KC_5, KC_6, TO(_RGB),
                KC_1, KC_2, KC_3, TO(_VS),
                LT(_MOD, KC_0), KC_DOT, KC_ENTER
                ),

    [_MOD] = LAYOUT(/* Base */
                KC_PSLS, KC_PAST, KC_PMNS, TO(_NUMPAD),
                KC_TRNS, KC_UP, KC_PPLS, TO(_RGB),
                KC_LEFT, KC_DOWN, KC_RIGHT, TO(_VS),
                KC_TRNS, KC_0, KC_ENTER
                ),

    [_RGB] = LAYOUT(/* Base */
                RGB_HUI,  RGB_SAI, RGB_VAI, KC_TRNS,
                RGB_HUD,  RGB_SAD, RGB_VAD, KC_TRNS,
                KC_NO,    KC_NO,   KC_NO,   KC_TRNS,
                RGB_RMOD, RGB_TOG, RGB_MOD
                ),

    [_VS] = LAYOUT(/* Base */
                KC_F11, KC_F10, LSFT(KC_F11) , KC_TRNS,
                KC_NO, KC_F5, KC_NO, KC_TRNS,
                KC_NO, KC_NO, KC_NO, KC_TRNS,
                LCTL(LSFT(KC_B)), KC_NO, KC_NO
                )
};



uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SFT_T(KC_SPC):
            return TAPPING_TERM + 1250;
        case LT(1, KC_GRV):
            return 130;
        default:
            return TAPPING_TERM;
    }
}


void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        switch (get_highest_layer(layer_state)) {     //break each encoder update into a switch statement for the current layer
            case _NUMPAD:
                if (clockwise) {
                    tap_code(KC_WH_D);
                } else {
                    tap_code(KC_WH_U);
                }
                break;
            case _RGB:
                if (clockwise) {
                    rgblight_increase_hue();
                } else {
                    rgblight_decrease_hue();
                }
                break;
            case _VS:
                if (clockwise) {
                    tap_code(KC_PGDOWN);
                } else {
                    tap_code(KC_PGUP);
                }
                break;
        }
    } else if (index == 1) { /* Second encoder */
        switch (get_highest_layer(layer_state)) {
            case _NUMPAD:
                if (clockwise) {
                    tap_code(KC_WH_R);
                } else {
                    tap_code(KC_WH_L);
                }
                break;
            case _RGB:
                if (clockwise) {
                    rgblight_increase_sat();
                } else {
                    rgblight_decrease_sat();
                }
                break;
            case _VS:
                if (clockwise) {
                    tap_code(KC_WH_R);
                } else {
                    tap_code(KC_WH_L);
                }
                break;
        }
    } else if (index == 2) { /* Third encoder */
        switch (get_highest_layer(layer_state)) {
            case _NUMPAD:
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
            case _RGB:
                if (clockwise) {
                    rgblight_increase_val();
                } else {
                    rgblight_decrease_val();
                }
                break;
            case _VS:
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
        }
    }
}

layer_state_t layer_state_set_user(layer_state_t state) { //This will run every time the layer is updated
    switch (get_highest_layer(state)) {
        case _NUMPAD:
            setrgb(255, 0, 0, &led[0]); //Set the top LED to white for the bottom layer
            setrgb(252, 227, 3, &led[1]);
            setrgb(3, 107, 252, &led[2]);
            break;
        case _RGB:
            setrgb(0, 0, 0, &led[0]); //Set the top LED to white for the bottom layer
            setrgb(0, 0, 0, &led[1]);
            setrgb(0, 0, 0, &led[2]);
            break;
        case _VS:
            setrgb(115, 3, 252, &led[0]); //Set the top LED to white for the bottom layer
            setrgb(115, 3, 252, &led[1]);
            setrgb(115, 3, 252, &led[2]);
            break;
    }
    rgblight_set();
    return state;
}

#ifdef OLED_DRIVER_ENABLE


oled_rotation_t oled_init_user(oled_rotation_t rotation) {
#ifdef LEFT_HAND
    return OLED_ROTATION_180;
#else
    return OLED_ROTATION_0;
#endif
}

void oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _NUMPAD:
            oled_write_P(PSTR("Numpad\n"), false);
            break;
        case _RGB:
            oled_write_P(PSTR("RGB\n"), false);
            break;
        case _VS:
            oled_write_P(PSTR("Visual Studio\n"), false);
            break;
        case _MOD:
            oled_write_P(PSTR("Modifier\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }
}
#endif
