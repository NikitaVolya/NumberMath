
#ifndef _GAME_H
#define _GAME_H

#define NUMBERS 51

#define HORISONTAL_LINE_PATTERN "==="
#define MIN_FIELD_DISPLAY_HEIGHT 15

#include"game_objects/game_field.h"
#include"custom_output.h"
#include"serializer.h"


enum GAME_KEY {
    ARROW_KEY = 16, /* 0b00010000   */
    UP = 17,        /* 0b00010001   */ 
    DOWN = 18,      /* 0b00010010   */
    LEFT = 19,      /* 0b00010011   */
    RIGHT = 20,     /* 0b00010100   */
    ENTER = 10,     /* 0b00001000   */
    HELP = 104,     /* 0b01101000 H */
    ADD_LINE = 97,  /* 0b01100001 A */
    NONE = 0
};
typedef enum GAME_KEY GAME_KEY;

int get_key();

GAME_KEY get_game_key();

short randshort(short start, short end);

/*  
    Expands the game field by duplicating all available cells  
    and appending their values to the end of the field.

    input:  
        field - pointer to the game_field structure to expand
*/
void expand_game_field(game_field *field);

/*  
    Checks if the game is over.

    input:  
        field - pointer to the game_field structure  

    output:  
        returns 1 if the game is over (field is cleared or no matches left),  
        0 otherwise  
*/
int check_game_is_over(game_field *field);



void init_game(game_field *field);

void display_available_numbers(game_field *field);

void display_game_screen(game_field *field);

void user_game_select(vector2i *cursor,
                      vector2i *selected_pos,
                      game_field *field);

void user_game_move(GAME_KEY key,
                    vector2i *cursor,
                    game_field *field);

void user_game_input(vector2i *cursor,
                     vector2i *selected_pos,
                     game_field *field);

void game_cycle(game_field *field);

void load_game();

void start_game();

#endif /* _GAME_H */