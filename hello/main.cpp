#include "lvgl.h"
#include <SDL2/SDL.h> 

static uint32_t my_tick_get() {
    return SDL_GetTicks();
}

int main()
{
    /* Initialize LVGL */
    lv_init();

    lv_tick_set_cb(my_tick_get);
    /* Create an SDL window for LVGL */
    lv_display_t * disp = lv_sdl_window_create(800, 480);
    (void)disp;

    /* Create SDL mouse input device */
    lv_sdl_mouse_create();

    /* Create SDL keyboard input device */
    lv_sdl_keyboard_create();

    /* Create SDL mousewheel input device */
    lv_sdl_mousewheel_create();

    /* ---- Hello World UI ---- */
    /* Set screen background color */
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);

    /* Create a label and set its text */
    lv_obj_t* label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello World!");
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    while (true) {
        uint32_t time_till_next = lv_timer_handler();
        lv_sleep_ms(time_till_next);
    }

    return 0;
}
