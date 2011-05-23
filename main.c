#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "libtcod.h"

bool end_game, entering_text;
float x, y, vx, vy;
bool w_key, a_key, s_key, d_key, map_seen[90][120];

void handle_input()
{
    TCOD_key_t key = TCOD_console_check_for_keypress(TCOD_KEY_PRESSED | TCOD_KEY_RELEASED);
    if (key.vk == TCODK_NONE) /* no key pressed */
        return;
    else if (key.vk == TCODK_ESCAPE) {
        end_game = true;
        return;
    }
    else if (key.vk == TCODK_ENTER) {
        entering_text = !entering_text;
    }
  
    if (key.c == 'w') w_key = !w_key;
    if (key.c == 'a') a_key = !a_key;
    if (key.c == 's') s_key = !s_key;
    if (key.c == 'd') d_key = !d_key;
}

void draw_game(TCOD_console_t map_console, TCOD_map_t map, TCOD_console_t chat_console)
{
    int w, h;
    TCOD_console_clear(map_console);
    TCOD_console_clear(chat_console);
    TCOD_console_print_frame(map_console, 0, 0, 122, 92, true, TCOD_BKGND_NONE, "MAP");
    TCOD_console_print_frame(chat_console, 0, 0, 122, 30, true, TCOD_BKGND_NONE, "CHAT");
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

void update_game(TCOD_map_t map)
{
        if (w_key) {
            if (TCOD_map_is_walkable(map, x, y-1)) y -= vy * TCOD_sys_get_last_frame_length();
        }
        if (a_key) {
            if (TCOD_map_is_walkable(map, x-1, y)) x -= vx * TCOD_sys_get_last_frame_length();
        }
        if (s_key) {
            if (TCOD_map_is_walkable(map, x, y+1)) y += vy * TCOD_sys_get_last_frame_length();
        }
        if (d_key) {
            if (TCOD_map_is_walkable(map, x+1, y)) x += vx * TCOD_sys_get_last_frame_length();
        }
}

int main(void)
{
    int w, h;
    for (h = 0; h < 90; ++h) {
        for (w = 0; w < 120; ++w) {
            map_seen[h][w] = false;
        }
    }
    TCOD_console_init_root(160, 128, "console rpg v0.01", true);
    TCOD_console_t map_console = TCOD_console_new(122, 92);
    TCOD_console_t chat_console = TCOD_console_new(122, 30);
    TCOD_map_t map = TCOD_map_new(120, 90);
    //TCOD_text_t chat = TCOD_text_init(1, 1, 120, 28, 255);
    //TCOD_text_set_properties(chat, 
    TCOD_mouse_show_cursor(true);
    TCOD_console_set_keyboard_repeat(0, 50); 
    TCOD_sys_set_fps(60);
    end_game = entering_text = false;
    x = y = 10.f;
    vx = vy = 6.f;
    w_key = a_key = s_key = d_key = false;
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
    while (!end_game && !TCOD_console_is_window_closed()) {
        handle_input();
        update_game(map);
        draw_game(map_console, map, chat_console);
    }
    return EXIT_SUCCESS;
}
