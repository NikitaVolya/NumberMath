/**
 * @file console_game_strategy.h
 * @brief Console-based output and input strategy for the NumberMatch game.
 *
 * This module provides functions for rendering the game field in the console,
 * handling user input (arrows, enter, help, add line), displaying menus,
 * messages, and tutorial screens. It implements a console-specific output
 * strategy compatible with the `output_config` interface.
 */

#ifndef _CONSOLE_GAME_STRATEGY_H
#define _CONSOLE_GAME_STRATEGY_H

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "custom_output.h"
#include "../../game.h"
#include "../../game_config.h"
#include "../output_config.h"

#define HORISONTAL_LINE_PATTERN "==="
#define MIN_FIELD_DISPLAY_HEIGHT 15

#define ENABLE_COLOR    "\033[0m"
#define UNENABLE_COLOR  "\033[35m"
#define HIGHLITED_COLOR "\033[36m"

#define BASE_PRINT "%s %d \033[0m"
#define SELECTED_PRINT "%s[%d]\033[0m"
#define CURSOR_PRINT "%s\033[4m %d \033[0m"
#define SELECTED_CURSOR_PRINT "%s\033[4m[%d]\033[0m"

/**
 * @enum GAME_KEY
 * @brief Represents keys recognized by the console game input system.
 */
enum GAME_KEY {
    ARROW_KEY = 16,             /**< Base value for arrow keys */
    UP        = ARROW_KEY + 1,  /**< Up arrow key */
    DOWN      = ARROW_KEY + 2,  /**< Down arrow key */
    LEFT      = ARROW_KEY + 3,  /**< Left arrow key */
    RIGHT     = ARROW_KEY + 4,  /**< Right arrow key */
    ENTER     = 10,             /**< Enter key */
    HELP      = 104,            /**< 'H' key for help */
    ADD_LINE  = 97,             /**< 'A' key to add a line */
    NONE      = 0               /**< Unrecognized key */
};
typedef enum GAME_KEY GAME_KEY;

/**
 * @brief Reads a single key press without waiting for Enter and without echo.
 * @return The integer code of the key pressed.
 */
int get_key();

/**
 * @brief Reads a key press and maps it to a GAME_KEY value.
 * @return A GAME_KEY value representing the key pressed.
 */
GAME_KEY get_game_key();

/**
 * @brief Moves the cursor in the game field according to an arrow key.
 * @param key Arrow key pressed (UP, DOWN, LEFT, RIGHT).
 * @param cursor Pointer to the current cursor position.
 * @param field Pointer to the game_field structure.
 */
void user_console_game_move(GAME_KEY key, vector2i *cursor, game_field *field);

/**
 * @brief Handles user input during the console game loop.
 * @param config Pointer to the game_config structure.
 */
void user_console_game_input(struct game_config* config);

/**
 * @brief Displays available numbers on the game field.
 * @param field Pointer to the game_field structure.
 */
void display_console_available_numbers(game_field *field);

/**
 * @brief Prints a single field cell with console formatting and colors.
 * @param cell Field cell to display.
 */
void print_field_cell(field_cell *cell);

/**
 * @brief Prints the entire game field to the console.
 * @param field Pointer to the game_field structure.
 */
void print_game_field(game_field *field);

/**
 * @brief Displays the full game screen including score, stage, hints, and field.
 * @param config Pointer to the game_config structure.
 */
void display_console_game_screen(struct game_config *config);

/**
 * @brief Displays the end-of-game screen with a "GAME OVER" message.
 * @param config Pointer to the game_config structure.
 */
void end_console_game_message(struct game_config *config);

/**
 * @brief Displays the tutorial for the game with navigation and exit options.
 */
void show_console_game_tutorial(void);

/**
 * @brief Executes the selected menu action.
 * @param config Pointer to the game_config structure.
 * @param position Index of the selected menu item.
 * @param exit Pointer to an integer flag controlling menu termination.
 * @return 1 if the action executed successfully, 0 if the position is invalid.
 */
int execute_console_game_action(struct game_config *config, int position, int *exit);

/**
 * @brief Displays and manages the main game menu in the console.
 * @param config Pointer to the game_config structure.
 */
void show_console_game_menu(struct game_config *config);

/**
 * @brief Displays a centered message on the console and waits for user input.
 * @param text Message string to display.
 */
void show_console_game_message(const char *text);

#endif /* _CONSOLE_GAME_STRATEGY_H */
