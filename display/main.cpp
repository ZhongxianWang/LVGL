#include "lvgl.h"
#include <SDL2/SDL.h>
#include <cstdlib>
#include <lvgl/stdlib/lv_string.h>

/* Set to 1 for SDL simulator, 0 for embedded hardware */
#define USE_SDL_DISPLAY 1

#define HOR_RES 800
#define VER_RES 480

static SDL_Window   * sdl_window;
static SDL_Renderer * sdl_renderer;
static SDL_Texture  * sdl_texture;

static uint32_t my_tick_get()
{
    return SDL_GetTicks();
}

/* SDL event handler timer callback */
static void sdl_event_handler(lv_timer_t * timer)
{
    (void)timer;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
    }
}

/* Flush callback: copy rendered pixels to SDL texture and present */
static void my_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    uint32_t stride = lv_draw_buf_width_to_stride(HOR_RES, lv_display_get_color_format(disp));
    SDL_Rect rect;
    rect.x = area->x1;
    rect.y = area->y1;
    rect.w = lv_area_get_width(area);
    rect.h = lv_area_get_height(area);
    SDL_UpdateTexture(sdl_texture, &rect, px_map, (int)stride);
    SDL_RenderClear(sdl_renderer);
    SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, NULL);
    SDL_RenderPresent(sdl_renderer);

    lv_display_flush_ready(disp);
}

int main()
{
    /* Initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    sdl_window = SDL_CreateWindow("LVGL Display",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  HOR_RES, VER_RES, 0);
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);

#if LV_COLOR_DEPTH == 32
    SDL_PixelFormatEnum px_format = SDL_PIXELFORMAT_RGB888;
#elif LV_COLOR_DEPTH == 24
    SDL_PixelFormatEnum px_format = SDL_PIXELFORMAT_BGR24;
#elif LV_COLOR_DEPTH == 16
    SDL_PixelFormatEnum px_format = SDL_PIXELFORMAT_RGB565;
#else
    #error "Unsupported LV_COLOR_DEPTH"
#endif

    sdl_texture = SDL_CreateTexture(sdl_renderer, px_format,
                                    SDL_TEXTUREACCESS_STATIC, HOR_RES, VER_RES);

    /* Initialize LVGL */
    lv_init();
    lv_tick_set_cb(my_tick_get);
    lv_delay_set_cb(SDL_Delay);

    /* Create display using lv_display_create */
    lv_display_t* disp = lv_display_create(HOR_RES, VER_RES);
        
    /* Allocate framebuffers and set display buffers (PARTIAL render mode) */
    lv_color_format_t cf = lv_display_get_color_format(disp);
    uint32_t stride = lv_draw_buf_width_to_stride(HOR_RES, cf);

    uint32_t buf_size = stride * VER_RES;
    uint8_t* fb_buf1 = new uint8_t[buf_size];   /* framebuffer 1 */
    uint8_t* fb_buf2 = new uint8_t[buf_size];   /* framebuffer 2 */
    lv_memset(fb_buf1, 0xFF, buf_size);
    lv_memset(fb_buf2, 0xFF, buf_size);
    lv_display_set_buffers(disp, fb_buf1, fb_buf2, buf_size, LV_DISPLAY_RENDER_MODE_DIRECT);
    
    /* Set flush callback */
    lv_display_set_flush_cb(disp, my_flush_cb);

    lv_timer_create(sdl_event_handler, 5, NULL);
    /* ---- Hello World UI ---- */
    /* Set screen background color */
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);

    /* Create a label and set its text */
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello World!");
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    while (true) {
        uint32_t time_till_next = lv_timer_handler();
        lv_delay_ms(time_till_next);
    }

    return 0;
}