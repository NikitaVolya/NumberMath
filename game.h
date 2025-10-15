
#ifndef _GAME_H
#define _GAME_H

#define NUMBERS 51

#include"game_field.h"


enum GAME_KEY {
    ARROW_KEY = 16, /* 0b0001000 */
    UP = 17,        /* 0b0001001 */ 
    DOWN = 18,      /* 0b0001010 */
    LEFT = 19,      /* 0b0001011 */
    RIGHT = 20,     /* 0b0001100 */
    ENTER = 32,     /* 0b0010000 */
    HELP = 64,      /* 0b0100000 */
    ADD_LINE = 128, /* 0b1000000 */
    NONE = 0        /* 0b0000000 */
};
typedef enum GAME_KEY GAME_KEY;

int get_key();

GAME_KEY get_game_key() ;

short randshort(short start, short end);

void init_game(game_field *field);

void user_game_input(vector2i *cursor,
                     vector2i *selected_pos,
                     game_field *field);

void start_game();

#endif /* _GAME_H */