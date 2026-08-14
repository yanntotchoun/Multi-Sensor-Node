idf_component_register(
    SRCS "oled.c"
    INCLUDE_DIRS "include"
    REQUIRES 
    i2c
    driver esp_driver_i2c
    esp_driver_gpio
    esp_timer
    esp_common
)