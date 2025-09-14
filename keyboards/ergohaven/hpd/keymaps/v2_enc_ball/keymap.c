#include QMK_KEYBOARD_H
#include "ergohaven.h"
#include "ergohaven_pointing.h"
#include "ergohaven_ruen.h"
#include "g/keymap_combo.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_EN] = LAYOUT( \
        _______,   _______,      _______,    _______,    _______,   _______,                                   _______,  _______,  _______,   _______,   _______,  _______, \
        _______,    KC_NO,        KC_X,         KC_Z,         TD(0),      TD(1),                          KC_Q,     KC_G,     KC_N,      KC_F,     KC_NO,     KC_NO, \
        _______,    LGUI_T(KC_H), LALT_T(KC_I), LSFT_T(KC_E), LCTL_T(KC_A),  TH_J_F,                            KC_P,     LT(_NUM, KC_D),     LSFT_T(KC_R),      KC_S,     KC_L,      KC_NO, \
        _______,    TH_K_Z,       TH_Y_X,       TH_O_C,       TH_U_V,        TH_NO_B,                           KC_B,     KC_C,     KC_M,      KC_W,     TH_V_SLS,      KC_NO, \
                    KC_NO,        KC_NO,         TO(0),     LT(_NAV, KC_SPACE),    OSL(_APP), KC_NO,        KC_BTN1, KC_BSPC, LT(_NUM, KC_T),   KC_NO,    KC_NO,   KC_NO,   \
                                                                                          KC_MUTE,          KC_MUTE
    ),

    [_RU] = LAYOUT( \
        _______, _______,      _______,      _______,      _______,       _______,                                            _______,   _______,   _______,   _______, _______,      _______,
        _______, KC_W,         KC_M,      KC_Z,         TD(2),      TD(3),                                             KC_P,     KC_D,    KC_R,    KC_L,   KC_X,          _______,
        _______, LGUI_T(KC_E), LALT_T(KC_B), LSFT_T(KC_T), LCTL_T(KC_J),  TH_RU_A_F,                                               KC_K,     LT(_NUM, KC_Y),    LSFT_T(KC_N),    KC_C,   KC_Q,          _______,
        _______, TH_RU_F_Z,    TH_RU_YE_X,   TH_RU_KHA_C,  TH_RU_YERU_V,  TH_RU_YU_B,                                              KC_COMMA, KC_V,    KC_G,    KC_U,   TH_RU_ZHE_SLS, _______,
                  KC_NO,         KC_NO,      TO(0),   LT(_NAV, KC_SPACE),    OSL(_APP), _______,          _______, KC_BSPC, LT(_NUM, KC_H),   KC_NO,    KC_NO,   KC_NO,   \
                                                                                       _______,         _______
    ),

    [_MOUSE] = LAYOUT( \
        _______, _______, _______, _______, _______, _______,                                           _______, _______, _______, _______, _______,  _______,
        _______, _______, EH_SNP, EH_SCR,  _______, _______,                                           _______, _______, _______, _______, _______,  _______,
        _______, KC_LCTL, LALT_T(KC_BTN3), LSFT_T(KC_BTN2), KC_BTN1, _______,                                           _______, _______, _______, _______, _______,  _______,
        _______, _______, _______, _______, _______, _______,                                           _______, _______, _______, _______, _______,  _______,
                          _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, \
                                                                      _______,         _______
    ),

    [_MCTRL] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                                           _______, _______, _______, _______, _______,  _______,
        _______, C(KC_Q), C(KC_W), C(KC_E), C(KC_R), C(KC_T),                                           C(KC_Y), C(KC_U), C(KC_I), C(KC_O), C(KC_P),  _______,
        _______, C(KC_A), C(KC_S), KC_LSFT, _______, C(KC_F),                                   C(KC_H), C(KC_J), C(KC_K), C(KC_L), C(KC_SCLN),  _______,
        _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), C(KC_B),                                           C(KC_N), C(KC_M), C(KC_COMM), C(KC_DOT), C(KC_SLSH),  _______,
                          _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, \
                                                                       _______,          _______
    ),

    [_MODS] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                                           _______, _______, _______, _______, _______,  _______,
        _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     _______,
        _______, LGUI_T(KC_A), LALT_T(KC_S), LSFT_T(KC_D), LCTL_T(KC_F),  _______,                      _______, LT(_NUM, KC_J), LSFT_T(KC_K), KC_L,   KC_SCLN,          _______,
        _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                              KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,   _______,
                          _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, \
                                                                       _______,          _______
    ),
    [_NAV] = LAYOUT(
        QK_BOOT, _______, _______, _______, _______, _______,                                           _______, _______, _______, _______,  _______,  _______,
        _______, _______, C(A(KC_GRV)), HYPR(KC_TAB), HYPR(KC_F1), _______,                             _______, KC_HOME, KC_UP,   KC_END,   _______,  _______,
        _______, C(KC_LBRC), C(KC_RBRC), KC_LSFT, KC_LCTL, _______,                                     _______, KC_LEFT, KC_DOWN, KC_RIGHT, _______,  _______,
        _______, G(S(KC_LBRC)), G(S(KC_RBRC)), S(KC_F6), MEH(KC_V), _______,                            _______, KC_PGUP, _______, KC_PGDN,  _______,  _______,
                          _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, \
                                                                       _______,          _______
    ),
    [_NUM] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                                           _______, _______, _______, _______, _______,  _______,
        _______, _______, KC_7, KC_8, KC_9, LG_SLASH,                                                   _______, _______, _______, _______, _______,  _______,
        _______, KC_MINS, KC_4, KC_5, KC_6, KC_EQL,                                                     _______, KC_LCTL, KC_LSFT, KC_LALT, KC_LGUI,  _______,
        _______, KC_0, KC_1, KC_2, KC_3,    C(KC_B),                                                    _______, _______, _______, _______, _______,  _______,
                          _______, _______, _______, A(KC_SPACE), A(KC_ENTER), _______,                 _______, _______, _______, _______, _______, _______, \
                                                                             _______,                   _______
    ),
    [_APP] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                                           _______, _______, _______, _______, _______,  _______,
        _______, MEH(KC_P), MEH(KC_W), MEH(KC_E), MEH(KC_R), MEH(KC_T),                                 MEH(KC_Y), MEH(KC_U), MEH(KC_I), MEH(KC_O), MEH(KC_P),  _______,
        _______, MEH(KC_A), MEH(KC_S), MEH(KC_D), MEH(KC_F), MEH(KC_G),                                 MEH(KC_H), MEH(KC_J), MEH(KC_K), MEH(KC_L), MEH(KC_SCLN),  _______,
        _______, MEH(KC_Z), MEH(KC_X), MEH(KC_C), HYPR(KC_V), MEH(KC_B),                                MEH(KC_N), MEH(KC_M), MEH(KC_COMMA), MEH(KC_DOT), MEH(KC_SLSH),  _______,
                           _______, _______, _______, KC_NO, _______, _______,                 _______, _______, _______, _______, _______, _______, \
                                                                       _______,                 _______
    ),
    [_SYM] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                                           _______, _______, _______, _______, _______,  _______,
        _______, _______, LG_AT, KC_UNDS, LG_DQUO, S(KC_9),                                           LG_COLON, LG_LBR, LG_HASH, LG_RBR, _______,  _______,
        _______, LG_TILD, S(KC_9), LG_COLON, S(KC_0), S(KC_EQL),                                           LG_NUM, LG_LCBR, LG_SCLN, LG_RCBR, LG_SLASH,  _______,
        _______, _______, S(KC_8), LG_CIRC, LG_PERC, _______,                                           LG_AMPR, LG_LT, LG_DLR, LG_GT, LG_PIPE,  _______,
                          _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, \
                                                                       _______,          _______
    ),
    [_FN] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                                           _______, _______, _______, _______, _______,  _______,
        _______, _______, KC_F7,   KC_F8,   KC_F9,   LG_SLASH,                                                   _______, _______, _______, _______, _______,  _______,
        _______, KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_F12,                                                     _______, KC_LCTL, KC_LSFT,  KC_LALT, KC_LGUI,  _______,
        _______, KC_F10,  KC_F1,   KC_F2,   KC_F3,   _______,                                                    _______, _______, _______, _______, _______,  _______,
                          _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, \
                                                                       _______,          _______
    ),
    [_OS] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                                           _______, _______, _______, _______, _______,  _______,
        _______, _______, _______, C(A(KC_2)), C(A(KC_3)), C(A(KC_4)),                                           _______, _______, _______, _______, _______,  _______,
        _______, _______, C(KC_RIGHT), KC_MPLY, KC_KB_VOLUME_UP, KC_BRIU,                                           _______, _______, _______, _______, _______,  _______,
        _______, _______, _______, _______, KC_KB_VOLUME_DOWN, KC_BRID,                                           _______, _______, _______, _______, _______,  _______,
                          _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, \
                                                                       _______,          _______
    ),
    [_GAME] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,                                           KC_7, KC_8, KC_9,   KC_0,    KC_MINS,  KC_EQL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                           KC_Y, KC_U, KC_I,   KC_O,    KC_P,     KC_LBRC,
        KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                           KC_H, KC_J, KC_K,   KC_L,    KC_SCLN,  KC_Q,
        KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                           KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,  KC_Q,
                          KC_LGUI,  KC_ESC,  KC_SPACE, KC_F2, KC_F1, KC_H,       KC_BSPC, _______, _______, _______, _______, _______, \
                                                                       _______,          _______
    ),
};
// clang-format on

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [1] = {ENCODER_CCW_CW(_______, _______)},
    [2] = {ENCODER_CCW_CW(_______, _______)},
    [3] = {ENCODER_CCW_CW(_______, _______)},
    [4] = {ENCODER_CCW_CW(_______, _______)},
    [5] = {ENCODER_CCW_CW(_______, _______)},
    [6] = {ENCODER_CCW_CW(_______, _______)},
    [7] = {ENCODER_CCW_CW(_______, _______)},
    [8] = {ENCODER_CCW_CW(_______, _______)},
    [9] = {ENCODER_CCW_CW(_______, _______)},
    [10] = {ENCODER_CCW_CW(_______, _______)},
    [11] = {ENCODER_CCW_CW(_______, _______)},
};
#endif

