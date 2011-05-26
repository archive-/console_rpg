#include "libtcod.h"

extern float x, y, map_color_scalar[90][120];
extern char *chat_history[1024];
extern bool entering_text;

/* TODO move to elsewhere */
int get_hour_of_day(void)
{
    float seconds = TCOD_sys_elapsed_seconds();
    return (int)(seconds) / 60;
}

/* TODO move to elsewhere */
int get_minute_of_day(void)
{
    float seconds = TCOD_sys_elapsed_seconds();
    return (int)(seconds) % 60;
}

void draw_game(TCOD_console_t map_console, TCOD_map_t map, TCOD_console_t chat_console, TCOD_text_t chat)
{
    int w, h, fov_range, minute_of_day;
    TCOD_console_clear(map_console);
    TCOD_console_clear(chat_console);
    TCOD_console_print_frame(map_console, 0, 0, 122, 92, true, TCOD_BKGND_NONE, "MAP");
    TCOD_console_print_frame(chat_console, 0, 0, 122, 30, true, TCOD_BKGND_NONE, "CHAT");

    if (entering_text) {
        TCOD_text_render(chat_console, chat);
    }

    minute_of_day = get_minute_of_day();
    if (minute_of_day < 10) fov_range = 3;
    else if (minute_of_day < 20) fov_range = 4;
    else if (minute_of_day < 30) fov_range = 5;
    else if (minute_of_day < 40) fov_range = 6;
    else if (minute_of_day < 50) fov_range = 5;
    else fov_range = 4;

    TCOD_map_compute_fov(map, (int)(x), (int)(y), fov_range, true, FOV_BASIC);
    for (h = 0; h < 90; ++h) {
        for (w = 0; w < 120; ++w) {
            if (TCOD_map_is_in_fov(map, w, h)) {
                map_color_scalar[h][w] = 1.f;
                if (TCOD_map_is_walkable(map, w, h)) {
                    TCOD_console_put_char_ex(map_console, w+1, h+1, ',', TCOD_green, TCOD_dark_green);
                }
                else {
                    TCOD_console_put_char_ex(map_console, w+1, h+1, '=', TCOD_orange, TCOD_dark_orange);
                }
            } else {
                map_color_scalar[h][w] -= 0.0005f;
                if (TCOD_map_is_walkable(map, w, h)) {
                    TCOD_console_put_char_ex(map_console, w+1, h+1, ',',
                            TCOD_color_multiply_scalar(TCOD_dark_green, map_color_scalar[h][w]), 
                            TCOD_color_multiply_scalar(TCOD_darker_green, map_color_scalar[h][w]));
                }
                else {
                    TCOD_console_put_char_ex(map_console, w+1, h+1, '=', 
                            TCOD_color_multiply_scalar(TCOD_dark_orange, map_color_scalar[h][w]),
                            TCOD_color_multiply_scalar(TCOD_darker_orange, map_color_scalar[h][w]));
                }
            }
        }
    }

    // print chat history in chat_console
    int i = 0;
    while (chat_history[i] != NULL) {
        TCOD_console_print_left(chat_console, 1, 2*i+2, TCOD_BKGND_NONE, "%s\n", chat_history[i]);
        ++i;
    }

    TCOD_console_put_char(map_console, (int)(x)+1, (int)(y)+1, '@', TCOD_BKGND_NONE);
    TCOD_console_blit(map_console, 0, 0, 122, 92, NULL, 37, 3, 1.f, 1.f);
    TCOD_console_blit(chat_console, 0, 0, 122, 30, NULL, 37, 97, 1.f, 1.f);
    TCOD_console_print_left(NULL, 1, 1, TCOD_BKGND_NONE, "fps: %d", TCOD_sys_get_fps());
    TCOD_console_print_left(NULL, 20, 1, TCOD_BKGND_NONE, "%2d:%02d", get_hour_of_day(), minute_of_day);
    TCOD_console_flush(); /* print screen */
}
