idf_component_register(
    SRCS "bme688_driver.c" "main.c"
    INCLUDE_DIRS "include"
    REQUIRES bme688
)