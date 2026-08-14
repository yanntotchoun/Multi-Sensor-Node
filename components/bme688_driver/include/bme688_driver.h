#include "bme688.h"
#include "bme688_defs.h"
#include "esp_err.h"
#include "esp_timer.h"




esp_err_t bme688_init(struct bme68x_dev);

esp_err_t bme688_set_forced_mode(struct bme68x_dev bme);

esp_err_t bme688_set_conf(struct bme68x_dev dev, struct bme68x_conf conf);

esp_err_t bme688_set_heatr_conf(uint8_t forced_mode, struct bme68x_dev bme, struct bme68x_heatr_conf heatr);

uint32_t bme_get_time_ms(struct bme68x_dev bme, struct bme68x_conf conf, struct bme68x_heatr_conf heatr);

void bme688_get_data(uint8_t forced_mode,struct bme68x_data data,struct bme68x_dev dev,struct bme68x_conf conf,struct bme68x_heatr_conf heatr);