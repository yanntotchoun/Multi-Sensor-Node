#include <stdio.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_sleep.h"

#include "bme688_driver.h"
#include "i2c.h"
#include "oled.h"
#include "gpio_driver.h"
#include "adc_driver.h"
#include "font5x7.h"


#include "esp_err.h"
#include "esp_timer.h"
#include "esp_log.h"

static struct bme68x_dev bme;
static struct bme68x_conf conf;
static struct bme68x_heatr_conf heatr;
static struct bme68x_data latest_data;

static bool wifi_connected = false;
static bool data_valid = false;
static int adc_raw;
static int voltage;      
static const char *TAG = "MAIN";

typedef enum
{
    SENSOR_PAGE,
    SYSTEM_PAGE,
    PAGE_COUNT

} oled_page_t;



typedef enum
{
    EVENT_TRIGGER_READING,
    EVENT_DATA_READY,
    EVENT_PAGE

} system_event_t;


static QueueHandle_t event_queue;
TaskHandle_t bme_task_handle;

void display_page(oled_page_t page)
{
    char buffer[32];

    oled_clear();

    if (!data_valid)
    {
        oled_draw_string(0, 0, "No data");
        oled_update();
        return;
    }
    switch (page)
    {
    case SENSOR_PAGE:
        
            snprintf(
                buffer,
                sizeof(buffer),
                "Temp: %.1f C",
                latest_data.temperature
            );

            oled_draw_string(0, 0, buffer);

            snprintf(
                buffer,
                sizeof(buffer),
                "Hum: %.1f %%",
                latest_data.humidity
            );

            oled_draw_string(0, 12, buffer);

            snprintf(
                buffer,
                sizeof(buffer),
                "Pressure: %.1f kPa",
                latest_data.pressure /1000.0f
            );

            oled_draw_string(0, 24, buffer);

            snprintf(
                buffer,
                sizeof(buffer),
                "Gas: %.0f Ohms",
                latest_data.gas_resistance
            );

            oled_draw_string(0, 36, buffer);

        break;

        case SYSTEM_PAGE:

             if (wifi_connected) {
            oled_draw_string(0, 0, "WiFi: Connected");
            } else {
           oled_draw_string(0, 0, "WiFi: Offline");
             }
        oled_update();

        snprintf(
                buffer,
                sizeof(buffer),
                "Battery Level: %d %%",
                (voltage*100)/4200
            );

            oled_draw_string(0, 12, buffer);



        break;
    
    default:
        break;
    }

    oled_update();
}



//TASKS

void button_task(void *arg)
{
    uint8_t previous_sensor =1; 
    uint8_t previous_page =1; 

    while (1)
    {

      uint8_t current_sensor = get_button_sensor();
       uint8_t current_page = get_button_page();

    
       if(current_page ==0 && previous_page ==1){
          vTaskDelay(pdMS_TO_TICKS(20));
              if(get_button_page()==0){
          system_event_t event = EVENT_PAGE;  
           xQueueSend(event_queue, &event, 0);  
      } }
      
      if(current_sensor ==0 && previous_sensor==1){
          vTaskDelay(pdMS_TO_TICKS(20));
              if(get_button_sensor()==0){
          system_event_t event = EVENT_TRIGGER_READING;  
           xQueueSend(event_queue, &event, 0);  
      } 
      }
    
    
       previous_sensor = current_sensor;
        previous_page = current_page;
    
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void battery_task(void *arg)
{
    while(1){
    adc_read_battery(&adc_raw);
    adc_get_voltage(adc_raw,&voltage);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }

 
}



void bme688_task(void *arg)
{
    while (1)
    {
        // Sleep until system_task requests a reading
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        uint8_t n_fields = 0;

        // Start forced-mode measurement
        bme688_trigger_reading(&bme, &conf, &heatr);

        // Wait until sensor says the measurement is ready
        while (!bme688_reading_ready())
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // Read the measurement
        if (bme688_read_reading(&n_fields, &latest_data, &bme))
        {
            data_valid = true;

            printf("Temperature: %.2f\n", latest_data.temperature);
            printf("Pressure: %.2f\n", latest_data.pressure);
            printf("Humidity: %.2f\n", latest_data.humidity);
            printf("Gas Resistance: %.2f\n",
                   latest_data.gas_resistance);

            // Tell system_task that new data is available
            system_event_t event = EVENT_DATA_READY;

            xQueueSend(
                event_queue,
                &event,
                portMAX_DELAY
            );
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read BME688");
        }}
    }

void periodic_read_task(void *arg)
{
    TickType_t last_wake_time = xTaskGetTickCount();

    const TickType_t period =
        pdMS_TO_TICKS(30 * 60 * 1000);

    while (1)
    {
        xTaskNotifyGive(bme_task_handle);

        vTaskDelayUntil(
            &last_wake_time,
            period
        );
    }
}
void system_task(void *arg)
{
    system_event_t event;
    oled_page_t current_page = SENSOR_PAGE;

    display_page(current_page);


    while (1)
    {
        if (xQueueReceive(event_queue, &event, portMAX_DELAY))
        {
            switch (event)
            {
                    case EVENT_DATA_READY:
                   display_page(current_page);
                   break;

                case EVENT_TRIGGER_READING:
                xTaskNotifyGive(bme_task_handle);
                    break;
                case EVENT_PAGE:
                    current_page++;

                    if (current_page >= PAGE_COUNT) {
                        current_page = SENSOR_PAGE;
                    }
                    display_page(current_page);
                    break;


            }
        }
    }
}


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

esp_init_i2c();
ESP_LOGI(TAG, "Initializing I2C");
i2cScanner();
//initialising the sensor

ESP_ERROR_CHECK(bme688_init(&bme));
bme688_set_conf(&bme,&conf);
bme688_set_heatr_conf(BME68X_FORCED_MODE,&bme,&heatr);
ESP_LOGI(TAG, "Initializing BME");

ESP_ERROR_CHECK(oled_init());
ESP_LOGI(TAG, "Initializing OLED");

gpio_init();

adc_init();



   event_queue = xQueueCreate(
        10,
        sizeof(system_event_t)
    );

    if (event_queue == NULL)
    {
        ESP_LOGE(TAG, "Failed to create queue");
        return;
    }

   
    xTaskCreate(
        button_task,
        "Button",
        2048,
        NULL,
        5,
        NULL
    );

    xTaskCreate(
        bme688_task,
        "BME688",
        4096,
        NULL,
        4,
         &bme_task_handle
    );
    xTaskCreate(
    system_task,
    "System",
    4096,
    NULL,
    3,
    NULL
);
 xTaskCreate(
    battery_task,
    "Battery",
    4096,
    NULL,
    3,
    NULL
);

xTaskCreate(
    periodic_read_task,
    "PeriodicRead",
    2048,
    NULL,
    2,
    NULL
);


}
