MCU = RP2040
BOARD = GENERIC_RP_RP2040
BOOTLOADER = rp2040
# ALLOW_WARNINGS = yes
# PICO_INTRINSICS_ENABLED = no

# Build options
SPLIT_KEYBOARD = yes
SERIAL_DRIVER = vendor
RGBLIGHT_ENABLE = yes
QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += st7789_spi
QUANTUM_PAINTER_LVGL_INTEGRATION = yes

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = azoteq_iqs5xx

NKRO_ENABLE = yes
BOOTMAGIC_ENABLE = yes
MOUSEKEY_ENABLE = yes
EXTRAKEY_ENABLE = yes
LTO_ENABLE = no
VIA_ENABLE = yes
TAP_DANCE_ENABLE = yes
COMBO_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes
CAPS_WORD_ENABLE = yes
REPEAT_KEY_ENABLE = yes
AUTO_SHIFT_ENABLE = yes
NO_USB_STARTUP_CHECK = yes


WPM_ENABLE = yes
RAW_ENABLE = yes

SRC += keyboards/ergohaven/ergohaven_rgb.c
SRC += keyboards/ergohaven/ergohaven_main.c
SRC += keyboards/ergohaven/ergohaven_ruen.c
SRC += keyboards/ergohaven/hid.c
SRC += lvgl_helpers.c \
	   display.c
