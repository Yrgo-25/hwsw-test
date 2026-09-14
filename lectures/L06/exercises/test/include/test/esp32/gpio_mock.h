/**
 * @file ESP32 GPIO driver mock.
 */
#ifndef ESP_GPIO_MOCK_H_
#define ESP_GPIO_MOCK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/** Error code indicating success. */
#define ESP_OK 0

/** Error code indicating invalid argument. */
#define ESP_ERR_INVALID_ARG -1

    /** Alias for ESP error codes. */
    typedef int esp_err_t;

    /** Alias for GPIO pins. */
    typedef uint8_t gpio_num_t;

    /**
     * @brief GPIO config structure.
     */
    typedef struct
    {
        /** Pin bit mask. */
        uint64_t pin_bit_mask;

        /** GPIO mode. */
        int mode;

        /** Pullup enablement flag. */
        int pull_up_en;

        /** Pulldown enablement flag. */
        int pull_down_en;

        /** Interrupt type. */
        int intr_type;
    } gpio_config_t;

    /**
     * @brief Set GPIO level.
     *
     * @param[in] pin GPIO pin to set.
     * @param[in] level GPIO level (1 = enabled, 0 = disabled).
     *
     * @return ESP_OK on success, ESP_ERR_INVALID_ARG if the pin is invalid.
     */
    esp_err_t gpio_set_level(gpio_num_t pin, uint32_t level);

    /**
     * @brief Get GPIO level.
     *
     * @param[in] pin GPIO pin to read.
     *
     * @return 1 if enabled, 0 if disabled.
     */
    int gpio_get_level(gpio_num_t pin);

    /**
     * @brief Configure GPIO.
     *
     * @param[in] config GPIO configuration.
     *
     * @return ESP_OK on success, ESP_ERR_INVALID_ARG if the configuration is invalid.
     */
    esp_err_t gpio_config(const gpio_config_t* config);

    /**
     * @brief Get the last configuration passed to gpio_config().
     *
     * @return Pointer to the last configuration passed to gpio_config().
     */
    const gpio_config_t* gpio_last_config(void);

#ifdef __cplusplus
} // extern "C"
#endif
#endif /** ESP_GPIO_MOCK_H_ */
