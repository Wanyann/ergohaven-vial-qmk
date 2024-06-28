#include "quantum.h"
#include "ergohaven.h"

#ifdef RGBLIGHT_ENABLE
const rgblight_segment_t PROGMEM layer0_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_WHITE});
const rgblight_segment_t PROGMEM layer1_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_RED});
const rgblight_segment_t PROGMEM layer2_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_CYAN});
const rgblight_segment_t PROGMEM layer3_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_GOLDENROD});
const rgblight_segment_t PROGMEM layer4_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_GREEN});
const rgblight_segment_t PROGMEM layer5_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_PURPLE});
const rgblight_segment_t PROGMEM layer6_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_BLUE});
const rgblight_segment_t PROGMEM layer7_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_PINK});
const rgblight_segment_t PROGMEM layer8_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_SPRINGGREEN});
const rgblight_segment_t PROGMEM layer9_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_YELLOW});
const rgblight_segment_t PROGMEM layer10_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_TEAL});
const rgblight_segment_t PROGMEM layer11_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_ORANGE});
const rgblight_segment_t PROGMEM layer12_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_AZURE});
const rgblight_segment_t PROGMEM layer13_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_CHARTREUSE});
const rgblight_segment_t PROGMEM layer14_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_CORAL});
const rgblight_segment_t PROGMEM layer15_rgb[] = RGBLIGHT_LAYER_SEGMENTS({0, 2, HSV_GOLD});

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    layer0_rgb, layer1_rgb, layer2_rgb, layer3_rgb, layer4_rgb, layer5_rgb, layer6_rgb, layer7_rgb,
    layer8_rgb, layer9_rgb, layer10_rgb, layer11_rgb, layer12_rgb, layer13_rgb, layer14_rgb, layer15_rgb
);
#endif

void keyboard_post_init_rgb(void) {
#ifdef RGBLIGHT_ENABLE
    rgblight_layers = my_rgb_layers;
#endif
}

void default_layer_state_set_rgb(layer_state_t state) {
#ifdef RGBLIGHT_ENABLE
    rgblight_set_layer_state(0, layer_state_cmp(state, _BASE));
#endif
}

void layer_state_set_rgb(layer_state_t state) {
#ifdef RGBLIGHT_ENABLE
    for (int layer = 1; layer <= _FIFTEEN; ++layer)
        rgblight_set_layer_state(layer, layer_state_cmp(state, layer));
#endif
}
