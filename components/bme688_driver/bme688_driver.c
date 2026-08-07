#include "include/bme688_driver.h"

struct bme688_conf
{
    /*! Humidity oversampling. Refer @ref osx*/
    uint8_t os_hum;

    /*! Temperature oversampling. Refer @ref osx */
    uint8_t os_temp;

    /*! Pressure oversampling. Refer @ref osx */
    uint8_t os_pres;

    /*! Filter coefficient. Refer @ref filter*/
    uint8_t filter;

    /*!
     * Standby time between sequential mode measurement profiles.
     * Refer @ref odr
     */
    uint8_t odr;
};

struct bme688_heatr_conf
{
    /*! Enable gas measurement. Refer @ref en_dis */
    uint8_t enable;

    /*! Store the heater temperature for forced mode degree Celsius */
    uint16_t heatr_temp;

    /*! Store the heating duration for forced mode in milliseconds */
    uint16_t heatr_dur;

    /*! Store the heater temperature profile in degree Celsius */
    uint16_t *heatr_temp_prof;

    /*! Store the heating duration profile in milliseconds */
    uint16_t *heatr_dur_prof;

    /*! Variable to store the length of the heating profile */
    uint8_t profile_len;

    /*!
     * Variable to store heating duration for parallel mode
     * in milliseconds
     */
    uint16_t shared_heatr_dur;
};

struct bme688_dev
{
    /*! Chip Id */
    uint8_t chip_id;

    /*!
     * The interface pointer is used to enable the user
     * to link their interface descriptors for reference during the
     * implementation of the read and write interfaces to the
     * hardware.
     */
    void *intf_ptr;

    /*!
     *             Variant id
     * ----------------------------------------
     *     Value   |           Variant
     * ----------------------------------------
     *      0      |   BME68X_VARIANT_GAS_LOW
     *      1      |   BME68X_VARIANT_GAS_HIGH
     * ----------------------------------------
     */
    uint32_t variant_id;

    /*! SPI/I2C interface */
    enum bme68x_intf intf;

    /*! Memory page used */
    uint8_t mem_page;

    /*! Ambient temperature in Degree C*/
    int8_t amb_temp;

    /*! Sensor calibration data */
    struct bme68x_calib_data calib;

    /*! Read function pointer */
    bme68x_read_fptr_t read;

    /*! Write function pointer */
    bme68x_write_fptr_t write;

    /*! Delay function pointer */
    bme68x_delay_us_fptr_t delay_us;

    /*! To store interface pointer error */
    BME68X_INTF_RET_TYPE intf_rslt;

    /*! Store the info messages */
    uint8_t info_msg;
};
struct bme688_driver bme;
struct bme688_conf conf;
struct bme688_heatr_conf heatr;
int8_t rslt;

//CONST
const int8_t forced_mode =1;
//REGISTERS
#define BME688_RESET 0x73
#define BME688_VARIANT_ID 0xF0
#define BME688_MODE 0x74
//I2C = BME68X_I2C_INTF



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

      
esp_err_t bme688_init(struct bme688_driver bme)
{
          rslt = bme68x_init(&bme);
          
          bme688_check_rslt("bme initialisation",rslt);

};


esp_err_t bme688_set_forced_mode(struct bme688_driver bme){

   rslt = bme68x_set_op_mode(BME68X_FORCED_MODE,&bme);

    bme688_check_rslt("bme set forced mode",rslt);
}

esp_err_t bme688_set_conf(struct bme688_driver dev,struct bme688_driver conf)
{
  conf.filter=BME68X_FILTER_OFF;
  conf.odr=BME68X_ODR_NONE;
  conf.os_hum=BME68X_OS_16X;
  conf.os_pres = BME68X_OS_1X;
  conf.os_temp = BME68X_OS_2X;

  rslt = bme68x_set_conf(conf,dev);
  bme688_check_rslt("bme set configuration",rslt);
};

esp_err_t bme688_set_heatr_conf(uint8_t forced_mode,struct bme688_driver &bme,struct bme688_heatr_conf)
{
heatr.enable = BME68X_ENABLE;
heatr.heatr_temp = 300;
heatr.heatr_dur =100;
rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE,&heatr,&bme);
bme688_check_rslt("bme heater configuration",rslt);
};

uint32_t bme_get_time_ms(struct bme688_driver bme, struct bme688_conf conf,struct bme688_heatr_conf heatr)
{
  rslt = bme68x_get_meas_dur(BME68X_FORCED_MODE,&conf,&bme)+(heatr.heatr_dur *1000);

  if(!(rslt = ESP_OK)){
    return 6767;
  }
};




