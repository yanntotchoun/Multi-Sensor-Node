idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES bme688_driver i2c 
)