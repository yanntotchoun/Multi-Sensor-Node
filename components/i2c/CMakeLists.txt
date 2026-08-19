idf_component_register(
    SRCS "i2c.c"
    INCLUDE_DIRS "include"
    REQUIRES 
    driver esp_driver_i2c
     esp_driver_gpio
    esp_timer
    log
    esp_common
    esp_driver_uart
    bme688
    freertos driver
)