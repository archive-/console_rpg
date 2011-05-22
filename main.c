#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "libtcod.h"

bool end_game, entering_text;
int x, y;

void handle_input(TCOD_map_t map)
{
    TCOD_key_t key = TCOD_console_check_for_keypress(TCOD_KEY_PRESSED);
    if (key.vk == TCODK_NONE) /* no key pressed */
        return;
    else if (key.vk == TCODK_ESCAPE) {
        end_game = true;
        return;
    }
    else if (key.vk == TCODK_ENTER) {
        entering_text ^= entering_text;
    }
    switch (key.c) {
        case 'w':
            if (TCOD_map_is_walkable(map, x, y-1)) --y;
            break;
        case 'a':
            if (TCOD_map_is_walkable(map, x-1, y)) --x;
            break;
        case 's':
            if (TCOD_map_is_walkable(map, x, y+1)) ++y;
            break;
        case 'd':
            if (TCOD_map_is_walkable(map, x+1, y)) ++x;
            break;
        case 'i': /* open inventory */
            break;
        case 0:
        default:
            break;
    }
}

void draw_game(TCOD_console_t map_console, TCOD_map_t map)
{
    int w, h;
    TCOD_console_clear(map_console);
    TCOD_console_print_frame(map_console, 0, 0, 120, 120, true, TCOD_BKGND_NONE, "MAP");
    TCOD_map_compute_fov(map, x, y, 10, true, FOV_SHADOW);
    for (h = 0; h < 100; ++h) {
        for (w = 0; w < 100; ++w) {
            if (TCOD_map_is_in_fov(map, w, h)) {
                if (TCOD_map_is_walkable(map, w, h)) {
                    TCOD_console_put_char_ex(map_console, w+10, h+10, ',', TCOD_green, TCOD_dark_green);
                }
                else {
                    TCOD_console_put_char_ex(map_console, w+10, h+10, '=', TCOD_orange, TCOD_dark_orange);
                }
            }
            else {
                if (TCOD_map_is_walkable(map, w, h)) {
                    TCOD_console_put_char_ex(map_console, w+10, h+10, ',', TCOD_dark_green, TCOD_darker_green);
                }
                else {
                    TCOD_console_put_char_ex(map_console, w+10, h+10, '=', TCOD_dark_orange, TCOD_darker_orange);
                }
            }
        }
    }
    TCOD_console_put_char(map_console, x+10, y+10, '@', TCOD_BKGND_NONE);
    TCOD_console_blit(map_console, 0, 0, 120, 120, NULL, 1, 3, 1.f, 1.f);
    TCOD_console_print_left(NULL, 1, 1, TCOD_BKGND_NONE, "fps: %d", TCOD_sys_get_fps());
    TCOD_console_flush(); /* print screen */
}

int main(void)
{
    int w, h;
    TCOD_console_init_root(160, 128, "Zombeh v0.01", true);
    TCOD_console_t map_console = TCOD_console_new(120, 120);
    TCOD_mouse_show_cursor(true);
    TCOD_sys_set_fps(60);
    end_game = entering_text = false;
    x = y = 10;
    TCOD_map_t map = TCOD_map_new(100, 100);
    for (h = 0; h < 100; ++h) {
        for (w = 0; w < 100; ++w) {
            if (h == 0 || h == 99 || w == 0 || w == 99 || (w == 24 && h != 14 && h != 15)) {
                TCOD_map_set_properties(map, w, h, false, false);
            }
            else {
                TCOD_map_set_properties(map, w, h, true, true);
            }
        }
    }
    while (!end_game && !TCOD_console_is_window_closed()) {
        handle_input(map);
        draw_game(map_console, map);
    }
    return EXIT_SUCCESS;
}
