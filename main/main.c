#include <stdio.h>
#include "bme688_driver.h"
#include "i2c.h"
#include "oled.h"
#include "esp_log.h"

static const char *TAG = "MAIN";


void i2cScanner(void){
   printf("Scanning ...\n");
  for(int i=1;i<=127;i++){

  esp_err_t status = probe(i);

    if(status == ESP_OK){
      printf("Device found at address  0x%02X\n",i);
   

    }
  }
  printf("Scan complete.\n");
}
void app_main(void)
{

struct bme68x_dev bme;
struct bme68x_calib_data bme_calib;
struct bme68x_conf conf;
struct bme68x_heatr_conf heatr;
struct bme68x_data data;

esp_init_i2c();
ESP_LOGI(TAG, "Initializing I2C");


i2cScanner();

//initialising the sensor
bme688_init(bme);


bme688_set_conf(bme,conf);

bme688_set_heatr_conf(BME68X_FORCED_MODE,bme,heatr);

ESP_LOGI(TAG, "Initializing BME");

printf("Sample, TimeStamp(ms), Temperature(deg C), Pressure(Pa), Humidity(%%), Gas resistance(ohm), Status\n");
bme688_get_data(BME68X_FORCED_MODE,data,bme,conf,heatr);
ESP_LOGI(TAG, "Initializing getting data");
}
