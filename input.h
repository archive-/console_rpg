#include "libtcod.h"

extern bool entering_text, end_game, w_key, a_key, s_key, d_key;
extern char *chat_history[1024];
extern int chat_index;

void handle_input(TCOD_text_t chat)
{
    TCOD_key_t key;

    if (entering_text) {
        key = TCOD_console_check_for_keypress(TCOD_KEY_PRESSED);
    }
    else {
        /* TODO: fix the getting stuck in entering_text because of ENTER on release */
        key = TCOD_console_check_for_keypress(TCOD_KEY_PRESSED | TCOD_KEY_RELEASED);
    }

    if (key.vk == TCODK_NONE) return;
    else if (key.vk == TCODK_ESCAPE) {
        end_game = true;
        return;
    }
    else if (!entering_text && key.pressed && key.vk == TCODK_ENTER) {
        entering_text = true;
        TCOD_console_set_keyboard_repeat(1000, 1000); 
        return;
    }

    if (entering_text) {
        entering_text = TCOD_text_update(chat, key);
        if (!entering_text) {
            chat_history[chat_index] = strdup(TCOD_text_get(chat));
            ++chat_index;
            TCOD_text_reset(chat);
            TCOD_console_set_keyboard_repeat(0, 50); 
        }
    }
    else {
        if (key.c == 'w') w_key = !w_key;
        if (key.c == 'a') a_key = !a_key;
        if (key.c == 's') s_key = !s_key;
        if (key.c == 'd') d_key = !d_key;

    }
}
