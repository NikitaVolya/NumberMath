/**
 * @file output_config.h
 * @brief Declares the output strategy configuration for rendering and interaction.
 *
 * This module defines the `output_config` structure, which stores function pointers
 * used to render the game, update it based on user input, display messages, and 
 * handle end-game screens. Different output implementations (Console, MLV, etc.)
 * populate this structure with their respective strategy functions.
 */

#ifndef _OUTPUT_CONFIG_H
#define _OUTPUT_CONFIG_H

#include "../game_config.h"

struct game_config;
struct output_config;

#include "console/console_game_strategy.h"
#include "mlv/mlv_game_strategy.h"

/**
 * @brief Defines a strategy interface for rendering and interacting with the game.
 *
 * Each field represents a function from the active output strategy.  
 * Implementations replace these pointers to switch between console, MLV, or other
 * rendering backends.
 *
 * Components:
 * - **display_game** — renders the current game state  
 * - **update_game** — processes user input and updates state  
 * - **end_game_message** — displays the final result after the game ends  
 * - **show_game_menu** — displays the main menu  
 * - **show_game_message** — displays a short temporary message (errors, hints, etc.)
 */
struct output_config {
    void (*display_game)(struct game_config *config);      /**< Draw game state */
    void (*update_game)(struct game_config *config);       /**< Handle input + logic */
    void (*end_game_message)(struct game_config *config);  /**< Show end-game screen */
    void (*show_game_menu)(struct game_config *config);    /**< Display main menu */
    void (*show_game_message)(const char *text);           /**< Display short message */
};

/**
 * @brief Applies the console-based output strategy.
 *
 * This function sets all fields of `output_config` to the console rendering
 * implementation.
 *
 * @param[out] config Pointer to the game configuration whose `output` field will be updated.
 */
void set_console_output(struct game_config *config);

/**
 * @brief Applies the MLV-based output strategy.
 *
 * This function sets the output function pointers to the MLV (MiniLibX-like) implementation.
 *
 * @param[out] config Pointer to the game configuration whose `output` field will be updated.
 */
void set_mlv_output(struct game_config *config);

#endif /* _OUTPUT_CONFIG_H */
