
#ifndef _GAME_H
#define _GAME_H

#define NUMBERS 51

#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#include"game_objects/game_field.h"
#include"serializer.h"
#include"console_game_strategy.h"



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

void show_game_hints(game_field *field);


void init_game(game_field *field);

void user_game_select(vector2i *cursor, vector2i *selected_pos, game_field *field);


void game_cycle(game_field *field);

void load_game();

void start_game();

#endif /* _GAME_H */