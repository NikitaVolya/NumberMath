
#ifndef _GAME_H
#define _GAME_H

#define NUMBERS 51

#include"game_field.h"


enum GAME_KEY {
    ARROW_KEY = 16, /* 0b0010000 */
    UP = 17,        /* 0b0010001 */ 
    DOWN = 18,      /* 0b0010010 */
    LEFT = 19,      /* 0b0010011 */
    RIGHT = 20,     /* 0b0010100 */
    ENTER = 10,     /* 0b0001000 */
    HELP = 104,    
    ADD_LINE = 97,
    NONE = 0
};
typedef enum GAME_KEY GAME_KEY;

int get_key();

GAME_KEY get_game_key();

short randshort(short start, short end);

void init_game(game_field *field);

void display_game_screen(game_field *field, vector2i cursor);

void user_game_select(vector2i *cursor, vector2i *selected_pos, game_field *field);

void user_game_input(vector2i *cursor,
                     vector2i *selected_pos,
                     game_field *field);

void start_game();

#endif /* _GAME_H */