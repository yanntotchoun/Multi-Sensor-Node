#include "include/gpio_driver.h"

#define BUTTON_POWER   23
#define BUTTON_SENSOR    16
#define BUTTON_PAGE    17

static const char *TAG = "GPIO";
esp_err_t gpio_init(void)
{

    ESP_RETURN_ON_ERROR(gpio_input_enable(BUTTON_SENSOR), TAG, "input on read button is not set"); 
    ESP_RETURN_ON_ERROR(gpio_input_enable(BUTTON_PAGE), TAG, "input on page button is not set");
     return ESP_OK;
}

uint8_t get_button_sensor(void){
  return gpio_get_level(BUTTON_SENSOR);
}
uint8_t get_button_page(void){
  return gpio_get_level(BUTTON_PAGE);
}