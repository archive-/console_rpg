#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "libtcod.h"
#include "txtfield.h"
#include "input.h"
#include "graphics.h"

bool end_game, entering_text;
float x, y, vx, vy, map_color_scalar[90][120];
bool w_key, a_key, s_key, d_key;

/* TODO networking */
void update_game(TCOD_map_t map)
{
        if (w_key) {
            if (TCOD_map_is_walkable(map, x, y-1))
                y -= vy * TCOD_sys_get_last_frame_length();
        }
        if (a_key) {
            if (TCOD_map_is_walkable(map, x-1, y))
                x -= vx * TCOD_sys_get_last_frame_length();
        }
        if (s_key) {
            if (TCOD_map_is_walkable(map, x, y+1))
                y += vy * TCOD_sys_get_last_frame_length();
        }
        if (d_key) {
            if (TCOD_map_is_walkable(map, x+1, y))
                x += vx * TCOD_sys_get_last_frame_length();
        }
}

int main(void)
{
    int w, h;
    for (h = 0; h < 90; ++h) {
        for (w = 0; w < 120; ++w) {
            map_color_scalar[h][w] = 0.f;
        }
    }
    
    TCOD_console_init_root(160, 128, "console rpg v0.01", true);
    TCOD_console_t map_console = TCOD_console_new(122, 92);
    TCOD_console_t chat_console = TCOD_console_new(122, 30);
    TCOD_map_t map = TCOD_map_new(120, 90);
    TCOD_text_t chat = TCOD_text_init(1, 28, 120, 1, 255);
    TCOD_text_set_properties(chat, TCOD_CHAR_BLOCK1, 2, ">> ", 4);
    TCOD_mouse_show_cursor(true);
    TCOD_console_set_keyboard_repeat(0, 50); 
    TCOD_sys_set_fps(60);
    end_game = entering_text = false;
    x = y = 10.f;
    vx = vy = 10.f;
    w_key = a_key = s_key = d_key = false;
    
    /* TODO generate map properties differently -- load from file, etc. */
    for (h = 0; h < 90; ++h) {
        for (w = 0; w < 120; ++w) {
            if (h == 0 || h == 89 || w == 0 || w == 99 || (w == 24 && h != 14 && h != 15)) {
                TCOD_map_set_properties(map, w, h, false, false);
            }
            else {
                TCOD_map_set_properties(map, w, h, true, true);
            }
        }
    }

    /* TODO game loop */
    while (!end_game && !TCOD_console_is_window_closed()) {
        handle_input(chat);
        update_game(map);
        draw_game(map_console, map, chat_console, chat);
    }

    return EXIT_SUCCESS;
}
