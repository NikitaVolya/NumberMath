#ifndef _CONSOLE_GAME_STRATEGY_H
#define _CONSOLE_GAME_STRATEGY_H

#define HORISONTAL_LINE_PATTERN "==="
#define MIN_FIELD_DISPLAY_HEIGHT 15

#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"custom_output.h"
#include"game_objects/game_field.h"
#include"game.h"


enum GAME_KEY {
    ARROW_KEY = 16,             /* 0b00010000   */
    UP        = ARROW_KEY + 1,  /* 0b00010001   */ 
    DOWN      = ARROW_KEY + 2,  /* 0b00010010   */
    LEFT      = ARROW_KEY + 3,  /* 0b00010011   */
    RIGHT     = ARROW_KEY + 4,  /* 0b00010100   */
    ENTER     = 10,             /* 0b00001000   */
    HELP      = 104,            /* 0b01101000 H */
    ADD_LINE  = 97,             /* 0b01100001 A */
    NONE      = 0
};
typedef enum GAME_KEY GAME_KEY;

int get_key();

GAME_KEY get_game_key();

void user_console_game_move(GAME_KEY key,
                            vector2i *cursor,
                            game_field *field);

void user_console_game_input(vector2i *cursor,
                             vector2i *selected_pos,
                             game_field *field);

void display_console_available_numbers(game_field *field);

/* 
    Displays a field cell on the console with appropriate color and formatting.

    input:    cell - field_cell structure to display
    output:   prints the cell to the console with its current visual state
*/
void print_field_cell(field_cell cell);

/*  
    Displays the entire game field on the console.

    input:  
        field - pointer to the game_field structure  

    output:  
        prints each cell of the field with proper formatting and colors  
*/
void print_game_field(game_field *field);

void display_console_game_screen(game_field *field);

void end_console_game_message(game_field *field);

/*  
    Displays the multi-page tutorial explaining the game rules and navigation.

    input:  
        none  

    output:  
        shows several tutorial pages on screen that the player  
        can navigate using LEFT and RIGHT arrows,  
        and exit using ENTER.  
*/
void show_console_game_tutorial();

/*  
    Executes a menu action based on the current menu selection.

    input:  
        position - index of the selected menu item  
        exit - pointer to an integer flag controlling menu termination  

    output:  
        launches the corresponding action:  
            0 - Start new game  
            1 - Load saved game  
            2 - Show tutorial  
            3 - Exit the game  
        returns 1 if execution succeeded,  
        0 if the position is invalid  
*/
int execute_cosnole_game_action(int position, int *exit);

/*  
    Displays and manages the main game menu interface.

    input:  
        none  

    output:  
        continuously renders the main menu with navigation controls,  
        processes user key inputs (UP, DOWN, ENTER),  
        and executes the selected menu action.  
        exits when the user chooses "Exit".  
*/
int show_console_game_menu();

void show_console_game_message(const char *text);

#endif /* _CONSOLE_GAME_STRATEGY_H */