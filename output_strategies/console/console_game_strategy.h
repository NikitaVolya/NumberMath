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
#include"../../game.h"
#include"../../game_config.h"
#include"../output_config.h"


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

/*  
    Reads a single key press from the user without waiting for Enter and without echoing.

    output:
        returns the integer code of the key pressed

    behavior:
        - Saves the current terminal settings.
        - Disables canonical mode and echo so input is immediate and invisible.
        - Reads one character from standard input.
        - Restores the previous terminal settings.
*/
int get_key();

/*  
    Reads a key press from the user and maps it to a GAME_KEY value.

    output:
        returns a GAME_KEY representing the key pressed (arrows, ENTER, HELP, ADD_LINE, or NONE)

    behavior:
        - Calls get_key() to read a single character.
        - Maps normal keys (ENTER, HELP, ADD_LINE) directly.
        - Detects arrow key sequences (ESC [ A/B/C/D) and maps them to UP, DOWN, RIGHT, LEFT.
        - Returns NONE for any unrecognized key.
*/
GAME_KEY get_game_key();

/*  
    Updates the cursor position on the game field based on the arrow key pressed.

    input:
        key    - the pressed arrow key (UP, DOWN, LEFT, RIGHT)
        cursor - pointer to the current cursor position
        field  - pointer to the game_field structure

    behavior:
        - Moves the cursor within bounds of the current row and field height.
        - Wraps horizontally when reaching the row edges.
        - Prevents vertical movement outside the field or into shorter rows.
*/
void user_console_game_move(GAME_KEY key, vector2i *cursor, game_field *field);

/*  
    Processes user input during the console game loop.

    input:
        config - pointer to game_config structure containing field, cursor, and output info

    behavior:
        - Reads a key from the user.
        - If an arrow key is pressed, moves the cursor accordingly.
        - If ENTER is pressed, selects the current cell and processes matching.
        - If ADD_LINE is pressed, expands the game field (duplicates available cells).
        - If HELP is pressed, highlights a possible match if hints are available.
        - Ignores other keys.
*/
void user_console_game_input(struct game_config* config);

/*  
    Displays the numbers currently available on the game field.

    input:
        field - pointer to the game_field structure

    behavior:
        - Scans all cells in the field.
        - Marks which numbers (1-9) are available.
        - Prints available numbers as digits, unavailable numbers as '-'.
*/
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

/*  
    Displays the full game screen in the console.  

    input:
        config - pointer to the game_config structure

    behavior:
        - Clears the console screen.
        - Prints current score and stage.
        - Shows number of hints and additions available.
        - Prints horizontal separator lines.
        - Displays available numbers.
        - Prints the current state of the game field.
        - Ensures minimum field height is displayed.
*/
void display_console_game_screen(struct game_config *config);

/*  
    Displays the final game screen and shows the "GAME OVER" message.

    input:
        config - pointer on game_config

    behavior:
        - Displays the current game field and messages.
        - Prints "GAME OVER !!!" at a specific position.
        - Waits for user input before continuing.
*/
void end_console_game_message(struct game_config *config);

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
        config - pointer on game_config
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
int execute_cosnole_game_action(struct game_config *config, int position, int *exit);

/*  
    Displays and manages the main game menu interface.

    input:  
        config - pointer on game_config

    output:  
        continuously renders the main menu with navigation controls,  
        processes user key inputs (UP, DOWN, ENTER),  
        and executes the selected menu action.  
        exits when the user chooses "Exit".  
*/
void show_console_game_menu(struct game_config *config);

/*  
    Displays a centered message on the console and waits for user input.

    input:
        text - message string to display

    behavior:
        - Calculates horizontal position to center the message.
        - Ensures the message does not start at a negative position.
        - Draws the message at row 9 of the console.
        - Waits for the user to press any key before returning.
*/
void show_console_game_message(const char *text);

#endif /* _CONSOLE_GAME_STRATEGY_H */