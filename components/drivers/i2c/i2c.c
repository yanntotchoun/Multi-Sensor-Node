#include "include/i2c.h"
#define SCL_IO 22
#define SDA_IO 21

esp_err_t esp_init_i2c(void){
i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t bme688;

i2c_master_bus_config_t i2c_mst_config = {
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port = I2C_NUM_1,
    .scl_io_num = SCL_IO,
    .sda_io_num = SDA_IO,
    .glitch_ignore_cnt = 7,
    .flags.enable_internal_pullup = true,
};

ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

i2c_device_config_t dev_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = 0x77,
    .scl_speed_hz = 100000,
};

ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &bme688));

return ESP_OK;
}