#ifndef PPM_DECODER_H
#define PPM_DECODER_H

#include "driver/gpio.h"
#include <stdbool.h>
#include <stdint.h>

#define PPM_CHANNELS 8

extern volatile uint16_t ppm_channels[PPM_CHANNELS];
extern volatile bool ppm_frame_ready;

esp_err_t ppm_decoder_init(uint8_t gpio_pin);

#endif // PPM_DECODER_H