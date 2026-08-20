#include "oled.h"

//COMMANDS

#define OLED_DISPLAY_OFF              0xAE
#define OLED_DISPLAY_ON               0xAF

#define OLED_SET_CONTRAST             0x81

#define OLED_NORMAL_DISPLAY           0xA6
#define OLED_INVERSE_DISPLAY          0xA7

#define OLED_DISPLAY_FROM_RAM         0xA4
#define OLED_ENTIRE_DISPLAY_ON        0xA5

#define OLED_SET_DISPLAY_CLOCK        0xD5
#define OLED_DISPLAY_CLOCK_DEFAULT    0x80

#define OLED_SET_MULTIPLEX_RATIO      0xA8
#define OLED_MULTIPLEX_64             0x3F

#define OLED_SET_DISPLAY_OFFSET       0xD3
#define OLED_DISPLAY_OFFSET_DEFAULT   0x00

#define OLED_SET_START_LINE           0x40

#define OLED_CHARGE_PUMP              0x8D
#define OLED_CHARGE_PUMP_ENABLE       0x14

#define OLED_SET_MEMORY_MODE          0x20
#define OLED_HORIZONTAL_ADDRESSING    0x00

#define OLED_SEGMENT_REMAP            0xA1
#define OLED_COM_SCAN_DIRECTION       0xC8

#define OLED_SET_COM_PINS             0xDA
#define OLED_COM_PINS_128X64          0x12

#define OLED_SET_PRECHARGE_PERIOD     0xD9
#define OLED_PRECHARGE_DEFAULT        0xF1

#define OLED_SET_VCOMH                0xDB
#define OLED_VCOMH_DEFAULT            0x40

#define OLED_SCROLL_ON                0x2F
#define OLED_SCROLL_OFF               0x2E
#define OLED_CONTINUOUS_SCROLL        0x29


#define OLED_SET_COLUMN_ADDRESS 0x21
#define OLED_SET_PAGE_ADDRESS   0x22
#define OLED_WIDTH  128
#define OLED_HEIGHT 64

static uint8_t framebuffer[OLED_WIDTH * OLED_HEIGHT / 8];

// Example 5x7 font table.
// Each character is 5 columns wide.
// Each byte represents 8 vertical pixels.
extern const uint8_t font5x7[][5];

static const char *TAG = "OLED";


esp_err_t oled_init(void)
{
    // Keep display off while configuring it
    ESP_RETURN_ON_ERROR(
        oled_turn_off_screen(),
        TAG,
        "OLED OFF failed"
    );

    // Brightness / contrast
    ESP_RETURN_ON_ERROR(
        oled_set_contrast(0x7F),
        TAG,
        "Contrast init failed"
    );

    // 64-row display
    ESP_RETURN_ON_ERROR(
        oled_set_multiplex(),
        TAG,
        "MUX init failed"
    );

    // No vertical offset
    ESP_RETURN_ON_ERROR(
        oled_set_display_offset(),
        TAG,
        "Offset init failed"
    );

    // Start displaying from line 0
    ESP_RETURN_ON_ERROR(
        oled_set_start_line(),
        TAG,
        "Start line init failed"
    );

    // Configure internal display oscillator
    ESP_RETURN_ON_ERROR(
        oled_set_clock(),
        TAG,
        "Clock init failed"
    );

    // Use horizontal framebuffer addressing
    ESP_RETURN_ON_ERROR(
        oled_set_memory_mode(),
        TAG,
        "Memory mode init failed"
    );

    // Configure horizontal / vertical orientation
    ESP_RETURN_ON_ERROR(
        oled_set_orientation(),
        TAG,
        "Orientation init failed"
    );

    // Configure COM pins for 128x64 panel
    ESP_RETURN_ON_ERROR(
        oled_set_com_pins(),
        TAG,
        "COM pin init failed"
    );

    // Pixel electrical timing
    ESP_RETURN_ON_ERROR(
        oled_set_precharge(),
        TAG,
        "Precharge init failed"
    );

    // COM deselect voltage
    ESP_RETURN_ON_ERROR(
        oled_set_vcomh(),
        TAG,
        "VCOMH init failed"
    );

    // Enable internal OLED panel voltage generation
    ESP_RETURN_ON_ERROR(
        oled_charge_pump_init(),
        TAG,
        "Charge pump init failed"
    );

    // Display contents stored in GDDRAM
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_DISPLAY_FROM_RAM),
        TAG,
        "Display RAM mode failed"
    );

    // Normal, non-inverted display
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_NORMAL_DISPLAY),
        TAG,
        "Normal display mode failed"
    );

    // Finally turn the panel on
    ESP_RETURN_ON_ERROR(
        oled_turn_on_screen(),
        TAG,
        "OLED ON failed"
    );

    // Start with an empty software framebuffer
    oled_clear();

    return ESP_OK;
}
//power
esp_err_t oled_turn_on_screen(void)
{
    return oled_send_command(OLED_DISPLAY_ON);
}

esp_err_t oled_turn_off_screen(void)
{
    return oled_send_command(OLED_DISPLAY_OFF);
}

//CHARGE pump (circuit that powers up the display)
esp_err_t oled_charge_pump_init(void)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_CHARGE_PUMP),
        TAG,
        "Failed to send charge pump command"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_CHARGE_PUMP_ENABLE),
        TAG,
        "Failed to enable charge pump"
    );

    return ESP_OK;
}

