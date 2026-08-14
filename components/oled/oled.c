#include "oled.h"

//COMMANDS
#define open_display 0xAE
#define close_display 0xAF
#define set_contrast 0x81
#define set_normal 0xA6
#define set_inverse 0xA7
#define set_entire_display_off 0xA5
#define set_entire_display_on 0xA4
#define turn_horizontal_scrolling 0x2F
#define turn_horizontal_scrolling_off 0x2E
#define  continuous_scrolling  0x29

void oled_init(void){
    oled_turn_on_screen();
}
void oled_turn_on_screen(void){
    oled_send_command(set_entire_display_on);
}
void oled_turn_off_screen(void){
    oled_send_command(set_entire_display_off);
}


