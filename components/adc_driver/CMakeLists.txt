idf_component_register(
    SRCS "adc_driver.c"
    INCLUDE_DIRS "include"
    REQUIRES
    esp_driver_i2c
    esp_driver_gpio
    esp_adc
    esp_timer
    esp_common
    esp_driver_uart
    i2c
    log
)