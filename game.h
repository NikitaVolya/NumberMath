
#ifndef _GAME_H
#define _GAME_H

#define INIT_CELLS_COUNT 51

#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#include"game_config.h"
#include"serializer.h"

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
    Expands the current game field by duplicating values from available cells.  

    input:  
        config - pointer to the game_config structure containing the current field and output settings  

    behavior:  
        - If there are available additions, duplicates all values from available cells  
          and appends them to the bottom of the game field.  
        - Decreases the count of available additions by one.  
        - If no additions are available, displays a warning message via the output strategy.  
        - Saves the updated game field state to "save.bin".  
*/
void expand_game_field(struct game_config *config);

/*  
    Advances the game to the next stage and resets stage-related parameters.  

    input:  
        field - pointer to the game_field structure  

    behavior:  
        - Increments the current stage number.  
        - Adds points to the score for clearing the field (CLEAR_FIELD_MATCH).  
        - Resets the number of available additions and hints to their maximum values.  
        - Reinitializes the game field with new random values.  
        - Saves the updated game state to "save.bin".  
*/
void update_stage(game_field *field);

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
    Displays a visual hint by highlighting two matching cells on the game field.  

    input:  
        config - pointer to the game_config structure containing the current field and output settings  

    behavior:  
        - If no hints are available, shows a "No hints available" message.  
        - If a matching pair is found, highlights both cells and decreases  
          the number of available hints by one.  
        - If no match exists, shows a "No match finded" message.  
        - Saves the updated game field state to "save.bin".  
*/
void show_game_hints(struct game_config *config);

/*  
    Handles user selection logic for matching cells in the game field.  

    input:  
        config - pointer to the game_config structure containing the current field,  
                 cursor position, and selected cell position  
*/
void user_game_select(struct game_config *config);

/*  
    Runs the main game loop.  

    input:
        config - pointer on game_config
*/
void game_cycle(struct game_config *config);

/*  
    Initializes the game field with random values.  

    input:  
        field - pointer to the game_field structure  

    behavior:  
        - Generates INIT_CELLS_COUNT random values in the range [1, 9].  
        - Clears the current game field by removing all existing rows.  
        - Adds the generated values to the field to start a new game state.  
*/  
void init_game_field(struct game_field *field);

/*  
    Loads a saved game from "save.bin" and starts the game loop.  

    input:
        config - pointer on game_config
*/
void load_game(struct game_config *config);

/*  
    Starts a new game by creating a fresh game field and running the game loop.  

    input:
        config - pointer on game_config
*/
void start_game(struct game_config *config);

/*  
    Executes the game by showing the main menu using the selected output strategy.

    input:
        config - pointer to the game_config structure

    behavior:
        - If config is NULL, prints an error message.
        - If config->output is NULL, prints an error about missing output strategy.
        - Otherwise, calls the show_game_menu function of the output strategy.
*/
void execute_game(struct game_config *config);

#endif /* _GAME_H */