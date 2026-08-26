#ifndef ADC_DRIVER_H
#define ADC_DRIVER__H


#include "hal/adc_types.h"
#include "esp_adc/adc_oneshot.h"
#include <stdlib.h> 
#include "esp_adc/adc_cali.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"



esp_err_t adc_init(void);
esp_err_t adc_read_battery(int *adc_raw);
esp_err_t adc_get_voltage(int adc_raw,int * voltage);

#endif