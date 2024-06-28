#pragma once
#include "ergohaven.h"

enum ruen_custom_keycodes {
    LG_TOGGLE = LAYER_PREV + 2,
    LG_SYNC,
    LG_SET_EN,
    LG_SET_RU,
    LG_SET_M0,
    LG_SET_M1M2,
    LG_SET_DFLT,

    // symbols exist in russian and english layout
    LG_RU_EN_START,
    LG_DOT = LG_RU_EN_START,
    LG_COMMA,
    LG_SCLN,
    LG_COLON,
    LG_DQUO,
    LG_QUES,
    LG_SLASH,

    // symbols exist in english only layout
    LG_EN_START,
    LG_LBR = LG_EN_START,
    LG_RBR,
    LG_LCBR,
    LG_RCBR,
    LG_LT,
    LG_GT,
    LG_GRAVE,
    LG_TILD,
    LG_AT,
    LG_HASH,
    LG_DLR,
    LG_CIRC,
    LG_AMPR,
    LG_PIPE,
    LG_QUOTE,

    // symbols exist in russian only layout
    LG_NUM,

    //
    LG_WORD,

    // end of ru en symbols range
    LG_END,
};

enum { LANG_EN = 0, LANG_RU };

enum { TG_DEFAULT = 0, TG_M0, TG_M1M2 };

bool pre_process_record_ruen(uint16_t keycode, keyrecord_t *record);

bool process_record_ruen(uint16_t keycode, keyrecord_t *record);

void housekeeping_task_ruen(void);

uint8_t get_cur_lang(void);
