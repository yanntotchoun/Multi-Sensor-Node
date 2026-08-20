idf_component_register(
    SRCS "gpio_driver.c"
    INCLUDE_DIRS "include"
    REQUIRES bme688  
    esp_driver_i2c
    esp_driver_gpio
    esp_timer
    esp_common
    esp_driver_uart
    i2c
    log
)