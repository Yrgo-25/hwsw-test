/**
 * @file Mocked ESP32 GPIO driver.
 */
#ifndef ESP32_GPIO_MOCK_H_
#define ESP32_GPIO_MOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "arch/env/hw_platform.h"

typedef uint8_t gpio_num_t;

esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level);

int gpio_get_level(gpio_num_t gpio_num);

#ifdef __cplusplus
}
#endif

#endif /** ESP32_GPIO_MOCK_H_ */