#include "ergohaven.h"
#include "ergohaven_ruen.h"
#include "ergohaven_rgb.h"
#include "ergohaven_oled.h"
#include "hid.h"

typedef union {
    uint32_t raw;
    struct {
        uint8_t ruen_toggle_mode : 2;
    };
} kb_config_t;

kb_config_t kb_config;

static bool numlock_enabled = false;
static bool scrolllock_enabled = false;
static bool mod_layer_on = false;
static bool alpha_layer_active = true;

void kb_config_update_ruen_toggle_mode(uint8_t mode)
{
    kb_config.ruen_toggle_mode = mode;
    eeconfig_update_kb(kb_config.raw);
}

#ifdef AUDIO_ENABLE
float base_sound[][2] = SONG(TERMINAL_SOUND);
float caps_sound[][2] = SONG(CAPS_LOCK_ON_SOUND);
#endif

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
// uint8_t prev_lang = LANG_EN;

// uint8_t mod_state;
// uint8_t os_mod_state;
// uint8_t weak_mod_state;

bool pre_process_record_kb(uint16_t keycode, keyrecord_t* record) {
    return pre_process_record_ruen(keycode, record) && pre_process_record_user(keycode, record);
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
//   #ifdef WPM_ENABLE
//     if (record->event.pressed) {
//         extern uint32_t tap_timer;
//         tap_timer = timer_read32();
//     }
//   #endif

  switch (keycode) { // This will do most of the grunt work with the keycodes.
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
      break;

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
      break;

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

    case KC_SCLN:
    case KC_QUOT:
    case KC_LBRC:
    case KC_RBRC:
    case KC_GRAVE:
    case KC_COMMA:
    case KC_DOT:
    case KC_A ... KC_Z:

      if(IS_LAYER_ON(2)) {
        layer_off(2);
      }

      return process_record_user(keycode, record);

    case LG_TOGGLE...LG_END:
      return process_record_ruen(keycode, record);
  }

  return process_record_user(keycode, record);
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
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
            return true;

        case KC_ENTER:
        default:
            return false; // Deactivate Caps Word.
    }
}

void caps_word_set_user(bool active) {
    if (active) {
        layer_on(3);
    } else {
        layer_off(3);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if(IS_LAYER_ON(13))
        {
            layer_off(2);
            // посмотреть тут че почему выключается скроллок когда тапаем мбтн1
        }
    }

    switch (keycode) {
        case SNIP:
            if (record->event.pressed) {
                if(!scrolllock_enabled) {
                    tap_code16(KC_SCROLL_LOCK);
                }
            } else {
                if(scrolllock_enabled) {
                    tap_code16(KC_SCROLL_LOCK);
                }
            }
            return true;

        case KC_NUM:
            return true;

        case KC_LSFT:
            return true;

        case KC_BTN1:
            if (record->event.pressed) {
                layer_on(13);
                register_code(KC_BTN1);
            } else {
                unregister_code(KC_BTN1);
                layer_off(13);
            }
            return false;

        case KC_LCTL:
            if (record->event.pressed) {
                if(scrolllock_enabled) {
                    tap_code16(C(S(KC_A)));
                    layer_off(2);

                    return false;
                }
            }
            return true;

        case TO(0):
            if(is_caps_word_on()) caps_word_off();
            if(get_oneshot_mods()) clear_oneshot_mods();
            return true;

        // case KC_LCTRL:
        //     if (record->event.pressed) {
        //         if(!numlock_enabled) {
        //             tap_code16(KC_NUM);
        //         }
        //     } else {
        //         if(numlock_enabled) {
        //             tap_code16(KC_NUM);
        //         }
        //     }

        case LALT(KC_SPACE):
            layer_off(2);
            return true;

        case KC_ENTER:

        default:
            if(numlock_enabled) tap_code16(KC_NUM);
            return true;
    }
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

    if (modifiersPressed()) {
        if(alpha_layer_active) {
            // проблема из-за того что когда мы на слое, то какой-то модификатор уже нажат, и не срабатывает комбинация для переключения языка
            // prev_lang = get_cur_lang();
            layer_on(3);
            // mod_state = get_mods();
            // os_mod_state = get_oneshot_mods();
            // weak_mod_state = get_weak_mods();

            // clear_mods();
            // clear_oneshot_mods();
            // clear_weak_mods();
            // set_lang(LANG_EN);
            mod_layer_on = true;

            // register_mods(mod_state);
            // set_oneshot_mods(os_mod_state);
            // set_weak_mods(weak_mod_state);
        }
    } else if (mod_layer_on && IS_LAYER_ON(3)) {
        layer_off(3);
        // if(get_cur_lang() != prev_lang) {
            // mod_state = get_mods();
            // os_mod_state = get_oneshot_mods();
            // weak_mod_state = get_weak_mods();

            // clear_mods();
            // clear_oneshot_mods();
            // clear_weak_mods();

            // set_lang(prev_lang);

            // register_mods(mod_state);
            // set_oneshot_mods(os_mod_state);
            // set_weak_mods(weak_mod_state);
        // }
        mod_layer_on = false;
    }

    matrix_scan_user();
}

void keyboard_post_init_kb(void) {
    kb_config.raw = eeconfig_read_kb();
    set_ruen_toggle_mode(kb_config.ruen_toggle_mode);

#ifdef RGBLIGHT_ENABLE
    keyboard_post_init_rgb();
#endif
    keyboard_post_init_hid();
    keyboard_post_init_user();
}

layer_state_t default_layer_state_set_kb(layer_state_t state) {
    state = default_layer_state_set_user(state);
#ifdef RGBLIGHT_ENABLE
    default_layer_state_set_rgb(state);
#endif
    return state;
}

layer_state_t layer_state_set_kb(layer_state_t state) {
  state = layer_state_set_user(state);
#ifdef RGBLIGHT_ENABLE
    layer_state_set_rgb(state);
#endif
    return state;
}

void housekeeping_task_kb(void) {
#if defined(OLED_ENABLE) && defined(SPLIT_KEYBOARD)
    housekeeping_task_oled();
#endif
    housekeeping_task_ruen();
    housekeeping_task_user();
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
    "# BAS",
    "# DKT",
    "# LWR",
    "# NAV",
    "# NUM",
    "# APP",
    "# SYM",
    "# FUN",
    "# OLG",
    "# L 9",
    "# TEN",
    "# L12",
    "# CTR",
    "# L14",
    "# GAM",
    "# GFN",
};

layer_state_t layer_state_set_user(layer_state_t state) {
    if(get_highest_layer(state) > 0) {
        alpha_layer_active = false;
    } else {
        alpha_layer_active = true;
    }

    if(get_highest_layer(state) != 3 || get_highest_layer(state) != 12)
    {
        if(scrolllock_enabled) tap_code16(KC_SCROLL_LOCK);
    }

    return state;
}

bool modifiersPressed(void) {
    return (get_mods() | get_oneshot_mods() | get_weak_mods()) & MOD_MASK_CAG; // (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI))
}

// bool alphaLayerIsActive() {
//     for(i = 2; i <= 14; i++) {
//         if(IS_LAYER_ON(i)) {
//             return false;
//         }
//     }
//     return true;
// }

const char* layer_name(int layer) {
    if (layer >= 0 && layer <= 15)
        return LAYER_NAME[layer];
    else
        return "Undef";
}

const char* layer_upper_name(int layer) {
    if (layer >= 0 && layer <= 15)
        return LAYER_UPPER_NAME[layer];
    else
        return "UNDEF";
}
