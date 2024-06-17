#include "ergohaven.h"
#include "info_config.h"
#include "ergohaven_ruen.h"
#include "hid.h"
#include "transactions.h"

#ifdef OLED_ENABLE

#    include "bongocat.c"

typedef union {
    uint32_t raw;
    struct {
        uint8_t oled_slave : 3;
        uint8_t oled_master : 3;
        bool    right_encoder : 1;
        bool    left_encoder : 1;
    };
} vial_config_t;

vial_config_t vial_config;

typedef enum {
    OLED_STATUS_CLASSIC = 0,
    OLED_SPLASH,
    OLED_STATUS_MODERN,
    OLED_STATUS_MINIMALISTIC,
    OLED_BONGOCAT,
    // OLED_MEDIA,
    OLED_DISABLED,
} oled_mode_t;

oled_mode_t get_oled_mode(void) {
    if (is_keyboard_master()) return vial_config.oled_master;

    // first two modes swapped for slave
    if (vial_config.oled_slave == OLED_STATUS_CLASSIC) return OLED_SPLASH;
    if (vial_config.oled_slave == OLED_SPLASH) return OLED_STATUS_CLASSIC;

    return vial_config.oled_slave;
}

oled_rotation_t get_desired_oled_rotation(void) {
    int mode = get_oled_mode();
    switch (mode) {
        case OLED_BONGOCAT:
            // case OLED_MEDIA:
            return is_keyboard_left() ? OLED_ROTATION_0 : OLED_ROTATION_180;
            break;
#    ifdef EH_K02
        case OLED_SPLASH:
            return OLED_ROTATION_180;
#    endif
        default:
            return OLED_ROTATION_270;
    }
}

static oled_rotation_t current_oled_rotation;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    current_oled_rotation = get_desired_oled_rotation();
    return current_oled_rotation;
}

void render_status_classic(void) {
    // Print current mode
    oled_clear();

    oled_set_cursor(0, 2);
    if (strlen(PRODUCT) <= 5) // Imperial44 is too long name
        oled_write_P(PSTR(PRODUCT), false);

    oled_set_cursor(0, 4);
    oled_write_P(PSTR(EH_VERSION_STR), false);

    oled_set_cursor(0, 7);
    oled_write(get_cur_lang() == LANG_EN ? " EN " : " RU ", false);
    
    oled_set_cursor(0, 10);
    oled_write_P(PSTR("LAYER"), false);
    // Print current layer
    oled_set_cursor(0, 12);
    oled_write_P(PSTR(layer_upper_name(get_highest_layer(layer_state))), false);

    oled_set_cursor(0, 15);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_P(PSTR("CPSLK"), led_usb_state.caps_lock);
}

