#include "ppm_decoder.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "globals.h"

#define PPM_SYNC_MIN 5000  // 5ms sync pulse minimum
#define PPM_PULSE_MIN 900  // 0.9ms minimum pulse
#define PPM_PULSE_MAX 2100 // 2.1ms maximum pulse

volatile uint16_t ppm_channels[PPM_CHANNELS] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500}; // Default to center
volatile bool ppm_frame_ready = false;
volatile uint8_t ppm_channel_count = 0;
volatile uint64_t last_interrupt_time = 0;

void IRAM_ATTR ppm_isr_handler(void* arg) {
    uint64_t current_time = esp_timer_get_time();
    uint32_t pulse_width = current_time - last_interrupt_time;
    last_interrupt_time = current_time;
    
    if (pulse_width > PPM_SYNC_MIN) {
        // Sync pulse detected, reset channel counter
        if (ppm_channel_count > 0){
            ppm_frame_ready = true;
        }
        ppm_channel_count = 0;
    } else if (pulse_width >= PPM_PULSE_MIN && pulse_width <= PPM_PULSE_MAX) {
        // Valid channel pulse
        if (ppm_channel_count < PPM_CHANNELS) {
            ppm_channels[ppm_channel_count] = pulse_width;
            ppm_channel_count++;
        }
    }
}

esp_err_t ppm_decoder_init(uint8_t gpio_pin) {
    gpio_config_t ppm_conf = {
        .pin_bit_mask = (1ULL << gpio_pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE
    };
    
    ESP_ERROR_CHECK(gpio_config(&ppm_conf));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(gpio_pin, ppm_isr_handler, NULL));

    last_interrupt_time = esp_timer_get_time();
    return ESP_OK;
}
