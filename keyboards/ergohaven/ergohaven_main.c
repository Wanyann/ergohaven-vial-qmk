#include "ergohaven.h"
#include "ergohaven_ruen.h"
#include "ergohaven_rgb.h"
#include "ergohaven_oled.h"
#include "ergohaven_rgb.h"
#include "ergohaven_display.h"
#include "ergohaven_pointing.h"
#include "hid.h"
#include "version.h"
#include "print.h"

extern combo_t key_combos[];

typedef union {
    uint32_t raw;
    struct {
        uint8_t ruen_toggle_mode : 2;
        bool    ruen_mac_layout : 1;
    };
} kb_config_t;

kb_config_t kb_config;

void kb_config_update(kb_config_t new_config) {
    if (new_config.raw != kb_config.raw) {
        kb_config = new_config;
        eeconfig_update_kb(kb_config.raw);
    }
}

static bool numlock_enabled = false;
static bool scrolllock_enabled = false;
static bool mod_layer_on = false;
static bool alpha_layer_active = true;
static bool ctrl_pressed = false;

void kb_config_update_ruen_toggle_mode(uint8_t mode)
{
    kb_config_t new_config      = kb_config;
    new_config.ruen_toggle_mode = mode;
    kb_config_update(new_config);
}

void kb_config_update_ruen_mac_layout(bool mac_layout) {
    kb_config_t new_config     = kb_config;
    new_config.ruen_mac_layout = mac_layout;
    kb_config_update(new_config);
}

#ifdef AUDIO_ENABLE
float base_sound[][2] = SONG(TERMINAL_SOUND);
float caps_sound[][2] = SONG(CAPS_LOCK_ON_SOUND);
#endif

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

uint8_t prev_lang = LANG_EN;

// combo

// const uint16_t PROGMEM ru_combo[] = {TH_O_C, TH_U_V, COMBO_END};
// const uint16_t PROGMEM en_combo[] = {KC_C, KC_M, COMBO_END};

// combo_t key_combos[] = {
//     COMBO(ru_combo, LG_SET_RU),
//     COMBO(en_combo, LG_SET_EN), // keycodes with modifiers are possible too!
// };

// combo end


// key overrides

// 1. Объявите ваши key_override_t как обычно
// const key_override_t soft_sign_override = ko_make_with_layers(MOD_MASK_SHIFT, RU_SOFT, KC_RBRC, 1<<_RU);

// const key_override_t *custom_key_overrides[] = {
//     &soft_sign_override,
//     NULL // Обязательный NULL-terminator
// };
// key overrides end

// tap dance stuff

// 2. Структура для хранения конфигурации Tap-Hold
typedef struct {
    uint16_t tap_key;
    uint16_t hold_key;
} th_config_t;

// 3. Конфигурация для всех Tap-Hold клавиш
th_config_t th_config[] = {
    [TH_K_Z - SAFE_RANGE] = {.tap_key = KC_K, .hold_key = LCTL(KC_Z)},
    [TH_Y_X - SAFE_RANGE] = {.tap_key = KC_Y, .hold_key = LCTL(KC_X)},
    [TH_O_C - SAFE_RANGE] = {.tap_key = KC_O, .hold_key = LCTL(KC_C)},
    [TH_U_V - SAFE_RANGE] = {.tap_key = KC_U, .hold_key = LCTL(KC_V)},
    [TH_NO_B - SAFE_RANGE] = {.tap_key = KC_NO, .hold_key = LGUI(KC_V)},
    [TH_J_F - SAFE_RANGE] = {.tap_key = KC_J, .hold_key = LCTL(KC_F)},
    [TH_V_SLS - SAFE_RANGE] = {.tap_key = KC_V, .hold_key = LCTL(KC_SLSH)},

    [TH_RU_F_Z - SAFE_RANGE] = {.tap_key = KC_A, .hold_key = LCTL(KC_Z)},
    [TH_RU_YE_X - SAFE_RANGE] = {.tap_key = KC_QUOT, .hold_key = LCTL(KC_X)},
    [TH_RU_KHA_C - SAFE_RANGE] = {.tap_key = KC_LBRC, .hold_key = LCTL(KC_C)},
    [TH_RU_YERU_V - SAFE_RANGE] = {.tap_key = KC_S, .hold_key = LCTL(KC_V)},
    [TH_RU_YU_B - SAFE_RANGE] = {.tap_key = KC_DOT, .hold_key = LGUI(KC_V)},
    [TH_RU_A_F - SAFE_RANGE] = {.tap_key = KC_F, .hold_key = LCTL(KC_F)},
    [TH_RU_ZHE_SLS - SAFE_RANGE] = {.tap_key = KC_SCLN, .hold_key = LCTL(KC_SLSH)},

    [TH_BTN2_V - SAFE_RANGE] = {.tap_key = KC_BTN2, .hold_key = LCTL(KC_V)},
    [TH_BTN3_F - SAFE_RANGE] = {.tap_key = KC_BTN3, .hold_key = LCTL(KC_F)},


    // Добавьте остальные конфигурации
};

