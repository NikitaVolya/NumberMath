
#ifndef _GAME_H
#define _GAME_H

#define INIT_CELLS_COUNT 51

#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#include"game_objects/game_field.h"
#include"serializer.h"
#include"output_strategies/console/console_game_strategy.h"

/*  
    Generates a random short integer within the specified range.  

    input:  
        start - lower bound of the range (inclusive)  
        end   - upper bound of the range (inclusive)  

    output:  
        returns a random short value between start and end  
*/
short randshort(short start, short end);

/*  
    Expands the game field by adding new values to available cells.  

    input:  
        field - pointer to the game_field structure  

    behavior:  
        - If additions are available, duplicates values from available cells  
          and adds them back into the field.  
        - Decreases the number of available additions by one.  
        - If no additions are available, displays a message.  
        - Saves the updated game field to "save.bin".  
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

/*  
    Shows a hint by highlighting a matching pair of cells on the game field.  

    input:  
        field - pointer to the game_field structure  

    behavior:  
        - If no hints are available, displays a message.  
        - If a match is found, highlights the two matching cells  
          and decreases the number of available hints by one.  
        - If no match is found, displays a message.  
        - Saves the updated game field to "save.bin".  
*/
void show_game_hints(game_field *field);

/*  
    Handles user selection and matching logic on the game field.  

    input:  
        cursor       - pointer to the current cursor position  
        selected_pos - pointer to the previously selected cell position  
        field        - pointer to the game_field structure  

    behavior:  
        - If a cell is already selected:  
            * Deselects it and checks for a match with the current cursor cell.  
            * If both positions match the same cell, deselects it.  
            * If the two cells match, marks them as unavailable, updates the score,  
              removes cleared rows if necessary, and saves the field state.  
            * Otherwise, updates the selected cell.  
        - If no cell is selected, selects the current cursor cell.  
*/
void user_game_select(vector2i *cursor, vector2i *selected_pos, game_field *field);

/*  
    Runs the main game loop.  

    input:  
        field - pointer to the game_field structure  

    behavior:  
        - Initializes the cursor and selection state.  
        - Continuously:  
            * Highlights the current cursor cell.  
            * Renders the game field.  
            * Processes user input.  
            * Checks if the field is cleared — if yes, increases stage,  
              resets available additions and hints, reinitializes the game,  
              and saves the field state.  
        - The loop continues until the game is over.  
        - Displays the end-game message and frees the game_field memory.  
*/
void game_cycle(game_field *field);

/*  
    Initializes the game field with random values.  

    input:  
        field - pointer to the game_field structure  

    behavior:  
        - Generates INIT_CELLS_COUNT random values in the range [1, 9].  
        - Clears the current game field by removing all existing rows.  
        - Adds the generated values to the field to start a new game state.  
*/  
void init_game_field(game_field *field);

/*  
    Loads a saved game from disk and starts the game loop.

    behavior:  
        - Tries to deserialize the game field from "save.bin".  
        - If deserialization fails, shows an error message.  
        - If successful, starts the main game cycle using the loaded field.  
*/  
void load_game();

/*  
    Starts a new game session.

    behavior:  
        - Creates a new game field of width 9.  
        - Initializes the field with starting values.  
        - Starts the main game loop.  
*/  
void start_game();

#endif /* _GAME_H */