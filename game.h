/**
* @file game.h
* @brief Core functions and game loop for NumberMatch.
*
* Provides functions for initializing the game field, running the main game loop,
* handling user input, hints, stage updates, and executing the game menu.
*
* @author Volianskyi Nikita
* @date 2025
*/

/**
* @brief Number of initial cells to generate when initializing a new game field.
*/
#define INIT_CELLS_COUNT 51
/**
* @brief Width of gird in a new game field.
*/
#define GRID_WIDTH 9

#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#include"game_config.h"
#include"serializer.h"

/**
* @brief Generates a random short integer within the specified range.
*
* @param[in] start Lower bound of the range (inclusive).
* @param[in] end   Upper bound of the range (inclusive).
*
* @return short A random short value between start and end.
*/
short randshort(short start, short end);

/**
* @brief Expands the current game field by duplicating values from available cells.
*
* @param[in,out] config Pointer to the game_config structure containing the current field 
*                       and output settings.
*
* @details
* - If there are available additions, duplicates all values from available cells 
*   and appends them to the bottom of the game field.
* - Decreases the count of available additions by one.
* - If no additions are available, displays a warning message via the output strategy.
* - Saves the updated game field state to "save.bin".
*/
void expand_game_field(struct game_config *config);

/**
* @brief Advances the game to the next stage and resets stage-related parameters.
*
* @param[in,out] field Pointer to the game_field structure.
*
* @details
* - Increments the current stage number.
* - Adds points to the score for clearing the field (CLEAR_FIELD_MATCH).
* - Resets the number of available additions and hints to their maximum values.
* - Reinitializes the game field with new random values.
* - Saves the updated game state to "save.bin".
*/
void update_stage(game_field *field);

/**
* @brief Checks whether the game has ended.
*
* @param[in] field Pointer to the game_field structure.
*
* @return 1 if the game is over (field is cleared or no matches left), 0 otherwise.
*/
int check_game_is_over(game_field *field);

/**
* @brief Displays a visual hint by highlighting a pair of matching cells.
*
* @param[in,out] config Pointer to the game_config structure containing the current field and output settings.
*
* @details
* - If no hints are available, shows a "No hints available" message.
* - If a matching pair is found, highlights both cells and decreases the number of available hints by one.
* - If no match exists, shows a "No match found" message.
* - Saves the updated game field state to "save.bin".
*/
void show_game_hints(struct game_config *config);

/**
* @brief Handles user selection logic for matching cells in the game field.
*
* @param[in,out] config Pointer to the game_config structure containing the current field,
*                       cursor position, and selected cell position.
*
* @return MATCH_TYPE value representing the result of the user's selection.
*/
MATCH_TYPE user_game_select(struct game_config *config);

/**
* @brief Runs the main game loop.
*
* @param[in,out] config Pointer to the game_config structure containing the current
*                       game state, field, score, and output settings.
*/
void game_cycle(struct game_config *config);

/**
* @brief Initializes the game field with random values.
*
* @param[in,out] field Pointer to the game_field structure to be initialized.
*
* @details
* - Generates **INIT_CELLS_COUNT** random values in the range [1, 9].
* - Clears the current game field by removing all existing rows.
* - Adds the generated values to the field to start a new game state.
*/
void init_game_field(struct game_field *field);

/**
* @brief Loads a saved game from "save.bin" and starts the main game loop.
* 
* @param[in,out] config Pointer to the game_config structure. On input, it may contain
*                       initial configuration; on output, it will be updated with
*                       loaded game state.
*/
void load_game(struct game_config *config);

/**
* @brief Starts a new game by creating a fresh game field and running the main game loop.
* 
* @param[in,out] config Pointer to the game_config structure. On input, it may contain
*                        initial settings; on output, it will be initialized with a
*                        new game state.
*/
void start_game(struct game_config *config);

/**
* @brief Executes the game by displaying the main menu using the selected output strategy.
*
* @param[in] config Pointer to the game_config structure. Must not be NULL.
*
* @details
* - If @p config is NULL, an error message is printed.
* - If @p config->output is NULL, an error about the missing output strategy is printed.
* - Otherwise, the function calls config->output->show_game_menu().
*/
void execute_game(struct game_config *config);