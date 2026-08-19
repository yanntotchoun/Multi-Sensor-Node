#include "bme688.h"
#include "i2c.h"
#include "esp_err.h"
#include "esp_timer.h"




esp_err_t bme688_init(struct bme68x_dev*);

esp_err_t bme688_set_forced_mode(struct bme68x_dev*);

esp_err_t bme688_set_conf(struct bme68x_dev*, struct bme68x_conf* );

esp_err_t bme688_set_heatr_conf(uint8_t , struct bme68x_dev*, struct bme68x_heatr_conf*);

void bme688_get_data(uint8_t*, uint8_t ,struct bme68x_data*,struct bme68x_dev* ,struct bme68x_conf* ,struct bme68x_heatr_conf*);