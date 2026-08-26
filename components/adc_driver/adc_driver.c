#include "include/adc_driver.h"


#define BATTERY_ADC_CHANNEL ADC_CHANNEL_3
#define ADC_SAMPLE_COUNT 16
static adc_oneshot_unit_handle_t adc2_handle;
static adc_cali_line_fitting_config_t cali_config;
static adc_cali_handle_t handle;
static const char *TAG = "ADC";


static int cmp_int(const void *a, const void *b)
{
    return (*(const int *)a - *(const int *)b);
}

esp_err_t adc_init(void){

    adc_oneshot_unit_init_cfg_t init_config1 = {
    .unit_id = ADC_UNIT_2,//change to ADC_UNIT_1 for final design
    .ulp_mode = ADC_ULP_MODE_DISABLE,
};
ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc2_handle));

adc_oneshot_chan_cfg_t config = {
    .bitwidth = ADC_BITWIDTH_DEFAULT,
    .atten = ADC_ATTEN_DB_12,
};
ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle,BATTERY_ADC_CHANNEL, &config));

ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");

adc_cali_line_fitting_config_t cali_config = {
    .unit_id = ADC_UNIT_2,
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_DEFAULT,
};
ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_config,&handle ));
return ESP_OK;
}

esp_err_t adc_read_battery(int *adc_raw){

 int samples[ADC_SAMPLE_COUNT];
  for (int i = 0; i < ADC_SAMPLE_COUNT; i++) {
        esp_err_t err = adc_oneshot_read(adc2_handle, BATTERY_ADC_CHANNEL, &samples[i]);
        if (err != ESP_OK) {
            return err;
        }
    }
    
      qsort(samples, ADC_SAMPLE_COUNT, sizeof(int), cmp_int);

    // median: average the two middle values for an even count
    *adc_raw = (samples[ADC_SAMPLE_COUNT / 2 - 1] + samples[ADC_SAMPLE_COUNT / 2]) / 2;

    ESP_LOGI(TAG, "ADC2 Channel[%d] Raw Data (median of %d): %d",
             BATTERY_ADC_CHANNEL, ADC_SAMPLE_COUNT, *adc_raw);

    return ESP_OK;
}

esp_err_t adc_get_voltage(int adc_raw,int *voltage){

esp_err_t err = adc_cali_raw_to_voltage(handle, adc_raw, voltage);


    if (err != ESP_OK) {
        return err;
    }

        if (*voltage < 250) {
        *voltage = 0;
    }

   ESP_LOGI(TAG, "ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT_2 + 1,BATTERY_ADC_CHANNEL, *voltage);

    return ESP_OK;
}