typedef union {
    uint32_t raw;
    struct {
        uint8_t text_mode : 3;
        uint8_t scroll_mode : 3;
        uint8_t sniper_mode : 2;
        uint8_t dpi_mode : 4;
        bool    sticky_pointing_mode : 1;
        bool    invert_scroll : 1;
        bool    acceleration : 1;
    };
} vial_config_t;

static vial_config_t vial_config;

const int     DPI_TABLE[15]    = {100, 200, 300, 400, 500, 600, 800, 1000, 1200, 1600, 2000, 2500, 3200, 4000, 5000};
const int32_t SNIPER_TABLE[15] = {2, 3, 4, 5};
const int32_t SCROLL_TABLE[15] = {6, 8, 11, 16, 23, 32, 45, 64};
const int32_t TEXT_TABLE[15]   = {6, 8, 11, 16, 23, 32, 45, 64};

int get_dpi(uint8_t dpi_mode) {
    if (dpi_mode < ARRAY_SIZE(DPI_TABLE))
        return DPI_TABLE[dpi_mode];
    else
        return DPI_TABLE[0];
}

void via_set_layout_options_kb(uint32_t value) {
    dprintf("via_set_layout_options_kb %lx\n", value);
    vial_config.raw = value;
    pointing_device_set_cpi(get_dpi(vial_config.dpi_mode));
    set_scroll_sens(SCROLL_TABLE[vial_config.scroll_mode]);
    set_sniper_sens(SNIPER_TABLE[vial_config.sniper_mode]);
    set_text_sens(TEXT_TABLE[vial_config.text_mode]);
    set_sticky_pointing_mode(vial_config.sticky_pointing_mode);
    set_invert_scroll(vial_config.invert_scroll);
    set_acceleration(vial_config.acceleration);
}

void keyboard_post_init_user(void) {
    vial_config.raw = via_get_layout_options();
    via_set_layout_options_kb(vial_config.raw);
    set_led_blinks(false);
}
