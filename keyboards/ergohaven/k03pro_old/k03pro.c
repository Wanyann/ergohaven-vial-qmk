// Copyright 2022 Ergohaven (@ergohaven)
// SPDX-License-Identifier: GPL-2.0-or-later
#include "quantum.h"
#include "display.h"

bool is_display_side(void) {
#ifdef K3_DISPLAY_RIGHT
    return !is_keyboard_left();
#endif
#ifdef K3_DISPLAY_LEFT
    return is_keyboard_left();
#endif
    return false;
}

void housekeeping_task_user(void) {
    if (is_display_enabled()) {
        display_housekeeping_task();
    }
}

void keyboard_post_init_user(void) {
    display_init_kb();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (is_display_enabled()) {
        display_process_layer_state(get_highest_layer(state));
    }
    return state;
}