// 4. Глобальные переменные для отслеживания состояния
static struct {
    uint16_t active_key;   // Текущая активная TH-клавиша
    uint16_t timer;        // Таймер для определения удержания
    bool is_holding;       // Флаг удержания
} th_state = {.active_key = KC_NO, .timer = 0, .is_holding = false};

// 5. Функция для отправки символа с учётом Caps Word
void send_key_with_caps(uint16_t keycode) {
    if (is_caps_word_on() && ((keycode >= KC_A && keycode <= KC_Z) ||
        (get_cur_lang() == LANG_RU && (keycode == KC_DOT || keycode == KC_COMM || keycode == KC_QUOT || keycode == KC_LBRC)))) {
        tap_code16(S(keycode)); // Заглавная буква
    } else {
        tap_code16(keycode); // Стандартная отправка
    }
}

// end tap dance stuff

bool pre_process_record_kb(uint16_t keycode, keyrecord_t* record) {
    return pre_process_record_ruen(keycode, record) && pre_process_record_user(keycode, record);
}


// Помести этот код в тот же .c файл, где объявлены
// pre_process_record_kb / process_record_kb / post_process_record_user.
// Если helper выше их определения — добавь прототипы.

static void process_as_full_keypress(uint16_t keycode) {
    keyrecord_t rec;

    // --- PRESS ---
    memset(&rec, 0, sizeof(rec));
    rec.event.pressed = true;
    rec.event.time = timer_read32();

    // run pre-process (как делают QMK)
    bool ok = true;
    // Если у тебя объявлена pre_process_record_kb — вызываем её, чтобы сработали pre hooks.
    // Если её нет в пределах видимости — закомментируй этот блок.
    ok = pre_process_record_kb ? pre_process_record_kb(keycode, &rec) : true;

    if (ok) {
        // основной обработчик (включает process_record_ruen и process_record_user)
        process_record_kb(keycode, &rec);

        // пост-обработка (если у тебя есть post_process_record_user)
        if (post_process_record_user) post_process_record_user(keycode, &rec);
    }

    // --- RELEASE ---
    memset(&rec, 0, sizeof(rec));
    rec.event.pressed = false;
    rec.event.time = timer_read32();

    ok = pre_process_record_kb ? pre_process_record_kb(keycode, &rec) : true;
    if (ok) {
        process_record_kb(keycode, &rec);
        if (post_process_record_user) post_process_record_user(keycode, &rec);
    }
}

void process_combo_event(uint16_t combo_index, bool pressed) {
    if (!pressed) return;

    uint16_t keycode = pgm_read_word(&key_combos[combo_index].keycode);

    if (LG_START <= keycode && keycode < LG_END) {
        process_as_full_keypress(keycode);
    }
}



bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
//   #ifdef WPM_ENABLE
//     if (record->event.pressed) {
//         extern uint32_t tap_timer;
//         tap_timer = timer_read32();
//     }
//   #endif

    switch (keycode) { // This will do most of the grunt work with the keycodes.
        case WRD_NXT:
            if (record->event.pressed) {
                register_code16(keymap_config.swap_lctl_lgui ? A(KC_RIGHT) : C(KC_RIGHT));
            } else
                unregister_code16(keymap_config.swap_lctl_lgui ? A(KC_RIGHT) : C(KC_RIGHT));
            return false;

        case WRD_PRV:
            if (record->event.pressed) {
                register_code16(keymap_config.swap_lctl_lgui ? A(KC_LEFT) : C(KC_LEFT));
            } else
                unregister_code16(keymap_config.swap_lctl_lgui ? A(KC_LEFT) : C(KC_LEFT));
            return false;

        case WNEXT:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(keymap_config.swap_lctl_lgui ? KC_LGUI : KC_LALT);
                }
                alt_tab_timer = timer_read();
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            return false;

        case WPREV:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(keymap_config.swap_lctl_lgui ? KC_LGUI : KC_LALT);
                }
                alt_tab_timer = timer_read();
                register_code16(S(KC_TAB));
            } else {
                unregister_code16(S(KC_TAB));
            }
            return false;

        case KC_CAPS:
            if (record->event.pressed) {
                #ifdef AUDIO_ENABLE
                    PLAY_SONG(caps_sound);
                #endif
                    }
            return true; // Let QMK send the enter press/release events

        case LAYER_NEXT:
            // Our logic will happen on presses, nothing is done on releases
            if (!record->event.pressed) {
                // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
                return false;
            }

            uint8_t current_layer = get_highest_layer(layer_state);

            // Check if we are within the range, if not quit
            if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
                return false;
            }

            uint8_t next_layer = current_layer + 1;
            if (next_layer > LAYER_CYCLE_END) {
                next_layer = LAYER_CYCLE_START;
            }
            layer_move(next_layer);
            return false;

        case LAYER_PREV:
        // Our logic will happen on presses, nothing is done on releases
            if (!record->event.pressed) {
                // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
                return false;
            }

            uint8_t this_layer  = get_highest_layer(layer_state);

            // Check if we are within the range, if not quit
            if (this_layer > LAYER_CYCLE_END || this_layer < LAYER_CYCLE_START) {
                return false;
            }

            uint8_t prev_layer = this_layer - 1;
            if (prev_layer > LAYER_CYCLE_END) {
                prev_layer = LAYER_CYCLE_START;
            }
            layer_move(prev_layer);
            return false;
        case LALT(KC_SPACE):
        case LCTL(KC_L):
        case LCTL(KC_T):
        case LCTL(KC_F):
            layer_off(_NAV);
            layer_off(_MOUSE);
            return true;
        case KC_SCLN:
        case KC_QUOT:
        case KC_LBRC:
        case KC_RBRC:
        case KC_GRAVE:
        case KC_COMMA:
        case KC_DOT:
        case KC_A ... KC_Z:
            if(IS_LAYER_ON(_MOUSE)) {
                layer_off(_MOUSE);
            }
            return process_record_user(keycode, record);

        case EH_PRINFO: {
            if (record->event.pressed) {
                send_string("Product: " PRODUCT "\n");
                send_string("FW version: " QMK_VERSION "\n");
                send_string("Build date: " QMK_BUILDDATE "\n");
                send_string("Git hash: " QMK_GIT_HASH "\n");

                send_string("Mac mode: ");
                send_string(keymap_config.swap_lctl_lgui ? "on\n" : "off\n");

                send_string("Unicode mode: ");
                uint8_t uc_input_mode = get_unicode_input_mode();
                switch (uc_input_mode) {
                    case UNICODE_MODE_MACOS:
                        send_string("Mac\n");
                        break;
                    case UNICODE_MODE_LINUX:
                        send_string("Linux\n");
                        break;
                    case UNICODE_MODE_WINDOWS:
                        send_string("Windows\n");
                        break;
                    case UNICODE_MODE_WINCOMPOSE:
                        send_string("WinCompose\n");
                        break;
                    default:
                        send_string("error\n");
                        break;
                }

                send_string("RuEn mode: ");
                uint8_t ruen_mode = get_ruen_toggle_mode();
                if (ruen_mode == TG_DEFAULT)
                    send_string("default\n");
                else if (ruen_mode == TG_M0)
                    send_string("M0\n");
                else if (ruen_mode == TG_M1M2)
                    send_string("M1M2\n");
                else
                    send_string("error\n");

                send_string("RuEn layout: ");
                send_string(get_ruen_mac_layout() ? "Mac\n" : "PC\n");

                send_string("Led blinks: ");
                send_string(get_led_blinks() ? "enabled\n" : "disabled\n");
            }
            return false;
        }

        // case LG_TOGGLE...LG_END:
        //     return process_record_ruen(keycode, record);
        // }
    }

    if (!process_record_ruen(keycode, record)) return false;

    if (!process_record_pointing(keycode, record)) return false;

    return process_record_user(keycode, record);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        if(IS_LAYER_ON(_MCTRL))
        {
            layer_off(_MOUSE);
        }
    }

    if (keycode >= TH_K_Z && keycode < TH_LAST) {
        uint16_t index = keycode - TH_K_Z;

        if (record->event.pressed) {
            // Нажатие: запоминаем клавишу и запускаем таймер
            th_state.active_key = keycode;
            th_state.timer = timer_read();
            th_state.is_holding = false;
        } else {
            // Отпускание
            if (!th_state.is_holding) {
                // Не было удержания - отправляем tap-действие
                send_key_with_caps(th_config[index].tap_key);
            }
            th_state.active_key = KC_NO;
        }
        return true;
    }

    switch (keycode) {

        case KC_LSFT:
            return true;

        case KC_BTN1:
            if (record->event.pressed) {
                ctrl_pressed = true;
                layer_on(_MCTRL);
                register_code(KC_BTN1);
            } else {
                unregister_code(KC_BTN1);
                layer_off(_MCTRL);
                ctrl_pressed = false;
                wait_ms(20);
                unregister_code(KC_LCTL);
            }
            return false;

        case DF(0):
        case DF(1):
        case TO(0):
            if(is_caps_word_on()) caps_word_off();
            if(get_oneshot_mods()) clear_oneshot_mods();
            set_pointing_mode(POINTING_MODE_NORMAL);
            return true;

        case LT(5, KC_SPACE):
            if (record->tap.count && record->event.pressed) {
                if(ctrl_pressed) {
                    register_code(KC_LCTL);
                    tap_code16(KC_TAB);
                    return false;
                } else if(((get_mods() | get_oneshot_mods() | get_weak_mods()) & MOD_BIT(KC_LCTL)) == MOD_BIT(KC_LCTL)) {
                    tap_code16(KC_TAB);
                    return false;
                } else {
                    return true;
                }
            }
        case KC_SCRL:

        default:
            return true;
    }
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (modifiersPressed()) {
        if(alpha_layer_active) {

            layer_on(_MODS);
            mod_layer_on = true;
        }
    } else if (mod_layer_on && IS_LAYER_ON(_MODS)) {
        layer_off(_MODS);

        mod_layer_on = false;
    }
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {

        case TH_K_Z ... TH_LAST:
        // Разрешаем Caps Word продолжать работу
            return true;
        // Keycodes for russian symbols
        case KC_SCLN:
        case KC_QUOT:
        case KC_LBRC:
        case KC_RBRC:
        case KC_GRAVE:
        case KC_COMMA:
        case KC_DOT:
            if (get_cur_lang() == LANG_RU) {
                add_weak_mods(MOD_BIT(KC_LSFT));
                return true;
            } else
                return false;

        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        // For some reason weak mode doesn't work on this keycodes
        // so we additionaly add weak mode in process_russian_letter(...)
        case LG_RU_BE:
        case LG_RU_YU:
        case LG_RU_ZHE:
        case LG_RU_E:
        case LG_RU_HRD_SGN:
        case LG_RU_KHA:
        case LG_RU_YO:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_MINS:
        case KC_EQL:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case QK_MACRO_1 ... QK_MACRO_31:
        case LG_SET_EN:
        case LG_SET_RU:

        case TD(4) ... TD(23):
            return true;

        case KC_ENTER: tap_code16(KC_ENTER);
        default:
            return false; // Deactivate Caps Word.
    }
}

