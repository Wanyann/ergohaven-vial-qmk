#pragma once

#include_next <mcuconf.h>

#undef RP_SPI_USE_SPI0
#define RP_SPI_USE_SPI0 TRUE

#undef RP_PWM_USE_PWM7
#define RP_PWM_USE_PWM7 TRUE

// #undef RP_PWM_USE_PWM4
// #define RP_PWM_USE_PWM4 TRUE
