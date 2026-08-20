
#include "include/i2c.h"
#define SCL_IO 22
#define SDA_IO 21
#define OLED_ADDRESS 0x3C

#define OLED_WIDTH  128
#define OLED_HEIGHT 64


i2c_master_bus_handle_t bus_handle=NULL;
i2c_master_dev_handle_t bme688=NULL;
i2c_master_dev_handle_t oled = NULL;

static const char *TAG = "I2C";

esp_err_t esp_init_i2c(void){

i2c_master_bus_config_t i2c_mst_config = {
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port = I2C_NUM_1,
    .scl_io_num = SCL_IO,
    .sda_io_num = SDA_IO,
    .glitch_ignore_cnt = 7,
    .flags.enable_internal_pullup = true,
};

ESP_RETURN_ON_ERROR(i2c_new_master_bus(&i2c_mst_config, &bus_handle),TAG,"master bus failed to create");

i2c_device_config_t dev_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = 0x77,
    .scl_speed_hz = 100000,
};

ESP_RETURN_ON_ERROR(i2c_master_bus_add_device(bus_handle, &dev_cfg, &bme688),TAG,"failed to add bme on bus");

i2c_device_config_t dev2_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = OLED_ADDRESS,
    .scl_speed_hz = 400000,
};
ESP_RETURN_ON_ERROR(i2c_master_bus_add_device(bus_handle, &dev2_cfg, &oled),TAG,"failed to add oled on bus");

return ESP_OK;
}
esp_err_t probe(uint16_t address){

    return i2c_master_probe(bus_handle,address,15);
}

esp_err_t oled_send_command(uint8_t command)
{
    uint8_t data[2];

    data[0] = 0x00;      // control byte: command
    data[1] = command;

  esp_err_t err = i2c_master_transmit(
        oled,
        data,
        sizeof(data),
        1000
    );
     ESP_LOGI("OLED",
             "Command 0x%02X -> %s",
             command,
             esp_err_to_name(err));

    return err;
}

esp_err_t oled_write_data(uint8_t command)
{
    uint8_t data[2];

    data[0] = 0x10;      // control byte: write
    data[1] = command;

  esp_err_t err = i2c_master_transmit(
        oled,
        data,
        sizeof(data),
        1000
    );
     ESP_LOGI("OLED",
             "Write 0x%02X -> %s",
             command,
             esp_err_to_name(err));

    return err;
}



BME68X_INTF_RET_TYPE user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    esp_err_t err = i2c_master_transmit_receive(
        bme688,
        &reg_addr, 1,
        reg_data, len,
        1000 / portTICK_PERIOD_MS
    );
    return (err == ESP_OK) ? 0 : -1;
}

BME68X_INTF_RET_TYPE user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t buf[len + 1];
    buf[0] = reg_addr;
    memcpy(&buf[1], reg_data, len);

    esp_err_t err = i2c_master_transmit(
        bme688,
        buf, len + 1,
        1000 / portTICK_PERIOD_MS
    );
    return (err == ESP_OK) ? 0 : -1;
}

void user_delay_us(uint32_t period, void *intf_ptr)
{
    esp_rom_delay_us(period);
}