void caps_word_set_user(bool active) {
    // if (active) {
    //     if (get_cur_lang() == LANG_RU)
    //     layer_on(3);
    // } else {
    //     layer_off(3);
    // }
}

bool led_update_user(led_t led_state) {
    numlock_enabled = led_state.num_lock;
    scrolllock_enabled = led_state.scroll_lock;
    return true;
}

void matrix_scan_kb(void) { // The very important timer.
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 650) {
        unregister_code(keymap_config.swap_lctl_lgui ? KC_LGUI : KC_LALT);
        is_alt_tab_active = false;
        }
    }

    matrix_scan_user();
}

void matrix_scan_user(void) {
    if (th_state.active_key != KC_NO && !th_state.is_holding) {
        if (timer_elapsed(th_state.timer) > TAPPING_TERM) {
            // Время удержания истекло - активируем hold-действие
            uint16_t index = th_state.active_key - TH_K_Z;
            tap_code16(th_config[index].hold_key);
            th_state.is_holding = true;
        }
    }

    // ... остальной код ...
}

void keyboard_post_init_kb(void) {
#ifdef CONSOLE_ENABLE
    debug_enable = true;
#endif

    kb_config.raw = eeconfig_read_kb();
    set_ruen_toggle_mode(kb_config.ruen_toggle_mode);
    set_ruen_mac_layout(kb_config.ruen_mac_layout);

#ifdef RGBLIGHT_ENABLE
    keyboard_post_init_rgb();
#endif
    keyboard_post_init_hid();
    keyboard_post_init_user();
}

