idf_component_register(
    SRCS "mq135.c"
    INCLUDE_DIRS "include"
    REQUIRES 
    driver esp_driver_i2c
     esp_driver_gpio
    esp_timer
    esp_common
    esp_driver_uart
)