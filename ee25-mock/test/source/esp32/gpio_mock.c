/**
 * @file Mocked ESP32 GPIO driver implementation details.
 */
#include <stdint.h>
#include <stdio.h>

#include "arch/env/hw_platform.h"
#include "esp32/gpio_mock.h"

/** Pin registry (1 = enabled, 0 = disabled). */
static uint64_t pin_reg = 0U;

esp_err_t gpio_set_level(const gpio_num_t gpio_num, const uint32_t level)
{
    if (level) { pin_reg |= (1U << gpio_num); }
    else { pin_reg &= ~(1U << gpio_num); }
    return 0;
}

int gpio_get_level(const gpio_num_t gpio_num)
{
    return 0U != (pin_reg & (1U << gpio_num)) ? 1U : 0U;
}