layer_state_t default_layer_state_set_kb(layer_state_t state) {
    state = default_layer_state_set_user(state);
#ifdef RGBLIGHT_ENABLE
    layer_state_set_rgb(layer_state | state);
#endif
    return state;
}

layer_state_t layer_state_set_kb(layer_state_t state) {
    if (is_alt_tab_active) {
        unregister_code(keymap_config.swap_lctl_lgui ? KC_LGUI : KC_LALT);
        is_alt_tab_active = false;
    }
    state = layer_state_set_user(state);
#ifdef RGBLIGHT_ENABLE
    layer_state_set_rgb(state);
#endif
    return state;
}

void housekeeping_task_kb(void) {
#ifdef CONSOLE_ENABLE
    {
        static uint32_t t0 = 0;
        uint32_t        dt = timer_elapsed32(t0);
        if (t0 == 0) dt = 0;
        t0 = timer_read32();

        static uint32_t last_print = 0;
        static uint32_t max_dt     = 0;
        static uint32_t hz         = 0;

        max_dt = MAX(max_dt, dt);
        hz += 1;
        if (last_print == 0 || timer_elapsed32(last_print) > 1000) {
            dprintf("hz=%ld max_dt=%ld \n", hz, max_dt);
            max_dt     = 0;
            hz         = 0;
            last_print = timer_read32();
        }
    }
#endif

    uint32_t activity_elapsed = last_input_activity_elapsed();

    if (activity_elapsed > EH_TIMEOUT) {
#ifdef RGBLIGHT_ENABLE
        rgb_off();
#endif
    } else {
#ifdef RGBLIGHT_ENABLE
        rgb_on();
#endif
    }

#if defined(OLED_ENABLE) && defined(SPLIT_KEYBOARD)
    housekeeping_task_split_oled();
#endif
    housekeeping_task_ruen();
    housekeeping_task_user();
}

uint8_t get_current_layer(void) {
    return get_highest_layer(layer_state | default_layer_state);
}

static const char* PROGMEM LAYER_NAME[] =   {
    "Base ",
    "Lower",
    "Raise",
    "Adjst",
    "Four ",
    "Five ",
    "Six  ",
    "Seven",
    "Eight",
    "Nine ",
    "Ten  ",
    "Elevn",
    "Twlve",
    "Thrtn",
    "Frtn ",
    "Fiftn",
};

static const char* PROGMEM LAYER_UPPER_NAME[] =   {
    "BASE ",
    "DIKTR",
    "SHCUT",
    "MOUSE",
    "# NAV",
    "# NUM",
    "# APP",
    "# SYM",
    "# FUN",
    "# OS ",
    "# L10",
    "# L11",
    "# L12",
    "# CTR",
    "# L14",
    "# GAM",
    "# GFN",
};

layer_state_t layer_state_set_user(layer_state_t state) {
    // printf("changed layer to %u", get_highest_layer(state));
    if(get_highest_layer(state) > 1) {
        alpha_layer_active = false;
    } else {
        alpha_layer_active = true;
    }
    return state;
}

bool modifiersPressed(void) {
    return (get_mods() | get_oneshot_mods() | get_weak_mods()) & MOD_MASK_CAG; // (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI))
}

const char* layer_name(uint8_t layer) {
    if (layer >= 0 && layer <= 15)
        return LAYER_NAME[layer];
    else
        return "Undef";
}

const char* layer_upper_name(uint8_t layer) {
    if (layer >= 0 && layer <= 15)
        return LAYER_UPPER_NAME[layer];
    else
        return "UNDEF";
}
