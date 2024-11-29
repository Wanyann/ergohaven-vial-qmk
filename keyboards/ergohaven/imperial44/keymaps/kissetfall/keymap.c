#include QMK_KEYBOARD_H
#include "oled/ergohaven_dark.c"
#include "ergohaven.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
      [_BASE] = LAYOUT(
         KC_TAB,  KC_Q,  KC_W,    KC_E,    KC_R,    KC_T,                                                   KC_Y,  KC_U,    KC_I,    KC_O,   KC_P,    KC_GRV,
         KC_LALT, KC_A,  KC_S,    KC_D,    KC_F,    KC_G,                                                   KC_H,  KC_J,    KC_K,    KC_L,   KC_SCLN, KC_ENT,
         KC_ESC,  KC_Z,  KC_X,    KC_C,    KC_V,    KC_B,                                                   KC_N,  KC_M,    KC_COMM, KC_DOT, KC_QUOT, KC_MPLY,
                         KC_VOLD, KC_VOLU, KC_LCTL, LOWER, KC_SPC, PREVWRD,               NEXTWRD, KC_BSPC, RAISE, KC_RSFT, KC_PGDN, KC_PGUP

      ),

      [_LOWER] = LAYOUT(
         _______, KC_3,    KC_2,    KC_1,    KC_0,    KC_4,                                                 KC_7,    KC_6,    KC_5,    KC_9,    KC_8,     _______,
         KC_ENT,  APP_A,   WNEXT,   KC_DEL,  KC_LSFT, LANG,                                                 _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______,
         _______, KC_UNDO, KC_CUT,  KC_COPY, KC_PSTE, APP_B,                                                _______, KC_HOME, CTRLDN,  CTRLUP,  KC_END,   _______,
                           KC_MPRV, KC_MNXT, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______

      ),

         [_RAISE] = LAYOUT(
         _______, _______, KC_TILD, KC_EQL,  KC_PLUS, _______,                                              _______, KC_LPRN, KC_UNDS, KC_RPRN, _______, _______,
         _______, KC_BSLS, KC_DLR,  KC_MINS, KC_HASH, KC_EXLM,                                              KC_QUES, KC_LBRC, KC_PERC, KC_RBRC, KC_AMPR, _______,
         _______, _______, KC_ASTR, KC_COLN, KC_SLSH, _______,                                              _______, KC_LCBR, KC_PIPE, KC_RCBR, _______, _______,
                           _______, _______, _______, _______,  _______, _______,         _______, _______, _______, _______, _______, _______
      )

};

// #ifdef ENCODER_MAP_ENABLE
// const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
//   [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(PREVWRD, NEXTWRD)},
//   [1] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(CTRLDN,   CTRLUP)},
//   [2] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______)},
// };
// #endif
