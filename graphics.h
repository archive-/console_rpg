#include "libtcod.h"

extern float x, y;
extern bool entering_text, map_seen[90][120];

void draw_game(TCOD_console_t map_console, TCOD_map_t map, TCOD_console_t chat_console, TCOD_text_t chat)
{
    int w, h;
    TCOD_console_clear(map_console);
    TCOD_console_clear(chat_console);
    TCOD_console_print_frame(map_console, 0, 0, 122, 92, true, TCOD_BKGND_NONE, "MAP");
    TCOD_console_print_frame(chat_console, 0, 0, 122, 30, true, TCOD_BKGND_NONE, "CHAT");
    if (entering_text) {
        TCOD_text_render(chat_console, chat);
    }
    else {
        TCOD_text_reset(chat);
    }
    TCOD_map_compute_fov(map, (int)(x), (int)(y), 10, true, FOV_SHADOW);
    for (h = 0; h < 90; ++h) {
        for (w = 0; w < 120; ++w) {
            if (TCOD_map_is_in_fov(map, w, h)) {
                map_seen[h][w] = true;
                if (TCOD_map_is_walkable(map, w, h)) {
                    TCOD_console_put_char_ex(map_console, w+1, h+1, ',', TCOD_green, TCOD_dark_green);
                }
                else {
                    TCOD_console_put_char_ex(map_console, w+1, h+1, '=', TCOD_orange, TCOD_dark_orange);
                }
            }
            else if (map_seen[h][w]) {
                if (TCOD_map_is_walkable(map, w, h)) {
                    TCOD_console_put_char_ex(map_console, w+1, h+1, ',', TCOD_dark_green, TCOD_darker_green);
                }
                else {
                    TCOD_console_put_char_ex(map_console, w+1, h+1, '=', TCOD_dark_orange, TCOD_darker_orange);
                }
            }
        }
    }
    TCOD_console_put_char(map_console, (int)(x)+1, (int)(y)+1, '@', TCOD_BKGND_NONE);
    TCOD_console_blit(map_console, 0, 0, 122, 92, NULL, 37, 3, 1.f, 1.f);
    TCOD_console_blit(chat_console, 0, 0, 122, 30, NULL, 37, 97, 1.f, 1.f);
    TCOD_console_print_left(NULL, 1, 1, TCOD_BKGND_NONE, "fps: %d", TCOD_sys_get_fps());
    TCOD_console_flush(); /* print screen */
}
