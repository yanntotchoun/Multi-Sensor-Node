#include "include/bme688_driver.h"




int8_t rslt;

//CONST
const int8_t forced_mode =1;
//REGISTERS
#define BME688_RESET 0x73
#define BME688_VARIANT_ID 0xF0
#define BME688_MODE 0x74
/* Macro for count of samples to be displayed */
#define SAMPLE_COUNT  UINT16_C(300)
#define BME688_ADDR 0x77

static uint8_t dev_addr = BME688_ADDR;
static const char *TAG = "BME688";

// i need to pass everything by pointer

void bme688_check_rslt(const char api_name[], int8_t rslt)
{
    switch (rslt)
    {
        case BME68X_OK:

            /* Do nothing */
            break;
        case BME68X_E_NULL_PTR:
            printf("API name [%s]  Error [%d] : Null pointer\r\n", api_name, rslt);
            break;
        case BME68X_E_COM_FAIL:
            printf("API name [%s]  Error [%d] : Communication failure\r\n", api_name, rslt);
            break;
        case BME68X_E_INVALID_LENGTH:
            printf("API name [%s]  Error [%d] : Incorrect length parameter\r\n", api_name, rslt);
            break;
        case BME68X_E_DEV_NOT_FOUND:
            printf("API name [%s]  Error [%d] : Device not found\r\n", api_name, rslt);
            break;
        case BME68X_E_SELF_TEST:
            printf("API name [%s]  Error [%d] : Self test error\r\n", api_name, rslt);
            break;
        case BME68X_W_NO_NEW_DATA:
            printf("API name [%s]  Warning [%d] : No new data found\r\n", api_name, rslt);
            break;
        default:
            printf("API name [%s]  Error [%d] : Unknown error code\r\n", api_name, rslt);
            break;
    }
}

//initiliases the sensor by reading the chip id and calibrating it too
esp_err_t bme688_init(struct bme68x_dev *bme)
{
        bme->intf = BME68X_I2C_INTF;
        bme->intf_ptr = &dev_addr;
        bme->amb_temp =25;
        bme->read =user_i2c_read;
        bme->write=user_i2c_write;
        bme->delay_us =user_delay_us;
        rslt = bme68x_init(bme);
          
        bme688_check_rslt("bme initialisation",rslt);
        return ESP_OK;
}

//sets the bme688 to forced mode
esp_err_t bme688_set_forced_mode(struct bme68x_dev *bme)
{
   rslt = bme68x_set_op_mode(BME68X_FORCED_MODE,bme);

    bme688_check_rslt("bme set forced mode",rslt);
    return rslt;
}
//it is used to set the oversampling, the filer and the odr configuration
esp_err_t bme688_set_conf(struct bme68x_dev* dev, struct bme68x_conf* conf)
{
  conf->filter=BME68X_FILTER_OFF;
  conf->odr=BME68X_ODR_NONE;
  conf->os_hum=BME68X_OS_16X;
  conf->os_pres = BME68X_OS_1X;
  conf->os_temp = BME68X_OS_2X;

  rslt = bme68x_set_conf(conf,dev);
  bme688_check_rslt("bme set configuration",rslt);
  return rslt;
}


// it is used to set the gas configuration of the sensor
esp_err_t bme688_set_heatr_conf(uint8_t forced_mode, struct bme68x_dev* bme, struct bme68x_heatr_conf* heatr)
{
heatr->enable = BME68X_ENABLE;
heatr->heatr_temp = 300;
heatr->heatr_dur = 100;
rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, heatr,bme);
bme688_check_rslt("bme heater configuration",rslt);
return rslt;
}

uint32_t bme_get_time_ms(struct bme68x_dev *bme,  struct bme68x_conf *conf,  struct bme68x_heatr_conf *heatr)
{
    uint32_t meas_dur = bme68x_get_meas_dur(BME68X_FORCED_MODE, conf, bme)+ (heatr->heatr_dur * 1000);
    return meas_dur;
}

void bme688_get_data(uint8_t* n_fields,uint8_t forced_mode,struct bme68x_data* data,struct bme68x_dev* dev,struct bme68x_conf* conf,struct bme68x_heatr_conf* heatr){
    uint32_t del_period=0;
    int64_t time_ms=0;
    uint16_t sample_count =1;

    while(sample_count<= SAMPLE_COUNT){
    //setting forced mode triggers a reading?
    bme688_set_forced_mode(dev);
    
    //calculates delay period in ms
    del_period = bme_get_time_ms(dev,conf,heatr);
    ESP_LOGI(TAG, "del_period = %lu", (unsigned long)del_period);
    dev->delay_us(del_period,dev->intf_ptr);
    
    time_ms = (esp_timer_get_time()/1000);

    //
    rslt = bme68x_get_data(BME68X_FORCED_MODE,data,n_fields,dev);
    bme688_check_rslt("bme data collection",rslt);

    if (*n_fields)
        {
    #ifdef BME68X_USE_FPU
            printf("%u, %lu, %.2f, %.2f, %.2f, %.2f, 0x%x\n",
                   sample_count,
                   (long unsigned int)time_ms,
                   data->temperature,
                   data->pressure,
                   data->humidity,
                   data->gas_resistance,
                   data->status);
    #else
            printf("%u, %lu, %d, %lu, %lu, %lu, 0x%x\n",
                   sample_count,
                   (long unsigned int)time_ms,
                   (data->temperature / 100),
                   (long unsigned int)data->pressure,
                   (long unsigned int)(data->humidity / 1000),
                   (long unsigned int)data->gas_resistance,
                   data->status);
    #endif
            sample_count++;
        }
    }
}