void render_status_modern(void) {
    oled_clear();
    oled_write_ln(layer_upper_name(get_highest_layer(layer_state)), false);
    oled_set_cursor(0, 1);
    if (keymap_config.swap_lctl_lgui)
        oled_write_P(PSTR("   \01\02   \03\04"), false);
    else
        oled_write_P(PSTR("          "), false);

    oled_set_cursor(0, 2);
    oled_write(get_cur_lang() == LANG_EN ? "EN" : "RU", false);

    oled_set_cursor(0, 4);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_P(led_usb_state.caps_lock ? PSTR("CPS\07\10") : PSTR("CPS\05\06"), false);
    oled_write_P(led_usb_state.num_lock ? PSTR("NUM\07\10") : PSTR("NUM\05\06"), false);

    oled_set_cursor(0, 7);
    oled_write_P((get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT ? PSTR("SFT\07\10") : PSTR("SFT\05\06"), false);
    oled_write_P((get_mods() | get_oneshot_mods()) & MOD_MASK_CTRL ? PSTR("CTL\07\10") : PSTR("CTL\05\06"), false);
    oled_write_P((get_mods() | get_oneshot_mods()) & MOD_MASK_ALT ? PSTR("ALT\07\10") : PSTR("ALT\05\06"), false);
    oled_write_P((get_mods() | get_oneshot_mods()) & MOD_MASK_GUI ? PSTR("GUI\07\10") : PSTR("GUI\05\06"), false);

    char buf[16];
    int  wpm = get_current_wpm();
    if (wpm < 10)
        sprintf(buf, "WPM %d", wpm);
    else if (wpm < 100)
        sprintf(buf, "W  %d", wpm);
    else
        sprintf(buf, "W %d", wpm);
    oled_set_cursor(0, 12);
    oled_write_ln(buf, false);

    struct hid_data_t* hid_data = get_hid_data();
    if (hid_data->time_changed) {
        sprintf(buf, "%02d:%02d", hid_data->hours, hid_data->minutes);
        oled_set_cursor(0, 14);
        oled_write_ln(buf, false);
    }
}

void render_status_minimalistic(void) {
    oled_clear();
    int layer = get_highest_layer(layer_state);
    if (layer == 0)
        oled_write_ln("     ", false);
    else
        oled_write_ln(layer_upper_name(layer), false);

    oled_set_cursor(0, 1);
    if (keymap_config.swap_lctl_lgui)
        oled_write_P(PSTR("   \01\02   \03\04"), false);
    else
        oled_write_P(PSTR("          "), false);

    oled_set_cursor(0, 2);
    oled_write(get_cur_lang() == LANG_EN ? "  " : "RU", false);

    led_t led_usb_state = host_keyboard_led_state();
    oled_set_cursor(0, 4);
    oled_write_P(led_usb_state.caps_lock ? PSTR("CAPS") : PSTR("     "), false);
    oled_set_cursor(0, 5);
    oled_write_P(led_usb_state.num_lock ? PSTR("NUM") : PSTR("     "), false);

    char buf[16];
    buf[0] = ((get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT) ? 'S' : ' ';
    buf[1] = ((get_mods() | get_oneshot_mods()) & MOD_MASK_CTRL) ? 'C' : ' ';
    buf[2] = ((get_mods() | get_oneshot_mods()) & MOD_MASK_ALT) ? 'A' : ' ';
    buf[3] = ((get_mods() | get_oneshot_mods()) & MOD_MASK_GUI) ? 'G' : ' ';
    buf[4] = '\0';

    oled_set_cursor(0, 7);
    oled_write(buf, false);

    int wpm = get_current_wpm();
    if (wpm > 0) {
        if (wpm < 10)
            sprintf(buf, "WPM %d", wpm);
        else if (wpm < 100)
            sprintf(buf, "W  %d", wpm);
        else
            sprintf(buf, "W %d", wpm);

    } else
        sprintf(buf, "     ");
    oled_set_cursor(0, 9);
    oled_write_ln(buf, false);

    struct hid_data_t* hid_data = get_hid_data();
    if (hid_data->time_changed) {
        sprintf(buf, "%02d:%02d", hid_data->hours, hid_data->minutes);
        oled_set_cursor(0, 11);
        oled_write_ln(buf, false);
    }
}

void render_media(void) {
    oled_clear();
    struct hid_data_t* hid_data = get_hid_data();
    if (hid_data->media_artist_changed || hid_data->media_title_changed) {
        oled_set_cursor(0, 1);
        hid_data->media_title[21] = '\0';
        oled_write_ln(hid_data->media_title, false);
        oled_set_cursor(0, 3);
        hid_data->media_artist[21] = '\0';
        oled_write_ln(hid_data->media_artist, false);
    }
}

__attribute__((weak)) void ergohaven_dark_draw(void) {}

static uint32_t last_layout_options_time = 0;

void via_set_layout_options_kb(uint32_t value) {
    if (vial_config.raw == value) return;
    vial_config.raw          = value;
    last_layout_options_time = sync_timer_read32();
}

bool oled_task_kb(void) {
    // Defer to the keymap if they want to override
    if (!oled_task_user()) {
        return false;
    }

    uint32_t activity_elapsed = MIN(last_input_activity_elapsed(), //
                                    sync_timer_elapsed32(last_layout_options_time));

#    ifdef RGBLIGHT_ENABLE
    if (activity_elapsed > EH_TIMEOUT) {
        rgblight_suspend();
    } else {
        rgblight_wakeup();
    }
#    endif

    if (activity_elapsed > EH_TIMEOUT || get_oled_mode() == OLED_DISABLED) {
        oled_off();
        return false;
    } else {
        oled_on();
    }

    if (get_desired_oled_rotation() != current_oled_rotation) oled_init(get_desired_oled_rotation());

    uint8_t mode = get_oled_mode();
    switch (mode) {
        case OLED_STATUS_CLASSIC:
            render_status_classic();
            break;

        case OLED_STATUS_MODERN:
            render_status_modern();
            break;

        case OLED_STATUS_MINIMALISTIC:
            render_status_minimalistic();
            break;

            // case OLED_MEDIA:
            //     render_media();
            //     break;

        case OLED_SPLASH:
            ergohaven_dark_draw();
            break;

        case OLED_BONGOCAT:
            render_bongocat();
            break;

        case OLED_DISABLED:
        default:
            oled_clear();
            break;
    }

    return false;
}

void sync_config(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    uint32_t value;
    memcpy(&value, in_data, sizeof(uint32_t));
    via_set_layout_options_kb(value);
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(RPC_SYNC_CONFIG, sync_config);
}

void housekeeping_task_oled(void) {
    if (is_keyboard_master()) {
        // Interact with slave every 500ms
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > 500) {
            if (transaction_rpc_send(RPC_SYNC_CONFIG, sizeof(vial_config_t), &vial_config)) {
                last_sync = timer_read32();
            }
        }
    }
}

#endif
