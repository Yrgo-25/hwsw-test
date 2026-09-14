/**
 * @file ESP32 GPIO driver mock implementation details.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "arch/env/hw_platform.h"
#include "test/esp32/gpio_mock.h"

/** Maximum valid pin number for ESP32-S3. */
#define PIN_MAX 48U

/** Pin registry (1 = enabled, 0 = disabled). */
static uint64_t pin_reg = 0U;

/** Last GPIO configuration passed to gpio_config(). */
static gpio_config_t last_config;

// -----------------------------------------------------------------------------
static inline bool is_pin_valid(const gpio_num_t pin) { return PIN_MAX >= pin; }

// -----------------------------------------------------------------------------
esp_err_t gpio_set_level(const gpio_num_t pin, const uint32_t level)
{
    // Check the pin number, return error if invalid.
    if (!is_pin_valid(pin)) { return ESP_ERR_INVALID_ARG; }

    // Set new GPIO level, then return true to indicate success.
    if (0U != level) { pin_reg |= (1ULL << pin); }
    else { pin_reg &= ~(1ULL << pin); }
    return ESP_OK;
}

// -----------------------------------------------------------------------------
int gpio_get_level(const gpio_num_t pin)
{
    // Read the GPIO level, return 1 if enabled, 0 if disabled.
    return (pin_reg & (1ULL << pin)) ? 1 : 0;
}

// -----------------------------------------------------------------------------
esp_err_t gpio_config(const gpio_config_t* config)
{
    // Check the configuration, return error if invalid.
    if (NULL == config) { return ESP_ERR_INVALID_ARG; }

    // Store a copy of the configuration, then return ESP_OK to indicate success.
    last_config = *config;
    return ESP_OK;
}

// -----------------------------------------------------------------------------
const gpio_config_t* gpio_last_config(void) { return &last_config; }
