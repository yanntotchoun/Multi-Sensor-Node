#include "driver/i2c_master.h"

#include "esp_err.h"
#include "bme688.h"

#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"
#include "esp_err.h"  
#include "esp_log.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void oled_send_command(uint8_t command);
esp_err_t esp_init_i2c(void);
esp_err_t probe(uint16_t address);
BME68X_INTF_RET_TYPE user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
BME68X_INTF_RET_TYPE user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
void user_delay_us(uint32_t period, void *intf_ptr);