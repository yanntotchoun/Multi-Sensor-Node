idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES bme688_driver i2c oled
    esp_driver_gpio
    esp_timer
    esp_common
    esp_driver_uart
    log
)