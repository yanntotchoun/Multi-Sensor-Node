#ifndef ADC_DRIVER_H
#define ADC_DRIVER__H


#include "hal/adc_types.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"



esp_err_t adc_init(void);

#endif