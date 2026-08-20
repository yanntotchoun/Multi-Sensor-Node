#ifndef OLED_H
#define OLED_H
#include "esp_err.h"
#include "esp_timer.h"
#include "i2c.h"
#include "font5x7.h"
#include "driver/i2c_master.h"


esp_err_t oled_init(void);
esp_err_t oled_init(void);

esp_err_t oled_send_command(uint8_t command);

esp_err_t oled_turn_on_screen(void);
esp_err_t oled_turn_off_screen(void);

esp_err_t oled_charge_pump_init(void);

esp_err_t oled_set_clock(void);
esp_err_t oled_set_multiplex(void);
esp_err_t oled_set_display_offset(void);
esp_err_t oled_set_start_line(void);
esp_err_t oled_set_memory_mode(void);
esp_err_t oled_set_orientation(void);
esp_err_t oled_set_com_pins(void);
esp_err_t oled_set_precharge(void);
esp_err_t oled_set_vcomh(void);
esp_err_t oled_normal_display(void);

esp_err_t oled_set_contrast(uint8_t contrast);


void oled_clear(void);
void oled_draw_pixel(uint8_t x, uint8_t y, bool on);
void oled_draw_char(uint8_t x, uint8_t y, char c);
void oled_draw_string(uint8_t x, uint8_t y, const char *str);

#endif