/**
 * @file game_config.h
 * @brief Defines the configuration and runtime state of the NumberMatch game.
 *
 * This module contains the `game_config` structure and helper functions used to
 * create, initialize, and manage the active game session. It stores pointers to
 * the game field, output strategy, cursor position, UI offset, and other values
 * required to update and render the current game state.
 */

#ifndef _GAME_CONFIG_H
#define _GAME_CONFIG_H

#include "game_objects/game_field.h"
#include "game_objects/vector2i.h"
#include "output_strategies/output_config.h"

struct output_config;

/**
 * @brief Holds the current state and configuration of the NumberMatch game.
 *
 * The structure stores pointers to the active game board and rendering strategy,
 * as well as cursor position, selected cell, and UI layout information.
 *
 * Components:
 * - **field** — pointer to the active game field (`game_field*`)
 * - **output** — pointer to the output strategy used for rendering
 * - **cursor_p** — current cursor position in the grid
 * - **selected_p** — selected cell position; (-1, -1) means nothing is selected
 * - **shift** — horizontal rendering offset
 */
struct game_config {
    game_field *field;                 /**< Game field containing all cells */
    struct output_config *output;      /**< Output strategy (console, MLV, etc.) */
    vector2i cursor_p;                 /**< Current cursor position */
    vector2i selected_p;               /**< Position of selected cell (or -1, -1) */
    int shift;                         /**< Horizontal shift for UI layout */
};


/**
 * @brief Allocates and initializes a new game configuration object.
 *
 * All fields are assigned safe default values:
 * - `field`      → NULL  
 * - `output`     → NULL  
 * - `cursor_p`   → (0, 0)  
 * - `selected_p` → (-1, -1)  
 * - `shift`      → 0  
 *
 * @return Pointer to a newly created `game_config` structure.
 */
struct game_config* create_game_config();


/**
 * @brief Frees all resources associated with a `game_config` object.
 *
 * @details
 * - Frees the game field if allocated.
 * - Frees the output strategy if allocated.
 * - Frees the `game_config` structure itself.
 *
 * @param config Pointer to the configuration object to destroy.
 */
void free_game_config(struct game_config *config);

#endif /* _GAME_CONFIG_H */
