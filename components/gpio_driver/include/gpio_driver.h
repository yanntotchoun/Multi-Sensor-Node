#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER__H
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "esp_check.h"

esp_err_t gpio_init(void);
uint8_t get_button_power(void);
uint8_t get_button_sensor(void);
uint8_t get_button_page(void);
#endif