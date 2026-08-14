#include "driver/i2c_master.h"

#include "esp_err.h"


#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"
#include "esp_err.h"  
#include "esp_log.h"
#include "esp_check.h"


void oled_send_command(uint8_t command);
esp_err_t esp_init_i2c(void);
esp_err_t probe(uint16_t address);