//CLOCK
esp_err_t oled_set_clock(void)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_SET_DISPLAY_CLOCK),
        TAG,
        "Failed to set display clock"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_DISPLAY_CLOCK_DEFAULT),
        TAG,
        "Failed to set display clock value"
    );

    return ESP_OK;
}

//Multiplex

esp_err_t oled_set_multiplex(void)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_SET_MULTIPLEX_RATIO),
        TAG,
        "Failed to set multiplex ratio"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_MULTIPLEX_64),
        TAG,
        "Failed to set multiplex value"
    );

    return ESP_OK;
}

//DISPLAY OFFSET

esp_err_t oled_set_display_offset(void)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_SET_DISPLAY_OFFSET),
        TAG,
        "Failed to set display offset"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_DISPLAY_OFFSET_DEFAULT),
        TAG,
        "Failed to set display offset value"
    );

    return ESP_OK;
}

//START LINE

esp_err_t oled_set_start_line(void)
{
    return oled_send_command(OLED_SET_START_LINE);
}

//MEMORY MODE

esp_err_t oled_set_memory_mode(void)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_SET_MEMORY_MODE),
        TAG,
        "Failed to set memory addressing mode"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_HORIZONTAL_ADDRESSING),
        TAG,
        "Failed to set horizontal addressing mode"
    );

    return ESP_OK;
}



// ORIENTATION


esp_err_t oled_set_orientation(void)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_SEGMENT_REMAP),
        TAG,
        "Failed to set segment remap"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_COM_SCAN_DIRECTION),
        TAG,
        "Failed to set COM scan direction"
    );

    return ESP_OK;
}



// COM PINS


esp_err_t oled_set_com_pins(void)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_SET_COM_PINS),
        TAG,
        "Failed to configure COM pins"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_COM_PINS_128X64),
        TAG,
        "Failed to set COM pin configuration"
    );

    return ESP_OK;
}



// CONTRAST


esp_err_t oled_set_contrast(uint8_t contrast)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_SET_CONTRAST),
        TAG,
        "Failed to send contrast command"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(contrast),
        TAG,
        "Failed to set contrast value"
    );

    return ESP_OK;
}



// PRECHARGE


esp_err_t oled_set_precharge(void)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_SET_PRECHARGE_PERIOD),
        TAG,
        "Failed to set precharge period"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_PRECHARGE_DEFAULT),
        TAG,
        "Failed to set precharge value"
    );

    return ESP_OK;
}



// VCOMH


esp_err_t oled_set_vcomh(void)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_SET_VCOMH),
        TAG,
        "Failed to set VCOMH"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_VCOMH_DEFAULT),
        TAG,
        "Failed to set VCOMH value"
    );

    return ESP_OK;
}



// DISPLAY MODE


esp_err_t oled_normal_display(void)
{
    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_DISPLAY_FROM_RAM),
        TAG,
        "Failed to resume display from RAM"
    );

    ESP_RETURN_ON_ERROR(
        oled_send_command(OLED_NORMAL_DISPLAY),
        TAG,
        "Failed to enable normal display"
    );

    return ESP_OK;
}



// TEST FUNCTIONS


esp_err_t oled_all_pixels_on(void)
{
    return oled_send_command(OLED_ENTIRE_DISPLAY_ON);
}

esp_err_t oled_display_from_ram(void)
{
    return oled_send_command(OLED_DISPLAY_FROM_RAM);
}
void oled_clear(void)
{
    memset(framebuffer, 0, sizeof(framebuffer));
}

void oled_draw_pixel(uint8_t x, uint8_t y, bool on)
{
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) {
        return;
    }
    // y/8 is getting the page 
    uint16_t index = x + (y / 8) * OLED_WIDTH;
    // what bit within the page 
    uint8_t bit = y % 8;

    if (on) {
        framebuffer[index] |= (1 << bit);
    } else {
        framebuffer[index] &= ~(1 << bit);
    }
}
void oled_draw_char(uint8_t x, uint8_t y, char c)
{
    if (c < 32 || c > 126) {
        return;
    }

    const uint8_t *glyph = font5x7[c - 32];

    for (uint8_t col = 0; col < 5; col++) {

        uint8_t column_data = glyph[col];

        for (uint8_t row = 0; row < 7; row++) {

            bool pixel_on = column_data & (1 << row);

            oled_draw_pixel(
                x + col,
                y + row,
                pixel_on
            );
        }
    }
}
void oled_draw_string(uint8_t x, uint8_t y, const char *str)
{
    while (*str != '\0') {

        oled_draw_char(x, y, *str);

        x += 6;   // 5 pixels for character + 1 pixel spacing

        str++;
    }
}

esp_err_t oled_update(void)
{
    ESP_RETURN_ON_ERROR(oled_send_command(0x21), TAG, "Column command failed");
    ESP_RETURN_ON_ERROR(oled_send_command(0x00), TAG, "Column start failed");
    ESP_RETURN_ON_ERROR(oled_send_command(0x7F), TAG, "Column end failed");

    ESP_RETURN_ON_ERROR(oled_send_command(0x22), TAG, "Page command failed");
    ESP_RETURN_ON_ERROR(oled_send_command(0x00), TAG, "Page start failed");
    ESP_RETURN_ON_ERROR(oled_send_command(0x07), TAG, "Page end failed");

    uint8_t data[1025];

    data[0] = 0x40;  // display data follows
    memcpy(&data[1], framebuffer, 1024);

    return i2c_master_transmit(
        oled,
        data,
        sizeof(data),
        1000
    );
}

