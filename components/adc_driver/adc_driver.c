#include "include/adc_driver.h"

#define BATTERY_ADC_CHANNEL ADC_CHANNEL_3
static adc_oneshot_unit_handle_t adc2_handle;
static const char *TAG = "ADC";

esp_err_t adc_init(void){

    adc_oneshot_unit_init_cfg_t init_config1 = {
    .unit_id = ADC_UNIT_2,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};
ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc2_handle));

adc_oneshot_chan_cfg_t config = {
    .bitwidth = ADC_BITWIDTH_DEFAULT,
    .atten = ADC_ATTEN_DB_12,
};
ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle,BATTERY_ADC_CHANNEL, &config));
return ESP_OK;
}

esp_err_t adc_read_battery(int *adc_raw){

   esp_err_t err = adc_oneshot_read(adc2_handle,BATTERY_ADC_CHANNEL, adc_raw);

    if (err != ESP_OK) {
        return err;
    }

    ESP_LOGI(TAG,"ADC2 Channel[%d] Raw Data: %d",BATTERY_ADC_CHANNEL, *adc_raw);

    return ESP_OK;
}