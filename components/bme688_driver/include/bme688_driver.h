#include "bme688.h"
#include "i2c.h"
#include "esp_err.h"
#include "esp_timer.h"




esp_err_t bme688_init(struct bme68x_dev*);

esp_err_t bme688_set_forced_mode(struct bme68x_dev*);

esp_err_t bme688_set_conf(struct bme68x_dev*, struct bme68x_conf* );

esp_err_t bme688_set_heatr_conf(uint8_t , struct bme68x_dev*, struct bme68x_heatr_conf*);

void bme688_trigger_reading(struct bme68x_dev *bme, struct bme68x_conf *conf, struct bme68x_heatr_conf *heatr);
bool bme688_read_reading(uint8_t* n_fields,struct bme68x_data* data,struct bme68x_dev* dev);
bool bme688_reading_ready(void);